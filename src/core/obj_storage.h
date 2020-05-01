#pragma once
#include <string>
#include <vector>
#include "server.pb.h"

class OBJStorageAdapter {
    public:
        virtual OBJData Get(const std::string& obj_id) = 0;
        virtual OBJMetaData GetMeta(const std::string& obj_id) = 0; 
        virtual int Store(const OBJData& obj_data) = 0;
        virtual int Delete(const std::string& obj_id) = 0;
        virtual int DeleteByReconstruction(const std::string& reconstruction_id) = 0;
        virtual std::vector<OBJMetaData> GetAll(const std::string& reconstruction_id) = 0;
        virtual ~OBJStorageAdapter(){};
};