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

#include "nonFree/sift/SIFT_describer_io.hpp"

#include <cereal/details/helpers.hpp>

#include <atomic>
#include <cstdlib>

#include "openMVG/cameras/cameras.hpp"
#include "openMVG/exif/exif_IO_EasyExif.hpp"
#include "openMVG/geodesy/geodesy.hpp"
#include "openMVG/image/image_io.hpp"
#include "openMVG/numeric/eigen_alias_definition.hpp"


#include <fstream>
#include <memory>
#include <utility>

#include <iostream>
#include <memory>
#include <string>

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

std::pair<bool, Vec3> checkGPS
(
  const std::string & filename,
  const int & GPS_to_XYZ_method = 0
)
{
  std::pair<bool, Vec3> val(false, Vec3::Zero());
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

OpenMVGReconstructionAgent::OpenMVGReconstructionAgent(const std::string& reconstruction_id, CameraIntrinsicsStorage* intrinsics_storage,
                                                       OpenMVGStorageAdapter* openmvg_storage):
                                                                                  _reconstruction_id(reconstruction_id), 
                                                                                  _intrinsics_storage(intrinsics_storage),
                                                                                  _openmvg_storage(openmvg_storage){
  
  
}

void OpenMVGReconstructionAgent::SetConfig(const OpenMVGReconstructionAgentConfig& config){
    this->_config = config;
    OpenMVGMetadata data;
    /*
      CHANGE THIS. Need some Initializer that can take reconstruction and setup a given reconstruction agent. 
    */
    data.root_path = config.root_path;
    this->_openmvg_storage->StoreMeta(this->_reconstruction_id, data);
    this->_sfm_data = this->_openmvg_storage->GetSFMData(this->_reconstruction_id, ALL);
}

bool OpenMVGReconstructionAgent::AddImage(const std::string& image_path){
  std::string sPriorWeights;
  std::pair<bool, Vec3> prior_w_info(false, Vec3(1.0,1.0,1.0));
  int i_User_camera_model = PINHOLE_CAMERA_RADIAL3;
  bool b_Group_camera_model = true;
  int i_GPS_XYZ_method = 0;
  double focal_pixels = -1.0;
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
  if (!openMVG::image::ReadImageHeader(image_path.c_str(), &imgHeader)){
      std::cout
          << sImFilenamePart << " Failed to read header" << "\n";
      return false; // image cannot be read
  }

  width = imgHeader.width;
  height = imgHeader.height;
  ppx = width / 2.0;
  ppy = height / 2.0;

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
          CameraIntrinsics intrinsics = this->_intrinsics_storage->Get(sCamModel);
          if (!intrinsics.model().empty())
          {
          // The camera model was found in the database so we can compute it's approximated focal length
            const double ccdw = intrinsics.sensor_size();
            focal = std::max ( width, height ) * exifReader->getFocal() / ccdw;
          }
          else
          {
              std::cout
              << stlplus::basename_part(image_path)
              << "\" model \"" << sCamModel << "\" doesn't exist in the database" << "\n"
              << "Please consider add your camera model and sensor width in the database." << "\n";
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

  this->_GenerateImageFeatures(image_path);
  // Build the view corresponding to the image
  if(intrinsic){
    v = std::make_shared<View>(sImFilenamePart, -1, -1, -1, width, height);
    this->_openmvg_storage->StoreViewAndIntrinsic(this->_reconstruction_id, v, intrinsic);
  }else{
    LOG(ERROR) << "Null Intrinsic for image " << sImFilenamePart;
    return false; 
  }

  // Group camera that share common properties if desired (leads to more faster & stable BA).
  if (b_Group_camera_model)
  {
    GroupSharedIntrinsics(*this->_sfm_data);
  }

  // Store SfM_Data views & intrinsic data
  if (!Save(
    *this->_sfm_data,
    stlplus::create_filespec(this->_config.sfm_dir, "sfm_data.json" ).c_str(),
    ESfM_Data(ALL)))
  {
    std::cout <<" failed to save sfm_json.data in " << this->_config.sfm_dir <<  "\n";
        return false;
  }
  this->GenerateMatches(image_path);
  return true;
}

bool OpenMVGReconstructionAgent::_GenerateImageFeatures(const std::string& image_path){
    LOG(INFO) << "Computing Features for image " << image_path;
    if (this->_config.features_dir.empty())  {
    std::cerr << "\nIt is an invalid output directory" << std::endl;
    return false;
  }

  // b. Init the image_describer
  // - retrieve the used one in case of pre-computed features
  // - else create the desired one

  std::unique_ptr<Image_describer> image_describer;

  const std::string sImage_describer = stlplus::create_filespec(this->_config.features_dir, "image_describer", "json");
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
    if (this->_config.sImage_Describer_Method == "SIFT")
    {
      image_describer.reset(new SIFT_Image_describer
        (SIFT_Image_describer::Params(), !this->_config.bUpRight));
    }
    else
    if (this->_config.sImage_Describer_Method == "SIFT_ANATOMY")
    {
      image_describer.reset(
        new SIFT_Anatomy_Image_describer(SIFT_Anatomy_Image_describer::Params()));
    }
    else
    if (this->_config.sImage_Describer_Method == "AKAZE_FLOAT")
    {
      image_describer = AKAZE_Image_describer::create
        (AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MSURF), !this->_config.bUpRight);
    }
    else
    if (this->_config.sImage_Describer_Method == "AKAZE_MLDB")
    {
      image_describer = AKAZE_Image_describer::create
        (AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MLDB), !this->_config.bUpRight);
    }
    if (!image_describer)
    {
      std::cerr << "Cannot create the designed Image_describer:"
        << this->_config.sImage_Describer_Method << "." << std::endl;
      return false;
    }
    else
    {
      if (!this->_config.sFeaturePreset.empty())
      if (!image_describer->Set_configuration_preset(stringToEnum(this->_config.sFeaturePreset)))
      {
        std::cerr << "Preset configuration failed." << std::endl;
        return false;
      }
    }

    // Export the used Image_describer and region type for:
    // - dynamic future regions computation and/or loading
    {
      std::ofstream stream(sImage_describer.c_str());
      if (!stream.is_open())
        return false;

      cereal::JSONOutputArchive archive(stream);
      archive(cereal::make_nvp("image_describer", image_describer));
      auto regionsType = image_describer->Allocate();
      archive(cereal::make_nvp("regions_type", regionsType));
    }
  }

  // Feature extraction routines
  // For each View of the SfM_Data container:
  // - if regions file exists continue,
  // - if no file, compute features
  {
    Image<unsigned char> imageGray;
    // Use a boolean to track if we must stop feature extraction
    std::atomic<bool> preemptive_exit(false);
#ifdef OPENMVG_USE_OPENMP
    const unsigned int nb_max_thread = omp_get_max_threads();

    if (iNumThreads > 0) {
        omp_set_num_threads(iNumThreads);
    } else {
        omp_set_num_threads(nb_max_thread);
    }

    #pragma omp parallel for schedule(dynamic) if (iNumThreads > 0) private(imageGray)
#endif

    const std::string
    sFeat = stlplus::create_filespec(this->_config.features_dir, stlplus::basename_part(image_path), "feat"),
    sDesc = stlplus::create_filespec(this->_config.features_dir, stlplus::basename_part(image_path), "desc");

    // If features or descriptors file are missing, compute them
    if (!preemptive_exit && 
            (this->_config.bForce || 
            !stlplus::file_exists(sFeat) || 
            !stlplus::file_exists(sDesc)))
    {
        if (!ReadImage(image_path.c_str(), &imageGray))
            return false;

        Image<unsigned char> * mask = nullptr; // The mask is null by default

        const std::string
            mask_filename_local =
            stlplus::create_filespec(this->_sfm_data->s_root_path,
                stlplus::basename_part(image_path) + "_mask", "png"),
            mask__filename_global =
            stlplus::create_filespec(this->_sfm_data->s_root_path, "mask", "png");

        Image<unsigned char> imageMask;
        // Try to read the local mask
        if (stlplus::file_exists(mask_filename_local))
        {
            if (!ReadImage(mask_filename_local.c_str(), &imageMask))
            {
                std::cerr << "Invalid mask: " << mask_filename_local << std::endl
                            << "Stopping feature extraction." << std::endl;
                preemptive_exit = true;
                return false;
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
                return false;
            }
            // Use the global mask only if it fits the current image size
            if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
                mask = &imageMask;
            }
        }

        // Compute features and descriptors and export them to files
        auto regions = image_describer->Describe(imageGray, mask);
        if (regions && !image_describer->Save(regions.get(), sFeat, sDesc)) {
            std::cerr << "Cannot save regions for images: " << image_path << std::endl
                    << "Stopping feature extraction." << std::endl;
            preemptive_exit = true;
            return false;
        }
    }
  }
  return true;
}

