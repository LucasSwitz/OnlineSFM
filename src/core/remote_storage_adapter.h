#pragma once

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

#include "storage.grpc.pb.h"
#include "storage.pb.h"
#include "image_data_storage.h"
#include "spc_data_storage.h"

#include <thread>
#include <chrono>

#define IMAGE_CHUNK_SIZE 1024*10

class RemoteStorageAdapter : public ImageDataStorage, 
                             public SPCDataStorage{
    public:
        RemoteStorageAdapter(const std::string& address) : _client(grpc::CreateChannel(
            address, grpc::InsecureChannelCredentials())){
        }

        int GetImage(const std::string& path, std::vector<char>& buf){
            std::vector<unsigned char> result;
            grpc::ClientContext ctx;
            GetItemRequest req;
            req.set_path(path);
            std::unique_ptr<grpc::ClientReader<GetItemResponse>> reader(this->_client.Get(&ctx, req));
            GetItemResponse resp;
            while(reader->Read(&resp)){
                buf.insert(buf.end(), resp.data().begin(), resp.data().end());
            }
            return buf.size();
        }

        std::string StoreImage(const ImageData& img, const std::string& path){
            StoreItemResponse resp;
            grpc::ClientContext ctx;
            std::unique_ptr<grpc::ClientWriter<StoreItemRequest>> stream(this->_client.Store(&ctx, &resp));
            for(int i = 0; i < img.data().size(); i += IMAGE_CHUNK_SIZE){
                StoreItemRequest req;
                std::string chunk = img.data().substr(i, IMAGE_CHUNK_SIZE);
                req.set_data(chunk);
                req.set_path(path);
                try{
                    if(!stream->Write(req)){
                        throw std::runtime_error("Failed to write chunk to remote storage");
                    }
                 }catch(const std::exception& e){
                    throw;
                }
            }
            stream->WritesDone();
            grpc::Status status = stream->Finish();
            return resp.path();
        }

        virtual void DeleteImage(const std::string& path){

        }

        virtual int GetSPC(const std::string& path, std::vector<char>& buf){
            std::vector<unsigned char> result;
            grpc::ClientContext ctx;
            GetItemRequest req;
            req.set_path(path);
            std::unique_ptr<grpc::ClientReader<GetItemResponse>> reader(this->_client.Get(&ctx, req));
            GetItemResponse resp;
            while(reader->Read(&resp)){
                buf.insert(buf.end(), resp.data().begin(), resp.data().end());
            }
            return buf.size();
        }

        virtual std::string StoreSPC(const SparsePointCloudData& spc, const std::string& path){
            StoreItemResponse resp;
            grpc::ClientContext ctx;
            std::unique_ptr<grpc::ClientWriter<StoreItemRequest>> stream(this->_client.Store(&ctx, &resp));
            for(int i = 0; i < spc.data().size(); i += IMAGE_CHUNK_SIZE){
                StoreItemRequest req;
                std::string chunk = spc.data().substr(i, IMAGE_CHUNK_SIZE);
                req.set_data(chunk);
                req.set_path(path);
                try{
                    if(!stream->Write(req)){
                        LOG(ERROR) << "Failed to write chunk";
                        break;
                    }
                 }catch(const std::exception& e){
                    LOG(ERROR) << e.what();
                }
            }
            stream->WritesDone();
            grpc::Status status = stream->Finish();
            return resp.path();
        }

        virtual void DeleteSPC(const std::string& path){

        }

    private:
        Storage::Stub _client;

};