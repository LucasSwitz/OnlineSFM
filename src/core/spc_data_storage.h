#pragma once

#include "server.pb.h"

class SPCDataStorage {
    public:
        virtual int GetSPC(const std::string& path, std::vector<char>& buf) = 0;
        virtual std::string StoreSPC(const SparsePointCloudData& img, const std::string& path) = 0;
        virtual void DeleteSPC(const std::string& path) = 0;
};