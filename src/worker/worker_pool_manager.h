#pragma once

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
    virtual ::grpc::Status ComputeFeatures(ServerContext* context, 
                                      const WorkerComputeFeaturesRequest* request, 
                                      WorkerComputeFeaturesResponse* response){
        auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
        auto reserver = std::make_unique<RedisWorkerReserver>(redis);
        auto worker_address = reserver->ReserveWorker();
        auto worker = GetWorkerClient(worker_address);
        grpc::ClientContext ctx;
        worker->ComputeFeatures(&ctx, *request, response);
        reserver->ReleaseWorker(worker_address);
    }

    virtual ::grpc::Status ComputeMatches(ServerContext* context, 
                                          const WorkerComputeMatchesRequest* request, 
                                          WorkerComputeMatchesReponse* response){
        auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
        auto reserver = std::make_unique<RedisWorkerReserver>(redis);
        auto worker_address = reserver->ReserveWorker();
        auto worker = GetWorkerClient(worker_address);
        grpc::ClientContext ctx;
        worker->ComputeMatches(&ctx, *request, response);
        reserver->ReleaseWorker(worker_address);
    }

    virtual ::grpc::Status Register(ServerContext* context, 
                                          const RegisterWorkerRequest* request, 
                                          RegisterWorkerResponse* response){
        auto redis = std::make_shared<sw::redis::Redis>(CONFIG_GET_STRING("redis.address"));
        auto reserver = std::make_unique<RedisWorkerReserver>(redis);
        reserver->AddNewWorker(request->address(), request->cores());
    }
};

int main(int argc, char* argv[]){
    google::InitGoogleLogging(argv[0]);
    CONFIG_LOAD(argv[1]);
    WorkerPoolManagerServer service;
    std::string server_address(CONFIG_GET_STRING("worker_pool_manager.server_address"));
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