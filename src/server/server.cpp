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
#include "session.h"

#include "CTPL/ctpl.h"

#include "config.h"
#include "redis_sfm_backlog.h"

/*
    TODO: 
*/

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;
using grpc::ServerWriter;

#define OBJ_CHUNK_SIZE 1024*10

class ReconstructionServer : public ReconstructionService::Service {
    public:
        ReconstructionServer(uint64_t thread_count) : thread_pool(thread_count){

        }

        Status NewReconstruction(ServerContext* context, const NewReconstructionRequest* request, NewReconstructionResponse* response){
            ReconstructionData new_reconstruction;
            std::string uuid = GetUUID();
            std::string reconstruction_root = CONFIG_GET_STRING("storage.root") + "/" + uuid;
            std::string images_dir = reconstruction_root + "/images";
            std::string features_dir = reconstruction_root + "/features";
            std::string sfm_dir = reconstruction_root + "/SFM";
            std::string mvs_dir = reconstruction_root + "/MVS";
            std::string matches_dir = reconstruction_root + "/matches";
            
            mkdir(reconstruction_root.c_str(), 0777);
            mkdir(images_dir.c_str(), 0777);
            mkdir(features_dir.c_str(), 0777);
            mkdir(sfm_dir.c_str(), 0777);
            mkdir(mvs_dir.c_str(), 0777);
            mkdir(matches_dir.c_str(), 0777);

            new_reconstruction.set_id(uuid);
            new_reconstruction.set_images_path(images_dir);
            new_reconstruction.set_features_path(features_dir);
            new_reconstruction.set_sfm_path(sfm_dir);
            new_reconstruction.set_mvs_path(mvs_dir);
            new_reconstruction.set_matches_path(matches_dir);
            response->set_reconstruction_id(new_reconstruction.id());
            
            try {
                ReconstructionFetcher rf;
                rf.Store(new_reconstruction);
            } catch(std::exception& e){
                LOG(ERROR) << e.what();
            }

            LOG(INFO) << "Made new reconstruction " << uuid;
            return Status::OK;
        }

