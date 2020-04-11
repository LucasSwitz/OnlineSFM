#pragma once

#include "worker.pb.h"
#include "worker.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "reconstruction.h"
#include "config.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerWriter;

class WorkerServer : public Worker::Service {
    virtual ::grpc::Status ComputeFeatures(ServerContext* context, 
                                      const WorkerComputeFeaturesRequest* request, 
                                      WorkerComputeFeaturesResponse* response){
         try{
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(request->reconstruction_id());
            reconstruction->ComputeFeatures({request->image_id});
         }catch(const std::exception& e){
            LOG(ERROR) << e.what();
         }
    }

    virtual ::grpc::Status ComputeMatches(ServerContext* context, 
                                          const WorkerComputeMatchesRequest* request, 
                                          WorkerComputeMatchesReponse* response){
         try{
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(request->reconstruction_id());
            reconstruction->ComputeMatches({request->image_id});
        }catch(const std::exception& e){
             LOG(ERROR) << e.what();
        }
    }
};

int main(int argc, char* argv[]){
    google::InitGoogleLogging(argv[0]);
    CONFIG_LOAD(argv[1]);
    WorkerServer service;
    std::string server_address(CONFIG_GET_STRING("worker.server_address"));
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