#pragma once
#include <string>
#include <vector>
#include "server.pb.h"

class ImageStorageAdapter{
    public:
        virtual ImageData Get(const std::string& image_id) = 0;
        virtual ImageMetaData GetMeta(const std::string& image_id) = 0; 
        virtual std::vector<ImageMetaData> GetAll(std::string reconstruction_id) = 0;
        virtual int Store(const ImageData& image_data) = 0;
        virtual int Delete(const std::string& image_id) = 0;
        virtual int DeleteByReconstruction(const std::string& reconstruction_id) = 0;
};