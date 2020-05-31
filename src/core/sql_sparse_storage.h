#pragma once
#include "sparse_storage.h"
#include "sql_storage.h"
#include "filesystem_storer.h"
#include "spc_data_storage.h"

class SQLSparseStorage : public SparseStorageAdapter, public SQLStorage
{
public:
    SQLSparseStorage(
        const std::string &table,
        std::shared_ptr<SPCDataStorage> _data_storage);
    SparsePointCloudData Get(const std::string &sparse_id);
    SparsePointCloudMetaData GetMeta(const std::string &sparse_id);
    void Store(const SparsePointCloudData &data);
    void Delete(const std::string &sparse_id);
    std::vector<SparsePointCloudMetaData> GetAll(const std::string &reconstruction_id);
    int DeleteByReconstruction(const std::string &reconstruction_id);
    SparsePointCloudMetaData GetMetaByReconstruction(const std::string &reconstruction_id);

private:
    std::string _table;
    std::shared_ptr<SPCDataStorage> _data_storage;
};