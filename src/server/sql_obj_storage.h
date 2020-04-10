#pragma once
#include "obj_storage.h"
#include "filesystem_storer.h"
#include "sql_storage.h"

class SQLOBJStorage : public OBJStorageAdapter, public SQLStorage, public FileSystemStorer<OBJMetaData>{
    public:
        SQLOBJStorage(const std::string& address, 
                     const std::string& user, 
                     const std::string& pass, 
                     const std::string& db,
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