bool OpenMVGReconstructionAgent::GenerateMatches(const std::string& new_image_paths){
  // Update our SFM data to get all the images to match with
  LOG(INFO) << "Generating matches for " << new_image_paths;
  this->_sfm_data = this->_openmvg_storage->GetSFMData(this->_reconstruction_id, 
                                                       ESfM_Data::VIEWS | ESfM_Data::INTRINSICS);
  Pair_Set pairs = this->_GatherMatchesToCompute(new_image_paths);
  std::shared_ptr<Regions_Provider> regions_provider;
  EGeometricModel eGeometricModelToCompute = FUNDAMENTAL_MATRIX;
  std::string sGeometricMatchesFilename = "";
  switch (this->_config.sGeometricModel[0])
  {
    case 'f': case 'F':
      eGeometricModelToCompute = FUNDAMENTAL_MATRIX;
      sGeometricMatchesFilename = "matches.f.bin";
    break;
    case 'e': case 'E':
      eGeometricModelToCompute = ESSENTIAL_MATRIX;
      sGeometricMatchesFilename = "matches.e.bin";
    break;
    case 'h': case 'H':
      eGeometricModelToCompute = HOMOGRAPHY_MATRIX;
      sGeometricMatchesFilename = "matches.h.bin";
    break;
    case 'a': case 'A':
      eGeometricModelToCompute = ESSENTIAL_MATRIX_ANGULAR;
      sGeometricMatchesFilename = "matches.f.bin";
    break;
    case 'o': case 'O':
      eGeometricModelToCompute = ESSENTIAL_MATRIX_ORTHO;
      sGeometricMatchesFilename = "matches.o.bin";
    break;
    default:
      std::cerr << "Unknown geometric model" << std::endl;
      return false;
  }

  const std::string sImage_describer = stlplus::create_filespec(this->_config.features_dir, "image_describer", "json");
  std::unique_ptr<Regions> regions_type = Init_region_type_from_file(sImage_describer);
  if (!regions_type)
  {
    std::cerr << "Invalid: "
      << sImage_describer << " regions type file." << std::endl;
    return false;
  }
  if (this->_config.ui_max_cache_size == 0)
  {
      // Default regions provider (load & store all regions in memory)
      regions_provider = std::make_shared<Regions_Provider>();
  }
  else
  {
      // Cached regions provider (load & store regions on demand)
      regions_provider = std::make_shared<Regions_Provider_Cache>(this->_config.ui_max_cache_size);
  }
  if (!regions_provider->load(*this->_sfm_data, this->_config.features_dir, regions_type)) {
      std::cerr << std::endl << "Invalid regions." << std::endl;
      return false;
  }

  PairWiseMatches map_PutativesMatches;

  // Build some alias from SfM_Data Views data:
  // - List views as a vector of filenames & image sizes
  std::vector<std::string> vec_fileNames;
  std::vector<std::pair<size_t, size_t>> vec_imagesSize;
  {
    vec_fileNames.reserve(this->_sfm_data->GetViews().size());
    vec_imagesSize.reserve(this->_sfm_data->GetViews().size());
    for (Views::const_iterator iter = this->_sfm_data->GetViews().begin();
      iter != this->_sfm_data->GetViews().end();
      ++iter)
    {
      const View * v = iter->second.get();
      vec_fileNames.push_back(stlplus::create_filespec(this->_sfm_data->s_root_path,
          v->s_Img_path));
      vec_imagesSize.push_back( std::make_pair( v->ui_width, v->ui_height) );
    }
  }

  std::cout << std::endl << " - PUTATIVE MATCHES - " << std::endl;
  // If the matches already exists, reload them
  if ((stlplus::file_exists(this->_config.matches_dir + "/matches.putative.txt")
        || stlplus::file_exists(this->_config.matches_dir + "/matches.putative.bin"))
  )
  {
    if (!(openMVG::matching::Load(map_PutativesMatches, this->_config.matches_dir + "/matches.putative.bin") ||
          openMVG::matching::Load(map_PutativesMatches,  this->_config.matches_dir + "/matches.putative.txt")) )
    {
      std::cerr << "Cannot load input matches file";
      return false;
    }
    std::cout << "\t PREVIOUS RESULTS LOADED;"
      << " #pair: " << map_PutativesMatches.size() << std::endl;
  }

  // Allocate the right Matcher according the Matching requested method
  std::unique_ptr<Matcher> collectionMatcher;
  if (this->_config.sNearestMatchingMethod == "AUTO")
  {
    if (regions_type->IsScalar())
    {
      std::cout << "Using FAST_CASCADE_HASHING_L2 matcher" << std::endl;
      collectionMatcher.reset(new Cascade_Hashing_Matcher_Regions(this->_config.fDistRatio));
    }
    else
    if (regions_type->IsBinary())
    {
      std::cout << "Using BRUTE_FORCE_HAMMING matcher" << std::endl;
      collectionMatcher.reset(new Matcher_Regions(this->_config.fDistRatio, BRUTE_FORCE_HAMMING));
    }
  }
  else
  if (this->_config.sNearestMatchingMethod == "BRUTEFORCEL2")
  {
    std::cout << "Using BRUTE_FORCE_L2 matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(this->_config.fDistRatio, BRUTE_FORCE_L2));
  }
  else
  if (this->_config.sNearestMatchingMethod == "BRUTEFORCEHAMMING")
  {
    std::cout << "Using BRUTE_FORCE_HAMMING matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(this->_config.fDistRatio, BRUTE_FORCE_HAMMING));
  }
  else
  if (this->_config.sNearestMatchingMethod == "HNSWL2")
  {
    std::cout << "Using HNSWL2 matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(this->_config.fDistRatio, HNSW_L2));
  }
  else
  if (this->_config.sNearestMatchingMethod == "ANNL2")
  {
    std::cout << "Using ANN_L2 matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(this->_config.fDistRatio, ANN_L2));
  }
  else
  if (this->_config.sNearestMatchingMethod == "CASCADEHASHINGL2")
  {
    std::cout << "Using CASCADE_HASHING_L2 matcher" << std::endl;
    collectionMatcher.reset(new Matcher_Regions(this->_config.fDistRatio, CASCADE_HASHING_L2));
  }
  else
  if (this->_config.sNearestMatchingMethod == "FASTCASCADEHASHINGL2")
  {
    std::cout << "Using FAST_CASCADE_HASHING_L2 matcher" << std::endl;
    collectionMatcher.reset(new Cascade_Hashing_Matcher_Regions(this->_config.fDistRatio));
  }
  if (!collectionMatcher)
  {
    std::cerr << "Invalid Nearest Neighbor method: " << this->_config.sNearestMatchingMethod << std::endl;
    return false;
  }

  collectionMatcher->Match(regions_provider, pairs, map_PutativesMatches);
  //---------------------------------------
  //-- Export putative matches
  //---------------------------------------
  if (!Save(map_PutativesMatches, std::string(this->_config.matches_dir + "/matches.putative.bin")))
  {
    std::cerr
      << "Cannot save computed matches in: "
      << std::string(this->_config.matches_dir + "/matches.putative.bin");
    return false;
  }

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
          GeometricFilter_HMatrix_AC(4.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching,
          bGeometric_only_guided_matching ? -1.0 : d_distance_ratio);
         geometric_matches = filter_ptr->Get_geometric_matches();
      }
      break;
      case FUNDAMENTAL_MATRIX:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_FMatrix_AC(4.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching, d_distance_ratio);
          geometric_matches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_EMatrix_AC(4.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching, d_distance_ratio);
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
          GeometricFilter_ESphericalMatrix_AC_Angular(4.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching);
         geometric_matches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX_ORTHO:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_EOMatrix_RA(2.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching, d_distance_ratio);
        geometric_matches = filter_ptr->Get_geometric_matches();
      }
      break;
    }

    this->_openmvg_storage->StoreMatches(this->_reconstruction_id, this->_config.sGeometricModel[0], geometric_matches);

    if (!Save(geometric_matches,
      std::string(this->_config.matches_dir + "/" + sGeometricMatchesFilename)))
    {
      std::cerr
          << "Cannot save computed matches in: "
          << std::string(this->_config.matches_dir + "/" + sGeometricMatchesFilename);
      return false;
    }
  }
  return true;
}

