#pragma once

#include "image_data_storage.h"
#include "filesystem_storer.h"

class FileSystemImageDataStorage : public ImageDataStorage, public FileSystemStorer
{
public:
    FileSystemImageDataStorage();
    int GetImage(const std::string &path, std::vector<char> &buf);
    std::string StoreImage(const ImageData &img, const std::string &path);
    void DeleteImage(const std::string &path);
};