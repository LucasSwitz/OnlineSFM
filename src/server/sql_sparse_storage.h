#pragma once
#include "sparse_storage.h"
#include "sql_storage.h"
#include "filesystem_storer.h"

class SQLSparseStorage : public SparseStorageAdapter, public SQLStorage, public FileSystemStorer<SparsePointCloudData> {
    public:
        SQLSparseStorage(const std::string& address, 
                                 const std::string& user, 
                                 const std::string& pass, 
                                 const std::string& db,
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