bool OpenMVGReconstructionAgent::IncrementalSFM(){
    using namespace openMVG::features;
  const std::string sImage_describer = stlplus::create_filespec(this->_config.features_dir, "image_describer", "json");
  std::unique_ptr<Regions> regions_type = Init_region_type_from_file(sImage_describer);
  if (!regions_type)
  {
    std::cerr << "Invalid: "
      << sImage_describer << " regions type file." << std::endl;
    return false;
  }

  // Features reading
  std::shared_ptr<Features_Provider> feats_provider = std::make_shared<Features_Provider>();
  if (!feats_provider->load(*this->_sfm_data, this->_config.features_dir, regions_type)) {
    std::cerr << std::endl
      << "Invalid features." << std::endl;
    return false;
  }

  std::shared_ptr<Matches_Provider> matches_provider = std::make_shared<Matches_Provider>();
  matches_provider->pairWise_matches_ = this->_openmvg_storage->GetMatches(this->_reconstruction_id);
  this->_sfm_data = this->_openmvg_storage->GetSFMData(this->_reconstruction_id, 
                                                       ESfM_Data::VIEWS | ESfM_Data::INTRINSICS | ESfM_Data::EXTRINSICS);
  
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
    this->_config.sfm_dir);

  // Configure the features_provider & the matches_provider
  sfmEngine.SetFeaturesProvider(feats_provider.get());
  sfmEngine.SetMatchesProvider(matches_provider.get());

  // Configure reconstruction parameters
  const cameras::Intrinsic_Parameter_Type intrinsic_refinement_options =
    cameras::StringTo_Intrinsic_Parameter_Type(this->_config.sIntrinsic_refinement_options);
  sfmEngine.Set_Intrinsics_Refinement_Type(intrinsic_refinement_options);
  sfmEngine.SetUnknownCameraType(EINTRINSIC(this->_config.i_User_camera_model));
  sfmEngine.Set_Use_Motion_Prior(this->_config.b_use_motion_priors);
  sfmEngine.SetTriangulationMethod(static_cast<ETriangulationMethod>(this->_config.triangulation_method));

  if (sfmEngine.Process())
  {
    std::cout << "...Export SfM_Data to disk." << std::endl;
    
    *this->_sfm_data = sfmEngine.Get_SfM_Data();  

    for(auto view_it : this->_sfm_data->GetViews()){
      std::shared_ptr<View> v = view_it.second;
      if(v->id_pose != -1){
        Pose3 pose = this->_sfm_data->poses[v->id_pose];
        this->_openmvg_storage->StorePoseUpdateView(this->_reconstruction_id, v, pose);
      }
    }

    Save(*this->_sfm_data,
      stlplus::create_filespec(this->_config.sfm_dir, "sfm_data", ".bin"),
      ESfM_Data(ALL));
    Save(*this->_sfm_data,
      stlplus::create_filespec(this->_config.sfm_dir, "sfm_data_2", ".json"),
      ESfM_Data(ALL));
  }else{
    std::cout << "...SFM process failed" << std::endl;
    this->_sfm_data->poses.clear();
    return false;
  }
  return true;
}

