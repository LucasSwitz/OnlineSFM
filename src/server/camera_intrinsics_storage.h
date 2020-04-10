#pragma once

#include "server.pb.h"
#include "openMVG/stl/split.hpp"

class CameraIntrinsicsStorage {
    public:
        virtual void Store(const CameraIntrinsics& camera_instrinsics) = 0;
        virtual void Delete(const std::string& model) = 0; 
        virtual CameraIntrinsics Get(const std::string& model) = 0;
        virtual bool Exists(const std::string& model)  = 0;
        std::string ExtractMaker(const std::string& model){
            return model.substr(0, model.find(' '));
        }
        std::string ExtractNumericModel(const std::string& model){
            std::vector<std::string> vec_db_model;
            stl::split(model, ' ', vec_db_model);
            for (const std::string & db_sub_model : vec_db_model)
            {
                if (std::find_if(db_sub_model.begin(), db_sub_model.end(), isdigit) != db_sub_model.end() )
                {
                    return db_sub_model;
                }
            }
            return "";
        }
    virtual ~CameraIntrinsicsStorage(){};
};      