        Status StoreImage(ServerContext* context, 
                          ServerReader<StoreImageRequest>* stream, 
                          StoreImageResponse* response){
            StoreImageRequest request;
            stream->Read(&request);
            ImageData final_image;
            final_image.CopyFrom(request.image());
            while(stream->Read(&request)){
                final_image.mutable_data()->append(request.image().data());  
            }
            
            try{
                ReconstructionFetcher rf;
                Reconstruction* reconstruction = rf.Fetch(final_image.metadata().reconstruction());
                LOG(INFO) << "Adding Image for " << final_image.metadata().reconstruction();
                reconstruction->StoreImage(final_image);
                delete reconstruction;
            } catch(std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
                
            return Status::OK;
        }

        Status SparseReconstruct(ServerContext* context, 
                                 const SparseReconstructRequest* request, 
                                 SparseReconstructResponse* response){
            ReconstructionFetcher rf;
            try {
                Reconstruction* reconstruction = rf.Fetch(request->reconstruction_id());
                response->set_success(reconstruction->SparseReconstruct());
                delete reconstruction;
            } catch (const std::exception& e){
                LOG(ERROR) << e.what();
            }
            return Status::OK;
        }

        Status MVS(ServerContext* context, 
                   const MVSRequest* request, 
                   MVSResponse* response){
            ReconstructionFetcher rf;
            try {
                Reconstruction* reconstruction = rf.Fetch(request->reconstruction_id());
                response->set_success(reconstruction->MVS(true));
                delete reconstruction;
            } catch (const std::exception& e){
                LOG(ERROR) << e.what();
            }
        }

        Status GetOBJ(ServerContext* context, 
                      const GetOBJRequest* request, 
                      ServerWriter<GetOBJResponse>* writer){
            LOG(INFO) << "Getting OBJ for " << request->reconstruction_id();
            ReconstructionFetcher rf;
            Reconstruction* reconstruction = nullptr;
            OBJ obj;
            try {
                reconstruction = rf.Fetch(request->reconstruction_id());
                obj = reconstruction->GetOBJ();
       
            } catch(const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }

            OBJData full_data = obj.Data();

            for(int i = 0; i < full_data.obj_data().size(); i+=OBJ_CHUNK_SIZE){
                OBJData data_chunked;
                GetOBJResponse resp;
                data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
                std::string chunked_bytes = full_data.obj_data().substr(i, OBJ_CHUNK_SIZE);
                data_chunked.set_obj_data(chunked_bytes);
                resp.mutable_obj()->CopyFrom(data_chunked);
                resp.set_success(true);
                writer->Write(resp);
            }
            for(int i = 0; i < full_data.texture_data().size(); i+=OBJ_CHUNK_SIZE){
                OBJData data_chunked;
                GetOBJResponse resp;
                data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
                std::string chunked_bytes = full_data.texture_data().substr(i, OBJ_CHUNK_SIZE);
                data_chunked.set_texture_data(chunked_bytes);
                resp.mutable_obj()->CopyFrom(data_chunked);
                resp.set_success(true);
                writer->Write(resp);
            }
            for(int i = 0; i < full_data.mtl_data().size(); i+=OBJ_CHUNK_SIZE){
                OBJData data_chunked;
                GetOBJResponse resp;
                data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
                std::string chunked_bytes = full_data.mtl_data().substr(i, OBJ_CHUNK_SIZE);
                data_chunked.set_mtl_data(chunked_bytes);
                resp.mutable_obj()->CopyFrom(data_chunked);
                resp.set_success(true);
                writer->Write(resp);
            }

            delete reconstruction;
            return Status::OK;
        };

        Status DeleteReconstruction(ServerContext* context, 
                                    const DeleteReconstructionRequest* request, 
                                    DeleteReconstructionResponse* response){
            try{
                ReconstructionFetcher fetcher;
                Reconstruction* reconstruction = fetcher.Fetch(request->id());
                reconstruction->Delete();
                response->set_id(request->id());
                response->set_success(true);
                delete reconstruction;
                return Status::OK;
            } catch (const std::exception& e){
                LOG(ERROR) << e.what();
                return Status::CANCELLED;
            }
        };

        Status StartSession(ServerContext* context, 
                            const StartSessionRequest* request, 
                            StartSessionResponse* response){
            LOG(INFO) << "Creating new session for reconstruction " << request->reconstruction_id();
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
            return Status::OK;
        }

        Status StopSession(ServerContext* context, 
                           const StopSessionRequest* request, 
                           StopSessionResponse* response){
            Session* s = this->_sessions[request->session_id()];
            response->set_session_id(request->session_id());
            LOG(INFO) << "Stopping session " << request->session_id();
            s->Stop();
            delete this->_sessions[request->session_id()];
            return Status::OK;
        }


        Status SessionAddImage(ServerContext* context, 
                               ServerReader<SessionAddImageRequest>* stream, 
                               SessionAddImageResponse* response){
            SessionAddImageRequest request;
            stream->Read(&request);
            ImageData final_image;
            final_image.CopyFrom(request.upload_image().image());
            while(stream->Read(&request)){
                StoreImageRequest upload_image_request = request.upload_image();
                final_image.mutable_data()->append(upload_image_request.image().data());  
            }
            LOG(INFO) << "Adding Image for " << final_image.metadata().reconstruction();
            try {
                this->thread_pool.push([final_image](int id) mutable{
                        ReconstructionFetcher rf;
                        Reconstruction* reconstruction = rf.Fetch(final_image.metadata().reconstruction());
                        std::string image_uuid = reconstruction->StoreImage(final_image);
                        reconstruction->ComputeFeatures({final_image.metadata().path()});
                        reconstruction->AddImage(final_image.metadata().id());
                        reconstruction->ComputeMatches({final_image.metadata().path()});
                        delete reconstruction;
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
            Reconstruction* reconstruction = rf.Fetch(request->reconstruction_id());
            SparseReconstruction sparse = reconstruction->GetSparse();
            SparsePointCloudData full_data = sparse.Data();
            for(int i = 0; i < full_data.data().size(); i+=OBJ_CHUNK_SIZE){
                SparsePointCloudData data_chunked;
                GetSparseResponse resp;
                data_chunked.mutable_metadata()->CopyFrom(full_data.metadata());
                std::string chunked_bytes = full_data.data().substr(i, OBJ_CHUNK_SIZE);
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
            Reconstruction* reconstruction = rf.Fetch(request.data().metadata().reconstruction());
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
            reconstruction->ComputeFeatures(new_image_ids);
            std::for_each(new_image_ids.begin(), 
                          new_image_ids.end(), 
                          [reconstruction](const std::string& image_id){
                            reconstruction->AddImage(image_id);
            });
            reconstruction->ComputeMatches(new_image_ids);

            delete reconstruction;
            return Status::OK;
       }

    private:
        std::unordered_map<std::string, Session*> _sessions;
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
    std::unique_ptr<Server> server(builder.BuildAndStart());
    LOG(INFO) << "Server waiting for connections...";
    server->Wait();

    return 1;
}