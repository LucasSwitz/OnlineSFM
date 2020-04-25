

#include "openmvg_reconstruction_agent.h"

#include "openMVG/cameras/Camera_Common.hpp"
#include "openMVG/features/feature.hpp"
#include "openMVG/features/svg_features.hpp"
#include "openMVG/geometry/frustum.hpp"
#include "openMVG/matching/indMatch.hpp"
#include "openMVG/matching/indMatch_utils.hpp"
#include "openMVG/matching_image_collection/Pair_Builder.hpp"
#include "openMVG/sfm/pipelines/structure_from_known_poses/structure_estimator.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider_cache.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_utils.hpp"
#include "openMVG/sfm/sfm_data_BA.hpp"
#include "openMVG/sfm/sfm_data_BA_ceres.hpp"
#include "openMVG/sfm/sfm_data_filters.hpp"
#include "openMVG/sfm/sfm_data_filters_frustum.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/sfm/sfm_report.hpp"
#include "openMVG/sfm/sfm_data_triangulation.hpp"
#include "openMVG/tracks/tracks.hpp"
#include "openMVG/system/timer.hpp"
#include "openMVG/types.hpp"

#include "openMVG/cameras/Cameras_Common_command_line_helper.hpp"
#include "openMVG/sfm/pipelines/sequential/sequential_SfM2.hpp"
#include "openMVG/sfm/pipelines/sequential/SfmSceneInitializerMaxPair.hpp"
#include "openMVG/sfm/pipelines/sequential/SfmSceneInitializerStellar.hpp"
#include "openMVG/sfm/pipelines/sfm_features_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_matches_provider.hpp"
#include "openMVG/sfm/sfm_view.hpp"

#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <glog/logging.h>


#include "openMVG/graph/graph.hpp"
#include "openMVG/graph/graph_stats.hpp"
#include "openMVG/features/akaze/image_describer_akaze.hpp"
#include "openMVG/features/descriptor.hpp"
#include "openMVG/matching_image_collection/Matcher_Regions.hpp"
#include "openMVG/matching_image_collection/Cascade_Hashing_Matcher_Regions.hpp"
#include "openMVG/matching_image_collection/GeometricFilter.hpp"
#include "openMVG/matching_image_collection/F_ACRobust.hpp"
#include "openMVG/matching_image_collection/E_ACRobust.hpp"
#include "openMVG/matching_image_collection/E_ACRobust_Angular.hpp"
#include "openMVG/matching_image_collection/Eo_Robust.hpp"
#include "openMVG/matching_image_collection/H_ACRobust.hpp"
#include "openMVG/matching/pairwiseAdjacencyDisplay.hpp"
#include "openMVG/stl/stl.hpp"

#include <cereal/archives/json.hpp>

#include "openMVG/features/akaze/image_describer_akaze_io.hpp"

#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer_io.hpp"
#include "openMVG/image/image_io.hpp"
#include "openMVG/features/regions_factory_io.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/system/timer.hpp"

#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include "openMVG_dependencies/nonFree/sift/SIFT_describer_io.hpp"

#include <cereal/details/helpers.hpp>

#include <atomic>
#include <cstdlib>

#include "openMVG/cameras/cameras.hpp"
#include "openMVG/exif/exif_IO_EasyExif.hpp"
#include "openMVG/geodesy/geodesy.hpp"
#include "openMVG/image/image_io.hpp"
#include "openMVG/numeric/eigen_alias_definition.hpp"
#include "openmvg_ext.h"

#include "index_helpers.h"

#include <fstream>
#include <memory>
#include <utility>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "config.h"
#include "util.h"

#include "sparse_storage.h"

#include "exceptions.h"

using namespace openMVG;
using namespace openMVG::cameras;
using namespace openMVG::sfm;
using namespace openMVG::matching;
using namespace openMVG::features;
using namespace openMVG::matching_image_collection;
using namespace openMVG::exif;
using namespace openMVG::geodesy;
using namespace openMVG::image;
using namespace std;

enum EGeometricModel
{
  FUNDAMENTAL_MATRIX = 0,
  ESSENTIAL_MATRIX   = 1,
  HOMOGRAPHY_MATRIX  = 2,
  ESSENTIAL_MATRIX_ANGULAR = 3,
  ESSENTIAL_MATRIX_ORTHO = 4
};

features::EDESCRIBER_PRESET stringToEnum(const std::string & sPreset)
{
  features::EDESCRIBER_PRESET preset;
  if (sPreset == "NORMAL")
    preset = features::NORMAL_PRESET;
  else
  if (sPreset == "HIGH")
    preset = features::HIGH_PRESET;
  else
  if (sPreset == "ULTRA")
    preset = features::ULTRA_PRESET;
  else
    preset = features::EDESCRIBER_PRESET(-1);
  return preset;
};

std::pair<bool, openMVG::Vec3> checkGPS
(
  const std::string & filename,
  const int & GPS_to_XYZ_method = 0
)
{
  std::pair<bool, openMVG::Vec3> val(false, openMVG::Vec3::Zero());
  std::unique_ptr<Exif_IO> exifReader(new Exif_IO_EasyExif);
  if (exifReader)
  {
    // Try to parse EXIF metada & check existence of EXIF data
    if ( exifReader->open( filename ) && exifReader->doesHaveExifInfo() )
    {
      // Check existence of GPS coordinates
      double latitude, longitude, altitude;
      if ( exifReader->GPSLatitude( &latitude ) &&
           exifReader->GPSLongitude( &longitude ) &&
           exifReader->GPSAltitude( &altitude ) )
      {
        // Add ECEF or UTM XYZ position to the GPS position array
        val.first = true;
        switch (GPS_to_XYZ_method)
        {
          case 1:
            val.second = lla_to_utm( latitude, longitude, altitude );
            break;
          case 0:
          default:
            val.second = lla_to_ecef( latitude, longitude, altitude );
            break;
        }
      }
    }
  }
  return val;
}

