#pragma once
#include "camera_intrinsics_storage.h"
#include "sql_storage.h"

class SQLCameraIntrinsicsStorage : public CameraIntrinsicsStorage, public SQLStorage {
    public:
        SQLCameraIntrinsicsStorage(sql::Driver* driver, 
                                   std::shared_ptr<sql::Connection> con,
                                   const std::string& table);
        void Store(const CameraIntrinsics& camera_instrinsics);
        void Delete(const std::string& model); 
        bool Exists(const std::string& model);
        CameraIntrinsics Get(const std::string& model);
    private:
        std::string _table;
};