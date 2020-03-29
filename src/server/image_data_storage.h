#pragma once

#include "server.pb.h"

class ImageDataStorage {
    public:
        virtual int Get(const std::string& path, std::vector<char>& buf) = 0;
        virtual std::string Store(const ImageData& img, const std::string& path) = 0;
        virtual void Delete(const std::string& path) = 0;
};