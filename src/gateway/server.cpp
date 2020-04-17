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

/*
    TODO: 
*/

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerWriter;

#define CHUNK_SIZE 1024*10

class ReconstructionServer : public ReconstructionService::Service {
    public:
        ReconstructionServer(uint64_t thread_count) : thread_pool(thread_count){

        }

        Status NewReconstruction(ServerContext* context, const NewReconstructionRequest* request, NewReconstructionResponse* response){
            ReconstructionData new_reconstruction;
            std::string uuid = GetUUID();
            new_reconstruction.set_id(uuid);
            response->set_reconstruction_id(new_reconstruction.id());
            
            try {
                ReconstructionFetcher rf;
                rf.Fetch(RemoteReconstructionContext(uuid))->StoreData(new_reconstruction);
            } catch(std::exception& e){
                LOG(ERROR) << e.what();
            }

            LOG(INFO) << "Made new reconstruction " << uuid;
            return Status::OK;
        }

        Status ReconstructionUploadImage(ServerContext* context, 
                                         ServerReader<ReconstructionUploadImageRequest>* stream, 
                                         ReconstructionUploadImageResponse* response){
            ReconstructionUploadImageRequest request;
            stream->Read(&request);
            ImageData final_image;
            final_image.CopyFrom(request.image());
            while(stream->Read(&request)){
                final_image.mutable_data()->append(request.image().data());  
            }
            
            try{
                ReconstructionFetcher rf;
                auto reconstruction = rf.Fetch(RemoteReconstructionContext(final_image.metadata().reconstruction()));
                std::string image_id = reconstruction->StoreImage(final_image);
                reconstruction->ComputeFeatures({image_id});
                reconstruction->AddImage(image_id);

                if(request.compute_matches()){
                    reconstruction->ComputeMatches({image_id});
                }
                response->set_image_id(image_id);
                
            } catch(std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
            return Status::OK;
        }

        Status ComputeMatches(ServerContext* context, 
                              const ComputeMatchesRequest* request, 
                              ComputeMatchesResponse* response){
            ReconstructionFetcher rf;
            try {
                auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
                reconstruction->ComputeMatches({request->image_id()});    
                return Status::OK;            
            } catch (const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
        }

        Status SparseReconstruct(ServerContext* context, 
                                    const SparseReconstructRequest* request, 
                                    SparseReconstructResponse* response){
            ReconstructionFetcher rf;
            try {
                auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
                bool success = reconstruction->SparseReconstruct();
                if(success)
                    success &= reconstruction->ComputeStructure();
                response->set_success(success);
                return Status::OK;
            } catch (const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
        }

        Status MVS(ServerContext* context, 
                    const MVSRequest* request, 
                    MVSResponse* response){
            ReconstructionFetcher rf;
            try {
                auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
                response->set_success(reconstruction->MVS(true));
                
                return Status::OK;
            } catch (const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
        }

        Status GetOBJ(ServerContext* context, 
                        const GetOBJRequest* request, 
                        ServerWriter<GetOBJResponse>* writer){
            LOG(INFO) << "Getting OBJ for " << request->reconstruction_id();
            ReconstructionFetcher rf;
            std::shared_ptr<Reconstruction> reconstruction = nullptr;
            OBJ obj;
            try {
                reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
                obj = reconstruction->GetOBJ();

            } catch(const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }

            OBJData full_data = obj.Data();

            for(int i = 0; i < full_data.obj_data().size(); i+=CHUNK_SIZE){
                OBJData data_chunked;
                GetOBJResponse resp;
                data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
                std::string chunked_bytes = full_data.obj_data().substr(i, CHUNK_SIZE);
                data_chunked.set_obj_data(chunked_bytes);
                resp.mutable_obj()->CopyFrom(data_chunked);
                resp.set_success(true);
                writer->Write(resp);
            }
            for(int i = 0; i < full_data.texture_data().size(); i+=CHUNK_SIZE){
                OBJData data_chunked;
                GetOBJResponse resp;
                data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
                std::string chunked_bytes = full_data.texture_data().substr(i, CHUNK_SIZE);
                data_chunked.set_texture_data(chunked_bytes);
                resp.mutable_obj()->CopyFrom(data_chunked);
                resp.set_success(true);
                writer->Write(resp);
            }
            for(int i = 0; i < full_data.mtl_data().size(); i+=CHUNK_SIZE){
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

        Status DeleteReconstruction(ServerContext* context, 
                                    const DeleteReconstructionRequest* request, 
                                    DeleteReconstructionResponse* response){
            try{
                ReconstructionFetcher fetcher;
                auto reconstruction = fetcher.Fetch(RemoteReconstructionContext(request->id()));
                reconstruction->Delete();
                response->set_id(request->id());
                response->set_success(true);  
                return Status::OK;
            } catch (const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
        };

        Status StartSession(ServerContext* context, 
                            const StartSessionRequest* request, 
                            StartSessionResponse* response){
           /* LOG(INFO) << "Creating new session for reconstruction " << request->reconstruction_id();
            std::string session_id = GetUUID();
            Session* s = new Session(session_id, 
                                        request->reconstruction_id(), 
                                        new RedisSFMBacklog(CONFIG_GET_STRING("redis.address"),
                                                                    CONFIG_GET_STRING("redis.user"),
                                                                    CONFIG_GET_STRING("redis.password")));
            
            this->_sessions[session_id] = s;
            response->set_reconstruction_id(request->reconstruction_id());
            response->set_session_id(session_id);
            LOG(INFO) << "Starting session " << session_id << " for reconstruction " << request->reconstruction_id();
            s->Start();
            return Status::OK;*/
        }

        Status StopSession(ServerContext* context, 
                            const StopSessionRequest* request, 
                            StopSessionResponse* response){
            /*Session* s = this->_sessions[request->session_id()];
            response->set_session_id(request->session_id());
            LOG(INFO) << "Stopping session " << request->session_id();
            s->Stop();
            delete this->_sessions[request->session_id()];
            return Status::OK;*/
        }


        Status SessionAddImage(ServerContext* context, 
                                ServerReader<SessionAddImageRequest>* stream, 
                                SessionAddImageResponse* response){
            SessionAddImageRequest request;
            stream->Read(&request);
            ImageData final_image;
            final_image.CopyFrom(request.upload_image().image());
            while(stream->Read(&request)){
                ReconstructionUploadImageRequest upload_image_request = request.upload_image();
                final_image.mutable_data()->append(upload_image_request.image().data());  
            }
            LOG(INFO) << "Adding Image for " << final_image.metadata().reconstruction();
            try {
                this->thread_pool.push([final_image](int id) mutable{
                        ReconstructionFetcher rf;
                        auto reconstruction = rf.Fetch(RemoteReconstructionContext(final_image.metadata().reconstruction()));
                        std::string image_uuid = reconstruction->StoreImage(final_image);
                        reconstruction->ComputeFeatures({final_image.metadata().path()});
                        reconstruction->AddImage(final_image.metadata().id());
                        reconstruction->ComputeMatches({final_image.metadata().path()});
                        
                });
                return Status::OK;
            } catch (const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
        }

        Status GetSparse(ServerContext* context, 
                            const GetSparseRequest* request, 
                            ServerWriter<GetSparseResponse>* writer){
            LOG(INFO) << "Getting Sparse for " << request->reconstruction_id();
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            SparseReconstruction sparse = reconstruction->GetSparse();
            SparsePointCloudData full_data = sparse.Data();
            for(int i = 0; i < full_data.data().size(); i+=CHUNK_SIZE){
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

        Status ReconstructionUploadImageBatch(ServerContext* context, 
                                                ServerReader<ReconstructionUploadImageBatchRequest>* reader, 
                                                ReconstructionUploadImageBatchResponse* response){
            int last_idx = 0;
            ReconstructionUploadImageBatchRequest request;
            ImageData final_image;
            ReconstructionFetcher rf;
            reader->Read(&request);
            try{
                auto reconstruction = rf.Fetch(RemoteReconstructionContext(request.data().metadata().reconstruction()));
                std::set<std::string> new_image_ids;
                final_image.CopyFrom(request.data());
                while(reader->Read(&request)){
                    if(request.idx() != last_idx){
                        new_image_ids.insert(reconstruction->StoreImage(final_image));
                        final_image.CopyFrom(request.data());
                        last_idx++;
                        continue;
                    }else{
                        final_image.mutable_data()->append(request.data().data());  
                    }
                }
                // insert last image
                new_image_ids.insert(reconstruction->StoreImage(final_image));

                reconstruction->ComputeFeatures(new_image_ids);
                std::for_each(new_image_ids.begin(), 
                            new_image_ids.end(), 
                            [reconstruction](const std::string& image_id){
                                reconstruction->AddImage(image_id);
                });
                reconstruction->ComputeMatches(new_image_ids);
                response->set_images_uploaded(last_idx);
                return Status::OK;
            } catch(const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
        }

        Status GetReconstructionConfig(ServerContext* context, 
                                       const GetReconstructionConfigRequest* request, 
                                       GetReconstructionConfigResponse* response){
             try{
                ReconstructionFetcher rf;
                auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
                ConfigurationContainerPtr config = reconstruction->GetConfig();
                response->set_config_json(config->jsonify());
                return Status::OK;
             } catch(const std::exception& e){
                 LOG(ERROR) << e.what();
                return Status::CANCELLED;
             }
        }

        Status GetAgentConfig(ServerContext* context, 
                              const GetAgentConfigRequest* request, 
                              GetAgentCOnfigResponse* response){
            try{
                ReconstructionFetcher rf;
                auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
                ConfigurationContainerPtr config = reconstruction->GetAgentConfig(request->agent_name());
                response->set_config_json(config->jsonify());
                return Status::OK;
             } catch(const std::exception& e){
                 LOG(ERROR) << e.what();
                return Status::CANCELLED;
             }
        }

        Status SetAgentConfigFields(ServerContext* context, 
                                const SetAgentConfigFieldsRequest* request, 
                                SetAgentConfigFieldsResponse* response){
        try {
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            reconstruction->SetAgentConfigFields(request->agent_name(), request->config_json());
            return Status::OK;
        } catch (const std::exception& e){
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
        }

        Status SetReconstructionConfigFields(ServerContext* context, 
                                            const SetReconstructionConfigFieldsRequest* request, 
                                            SetReconstructionConfigFieldsResponse* response){
            try {
            ReconstructionFetcher rf;
            auto reconstruction = rf.Fetch(RemoteReconstructionContext(request->reconstruction_id()));
            reconstruction->SetConfigFields(request->config_json());
            return Status::OK;
        } catch (const std::exception& e){
            LOG(ERROR) << e.what();
            return Status::CANCELLED;
        }
        }

    private:
        //std::unordered_map<std::string, Session*> _sessions;
        ctpl::thread_pool thread_pool;
};

int main(int argc, char* argv[]){
    google::InitGoogleLogging(argv[0]);
    CONFIG_LOAD(argv[1]);
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