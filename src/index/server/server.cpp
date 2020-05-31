#include "index.pb.h"
#include "index.grpc.pb.h"
#include "config.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "vocab_index.h"
#include "image_indexer.h"
#include "search_engine.h"
#include "tfidf_ranker.h"
#include "sql_descriptor_storage.h"
#include "sql_image_storage.h"
#include "remote_storage_adapter.h"
#include "sql_indexable_descriptor_storage.h"
#include <cppconn/driver.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerWriter;
using grpc::Status;

std::shared_ptr<VisualVocabularyIndex> visual_vocab_index;

class VisualIndexingServer : public VisualIndexingService::Service
{
    virtual ::grpc::Status IndexImage(ServerContext *context,
                                      const IndexImageRequest *request,
                                      IndexImageResponse *response)
    {
        try
        {
            auto image_indexer =
                ImageIndexerFactory::GetImageIndexer(visual_vocab_index);
            LOG(INFO) << "Indexing image " << request->image_id();
            image_indexer->Index(request->reconstruction_id(), request->image_id());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
        }
    }

    virtual ::grpc::Status GetBagOfWords(ServerContext *context,
                                         const GetBagOfWordsRequest *request,
                                         GetBagOfWordsResponse *response)
    {
        try
        {
            auto image_indexer =
                ImageIndexerFactory::GetImageIndexer(visual_vocab_index);
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
        }
    }

    virtual ::grpc::Status ClosestN(ServerContext *context,
                                    const ClosestNRequest *request,
                                    ClosestNResponse *response)
    {
        LOG(INFO) << "Searching for " << request->n() << " closest images to " << request->image_id();
        try
        {
            auto image_indexer =
                ImageIndexerFactory::GetImageIndexer(visual_vocab_index);
            std::unique_ptr<Ranker> ranker = std::make_unique<TFIDFRanker>(
                /*std::make_shared<SQLDescriptorStorage>(
                    CONFIG_GET_STRING("sql.words_table")),*/
                std::make_shared<SQLIndexableDescriptorStorage>(
                    CONFIG_GET_STRING("sql.words_table"),
                    CONFIG_GET_STRING("sql.word_frequencies_table")),
                std::make_shared<SQLImageStorage>(
                    std::make_shared<RemoteStorageAdapter>(CONFIG_GET_STRING("storage.address")),
                    CONFIG_GET_STRING("sql.views_table")));
            SearchEngine search_engine(std::move(image_indexer), std::move(ranker));
            std::vector<std::string> results;
            if (request->include_details())
            {
                results = search_engine.Search(request->reconstruction_id(),
                                               request->image_id(),
                                               request->n(),
                                               response->mutable_details());
            }
            else
            {
                results = search_engine.Search(request->reconstruction_id(),
                                               request->image_id(),
                                               request->n());
            }

            for (auto result : results)
            {
                response->add_image_ids(result);
            }
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
        }
    }
};

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    CONFIG_LOAD(argv[1]);
    ElasticDumper::Init(CONFIG_GET_LIST("elastic.hosts"),
                        CONFIG_GET_INT("elastic.timer_interval"));
    ElasticDumper::Instance()->Start();
    SQLStorage::InitConnectionPool(10);
    visual_vocab_index = std::make_shared<VisualVocabularyIndex>(std::make_unique<KMeansIndex<SIFTDistance>>());
    LOG(INFO) << "Loading index from " << argv[2];
    visual_vocab_index->Load(argv[2]);
    VisualIndexingServer service;
    std::string server_address(CONFIG_GET_STRING("index.address"));
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