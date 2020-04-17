#include "worker.pb.h"
#include "worker.grpc.pb.h"

#include <grpcpp/grpcpp.h>

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

class WorkerPoolManagerServer : public WorkerPoolManager::Service {
    virtual Status ComputeFeatures(ServerContext* context, 
                                      const WorkerComputeFeaturesRequest* request, 
                                      WorkerComputeFeaturesResponse* response){
        try{
            LOG(INFO) << "Pooling ComputeFeatures: " << request->image_id();
            auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
            auto reserver = std::make_unique<RedisWorkerReserver>(redis);
            auto worker_address = reserver->ReserveWorker();
            auto worker = GetWorkerClient(worker_address);
            grpc::ClientContext ctx;
            worker->ComputeFeatures(&ctx, *request, response);
            reserver->ReleaseWorker(worker_address);
            return Status::OK;
         }catch(const std::exception& e){
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    virtual Status ComputeMatches(ServerContext* context, 
                                          const WorkerComputeMatchesRequest* request, 
                                          WorkerComputeMatchesResponse* response){
        try{
            LOG(INFO) << "Pooling ComputeMatches: " << request->image_id();
            auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
            auto reserver = std::make_unique<RedisWorkerReserver>(redis);
            auto worker_address = reserver->ReserveWorker();
            auto worker = GetWorkerClient(worker_address);
            grpc::ClientContext ctx;
            worker->ComputeMatches(&ctx, *request, response);
            reserver->ReleaseWorker(worker_address);
            return Status::OK;
         }catch(const std::exception& e){
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    virtual Status AddImage(ServerContext* context, 
                                    const WorkerAddImageRequest* request, 
                                    WorkerAddImageResponse* response){
        try{ 
            LOG(INFO) << "Pooling AddImage: " << request->image_id();
            auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
            auto reserver = std::make_unique<RedisWorkerReserver>(redis);
            auto worker_address = reserver->ReserveWorker();
            auto worker = GetWorkerClient(worker_address);
            grpc::ClientContext ctx;
            worker->AddImage(&ctx, *request, response);
            reserver->ReleaseWorker(worker_address);
            return Status::OK;
         }catch(const std::exception& e){
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    virtual Status IncrementalSFM(ServerContext* context, 
                                  const ::WorkerIncrementalSFMRequest* request, 
                                  WorkerIncrementalSFMResponse* response){
        LOG(INFO) << "Pooling IncrementalSFM: " << request->reconstruction_id();
        try{ 
            auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
            auto reserver = std::make_unique<RedisWorkerReserver>(redis);
            auto worker_address = reserver->ReserveWorker();
            auto worker = GetWorkerClient(worker_address);
            grpc::ClientContext ctx;
            worker->IncrementalSFM(&ctx, *request, response);
            reserver->ReleaseWorker(worker_address);
            return Status::OK;
         }catch(const std::exception& e){
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    virtual Status ComputeStructure(ServerContext* context, 
                                    const WorkerComputeStructureRequest* request, 
                                    WorkerComputeStructureResponse* response){
        LOG(INFO) << "Pooling ComputeStructure: " << request->reconstruction_id();
        try{ 
            auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
            auto reserver = std::make_unique<RedisWorkerReserver>(redis);
            auto worker_address = reserver->ReserveWorker();
            auto worker = GetWorkerClient(worker_address);
            grpc::ClientContext ctx;
            worker->ComputeStructure(&ctx, *request, response);
            reserver->ReleaseWorker(worker_address);
            return Status::OK;
         }catch(const std::exception& e){
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    virtual Status Register(ServerContext* context, 
                                          const RegisterWorkerRequest* request, 
                                          RegisterWorkerResponse* response){
                
        LOG(INFO) << "Adding Worker: " << request->address();
        try{ 
            auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
            auto reserver = std::make_unique<RedisWorkerReserver>(redis);
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