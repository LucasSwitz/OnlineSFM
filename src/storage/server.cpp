#include "storage.pb.h"
#include "storage.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "reconstruction.h"
#include "config.h"

#include "filesystem_storer.h"

#include "reconstruction.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerWriter;
using grpc::Status;

#define CHUNK_SIZE 1024 * 10

class StorageServer : public Storage::Service
{
public:
    StorageServer() : _storage(CONFIG_GET_STRING("storage.root")) {}

    virtual Status Store(ServerContext *context,
                         ServerReader<StoreItemRequest> *stream,
                         StoreItemResponse *response)
    {
        LOG(INFO) << "Storing new file";
        std::string final_data;
        StoreItemRequest request;
        while (stream->Read(&request))
        {
            final_data += request.data();
        }
        response->set_path(this->_storage.Store(final_data, request.path()));
        return Status::OK;
    }

    virtual Status Get(ServerContext *context,
                       const GetItemRequest *request,
                       ServerWriter<GetItemResponse> *writer)
    {
        LOG(INFO) << "Getting " << request->path();
        std::vector<char> data;
        this->_storage.Read(request->path(), data);
        std::string data_full(data.begin(), data.end());
        for (int i = 0; i < data.size(); i += CHUNK_SIZE)
        {
            GetItemResponse data_chunked;
            std::string chunked_bytes = data_full.substr(i, CHUNK_SIZE);
            data_chunked.set_data(chunked_bytes);
            writer->Write(data_chunked);
        }
        return Status::OK;
    }

private:
    FileSystemStorer _storage;
};

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    CONFIG_LOAD(argv[1]);
    StorageServer service;
    std::string server_address(CONFIG_GET_STRING("storage.address"));
    LOG(INFO) << "Starting storage server at address " << server_address;
    grpc::EnableDefaultHealthCheckService(true);
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::shared_ptr<Server> server(builder.BuildAndStart());
    LOG(INFO) << "Server waiting for connections...";
    server->Wait();
    return 1;
}