OpenMVGReconstructionAgent::OpenMVGReconstructionAgent(const std::string& reconstruction_id, 
                                                       std::shared_ptr<CameraIntrinsicsStorage> intrinsics_storage,
                                                       std::shared_ptr<OpenMVGStorageAdapter> openmvg_storage,
                                                       std::shared_ptr<ConfigurationAdapter> configuration_adapter,
                                                       std::shared_ptr<RegionsStorage<SIFT_Anatomy_Image_describer::Regions_type>> regions_storage,
                                                       std::shared_ptr<ImageStorageAdapter> image_storage,
                                                       std::shared_ptr<SparseStorageAdapter> sparse_storage):
                                                                                  _reconstruction_id(reconstruction_id), 
                                                                                  _intrinsics_storage(intrinsics_storage),
                                                                                  _openmvg_storage(openmvg_storage),
                                                                                  _configuration_adapter(configuration_adapter),
                                                                                  _regions_storage(regions_storage),
                                                                                  _image_storage(image_storage),
                                                                                  _openmvg_images_storage(image_storage),
                                                                                  _sparse_storage(sparse_storage){
  this->_sfm_data = this->_openmvg_storage->GetSFMData(this->_reconstruction_id, ESfM_Data::ALL);
}

bool OpenMVGReconstructionAgent::AddImage(const std::string& image_id){
  std::string image_path = this->_image_storage->GetMeta(image_id).path();
  int i_User_camera_model = PINHOLE_CAMERA_RADIAL3;
  std::shared_ptr<View> v;

  // Expected properties for each image
  double width = -1, height = -1, focal = -1, ppx = -1,  ppy = -1;
  const EINTRINSIC e_User_camera_model = EINTRINSIC(i_User_camera_model);

  // Read meta data to fill camera parameter (w,h,focal,ppx,ppy) fields.
  width = height = ppx = ppy = focal = -1.0;

  const std::string sImFilenamePart = stlplus::filename_part(image_path);
  // Test if the image format is supported:
  if (openMVG::image::GetFormat(image_path.c_str()) == openMVG::image::Unknown)
  {
      std::cout
          << sImFilenamePart << ": Unkown image file format." << "\n";
      return false; // image cannot be opened
  }

  if (sImFilenamePart.find("mask.png") != std::string::npos
      || sImFilenamePart.find("_mask.png") != std::string::npos)
  {
      std::cout
          << sImFilenamePart << " is a mask image" << "\n";
      return false;
  }

  ImageHeader imgHeader;
  if (!this->_openmvg_images_storage.ReadImageHeader(image_id, &imgHeader)){
      std::cout
          << sImFilenamePart << " Failed to read header" << "\n";
      return false; // image cannot be read
  }

  width = imgHeader.width;
  height = imgHeader.height;
  ppx = width / 2.0;
  ppy = height / 2.0;
  CameraIntrinsics intrinsics;

  // If not manually provided or wrongly provided
  if (focal == -1)
  {
    std::unique_ptr<Exif_IO> exifReader(new Exif_IO_EasyExif);
    exifReader->open(image_path);

    const bool bHaveValidExifMetadata =
    exifReader->doesHaveExifInfo()
    && !exifReader->getModel().empty();

    if (bHaveValidExifMetadata) // If image contains meta data
    {
      const std::string sCamModel = exifReader->getModel();
      // Handle case where focal length is equal to 0
      if (exifReader->getFocal() == 0.0f)
      {
          std::cout
          << stlplus::basename_part(image_path) 
          << ": Focal length is missing." << "\n";
          focal = -1.0;
      }
      else
      {
          intrinsics = this->_intrinsics_storage->Get(sCamModel);
          if (!intrinsics.model().empty())
          {
          // The camera model was found in the database so we can compute it's approximated focal length
            const double ccdw = intrinsics.sensor_size();
            focal = std::max ( width, height ) * exifReader->getFocal() / ccdw;
          }
          else
          {
              throw CameraIntrinsicNotFoundException(sCamModel);
          }
      }
    }
  }
  // Build intrinsic parameter related to the view
  std::shared_ptr<IntrinsicBase> intrinsic;
  if (focal > 0 && ppx > 0 && ppy > 0 && width > 0 && height > 0)
  {
      // Create the desired camera type
      switch (e_User_camera_model)
      {
      case PINHOLE_CAMERA:
          intrinsic = std::make_shared<Pinhole_Intrinsic>
          (width, height, focal, ppx, ppy);
      break;
      case PINHOLE_CAMERA_RADIAL1:
          intrinsic = std::make_shared<Pinhole_Intrinsic_Radial_K1>
          (width, height, focal, ppx, ppy, 0.0); // setup no distortion as initial guess
      break;
      case PINHOLE_CAMERA_RADIAL3:
          intrinsic = std::make_shared<Pinhole_Intrinsic_Radial_K3>
          (width, height, focal, ppx, ppy, 0.0, 0.0, 0.0);  // setup no distortion as initial guess
      break;
      case PINHOLE_CAMERA_BROWN:
          intrinsic = std::make_shared<Pinhole_Intrinsic_Brown_T2>
          (width, height, focal, ppx, ppy, 0.0, 0.0, 0.0, 0.0, 0.0); // setup no distortion as initial guess
      break;
      case PINHOLE_CAMERA_FISHEYE:
          intrinsic = std::make_shared<Pinhole_Intrinsic_Fisheye>
          (width, height, focal, ppx, ppy, 0.0, 0.0, 0.0, 0.0); // setup no distortion as initial guess
      break;
      case CAMERA_SPHERICAL:
          intrinsic = std::make_shared<Intrinsic_Spherical>
              (width, height);
      break;
      default:
          std::cerr << "Error: unknown camera model: " << (int) e_User_camera_model << std::endl;
          return false;
      }
  }
  
  if(intrinsic){
    v = std::make_shared<View>(sImFilenamePart, -1, -1, -1, width, height);
    this->_openmvg_storage->StoreViewAndIntrinsic(this->_reconstruction_id, image_id, v, intrinsic);
  }else{
    LOG(ERROR) << "Null Intrinsic for image " << sImFilenamePart;
    return false; 
  }

  // Group camera that share common properties if desired (leads to more faster & stable BA).
  GroupSharedIntrinsics(*this->_sfm_data);

  // Undistort image and store undistorted
  /*Image<unsigned char> imageRGB, imageRGB_ud;
  this->_openmvg_images_storage.ReadImage(image_id, &imageRGB); 
  UndistortImage(imageRGB, intrinsic.get(), imageRGB_ud, BLACK);
  ImageData img_undistorted;
  ImageMetaData meta_undistorted;
  meta_undistorted.set_reconstruction(this->_reconstruction_id);
  meta_undistorted.set_id(GetUUID());
  img_undistorted.mutable_metadata()->CopyFrom(meta_undistorted);
  memcpy(img_undistorted.mutable_data()->data(), imageRGB.data(), imageRGB.size()); 
  this->_image_storage->StoreUndistorted(image_id, img_undistorted);
  return true;*/
}

