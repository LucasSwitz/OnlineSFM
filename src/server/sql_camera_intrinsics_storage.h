#pragma once
#include "camera_intrinsics_storage.h"
#include "sql_storage.h"

class SQLCameraIntrinsicsStorage : public CameraIntrinsicsStorage, public SQLStorage {
    public:
        SQLCameraIntrinsicsStorage(const std::string& address, 
                     const std::string& user, 
                     const std::string& pass, 
                     const std::string& db,
                     const std::string& table);
        void Store(const CameraIntrinsics& camera_instrinsics);
        void Delete(const std::string& model); 
        bool Exists(const std::string& model);
        CameraIntrinsics Get(const std::string& model);
    private:
        std::string _table;
};