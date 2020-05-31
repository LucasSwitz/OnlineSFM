#include "worker_helpers.h"
#include "grpc_service_provider.h"

std::shared_ptr<Worker::Stub> GetWorkerClient(const std::string &address)
{
    return GRPC_PROVIDER->Get<Worker>(address);
}