std::unique_ptr<features::Regions> Init_region_type_from_json(const std::string& json)
{
    std::unique_ptr<Regions> regions_type;
    std::istringstream stream(json);
    {
        cereal::JSONInputArchive archive(stream);
        archive(cereal::make_nvp("regions_type", regions_type));
    }
    return regions_type;
}

bool OpenMVGReconstructionAgent::ComputeFeatures(const std::set<std::string>& image_ids){
  LOG(INFO) << "Computing features for " << image_ids.size() << " new images";
  ConfigurationContainerPtr config = this->_configuration_adapter->GetAgentConfigOrDefault(
    this->_reconstruction_id,
    "openmvg",
    nullptr
  );
  bool bUpRight = config->get_bool("up_right");
  std::string sImage_Describer_Method = config->get_string("image_describer_method");
  bool bForce = config->get_bool("brute_force");
  std::string sFeaturePreset = config->get_string("feature_preset");


  // b. Init the image_describer
  // - retrieve the used one in case of pre-computed features
  // - else create the desired one

  std::unique_ptr<Image_describer> image_describer;

  ConfigurationContainerPtr image_describer_config = config->get_container("image_describer");
  std::string json_image_desciber = image_describer_config->jsonify();
  std::istringstream stream(json_image_desciber);
  try
  {
    cereal::JSONInputArchive archive(stream);
    archive(cereal::make_nvp("image_describer", image_describer));
  }
  catch (const cereal::Exception & e)
  {
    throw;
  }
  
  /*const std::string sImage_describer = stlplus::create_filespec(this->_config.features_dir, "image_describer", "json");
  if (stlplus::is_file(sImage_describer))
  {
    // Dynamically load the image_describer from the file (will restore old used settings)
    std::ifstream stream(sImage_describer.c_str());
    if (!stream.is_open())
      return false;

    try
    {
      cereal::JSONInputArchive archive(stream);
      archive(cereal::make_nvp("image_describer", image_describer));
    }
    catch (const cereal::Exception & e)
    {
      std::cerr << e.what() << std::endl
        << "Cannot dynamically allocate the Image_describer interface." << std::endl;
      return false;
    }
  }
  else
  {
    // Create the desired Image_describer method.
    // Don't use a factory, perform direct allocation
    if (sImage_Describer_Method == "SIFT")
    {
      image_describer.reset(new SIFT_Image_describer
        (SIFT_Image_describer::Params(), bUpRight));
    }
    else
    if (sImage_Describer_Method == "SIFT_ANATOMY")
    {
      image_describer.reset(
        new SIFT_Anatomy_Image_describer(SIFT_Anatomy_Image_describer::Params()));
    }
    else
    if (sImage_Describer_Method == "AKAZE_FLOAT")
    {
      image_describer = AKAZE_Image_describer::create
        (AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MSURF), bUpRight);
    }
    else
    if (sImage_Describer_Method == "AKAZE_MLDB")
    {
      image_describer = AKAZE_Image_describer::create
        (AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MLDB), bUpRight);
    }
    if (!image_describer)
    {
      std::cerr << "Cannot create the designed Image_describer:"
        << sImage_Describer_Method << "." << std::endl;
      return false;
    }
    else
    {
      if (!sFeaturePreset.empty())
      if (!image_describer->Set_configuration_preset(stringToEnum(sFeaturePreset)))
      {
        std::cerr << "Preset configuration failed." << std::endl;
        return false;
      }
    }
  }*/

  // Feature extraction routines
  // For each View of the SfM_Data container:
  // - if regions file exists continue,
  // - if no file, compute features
  {
    Image<unsigned char> imageGray;
    // Use a boolean to track if we must stop feature extraction
    std::atomic<bool> preemptive_exit(false);

    std::unordered_map<std::string, std::string> id_path_map;
    for(std::string id : image_ids){
      id_path_map[id] = this->_image_storage->GetMeta(id).path();
    }
    for(int i = 0; i < id_path_map.size(); ++i){
      auto iter = id_path_map.begin();
      std::advance(iter, i);
      std::string image_id = std::get<0>(*iter);
      std::string image_path = std::get<1>(*iter);
      LOG(INFO) << "Computing features for " << stlplus::basename_part(image_path);
      if (!this->_openmvg_images_storage.ReadImage(image_id, &imageGray))
          continue;

      Image<unsigned char> * mask = nullptr; // The mask is null by default

      /*
        Image<unsigned char> imageMask;
        const std::string
          mask_filename_local =
          stlplus::basename_part(image_path) + "_mask.png",
          mask__filename_global = "mask.png";

      // Try to read the local mask
      if (stlplus::file_exists(mask_filename_local))
      {
          if (!ReadImage(mask_filename_local.c_str(), &imageMask))
          {
              std::cerr << "Invalid mask: " << mask_filename_local << std::endl
                          << "Stopping feature extraction." << std::endl;
              preemptive_exit = true;
          }
          // Use the local mask only if it fits the current image size
          if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
          mask = &imageMask;
      }
      else
      {
          // Try to read the global mask
          if (stlplus::file_exists(mask__filename_global))
          {
            if (!ReadImage(mask__filename_global.c_str(), &imageMask))
            {
                std::cerr << "Invalid mask: " << mask__filename_global << std::endl
                        << "Stopping feature extraction." << std::endl;
                preemptive_exit = true;
            }
            // Use the global mask only if it fits the current image size
            if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
                mask = &imageMask;
          }
      }*/

      // Compute features and descriptors and export them to files and now sql. TODO: Remove file storage.
      // TODO: this casting bullshit is dangerous and disgusting. Needs be be fixed asap. 
      auto sift_regions = std::unique_ptr<SIFT_Anatomy_Image_describer::Regions_type>(
        dynamic_cast<SIFT_Anatomy_Image_describer::Regions_type*>(image_describer->Describe(imageGray, mask).release()));
      if (!sift_regions){
          LOG(ERROR) << "Failed to compute regions for " << image_id;
          return false;
      }
      this->_regions_storage->Store(this->_reconstruction_id, image_id, std::move(sift_regions));
    }
    return true;
  }
}

