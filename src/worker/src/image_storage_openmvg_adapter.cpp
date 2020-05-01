#include "image_storage_openmvg_adapter.h"

int ReadJpgMemory(unsigned char* data,
                  size_t size,
                  std::vector<unsigned char> * ptr,
                  int * w,
                  int * h,
                  int * depth) {
  jpeg_decompress_struct cinfo;
  my_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = &jpeg_error;

  if (setjmp(jerr.setjmp_buffer)) {
    std::cerr << "Error JPG: Failed to decompress.";
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_mem_src(&cinfo, data, size);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  int row_stride = cinfo.output_width * cinfo.output_components;

  *h = cinfo.output_height;
  *w = cinfo.output_width;
  *depth = cinfo.output_components;
  ptr->resize((*h)*(*w)*(*depth));

  unsigned char *ptrCpy = &(*ptr)[0];

  while (cinfo.output_scanline < cinfo.output_height) {
    JSAMPROW scanline[1] = { ptrCpy };
    jpeg_read_scanlines(&cinfo, scanline, 1);
    ptrCpy += row_stride;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  return 1;
}

bool Read_JPG_ImageHeaderMemory(unsigned char* data,
                                size_t size,
                                openMVG::image::ImageHeader * imgheader)
{
  bool bStatus = false;

  jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = &jpeg_error;

  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    return false;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_mem_src(&cinfo, data, size);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  if (imgheader)
  {
    imgheader->width = cinfo.output_width;
    imgheader->height = cinfo.output_height;
    bStatus = true;
  }

  jpeg_destroy_decompress(&cinfo);
  return bStatus;
}

int WriteJpgMemory(unsigned char** dest,
            unsigned long* size,
            const std::vector<unsigned char> & array,
            int w,
            int h,
            int depth,
            int quality) {
    if (quality < 0 || quality > 100)
        std::cerr << "Error: The quality parameter should be between 0 and 100";

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo); 
    jpeg_mem_dest(&cinfo, dest, size);

    cinfo.image_width = w;
    cinfo.image_height = h;
    cinfo.input_components = depth;

    if (cinfo.input_components==3) {
        cinfo.in_color_space = JCS_RGB;
    } else if (cinfo.input_components==1) {
        cinfo.in_color_space = JCS_GRAYSCALE;
    } else {
        std::cerr << "Error: Unsupported number of channels in file";
        jpeg_destroy_compress(&cinfo);
        return 0;
    }

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    const unsigned char *ptr = &array[0];
    int row_bytes = cinfo.image_width*cinfo.input_components;

    JSAMPLE *row = new JSAMPLE[row_bytes];

    while (cinfo.next_scanline < cinfo.image_height) {
        std::memcpy(&row[0], &ptr[0], row_bytes * sizeof(unsigned char));
        jpeg_write_scanlines(&cinfo, &row, 1);
        ptr += row_bytes;
    }

    delete [] row;

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    return 1;
}

ImageStorageOpenMVGAdapter::ImageStorageOpenMVGAdapter(std::shared_ptr<ImageStorageAdapter> storage) : _image_storage(storage){
            

}

bool ImageStorageOpenMVGAdapter::ReadImageHeader(const std::string& image_id,  openMVG::image::ImageHeader * hdr ){
    auto meta = this->_image_storage->GetMeta(image_id);
    ImageData image_data = this->_image_storage->Get(image_id);
    std::vector<unsigned char> image_bytes(image_data.data().begin(), image_data.data().end());
    return Read_JPG_ImageHeaderMemory(image_bytes.data(), image_bytes.size(), hdr);
}