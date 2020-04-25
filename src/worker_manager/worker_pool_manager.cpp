#include "worker.pb.h"
#include "worker.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/status_code_enum.h>


#include <gflags/gflags.h>
#include <glog/logging.h>

#include "redis_worker_reserver.h"
#include "config.h"
#include "worker_helpers.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerWriter;
using namespace grpc;

#include "expotential_backoff.h"


std::shared_ptr<sw::redis::Redis> make_redis(){
    std::shared_ptr<sw::redis::Redis> redis;
    ExpotentialBackoff("redis",[&redis]() mutable{
        try{
            redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
            redis->ping();
            return true;
        }catch(...){
            return false;
        }
    }, 100);

    return redis;
}


template<typename T>
Status worker_scope(const std::string& f_name, T f, unsigned int max_retry){
    std::string worker_address;
    std::shared_ptr<RedisWorkerReserver> reserver;
    try{ 
        auto reserver = std::make_unique<RedisWorkerReserver>(make_redis());
        auto worker_address = reserver->ReserveWorker();
        auto worker = GetWorkerClient(worker_address);
        Status status;
        ExpotentialBackoff(f_name, [worker, &f, &status, worker_address]() mutable {
                grpc::ClientContext ctx;
                status = f(ctx, worker);
                return status.error_code() != StatusCode::DEADLINE_EXCEEDED && 
                       status.error_code() != StatusCode::UNAVAILABLE;
        }, max_retry);
        reserver->ReleaseWorker(worker_address);
        return status;
    }catch(const ExpotentialBackoffFailure& e){
        // This worker is dead. Get a new one and try again.
        reserver->RemoveWorker(worker_address);
        return worker_scope(f_name, f, max_retry);
    }catch(const std::exception& e){
        if(!worker_address.empty()){
            reserver->ReleaseWorker(worker_address);
        }
        LOG(ERROR) << e.what();
        return Status::CANCELLED;
    }
}


class WorkerPoolManagerServer : public WorkerPoolManager::Service {
    virtual Status ComputeFeatures(ServerContext* context, 
                                   const WorkerComputeFeaturesRequest* request, 
                                   WorkerComputeFeaturesResponse* response){
        LOG(INFO) << "Pooling ComputeFeatures: " << request->image_id();
        return worker_scope("ComputeFeatures", [request, response](grpc::ClientContext& ctx, 
                                                                   std::shared_ptr<Worker::Stub> worker) mutable{
            return worker->ComputeFeatures(&ctx, *request, response);
        }, 3);
    }

    virtual Status ComputeMatches(ServerContext* context, 
                                  const WorkerComputeMatchesRequest* request, 
                                  WorkerComputeMatchesResponse* response){
        LOG(INFO) << "Pooling ComputeMatches: " << request->image_id();
        return worker_scope("ComputeMatches", [request, response](grpc::ClientContext& ctx, 
                                                                  std::shared_ptr<Worker::Stub> worker) mutable{
            return worker->ComputeMatches(&ctx, *request, response);
        }, 3);
    }

    virtual Status AddImage(ServerContext* context, 
                            const WorkerAddImageRequest* request, 
                            WorkerAddImageResponse* response){
        LOG(INFO) << "Pooling AddImage: " << request->image_id();
        return worker_scope("AddImage", [request, response](grpc::ClientContext& ctx, 
                                                            std::shared_ptr<Worker::Stub> worker) mutable{
            return worker->AddImage(&ctx, *request, response);
        }, 3);
    }

    virtual Status IncrementalSFM(ServerContext* context, 
                                  const ::WorkerIncrementalSFMRequest* request, 
                                  WorkerIncrementalSFMResponse* response){
        LOG(INFO) << "Pooling IncrementalSFM: " << request->reconstruction_id();
        return worker_scope("IncrementalSFM", [request, response](grpc::ClientContext& ctx, 
                                                                  std::shared_ptr<Worker::Stub> worker) mutable{
            return worker->IncrementalSFM(&ctx, *request, response);
        }, 3);
    }

    virtual Status ComputeStructure(ServerContext* context, 
                                    const WorkerComputeStructureRequest* request, 
                                    WorkerComputeStructureResponse* response){
        LOG(INFO) << "Pooling ComputeStructure: " << request->reconstruction_id();
        return worker_scope("ComputeStructure", [request, response](grpc::ClientContext& ctx, 
                                                                    std::shared_ptr<Worker::Stub> worker) mutable{
            return worker->ComputeStructure(&ctx, *request, response);
        }, 3);
    }

    virtual Status Register(ServerContext* context, 
                                          const RegisterWorkerRequest* request, 
                                          RegisterWorkerResponse* response){
                
        LOG(INFO) << "Adding Worker: " << request->address();
        try{ 
            auto reserver = std::make_unique<RedisWorkerReserver>(make_redis());
            reserver->AddNewWorker(request->address(), request->cores());
            return Status::OK;
         }catch(const std::exception& e){
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }
};

int main(int argc, char* argv[]){
    google::InitGoogleLogging(argv[0]);
    CONFIG_LOAD(argv[1]);
    WorkerPoolManagerServer service;
    std::string server_address(CONFIG_GET_STRING("worker_pool.address"));
    LOG(INFO) << "Starting server at address " << server_address; 
    grpc::EnableDefaultHealthCheckService(true);
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::shared_ptr<Server> server(builder.BuildAndStart());
    LOG(INFO) << "Server waiting for connections...";
    server->Wait();

    return 1;
}