bool OpenMVGReconstructionAgent::ComputeMatches(const std::set<std::string>& new_image_ids){
  // Update our SFM data to get all the images to match with

  ConfigurationContainerPtr config = this->_configuration_adapter->GetAgentConfigOrDefault(
    this->_reconstruction_id,
    "openmvg",
    nullptr
  );
  std::string sGeometricModel = config->get_string("geometric_model");
  float fDistRatio = config->get_double("dist_ratio");
  std::string sNearestMatchingMethod = config->get_string("nearest_matching_method");
  bool bGuided_matching = config->get_bool("guided_matching");
  int imax_iteration = config->get_int("max_iterations");
  int ui_max_cache_size = config->get_int("ui_max_cache_size");
  Pair_Set pairs = this->_GatherMatchesToCompute(new_image_ids);
  EGeometricModel eGeometricModelToCompute = FUNDAMENTAL_MATRIX;

  switch (sGeometricModel[0])
  {
    case 'f': case 'F':
      eGeometricModelToCompute = FUNDAMENTAL_MATRIX;
    break;
    case 'e': case 'E':
      eGeometricModelToCompute = ESSENTIAL_MATRIX;
    break;
    case 'h': case 'H':
      eGeometricModelToCompute = HOMOGRAPHY_MATRIX;
    break;
    case 'a': case 'A':
      eGeometricModelToCompute = ESSENTIAL_MATRIX_ANGULAR;
    break;
    case 'o': case 'O':
      eGeometricModelToCompute = ESSENTIAL_MATRIX_ORTHO;
    break;
    default:
      std::cerr << "Unknown geometric model" << std::endl;
      return false;
  }
  ConfigurationContainerPtr image_describer = config->get_container("image_describer");
  std::unique_ptr<Regions> regions_type = Init_region_type_from_json(image_describer->jsonify());
  if (!regions_type)
  {
    throw std::runtime_error("Invalid or null regions type");
  }

  PairWiseMatches map_PutativesMatches;

  // Allocate the right Matcher according the Matching requested method
  std::unique_ptr<Matcher> collectionMatcher;
  if (sNearestMatchingMethod == "AUTO")
  {
    if (regions_type->IsScalar())
    {
      std::cout << "Using FAST_CASCADE_HASHING_L2 matcher" << std::endl;
      collectionMatcher.reset(new Cascade_Hashing_Matcher_Regions(fDistRatio));
    }
    else
    if (regions_type->IsBinary())
    {
      std::cout << "Using BRUTE_FORCE_HAMMING matcher" << std::endl;
      collectionMatcher.reset(new Matcher_Regions(fDistRatio, BRUTE_FORCE_HAMMING));
    }
  }
  else
  if (sNearestMatchingMethod == "BRUTEFORCEL2")
  {
    std::cout << "Using BRUTE_FORCE_L2 matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(fDistRatio, BRUTE_FORCE_L2));
  }
  else
  if (sNearestMatchingMethod == "BRUTEFORCEHAMMING")
  {
    std::cout << "Using BRUTE_FORCE_HAMMING matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(fDistRatio, BRUTE_FORCE_HAMMING));
  }
  else
  if (sNearestMatchingMethod == "HNSWL2")
  {
    std::cout << "Using HNSWL2 matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(fDistRatio, HNSW_L2));
  }
  else
  if (sNearestMatchingMethod == "ANNL2")
  {
    std::cout << "Using ANN_L2 matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(fDistRatio, ANN_L2));
  }
  else
  if (sNearestMatchingMethod == "CASCADEHASHINGL2")
  {
    std::cout << "Using CASCADE_HASHING_L2 matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(fDistRatio, CASCADE_HASHING_L2));
  }
  else
  if (sNearestMatchingMethod == "FASTCASCADEHASHINGL2")
  {
    std::cout << "Using FAST_CASCADE_HASHING_L2 matcher" << std::endl;
    collectionMatcher.reset(new Cascade_Hashing_Matcher_Regions(fDistRatio));
  }
  if (!collectionMatcher)
  {
    std::cerr << "Invalid Nearest Neighbor method: " << sNearestMatchingMethod << std::endl;
    return false;
  }

  std::shared_ptr<RegionsProviderFromStorage<SIFT_Anatomy_Image_describer::Regions_type>> regions_provider 
    = std::make_shared<RegionsProviderFromStorage<SIFT_Anatomy_Image_describer::Regions_type>>(this->_regions_storage, this->_openmvg_storage, std::move(regions_type));
  regions_provider->load_from_storage(this->_reconstruction_id);
  collectionMatcher->Match(regions_provider, pairs, map_PutativesMatches);

  //---------------------------------------
  // b. Geometric filtering of putative matches
  //    - AContrario Estimation of the desired geometric model
  //    - Use an upper bound for the a contrario estimated threshold
  //---------------------------------------
  PairWiseMatches geometric_matches;
  std::unique_ptr<ImageCollectionGeometricFilter> filter_ptr(
    new ImageCollectionGeometricFilter(this->_sfm_data.get(), regions_provider));

  if (filter_ptr)
  {
    const double d_distance_ratio = 0.6;
    switch (eGeometricModelToCompute)
    {
      case HOMOGRAPHY_MATRIX:
      {
        const bool bGeometric_only_guided_matching = true;
        filter_ptr->Robust_model_estimation(
          GeometricFilter_HMatrix_AC(4.0, imax_iteration),
          map_PutativesMatches, bGuided_matching,
          bGeometric_only_guided_matching ? -1.0 : d_distance_ratio);
         geometric_matches = filter_ptr->Get_geometric_matches();
      }
      break;
      case FUNDAMENTAL_MATRIX:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_FMatrix_AC(4.0, imax_iteration),
          map_PutativesMatches, bGuided_matching, d_distance_ratio);
          geometric_matches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_EMatrix_AC(4.0, imax_iteration),
          map_PutativesMatches, bGuided_matching, d_distance_ratio);
         geometric_matches = filter_ptr->Get_geometric_matches();

        //-- Perform an additional check to remove pairs with poor overlap
        std::vector<PairWiseMatches::key_type> vec_toRemove;
        for (const auto & pairwisematches_it : geometric_matches)
        {
          const size_t putativePhotometricCount = map_PutativesMatches.find(pairwisematches_it.first)->second.size();
          const size_t putativeGeometricCount = pairwisematches_it.second.size();
          const float ratio = putativeGeometricCount / static_cast<float>(putativePhotometricCount);
          if (putativeGeometricCount < 50 || ratio < .3f)  {
            // the pair will be removed
            vec_toRemove.push_back(pairwisematches_it.first);
          }
        }
        //-- remove discarded pairs
        for (const auto & pair_to_remove_it : vec_toRemove)
        {
          geometric_matches.erase(pair_to_remove_it);
        }
      }
      break;
      case ESSENTIAL_MATRIX_ANGULAR:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_ESphericalMatrix_AC_Angular(4.0, imax_iteration),
          map_PutativesMatches, bGuided_matching);
         geometric_matches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX_ORTHO:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_EOMatrix_RA(2.0, imax_iteration),
          map_PutativesMatches, bGuided_matching, d_distance_ratio);
        geometric_matches = filter_ptr->Get_geometric_matches();
      }
      break;
    }
  }
  LOG(INFO) << "Storing " << geometric_matches.size() << " new matches for reconstruction " << this->_reconstruction_id;
  this->_openmvg_storage->StoreMatches(this->_reconstruction_id, sGeometricModel[0], geometric_matches);
  return true;
}

