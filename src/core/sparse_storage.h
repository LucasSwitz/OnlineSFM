#pragma once
#include <string>
#include <server.pb.h>

class SparseStorageAdapter {
    public:
        virtual SparsePointCloudData Get(const std::string& sparse_id) = 0;
        virtual SparsePointCloudMetaData GetMeta(const std::string& sparse_id) = 0;
        virtual SparsePointCloudMetaData GetMetaByReconstruction(const std::string& reconstruction_id) = 0;
        virtual void Store(const SparsePointCloudData& data) = 0;
        virtual void Delete(const std::string& sparse_id) = 0;
        virtual int DeleteByReconstruction(const std::string& reconstruction_id) = 0;
};