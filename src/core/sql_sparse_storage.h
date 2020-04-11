#pragma once
#include "sparse_storage.h"
#include "sql_storage.h"
#include "filesystem_storer.h"

class SQLSparseStorage : public SparseStorageAdapter, public SQLStorage, public FileSystemStorer {
    public:
        SQLSparseStorage(sql::Driver* driver, 
                         std::shared_ptr<sql::Connection> con,
                         const std::string& table);
        SparsePointCloudData Get(const std::string& sparse_id);
        SparsePointCloudMetaData GetMeta(const std::string& sparse_id);
        void Store(const SparsePointCloudMetaData& data);
        void Delete(const std::string& sparse_id);
        std::vector<SparsePointCloudMetaData>  GetAll(const std::string& reconstruction_id);
        int DeleteByReconstruction(const std::string& reconstruction_id);
        SparsePointCloudMetaData GetMetaByReconstruction(const std::string& reconstruction_id);
    private:
        std::string _table;
};