openMVG::Pair_Set OpenMVGReconstructionAgent::_GatherMatchesToCompute(const std::set<std::string>& new_image_ids){    
    std::set<IndexT> new_ids;
    openMVG::Pair_Set matches_to_compute;
    auto search_client =  GetIndexingClient(CONFIG_GET_STRING("index.address"));
    if(this->_sfm_data->GetViews().size() < 2){
      return openMVG::Pair_Set();
    }

    for(const std::string& image_id : new_image_ids){
      LOG(INFO) << "Gathering matches for " << image_id;
      grpc::ClientContext context;
      ClosestNRequest req;
      ClosestNResponse resp;
      req.set_image_id(image_id);
      req.set_n(10);
      search_client->ClosestN(&context, req, &resp);
      auto search_result = resp.image_ids();
      IndexT view_idx_1 = this->_openmvg_storage->GetViewIdxByImageID(image_id);
      for(const std::string& similar_id : search_result){
        IndexT view_idx_2 = this->_openmvg_storage->GetViewIdxByImageID(similar_id);
        if(matches_to_compute.find({view_idx_2, view_idx_1}) == matches_to_compute.end())
          matches_to_compute.insert({view_idx_1, view_idx_2});
      }
    }

    return matches_to_compute;
}

bool OpenMVGReconstructionAgent::IncrementalSFM(){
  ConfigurationContainerPtr config = this->_configuration_adapter->GetAgentConfigOrDefault(
    this->_reconstruction_id,
    "openmvg",
    nullptr
  );
  std::string sIntrinsic_refinement_options = config->get_string("intrinsic_refinement_options");
  int i_User_camera_model = config->get_int("user_camera_model");
  bool b_use_motion_priors = config->get_bool("use_motion_priors");
  int triangulation_method = config->get_int("triangulation_method");
  ConfigurationContainerPtr image_describer = config->get_container("image_describer");
  std::unique_ptr<Regions> regions_type = Init_region_type_from_json(image_describer->jsonify());
  if (!regions_type)
  {
      throw std::runtime_error("Invalid regions type");
  }

  std::shared_ptr<Matches_Provider> matches_provider = std::make_shared<Matches_Provider>();
  matches_provider->pairWise_matches_ = std::move(this->_openmvg_storage->GetMatches(this->_reconstruction_id));
  std::shared_ptr<FeaturesProviderFromStorage<SIFT_Anatomy_Image_describer::Regions_type>> feats_provider 
    = std::make_shared<FeaturesProviderFromStorage<SIFT_Anatomy_Image_describer::Regions_type>>(this->_regions_storage, this->_openmvg_storage);
  feats_provider->load_from_storage(this->_reconstruction_id);
  std::unique_ptr<SfMSceneInitializer> scene_initializer;
    
  if(!this->_sfm_data->poses.empty()){
      LOG(INFO) << "Initailizing using a previous reconstruction.";
      scene_initializer.reset(new SfMSceneInitializer(*this->_sfm_data,
      feats_provider.get(),
      matches_provider.get()));
  }else{
      LOG(INFO) << "Initializing using Stellar.";
      scene_initializer.reset(new SfMSceneInitializerStellar(*this->_sfm_data,
      feats_provider.get(),
      matches_provider.get()));
  }

  SequentialSfMReconstructionEngine2 sfmEngine(
    scene_initializer.get(),
    *this->_sfm_data,
    "/tmp");

  sfmEngine.SetFeaturesProvider(feats_provider.get());
  sfmEngine.SetMatchesProvider(matches_provider.get());

  const cameras::Intrinsic_Parameter_Type intrinsic_refinement_options =
    cameras::StringTo_Intrinsic_Parameter_Type(sIntrinsic_refinement_options);
  sfmEngine.Set_Intrinsics_Refinement_Type(intrinsic_refinement_options);
  sfmEngine.SetUnknownCameraType(EINTRINSIC(i_User_camera_model));
  sfmEngine.Set_Use_Motion_Prior(b_use_motion_priors);
  sfmEngine.SetTriangulationMethod(static_cast<ETriangulationMethod>(triangulation_method));

  if (sfmEngine.Process())
  {    
    *this->_sfm_data = sfmEngine.Get_SfM_Data();  

    for(auto view_it : this->_sfm_data->GetViews()){
      std::shared_ptr<View> v = view_it.second;
      if(v->id_pose != -1){
        Pose3 pose = this->_sfm_data->poses[v->id_pose];
        this->_openmvg_storage->StorePoseUpdateView(this->_reconstruction_id, v, pose);
      }
    }
  }else{
    LOG(ERROR) << "SFM Process failed";
    this->_sfm_data->poses.clear();
    return false;
  }
  return true;
}