bool OpenMVGReconstructionAgent::ComputeStructure(){
  // Init the regions_type from the image describer file (used for image regions extraction)
  const std::string sImage_describer = stlplus::create_filespec(this->_config.features_dir, "image_describer", "json");
  std::unique_ptr<Regions> regions_type = Init_region_type_from_file(sImage_describer);
  if (!regions_type)
  {
    std::cerr << "Invalid: "
      << sImage_describer << " regions type file." << std::endl;
    return false;
  }

  // Prepare the Regions provider
  std::shared_ptr<Regions_Provider> regions_provider;
  if (this->_config.ui_max_cache_size == 0)
  {
    // Default regions provider (load & store all regions in memory)
    regions_provider = std::make_shared<Regions_Provider>();
  }
  else
  {
    // Cached regions provider (load & store regions on demand)
    regions_provider = std::make_shared<Regions_Provider_Cache>(this->_config.ui_max_cache_size);
  }

  if (!regions_provider->load(*this->_sfm_data, this->_config.features_dir, regions_type)) {
    std::cerr << std::endl
      << "Invalid regions." << std::endl;
    return false;
  }
    //--
    //- Pair selection method:
    //  - geometry guided -> camera frustum intersection,
    //  - putative matches guided (photometric matches)
    //     (keep pairs that have valid Intrinsic & Pose ids).
    //--
    PairWiseMatches matches = this->_openmvg_storage->GetMatches(this->_reconstruction_id);
    Pair_Set pairs = getPairs(matches);
        // Keep only Pairs that belong to valid view indexes.
    const std::set<IndexT> valid_viewIdx = Get_Valid_Views(*this->_sfm_data);
    pairs = Pair_filter(pairs, valid_viewIdx);
    

    //------------------------------------------
    // Compute Structure from known camera poses
    //------------------------------------------
    SfM_Data_Structure_Estimation_From_Known_Poses structure_estimator(this->_config.dMax_reprojection_error);
    structure_estimator.run(*this->_sfm_data, pairs, regions_provider,
        static_cast<ETriangulationMethod>(this->_config.triangulation_method));

          std::cout
    << "\n#landmark found: " << this->_sfm_data->GetLandmarks().size() << std::endl;
  
    regions_provider.reset(); // Regions are not longer needed.
    RemoveOutliers_AngleError(*this->_sfm_data, 2.0);

    std::cout
    << "Found a this->_sfm_data scene with:\n"
    << " #views: " << this->_sfm_data->GetViews().size() << "\n"
    << " #poses: " << this->_sfm_data->GetPoses().size() << "\n"
    << " #intrinsics: " << this->_sfm_data->GetIntrinsics().size() <<  "\n"
    << " #tracks: " << this->_sfm_data->GetLandmarks().size()
    << std::endl;

  if (stlplus::extension_part(this->_config.sOutFile) != "ply") {
    Save(*this->_sfm_data,
      stlplus::create_filespec(
        stlplus::folder_part(this->_config.sOutFile),
        stlplus::basename_part(this->_config.sOutFile), "ply"),
      ESfM_Data(ALL));
  }

  if (Save(*this->_sfm_data, this->_config.sOutFile, ESfM_Data(ALL)))
  {
    return true;
  }
  return false;
}

