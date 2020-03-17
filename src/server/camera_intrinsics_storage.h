#pragma once

#include "server.pb.h"

class CameraIntrinsicsStorage {
    public:
        virtual void Store(const CameraIntrinsics& camera_instrinsics) = 0;
        virtual void Delete(const std::string& model) = 0; 
        virtual CameraIntrinsics Get(const std::string& model) = 0;
};      