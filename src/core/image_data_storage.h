#pragma once

#include "server.pb.h"

class ImageDataStorage
{
public:
    virtual int GetImage(const std::string &path, std::vector<char> &buf) = 0;
    virtual std::string StoreImage(const ImageData &img, const std::string &path) = 0;
    virtual void DeleteImage(const std::string &path) = 0;
};