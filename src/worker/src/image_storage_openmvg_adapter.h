#pragma once

#include <cmath>
#include <cstring>
#include <iostream>

#include "image_storage.h"

#include "openMVG/image/image_container.hpp"
#include "openMVG/image/image_converter.hpp"
#include "openMVG/image/image_io.hpp"
#include "openMVG/image/image_resampling.hpp"

#include "image_data_storage.h"
#include <vector>

extern "C"
{
#include "png.h"
#include "tiffio.h"
#include "jpeglib.h"
}

struct my_error_mgr
{
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

METHODDEF(void)
jpeg_error(j_common_ptr cinfo)
{
    my_error_mgr *myerr = (my_error_mgr *)(cinfo->err);
    (*cinfo->err->output_message)(cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

int ReadJpgMemory(unsigned char *data,
                  size_t size,
                  std::vector<unsigned char> *ptr,
                  int *w,
                  int *h,
                  int *depth);

bool Read_JPG_ImageHeaderMemory(unsigned char *data,
                                size_t size,
                                openMVG::image::ImageHeader *imgheader);

int WriteJpgMemory(unsigned char **dest,
                   unsigned long *size,
                   const std::vector<unsigned char> &array,
                   int w,
                   int h,
                   int depth,
                   int quality);

class ImageStorageOpenMVGAdapter
{
public:
    ImageStorageOpenMVGAdapter(std::shared_ptr<ImageStorageAdapter> storage);
    bool ReadImageHeader(const std::string &image_id, openMVG::image::ImageHeader *hdr);
    int ReadImage(const std::string &image_id, openMVG::image::Image<unsigned char> *im, int scale_down = 0)
    {
        auto meta = this->_image_storage->GetMeta(image_id);
        ImageData image_data = this->_image_storage->Get(image_id);
        std::vector<unsigned char> image_bytes(image_data.data().begin(), image_data.data().end());
        std::vector<unsigned char> ptr;
        int w, h, depth;
        const int res = ReadJpgMemory(image_bytes.data(), image_bytes.size(), &ptr, &w, &h, &depth);
        if (res == 1 && depth == 1)
        {
            //convert raw array to Image
            (*im) = Eigen::Map<openMVG::image::Image<unsigned char>::Base>(&ptr[0], h, w);
        }
        else if (res == 1 && depth == 3)
        {
            //-- Must convert RGB to gray
            openMVG::image::RGBColor *ptrCol = reinterpret_cast<openMVG::image::RGBColor *>(&ptr[0]);
            openMVG::image::Image<openMVG::image::RGBColor> rgbColIm;
            rgbColIm = Eigen::Map<openMVG::image::Image<openMVG::image::RGBColor>::Base>(ptrCol, h, w);
            //convert RGB to gray
            ConvertPixelType(rgbColIm, im);
        }
        else if (res == 1 && depth == 4)
        {
            //-- Must convert RGBA to gray
            openMVG::image::RGBAColor *ptrCol = reinterpret_cast<openMVG::image::RGBAColor *>(&ptr[0]);
            openMVG::image::Image<openMVG::image::RGBAColor> rgbaColIm;
            rgbaColIm = Eigen::Map<openMVG::image::Image<openMVG::image::RGBAColor>::Base>(ptrCol, h, w);
            //convert RGBA to gray
            ConvertPixelType(rgbaColIm, im);
        }
        else if (depth != 1)
        {
            return 0;
        }

        if (scale_down)
        {
            std::vector<std::pair<float, float>> sampling_grid;
            double scaled_width = im->Width() / scale_down;
            double scaled_height = im->Height() / scale_down;
            sampling_grid.reserve(scaled_height * scaled_width);
            for (int i = 0; i < scaled_height; ++i)
            {
                for (int j = 0; j < scaled_width; ++j)
                {
                    sampling_grid.emplace_back(i * scale_down, j * scale_down);
                }
            }
            openMVG::image::Sampler2d<openMVG::image::SamplerLinear> sampler;
            openMVG::image::Image<unsigned char> imageOut;
            openMVG::image::GenericRessample(*im, sampling_grid, scaled_width, scaled_height, sampler, imageOut);
            im->resize(scaled_width, scaled_height);
            *im = imageOut;
        }
        return res;
    }
    int ReadImage(const std::string &image_id, openMVG::image::Image<openMVG::image::RGBColor> *im)
    {
        auto meta = this->_image_storage->GetMeta(image_id);
        ImageData image_data = this->_image_storage->Get(image_id);
        std::vector<unsigned char> image_bytes(image_data.data().begin(), image_data.data().end());
        std::vector<unsigned char> ptr;
        int w, h, depth;
        const int res = ReadJpgMemory(image_bytes.data(), image_bytes.size(), &ptr, &w, &h, &depth);
        if (res == 1 && depth == 3)
        {
            openMVG::image::RGBColor *ptrCol = reinterpret_cast<openMVG::image::RGBColor *>(&ptr[0]);
            //convert raw array to Image
            (*im) = Eigen::Map<openMVG::image::Image<openMVG::image::RGBColor>::Base>(ptrCol, h, w);
        }
        else if (res == 1 && depth == 4)
        {
            //-- Must convert RGBA to RGB
            openMVG::image::RGBAColor *ptrCol = reinterpret_cast<openMVG::image::RGBAColor *>(&ptr[0]);
            openMVG::image::Image<openMVG::image::RGBAColor> rgbaColIm;
            rgbaColIm = Eigen::Map<openMVG::image::Image<openMVG::image::RGBAColor>::Base>(ptrCol, h, w);
            //convert RGBA to RGB
            ConvertPixelType(rgbaColIm, im);
        }
        else
        {
            return 0;
        }
        return res;
    }

    template <typename T>
    int WriteImageToMemory(std::string *out, const openMVG::image::Image<T> &im)
    {
        const unsigned char *ptr = (unsigned char *)(im.GetMat().data());
        const int depth = sizeof(T) / sizeof(unsigned char);
        std::vector<unsigned char> array(ptr, ptr + im.Width() * im.Height() * depth);
        const int w = im.Width(), h = im.Height();
        unsigned char *data_raw = nullptr;
        unsigned long size;
        WriteJpgMemory(&data_raw, &size, array, w, h, depth, 90);
        *out = std::string((char *)data_raw, size);
        free(data_raw);
        return size;
    }

private:
    std::shared_ptr<ImageStorageAdapter> _image_storage;
};