bool GetPLYDataFromSFMData(const SfM_Data& sfm_data, std::string* data_out){
  const bool b_structure = true;
  const bool b_control_points = true;
  const bool b_extrinsics = true;
  const bool b_write_in_ascii = false;

  if (!(b_structure || b_extrinsics || b_control_points))
      return false;

  // Create the stream and check its status
  std::ostringstream stream;
  bool bOk = false;
  {
      // Count how many views having valid poses:
      IndexT view_with_pose_count = 0;
      IndexT view_with_pose_prior_count = 0;
      if (b_extrinsics)
      {
          for (const auto & view : sfm_data.GetViews())
          {
              view_with_pose_count += sfm_data.IsPoseAndIntrinsicDefined(view.second.get());
          }

          for (const auto & view : sfm_data.GetViews())
          {
              if (const sfm::ViewPriors *prior = dynamic_cast<sfm::ViewPriors*>(view.second.get()))
              {
                  view_with_pose_prior_count += prior->b_use_pose_center_;
              }
          }
      }

      stream << std::fixed << std::setprecision (std::numeric_limits<double>::digits10 + 1);

      using Vec3uc = Eigen::Matrix<unsigned char, 3, 1>;

      stream << "ply"
      << '\n' << "format "
              << (b_write_in_ascii ? "ascii 1.0" : "binary_little_endian 1.0")
      << '\n' << "comment generated by OpenMVG"
      << '\n' << "element vertex "
          // Vertex count: (#landmark + #GCP + #view_with_valid_pose)
          << (  (b_structure ? sfm_data.GetLandmarks().size() : 0)
              + (b_control_points ? sfm_data.GetControl_Points().size() : 0)
              + view_with_pose_count
              + view_with_pose_prior_count)
      << '\n' << "property double x"
      << '\n' << "property double y"
      << '\n' << "property double z"
      << '\n' << "property uchar red"
      << '\n' << "property uchar green"
      << '\n' << "property uchar blue"
      << '\n' << "end_header" << std::endl;

      if (b_extrinsics)
      {
          for (const auto & view : sfm_data.GetViews())
          {
          // Export pose as Green points
          if (sfm_data.IsPoseAndIntrinsicDefined(view.second.get()))
          {
              const geometry::Pose3 pose = sfm_data.GetPoseOrDie(view.second.get());
              if (b_write_in_ascii)
              {
              stream
                  << pose.center()(0) << ' '
                  << pose.center()(1) << ' '
                  << pose.center()(2) << ' '
                  << "0 255 0\n";
              }
              else
              {
              stream.write( reinterpret_cast<const char*> ( pose.center().data() ), sizeof(openMVG::Vec3 ) );
              stream.write( reinterpret_cast<const char*> ( Vec3uc(0, 255, 0).data() ), sizeof( Vec3uc ) );
              }
          }

          // Export pose priors as Blue points
          if (const sfm::ViewPriors *prior = dynamic_cast<sfm::ViewPriors*>(view.second.get()))
          {
              if (prior->b_use_pose_center_)
              {
              if (b_write_in_ascii)
              {
                  stream
                  << prior->pose_center_(0) << ' '
                  << prior->pose_center_(1) << ' '
                  << prior->pose_center_(2) << ' '
                  << "0 0 255\n";
              }
              else
              {
                  stream.write( reinterpret_cast<const char*> ( prior->pose_center_.data() ), sizeof( openMVG::Vec3 ) );
                  stream.write( reinterpret_cast<const char*> ( Vec3uc(0, 0, 255).data() ), sizeof( Vec3uc ) );
              }
              }
          }
          }
      }

      if (b_structure)
      {
          // Export structure points as White points
          const Landmarks & landmarks = sfm_data.GetLandmarks();
          for ( const auto & iterLandmarks : landmarks )
          {
          if (b_write_in_ascii)
          {
              stream
              << iterLandmarks.second.X(0) << ' '
              << iterLandmarks.second.X(1) << ' '
              << iterLandmarks.second.X(2) << ' '
              << "255 255 255\n";
          }
          else
          {
              stream.write( reinterpret_cast<const char*> ( iterLandmarks.second.X.data() ), sizeof( openMVG::Vec3 ) );
              stream.write( reinterpret_cast<const char*> ( Vec3uc(255, 255, 255).data() ), sizeof( Vec3uc ) );
          }
          }
      }

      if (b_control_points)
      {
          // Export GCP as Red points
          const Landmarks & landmarks = sfm_data.GetControl_Points();
          for ( const auto & iterGCP : landmarks )
          {
          if (b_write_in_ascii)
          {
              stream
              << iterGCP.second.X(0) << ' '
              << iterGCP.second.X(1) << ' '
              << iterGCP.second.X(2) << ' '
              << "255 0 0\n";
          }
          else
          {
              stream.write( reinterpret_cast<const char*> ( iterGCP.second.X.data() ), sizeof( openMVG::Vec3 ) );
              stream.write( reinterpret_cast<const char*> ( Vec3uc(255, 0, 0).data() ), sizeof( Vec3uc ) );
          }
          }
      }

      stream.flush();
      bOk = stream.good();
      *data_out = stream.str();
      return bOk;
  }
}

