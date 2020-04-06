#pragma once
#include "index.grpc.pb.h"
#include <grpcpp/grpcpp.h>


std::unique_ptr<VisualIndexingService::Stub> GetIndexingClient(const std::string& address){
    return std::make_unique<VisualIndexingService::Stub>(grpc::CreateChannel("localhost:9998",
                          grpc::InsecureChannelCredentials()));
}
