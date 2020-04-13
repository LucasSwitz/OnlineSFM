#include "image_storage_openmvg_adapter.h"

struct my_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

METHODDEF(void)
jpeg_error (j_common_ptr cinfo)
{
  my_error_mgr *myerr = (my_error_mgr*) (cinfo->err);
  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

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

ImageStorageOpenMVGAdapter::ImageStorageOpenMVGAdapter(std::shared_ptr<ImageStorageAdapter> storage) : _image_storage(storage){
            

}

bool ImageStorageOpenMVGAdapter::ReadImageHeader(const std::string& image_id,  openMVG::image::ImageHeader * hdr ){
    auto meta = this->_image_storage->GetMeta(image_id);
    ImageData image_data = this->_image_storage->Get(image_id);
    std::vector<unsigned char> image_bytes(image_data.data().begin(), image_data.data().end());
    return Read_JPG_ImageHeaderMemory(image_bytes.data(), image_bytes.size(), hdr);
}

int ImageStorageOpenMVGAdapter::ReadImage(const std::string& image_id, openMVG::image::Image<unsigned char> * im){
    auto meta = this->_image_storage->GetMeta(image_id);
    ImageData image_data = this->_image_storage->Get(image_id);
    std::vector<unsigned char> image_bytes(image_data.data().begin(), image_data.data().end());
    std::vector<unsigned char> ptr;
    int w, h, depth;
    const int res = ReadJpgMemory(image_bytes.data(), image_bytes.size(), &ptr, &w, &h, &depth);
    if ( res == 1 && depth == 1 )
    {
        //convert raw array to Image
        ( *im ) = Eigen::Map<openMVG::image::Image<unsigned char>::Base>( &ptr[0], h, w );
    }
    else if ( res == 1 && depth == 3 )
    {
        //-- Must convert RGB to gray
        openMVG::image::RGBColor * ptrCol = reinterpret_cast<openMVG::image::RGBColor*>( &ptr[0] );
        openMVG::image::Image<openMVG::image::RGBColor> rgbColIm;
        rgbColIm = Eigen::Map<openMVG::image::Image<openMVG::image::RGBColor>::Base>( ptrCol, h, w );
        //convert RGB to gray
        ConvertPixelType( rgbColIm, im );
    }
    else if ( res == 1 && depth == 4 )
    {
        //-- Must convert RGBA to gray
        openMVG::image::RGBAColor * ptrCol = reinterpret_cast< openMVG::image::RGBAColor*>( &ptr[0] );
        openMVG::image::Image<openMVG::image::RGBAColor> rgbaColIm;
        rgbaColIm = Eigen::Map<openMVG::image::Image<openMVG::image::RGBAColor>::Base>( ptrCol, h, w );
        //convert RGBA to gray
        ConvertPixelType( rgbaColIm, im );
    }
    else if ( depth != 1 )
    {
        return 0;
    }
    return res;
}