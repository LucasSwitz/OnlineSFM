#pragma once

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

#include "storage.grpc.pb.h"
#include "storage.pb.h"
#include "image_data_storage.h"
#include "spc_data_storage.h"
#include "obj_data_storage.h"

#include <thread>
#include <chrono>

#define IMAGE_CHUNK_SIZE 1024 * 10

class RemoteStorageAdapter : public ImageDataStorage,
                             public SPCDataStorage,
                             public OBJDataStorage
{
public:
    RemoteStorageAdapter(const std::string &address) : _client(grpc::CreateChannel(
                                                           address, grpc::InsecureChannelCredentials()))
    {
    }

    int GetImage(const std::string &path, std::vector<char> &buf)
    {
        return GetData(path, buf);
    }

    std::string StoreImage(const ImageData &img, const std::string &path)
    {
        StoreItemResponse resp;
        grpc::ClientContext ctx;
        std::unique_ptr<grpc::ClientWriter<StoreItemRequest>> stream(this->_client.Store(&ctx, &resp));
        for (int i = 0; i < img.data().size(); i += IMAGE_CHUNK_SIZE)
        {
            StoreItemRequest req;
            std::string chunk = img.data().substr(i, IMAGE_CHUNK_SIZE);
            req.set_data(chunk);
            req.set_path(path);
            try
            {
                if (!stream->Write(req))
                {
                    throw std::runtime_error("Failed to write chunk to remote storage");
                }
            }
            catch (const std::exception &e)
            {
                throw;
            }
        }
        stream->WritesDone();
        grpc::Status status = stream->Finish();
        return resp.path();
    }

    virtual void DeleteImage(const std::string &path)
    {
    }

    virtual int GetSPC(const std::string &path, std::vector<char> &buf)
    {
        return GetData(path, buf);
    }

    virtual std::string StoreSPC(const SparsePointCloudData &spc, const std::string &path)
    {
        StoreItemResponse resp;
        grpc::ClientContext ctx;
        std::unique_ptr<grpc::ClientWriter<StoreItemRequest>> stream(this->_client.Store(&ctx, &resp));
        for (int i = 0; i < spc.data().size(); i += IMAGE_CHUNK_SIZE)
        {
            StoreItemRequest req;
            std::string chunk = spc.data().substr(i, IMAGE_CHUNK_SIZE);
            req.set_data(chunk);
            req.set_path(path);
            try
            {
                if (!stream->Write(req))
                {
                    LOG(ERROR) << "Failed to write chunk";
                    break;
                }
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
            }
        }
        stream->WritesDone();
        grpc::Status status = stream->Finish();
        return resp.path();
    }

    virtual void DeleteSPC(const std::string &path)
    {
    }

    void StoreOBJ(const OBJData &obj,
                  std::string *obj_path,
                  std::string *mtl_path,
                  std::string *texture_path)
    {
        StoreItemResponse resp;
        grpc::ClientContext obj_ctx;
        std::unique_ptr<grpc::ClientWriter<StoreItemRequest>> stream_obj(this->_client.Store(&obj_ctx, &resp));
        for (int i = 0; i < obj.obj_data().size(); i += IMAGE_CHUNK_SIZE)
        {
            StoreItemRequest req;
            std::string chunk = obj.obj_data().substr(i, IMAGE_CHUNK_SIZE);
            req.set_data(chunk);
            req.set_path(obj.metadata().path());
            try
            {
                if (!stream_obj->Write(req))
                {
                    LOG(ERROR) << "Failed to write chunk";
                    break;
                }
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
            }
        }
        stream_obj->WritesDone();
        stream_obj->Finish();
        *obj_path = resp.path();

        grpc::ClientContext mtl_ctx;
        std::unique_ptr<grpc::ClientWriter<StoreItemRequest>> stream_mtl(this->_client.Store(&mtl_ctx, &resp));
        for (int i = 0; i < obj.mtl_data().size(); i += IMAGE_CHUNK_SIZE)
        {
            StoreItemRequest req;
            std::string chunk = obj.mtl_data().substr(i, IMAGE_CHUNK_SIZE);
            req.set_data(chunk);
            req.set_path(obj.metadata().mtl_path());
            try
            {
                if (!stream_mtl->Write(req))
                {
                    LOG(ERROR) << "Failed to write chunk";
                    break;
                }
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
            }
        }
        stream_mtl->WritesDone();
        stream_mtl->Finish();
        *mtl_path = resp.path();

        grpc::ClientContext texture_ctx;
        std::unique_ptr<grpc::ClientWriter<StoreItemRequest>> stream_texture(this->_client.Store(&texture_ctx, &resp));
        for (int i = 0; i < obj.texture_data().size(); i += IMAGE_CHUNK_SIZE)
        {
            StoreItemRequest req;
            std::string chunk = obj.texture_data().substr(i, IMAGE_CHUNK_SIZE);
            req.set_data(chunk);
            req.set_path(obj.metadata().texture_path());
            try
            {
                if (!stream_texture->Write(req))
                {
                    LOG(ERROR) << "Failed to write chunk";
                    break;
                }
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
            }
        }
        stream_texture->WritesDone();
        stream_texture->Finish();
        *texture_path = resp.path();
    }

    int GetOBJ(const std::string &path, std::vector<char> &buf)
    {
        return GetData(path, buf);
    }

    void DeleteOBJ(const std::string &path)
    {
    }

    int GetMTL(const std::string &path, std::vector<char> &buf)
    {
        return GetData(path, buf);
    }

    void DeleteMTL(const std::string &path)
    {
    }

    int GetTexture(const std::string &path, std::vector<char> &buf)
    {
        return GetData(path, buf);
    }

    void DeleteTexture(const std::string &path)
    {
    }

    int GetData(const std::string &path, std::vector<char> &buf)
    {
        grpc::ClientContext ctx;
        GetItemRequest req;
        req.set_path(path);
        std::unique_ptr<grpc::ClientReader<GetItemResponse>> reader(this->_client.Get(&ctx, req));
        GetItemResponse resp;
        while (reader->Read(&resp))
        {
            buf.insert(buf.end(), resp.data().begin(), resp.data().end());
        }
        return buf.size();
    }

private:
    Storage::Stub _client;
};