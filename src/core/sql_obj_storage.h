#pragma once
#include "obj_storage.h"
#include "filesystem_storer.h"
#include "sql_storage.h"
#include "obj_data_storage.h"

class SQLOBJStorage : public OBJStorageAdapter, public SQLStorage {
    public:
        SQLOBJStorage(
                      const std::string& table,
                      std::shared_ptr<OBJDataStorage> data_storage);
        OBJMetaData GetMeta(const std::string& obj_id); 
        OBJData Get(const std::string& obj_id);
        std::vector<OBJMetaData> GetAll(const std::string& reconstruction_id);
        int Store(const OBJData& obj_data);
        int Delete(const std::string& obj_id);
        int DeleteByReconstruction(const std::string& reconstruction_id);        
    private:
        std::string _table;
        std::shared_ptr<OBJDataStorage> _data_storage;
};