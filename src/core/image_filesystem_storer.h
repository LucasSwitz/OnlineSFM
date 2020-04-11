#pragma once

#include "image_data_storage.h"
#include "filesystem_storer.h"

class FileSystemImageDataStorage : public ImageDataStorage, public FileSystemStorer {
    public:
        FileSystemImageDataStorage();
        int Get(const std::string& path, std::vector<char>& buf);
        std::string Store(const ImageData& img, const std::string& path);
        void Delete(const std::string& path);
};