#pragma once

#include <cmath>
#include <cstring>
#include <iostream>

#include "image_storage.h"
#include "remote_image_storage_adapter.h"

#include "openMVG/image/image_container.hpp"
#include "openMVG/image/image_converter.hpp"
#include "openMVG/image/image_io.hpp"

#include "image_data_storage.h"
#include <vector>

extern "C" {
  #include "png.h"
  #include "tiffio.h"
  #include "jpeglib.h"
}

class ImageStorageOpenMVGAdapter {
    public:
        ImageStorageOpenMVGAdapter(std::shared_ptr<ImageStorageAdapter> storage);
        bool ReadImageHeader(const std::string& image_id,  openMVG::image::ImageHeader * hdr );
        int ReadImage(const std::string& image_id, openMVG::image::Image<unsigned char> * im);

    private:
        std::shared_ptr<ImageStorageAdapter> _image_storage;
        
};