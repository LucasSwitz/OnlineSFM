#include "worker_helpers.h"

std::shared_ptr<Worker::Stub> GetWorkerClient(const std::string& address){
    return std::make_unique<Worker::Stub>(grpc::CreateChannel(address,
                             grpc::InsecureChannelCredentials()));
}