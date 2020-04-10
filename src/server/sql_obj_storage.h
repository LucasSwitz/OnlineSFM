#pragma once
#include "obj_storage.h"
#include "filesystem_storer.h"
#include "sql_storage.h"

class SQLOBJStorage : public OBJStorageAdapter, public SQLStorage, public FileSystemStorer<OBJMetaData>{
    public:
        SQLOBJStorage(sql::Driver* driver, 
                      std::shared_ptr<sql::Connection> con,
                      const std::string& table);
        OBJMetaData GetMeta(const std::string& obj_id); 
        OBJData Get(const std::string& obj_id);
        std::vector<OBJMetaData> GetAll(const std::string& reconstruction_id);
        int Store(const OBJMetaData& obj_data);
        int Delete(const std::string& obj_id);
        int DeleteByReconstruction(const std::string& reconstruction_id);        
    private:
        std::string _table;
};