#include "index_helpers.h"
#include "grpc_service_provider.h"

std::unique_ptr<VisualIndexingService::Stub> GetIndexingClient(const std::string &address)
{
    return GRPC_PROVIDER->Get<VisualIndexingService>(address);
}