bool OpenMVGReconstructionAgent::ComputeStructure(){
   ConfigurationContainerPtr config = this->_configuration_adapter->GetAgentConfigOrDefault(
    this->_reconstruction_id,
    "openmvg",
    nullptr
  );
  double dMax_reprojection_error = config->get_double("max_reprojection_error");
  unsigned int ui_max_cache_size = config->get_int("ui_max_cache_size");
  int triangulation_method = config->get_int("triangulation_method");

  ConfigurationContainerPtr image_describer = config->get_container("image_describer");
  std::unique_ptr<Regions> regions_type = Init_region_type_from_json(image_describer->jsonify());
  if (!regions_type)
  {
    throw std::runtime_error("Invalid regions type");
  }
  std::shared_ptr<RegionsProviderFromStorage<SIFT_Anatomy_Image_describer::Regions_type>> regions_provider 
    = std::make_shared<RegionsProviderFromStorage<SIFT_Anatomy_Image_describer::Regions_type>>(this->_regions_storage, this->_openmvg_storage, std::move(regions_type));
  regions_provider->load_from_storage(this->_reconstruction_id);

  PairWiseMatches matches = this->_openmvg_storage->GetMatches(this->_reconstruction_id);
  Pair_Set pairs = getPairs(matches);

  const std::set<IndexT> valid_viewIdx = Get_Valid_Views(*this->_sfm_data);
  pairs = Pair_filter(pairs, valid_viewIdx);
  
  SfM_Data_Structure_Estimation_From_Known_Poses structure_estimator(dMax_reprojection_error);
  structure_estimator.run(*this->_sfm_data, pairs, regions_provider,
      static_cast<ETriangulationMethod>(triangulation_method));

  regions_provider.reset();
  RemoveOutliers_AngleError(*this->_sfm_data, 2.0);

  SparsePointCloudData spc_data;
  GetPLYDataFromSFMData(*this->_sfm_data, spc_data.mutable_data());
  SparsePointCloudMetaData spc_meta_data;
  spc_meta_data.set_id(GetUUID());
  spc_meta_data.set_reconstruction(this->_reconstruction_id);
  spc_data.mutable_metadata()->CopyFrom(spc_meta_data);
  this->_sparse_storage->Store(spc_data);

  return true;
}
#include "openmvs_ext.h"

/*void OpenMVGReconstructionAgent::DensifyPointCloud(const std::string& reconstruction_id){
  int nFusionMode;
  int nMaxThreads;
  int nArchiveType;
  DistributedScene scene;
	if (!scene.LoadFromeRemoteScene(reconstruction_id)){
    //LOG(ERROR) << "Failed to load scene for " << reconstruction_id;
  }
		
	if (scene.pointcloud.IsEmpty()) {
    //LOG(ERROR) << "Input pointcloud is empty for " << reconstruction_id;
    return;
	}

	if ((ARCHIVE_TYPE)nArchiveType != ARCHIVE_MVS) {
		if (!scene.DenseReconstruction(nFusionMode)) {
			if (ABS(nFusionMode) != 1)
				return;
			return;
		}
	}

  //TODO: Store scene elsewhere
	//scene.Save(baseFileName+_T(".mvs"), (ARCHIVE_TYPE)nArchiveType);
	//scene.pointcloud.Save(baseFileName+_T(".ply"));
}*/

OpenMVGReconstructionAgent::~OpenMVGReconstructionAgent(){}

