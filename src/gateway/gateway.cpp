#include "server.pb.h"
#include "server.grpc.pb.h"
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <iostream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <boost/asio.hpp>

#include "reconstruction.h"
#include "util.h"
#include <sys/stat.h>
#include <unordered_map>

#include "CTPL/ctpl.h"

#include "config.h"
#include "redis_sfm_backlog.h"
#include "remote.h"

#include "sql_storage.h"
#include "sql_regions_storage.h"

#include "index.grpc.pb.h"
#include "index_helpers.h"
#include "sql_image_storage.h"
#include "job_index_service.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerWriter;
using grpc::Status;

#define CHUNK_SIZE 1024 * 10

class ReconstructionServer : public ReconstructionService::Service
{
public:
    ReconstructionServer(uint64_t thread_count) : thread_pool(thread_count)
    {
    }

    Status NewReconstruction(ServerContext *context, const NewReconstructionRequest *request, NewReconstructionResponse *response)
    {
        ReconstructionData new_reconstruction;
        std::string uuid = GetUUID();
        new_reconstruction.set_id(uuid);
        response->set_reconstruction_id(new_reconstruction.id());

        try
        {
            ReconstructionFetcher rf;
            rf.Fetch(RemoteReconstructionContext(uuid))->StoreData(new_reconstruction);
            LOG(INFO) << "Made new reconstruction " << uuid;
            return Status::OK;
        }
        catch (std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status ReconstructionUploadImage(ServerContext *context,
                                     ServerReader<ReconstructionUploadImageRequest> *stream,
                                     ReconstructionUploadImageResponse *response)
    {
        ReconstructionUploadImageRequest request;
        stream->Read(&request);
        ImageData final_image;
        final_image.CopyFrom(request.image());
        while (stream->Read(&request))
        {
            final_image.mutable_data()->append(request.image().data());
        }

        try
        {
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(final_image.metadata().reconstruction()));
            std::string image_id = reconstruction->StoreImage(final_image);
            if (!reconstruction->ComputeFeatures({image_id}))
            {
                LOG(ERROR) << "Failed to compute features for " << image_id;
                return Status::CANCELLED;
            }
            if (!reconstruction->AddImage(image_id))
            {
                LOG(ERROR) << "Failed to add image " << image_id;
                return Status::CANCELLED;
            }
            if (request.compute_matches() && !reconstruction->ComputeMatches({image_id}))
            {
                LOG(ERROR) << "Failed compute matches for " << image_id;
                return Status::CANCELLED;
            }

            response->set_image_id(image_id);
        }
        catch (std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
        return Status::OK;
    }

    Status ComputeMatches(ServerContext *context,
                          const ComputeMatchesRequest *request,
                          ComputeMatchesResponse *response)
    {
        ReconstructionFetcher rf;
        try
        {
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            if (!reconstruction->ComputeMatches({request->image_id()}))
            {
                LOG(ERROR) << "Failed compute matches for " << request->image_id();
                return Status::CANCELLED;
            }
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status SparseReconstruct(ServerContext *context,
                             const SparseReconstructRequest *request,
                             SparseReconstructResponse *response)
    {
        ReconstructionFetcher rf;
        try
        {
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            if (!reconstruction->SparseReconstruct())
            {
                LOG(ERROR) << "Failed sparse reconstruct for " << request->reconstruction_id();
                return Status::CANCELLED;
            }

            if (!reconstruction->ComputeStructure())
            {
                LOG(ERROR) << "Failed compute structure for " << request->reconstruction_id();
                return Status::CANCELLED;
            }
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status MVS(ServerContext *context,
               const MVSRequest *request,
               MVSResponse *response)
    {
        ReconstructionFetcher rf;
        LOG(INFO) << "Doing MVS for " << request->reconstruction_id();
        try
        {
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            response->set_success(reconstruction->MVS());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status GetOBJ(ServerContext *context,
                  const GetOBJRequest *request,
                  ServerWriter<GetOBJResponse> *writer)
    {
        LOG(INFO) << "Getting OBJ for " << request->reconstruction_id();
        ReconstructionFetcher rf;
        std::shared_ptr<Reconstruction> reconstruction = nullptr;
        OBJ obj;
        try
        {
            reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            obj = reconstruction->GetOBJ();
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }

        OBJData full_data = obj.Data();

        for (int i = 0; i < full_data.obj_data().size(); i += CHUNK_SIZE)
        {
            OBJData data_chunked;
            GetOBJResponse resp;
            data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
            std::string chunked_bytes = full_data.obj_data().substr(i, CHUNK_SIZE);
            data_chunked.set_obj_data(chunked_bytes);
            resp.mutable_obj()->CopyFrom(data_chunked);
            resp.set_success(true);
            writer->Write(resp);
        }
        for (int i = 0; i < full_data.texture_data().size(); i += CHUNK_SIZE)
        {
            OBJData data_chunked;
            GetOBJResponse resp;
            data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
            std::string chunked_bytes = full_data.texture_data().substr(i, CHUNK_SIZE);
            data_chunked.set_texture_data(chunked_bytes);
            resp.mutable_obj()->CopyFrom(data_chunked);
            resp.set_success(true);
            writer->Write(resp);
        }
        for (int i = 0; i < full_data.mtl_data().size(); i += CHUNK_SIZE)
        {
            OBJData data_chunked;
            GetOBJResponse resp;
            data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
            std::string chunked_bytes = full_data.mtl_data().substr(i, CHUNK_SIZE);
            data_chunked.set_mtl_data(chunked_bytes);
            resp.mutable_obj()->CopyFrom(data_chunked);
            resp.set_success(true);
            writer->Write(resp);
        }
        return Status::OK;
    };

    Status DeleteReconstruction(ServerContext *context,
                                const DeleteReconstructionRequest *request,
                                DeleteReconstructionResponse *response)
    {
        try
        {
            ReconstructionFetcher fetcher;
            auto reconstruction = fetcher.Fetch(RemoteReconstructionContext(request->id()));
            reconstruction->Delete();
            response->set_id(request->id());
            response->set_success(true);
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    };

    Status GetSparse(ServerContext *context,
                     const GetSparseRequest *request,
                     ServerWriter<GetSparseResponse> *writer)
    {
        LOG(INFO) << "Getting Sparse for " << request->reconstruction_id();
        ReconstructionFetcher rf;
        auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
        SparseReconstruction sparse = reconstruction->GetSparse();
        SparsePointCloudData full_data = sparse.Data();
        for (int i = 0; i < full_data.data().size(); i += CHUNK_SIZE)
        {
            SparsePointCloudData data_chunked;
            GetSparseResponse resp;
            data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
            std::string chunked_bytes = full_data.data().substr(i, CHUNK_SIZE);
            data_chunked.set_data(chunked_bytes);
            resp.mutable_sparse()->CopyFrom(data_chunked);
            writer->Write(resp);
        }
        return Status::OK;
    }

    //TODO: Maybe remove this. I don't think this will actually be used that often.
    Status ReconstructionUploadImageBatch(ServerContext *context,
                                          ServerReader<ReconstructionUploadImageBatchRequest> *reader,
                                          ReconstructionUploadImageBatchResponse *response)
    {
        int last_idx = 0;
        ReconstructionUploadImageBatchRequest request;
        ImageData final_image;
        ReconstructionFetcher rf;
        reader->Read(&request);
        try
        {
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request.data().metadata().reconstruction()));
            std::set<std::string> new_image_ids;
            final_image.CopyFrom(request.data());
            while (reader->Read(&request))
            {
                if (request.idx() != last_idx)
                {
                    new_image_ids.insert(reconstruction->StoreImage(final_image));
                    final_image.CopyFrom(request.data());
                    last_idx++;
                    continue;
                }
                else
                {
                    final_image.mutable_data()->append(request.data().data());
                }
            }
            // insert last image
            new_image_ids.insert(reconstruction->StoreImage(final_image));

            reconstruction->ComputeFeatures(new_image_ids);
            std::for_each(new_image_ids.begin(),
                          new_image_ids.end(),
                          [reconstruction](const std::string &image_id) {
                              reconstruction->AddImage(image_id);
                          });
            reconstruction->ComputeMatches(new_image_ids);
            response->set_images_uploaded(last_idx);
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status GetReconstructionConfig(ServerContext *context,
                                   const GetReconstructionConfigRequest *request,
                                   GetReconstructionConfigResponse *response)
    {
        try
        {
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            ConfigurationContainerPtr config = reconstruction->GetConfig();
            response->set_config_json(config->jsonify());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status GetAgentConfig(ServerContext *context,
                          const GetAgentConfigRequest *request,
                          GetAgentCOnfigResponse *response)
    {
        try
        {
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            ConfigurationContainerPtr config = reconstruction->GetAgentConfig(request->agent_name());
            response->set_config_json(config->jsonify());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status SetAgentConfigFields(ServerContext *context,
                                const SetAgentConfigFieldsRequest *request,
                                SetAgentConfigFieldsResponse *response)
    {
        try
        {
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            reconstruction->SetAgentConfigFields(request->agent_name(), request->config_json());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status SetReconstructionConfigFields(ServerContext *context,
                                         const SetReconstructionConfigFieldsRequest *request,
                                         SetReconstructionConfigFieldsResponse *response)
    {
        try
        {
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            reconstruction->SetConfigFields(request->config_json());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status GetAllImages(ServerContext *context,
                        const GetAllImagesRequest *request,
                        GetAllImagesResponse *response)
    {
        try
        {
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            auto images = reconstruction->GetImages();
            for (auto image : images)
            {
                response->add_images(image.ID());
            }
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status GetImageRegions(ServerContext *context, const GetImageRegionsRequest *request, GetImageRegionsResponse *response)
    {
        try
        {
            auto regions_storage = std::make_shared<SQLRegionsStorage<openMVG::features::SIFT_Regions>>(CONFIG_GET_STRING("sql.image_regions_table"));
            auto regions = regions_storage->GetAllRegions(request->reconstruction_id(), request->image_id());
            for (int i = 0; i < regions->RegionCount(); i++)
            {
                auto desc = regions->Descriptors()[i];
                std::string desc_str((char *)desc.data(), desc.static_size);
                auto feature = regions->Features()[i];
                if (response->regions().find(desc_str) == response->regions().end())
                {
                    (*response->mutable_regions())[desc_str] = GetImageRegionsResponse_FeatureList();
                }
                auto *f = (*response->mutable_regions())[desc_str].add_features();
                f->set_x(feature.x());
                f->set_y(feature.y());
            }
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

    Status ScoreImages(ServerContext *context, const ScoreImagesRequest *request, ScoreImagesResponse *response)
    {
        try
        {
            auto image_storage = std::make_shared<SQLImageStorage>(
                std::make_shared<RemoteStorageAdapter>(CONFIG_GET_STRING("storage.address")),
                CONFIG_GET_STRING("sql.views_table"));
            auto images = image_storage->GetAllImageIds(request->reconstruction_id());
            std::vector<std::string> i(images.begin(), images.end());
            JobIndexService is;
            return is.Score(request->reconstruction_id(), i) ? Status::OK : Status::CANCELLED;
        }
        catch (const std::exception &e)
        {
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
    }

private:
    //std::unordered_map<std::string, Session*> _sessions;
    ctpl::thread_pool thread_pool;
};

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    CONFIG_LOAD(argv[1]);
    SQLStorage::InitConnectionPool(10);
    mkdir(CONFIG_GET_STRING("storage.root").c_str(), 0777);
    ReconstructionServer service(4);
    std::string server_address(CONFIG_GET_STRING("grpc_server.server_address"));
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