#include "matcher.h"
#include "openMVG/graph/graph.hpp"
#include "openMVG/graph/graph_stats.hpp"
#include "openMVG/features/akaze/image_describer_akaze.hpp"
#include "openMVG/features/descriptor.hpp"
#include "openMVG/features/feature.hpp"
#include "openMVG/matching/indMatch.hpp"
#include "openMVG/matching/indMatch_utils.hpp"
#include "openMVG/matching_image_collection/Matcher_Regions.hpp"
#include "openMVG/matching_image_collection/Cascade_Hashing_Matcher_Regions.hpp"
#include "openMVG/matching_image_collection/GeometricFilter.hpp"
#include "openMVG/sfm/pipelines/sfm_features_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider_cache.hpp"
#include "openMVG/matching_image_collection/F_ACRobust.hpp"
#include "openMVG/matching_image_collection/E_ACRobust.hpp"
#include "openMVG/matching_image_collection/E_ACRobust_Angular.hpp"
#include "openMVG/matching_image_collection/Eo_Robust.hpp"
#include "openMVG/matching_image_collection/H_ACRobust.hpp"
#include "openMVG/matching_image_collection/Pair_Builder.hpp"
#include "openMVG/matching/pairwiseAdjacencyDisplay.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/stl/stl.hpp"
#include "openMVG/system/timer.hpp"
#include <memory>

using namespace openMVG;
using namespace openMVG::matching;
using namespace openMVG::robust;
using namespace openMVG::sfm;
using namespace openMVG::matching_image_collection;
using namespace std;

enum EGeometricModel
{
  FUNDAMENTAL_MATRIX = 0,
  ESSENTIAL_MATRIX   = 1,
  HOMOGRAPHY_MATRIX  = 2,
  ESSENTIAL_MATRIX_ANGULAR = 3,
  ESSENTIAL_MATRIX_ORTHO = 4
};

ContinuousMatcher::ContinuousMatcher(const MatcherConfig& config) : _config(config){

}

bool ContinuousMatcher::Match(Pair_Set& pairs){
  std::shared_ptr<Regions_Provider> regions_provider;
  EGeometricModel eGeometricModelToCompute = FUNDAMENTAL_MATRIX;
  std::string sGeometricMatchesFilename = "";

  SfM_Data sfm_data;
  if (!Load(sfm_data, this->_config.sSfM_Data_Filename, ESfM_Data(VIEWS|INTRINSICS))) {
    std::cerr << std::endl
      << "The input SfM_Data file \""<< this->_config.sSfM_Data_Filename << "\" cannot be read." << std::endl;
    return EXIT_FAILURE;
  }

  using namespace openMVG::features;
  const std::string sImage_describer = stlplus::create_filespec(this->_config.sMatchesDirectory, "image_describer", "json");
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
  if (!regions_provider->load(sfm_data, this->_config.sMatchesDirectory, regions_type)) {
      std::cerr << std::endl << "Invalid regions." << std::endl;
      return false;
  }

  PairWiseMatches map_PutativesMatches;

  // Build some alias from SfM_Data Views data:
  // - List views as a vector of filenames & image sizes
  std::vector<std::string> vec_fileNames;
  std::vector<std::pair<size_t, size_t>> vec_imagesSize;
  {
    vec_fileNames.reserve(sfm_data.GetViews().size());
    vec_imagesSize.reserve(sfm_data.GetViews().size());
    for (Views::const_iterator iter = sfm_data.GetViews().begin();
      iter != sfm_data.GetViews().end();
      ++iter)
    {
      const View * v = iter->second.get();
      vec_fileNames.push_back(stlplus::create_filespec(sfm_data.s_root_path,
          v->s_Img_path));
      vec_imagesSize.push_back( std::make_pair( v->ui_width, v->ui_height) );
    }
  }

  std::cout << std::endl << " - PUTATIVE MATCHES - " << std::endl;
  // If the matches already exists, reload them
  if ((stlplus::file_exists(this->_config.sMatchesDirectory + "/matches.putative.txt")
        || stlplus::file_exists(this->_config.sMatchesDirectory + "/matches.putative.bin"))
  )
  {
    if (!(Load(map_PutativesMatches, this->_config.sMatchesDirectory + "/matches.putative.bin") ||
          Load(map_PutativesMatches,  this->_config.sMatchesDirectory + "/matches.putative.txt")) )
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
  if (!Save(map_PutativesMatches, std::string(this->_config.sMatchesDirectory + "/matches.putative.bin")))
  {
    std::cerr
      << "Cannot save computed matches in: "
      << std::string(this->_config.sMatchesDirectory + "/matches.putative.bin");
    return false;
  }
  
  //---------------------------------------
  // b. Geometric filtering of putative matches
  //    - AContrario Estimation of the desired geometric model
  //    - Use an upper bound for the a contrario estimated threshold
  //---------------------------------------

  std::unique_ptr<ImageCollectionGeometricFilter> filter_ptr(
    new ImageCollectionGeometricFilter(&sfm_data, regions_provider));

  if (filter_ptr)
  {
    system::Timer timer;
    const double d_distance_ratio = 0.6;

    PairWiseMatches map_GeometricMatches;
    switch (eGeometricModelToCompute)
    {
      case HOMOGRAPHY_MATRIX:
      {
        const bool bGeometric_only_guided_matching = true;
        filter_ptr->Robust_model_estimation(
          GeometricFilter_HMatrix_AC(4.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching,
          bGeometric_only_guided_matching ? -1.0 : d_distance_ratio);
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
      case FUNDAMENTAL_MATRIX:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_FMatrix_AC(4.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching, d_distance_ratio);
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_EMatrix_AC(4.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching, d_distance_ratio);
        map_GeometricMatches = filter_ptr->Get_geometric_matches();

        //-- Perform an additional check to remove pairs with poor overlap
        std::vector<PairWiseMatches::key_type> vec_toRemove;
        for (const auto & pairwisematches_it : map_GeometricMatches)
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
          map_GeometricMatches.erase(pair_to_remove_it);
        }
      }
      break;
      case ESSENTIAL_MATRIX_ANGULAR:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_ESphericalMatrix_AC_Angular(4.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching);
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
      case ESSENTIAL_MATRIX_ORTHO:
      {
        filter_ptr->Robust_model_estimation(
          GeometricFilter_EOMatrix_RA(2.0, this->_config.imax_iteration),
          map_PutativesMatches, this->_config.bGuided_matching, d_distance_ratio);
        map_GeometricMatches = filter_ptr->Get_geometric_matches();
      }
      break;
    }

    if (!Save(map_GeometricMatches,
      std::string(this->_config.sMatchesDirectory + "/" + sGeometricMatchesFilename)))
    {
      std::cerr
          << "Cannot save computed matches in: "
          << std::string(this->_config.sMatchesDirectory + "/" + sGeometricMatchesFilename);
      return false;
    }
  }
}