/*template<typename _Tp>
bool SerializeSave(const _Tp& obj, std::ostream& stream, uint32_t version=MVSI_PROJECT_VER) {  
	if (version > 0) {
		// save project ID
		stream.write(MVSI_PROJECT_ID, 4);
		// save project version
		stream.write((const char*)&version, sizeof(uint32_t));
		// reserve some bytes
		const uint32_t reserved(0);
		stream.write((const char*)&reserved, sizeof(uint32_t));
	}
	// serialize out the current state
	ARCHIVE::ArchiveSave serializer(stream, version);
	serializer & obj;
	return true;
}

bool ExportToOpenMVS(
  const SfM_Data & sfm_data,
  std::vector<char>& mvs_raw
  )
{
  MVS::Interface scene;
  size_t nPoses(0);
  const uint32_t nViews((uint32_t)sfm_data.GetViews().size());

  // OpenMVG can have not contiguous index, use a map to create the required OpenMVS contiguous ID index
  std::map<openMVG::IndexT, uint32_t> map_intrinsic, map_view;

  // define a platform with all the intrinsic group
  for (const auto& intrinsic: sfm_data.GetIntrinsics())
  {
    if (isPinhole(intrinsic.second->getType()))
    {
      const Pinhole_Intrinsic * cam = dynamic_cast<const Pinhole_Intrinsic*>(intrinsic.second.get());
      if (map_intrinsic.count(intrinsic.first) == 0)
        map_intrinsic.insert(std::make_pair(intrinsic.first, scene.platforms.size()));
      MVS::Interface::Platform platform;
      // add the camera
      MVS::Interface::Platform::Camera camera;
      camera.K = cam->K();
      // sub-pose
      camera.R = openMVG::Mat3::Identity();
      camera.C = openMVG::Vec3::Zero();
      platform.cameras.push_back(camera);
      scene.platforms.push_back(platform);
    }
  }

  // define images & poses
  scene.images.reserve(nViews);
  for (const auto& view : sfm_data.GetViews())
  {
    map_view[view.first] = scene.images.size();
    MVS::Interface::Image image;
    const std::string srcImage = stlplus::create_filespec(sfm_data.s_root_path, view.second->s_Img_path);
    //image.name = stlplus::create_filespec(sOutDir, view.second->s_Img_path);
    image.platformID = map_intrinsic.at(view.second->id_intrinsic);
    MVS::Interface::Platform& platform = scene.platforms[image.platformID];
    image.cameraID = 0;
    if (!stlplus::is_file(srcImage))
    {
      std::cout << "Cannot read the corresponding image: " << srcImage << std::endl;
      return EXIT_FAILURE;
    }
    if (sfm_data.IsPoseAndIntrinsicDefined(view.second.get()))
    {
      MVS::Interface::Platform::Pose pose;
      image.poseID = platform.poses.size();
      const openMVG::geometry::Pose3 poseMVG(sfm_data.GetPoseOrDie(view.second.get()));
      pose.R = poseMVG.rotation();
      pose.C = poseMVG.center();
      platform.poses.push_back(pose);
      ++nPoses;
    }
    else
    {
      // image have not valid pose, so set an undefined pose
      image.poseID = NO_ID;
      // just copy the image
      //stlplus::file_copy(srcImage, image.name);
    }
    scene.images.emplace_back(image);
  }

  // define structure
  scene.vertices.reserve(sfm_data.GetLandmarks().size());
  for (const auto& vertex: sfm_data.GetLandmarks())
  {
    const Landmark & landmark = vertex.second;
    MVS::Interface::Vertex vert;
    MVS::Interface::Vertex::ViewArr& views = vert.views;
    for (const auto& observation: landmark.obs)
    {
      const auto it(map_view.find(observation.first));
      if (it != map_view.end()) {
        MVS::Interface::Vertex::View view;
        view.imageID = it->second;
        view.confidence = 0;
        views.push_back(view);
      }
    }
    if (views.size() < 2)
      continue;
    std::sort(
      views.begin(), views.end(),
      [] (const MVS::Interface::Vertex::View& view0, const MVS::Interface::Vertex::View& view1)
      {
        return view0.imageID < view1.imageID;
      }
    );
    vert.X = landmark.X.cast<float>();
    scene.vertices.push_back(vert);
  }

  // normalize camera intrinsics
  for (size_t p=0; p<scene.platforms.size(); ++p)
  {
    MVS::Interface::Platform& platform = scene.platforms[p];
    for (size_t c=0; c<platform.cameras.size(); ++c) {
      MVS::Interface::Platform::Camera& camera = platform.cameras[c];
      // find one image using this camera
      MVS::Interface::Image* pImage(nullptr);
      for (MVS::Interface::Image& image: scene.images)
      {
        if (image.platformID == p && image.cameraID == c && image.poseID != NO_ID)
        {
          pImage = &image;
          break;
        }
      }
      if (!pImage)
      {
        std::cerr << "error: no image using camera " << c << " of platform " << p << std::endl;
        continue;
      }
      // read image meta-data
      ImageHeader imageHeader;
      ReadImageHeader(pImage->name.c_str(), &imageHeader);
      const double fScale(1.0/std::max(imageHeader.width, imageHeader.height));
      camera.K(0, 0) *= fScale;
      camera.K(1, 1) *= fScale;
      camera.K(0, 2) *= fScale;
      camera.K(1, 2) *= fScale;
    }
  }

  std::ostringstream ostream;
  if (!SerializeSave(scene, ostream))
    return false;

  std::cout
    << "Scene saved to OpenMVS interface format:\n"
    << " #platforms: " << scene.platforms.size() << std::endl;
    for (int i = 0; i < scene.platforms.size(); ++i)
    {
      std::cout << "  platform ( " << i << " ) #cameras: " << scene.platforms[i].cameras.size() << std::endl;
    }
  std::cout
    << "  " << scene.images.size() << " images (" << nPoses << " calibrated)\n"
    << "  " << scene.vertices.size() << " Landmarks\n";
  return true;
}

bool OpenMVGReconstructionAgent::ExportToMVS(){
  auto sfm_data = this->_openmvg_storage->GetSFMData(this->_reconstruction_id, ESfM_Data::ALL);


 
}*/