openMVG::Pair_Set OpenMVGReconstructionAgent::_GatherMatchesToCompute(const std::string& new_image_path){
    LOG(INFO) << "Gathering matches for new image: " << new_image_path;
    
    std::set<IndexT> new_ids;
    openMVG::Pair_Set matches_to_compute;

    if(this->_sfm_data->GetViews().size() < 2){
      return openMVG::Pair_Set();
    }

    for(std::pair<openMVG::IndexT, std::shared_ptr<openMVG::sfm::View>> e : this->_sfm_data->GetViews()){
        std::shared_ptr<openMVG::sfm::View> view = e.second;
        std::string full_path = this->_sfm_data->s_root_path + "/" + view->s_Img_path;
        if(full_path.compare(new_image_path) == 0){
            new_ids.insert(view->id_view);
        }
    }

    for(int new_id : new_ids){
        for(std::pair<openMVG::IndexT, std::shared_ptr<openMVG::sfm::View>> e : this->_sfm_data->GetViews()){
            std::shared_ptr<View> view = e.second;
            if(new_id != view->id_view){
                matches_to_compute.insert(std::pair<openMVG::IndexT,openMVG::IndexT>(view->id_view, new_id));
            }
         }
    }
    return matches_to_compute;
}

void OpenMVGReconstructionAgent::Load(const std::string& sfm_data_path){
    if (!openMVG::sfm::Load(*this->_sfm_data, sfm_data_path, openMVG::sfm::ESfM_Data(openMVG::sfm::ESfM_Data::ALL))){
        LOG(ERROR) << "Could not read smf file";
    }
}