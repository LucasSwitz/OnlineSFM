#pragma once
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/matching/indMatch.hpp"
#include "openMVG/multiview/triangulation_method.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/numeric/eigen_alias_definition.hpp"
#include "openMVG/cameras/Camera_Common.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/numeric/eigen_alias_definition.hpp"
#include "openMVG/cameras/Camera_Common.hpp"
#include "openmvg_storage_adapter.h"

#include "reconstruction_agent.h"
#include "camera_intrinsics_storage.h"

typedef struct OpenMVGReconstructionAgentConfig {
    std::string features_dir = "",
    sfm_dir = "",
    matches_dir = "",
    sKmatrix;
    std::string sPriorWeights;
    std::pair<bool, openMVG::Vec3> prior_w_info = std::pair<bool, openMVG::Vec3>(false, openMVG::Vec3(1.0,1.0,1.0));
    bool b_Group_camera_model = true;
    int i_GPS_XYZ_method = 0;
    double focal_pixels = -1.0;
    
    bool bUpRight = false;
    std::string sImage_Describer_Method = "SIFT_ANATOMY"; //Using regular SIFT causes memory errors since the describer uses external threads
    bool bForce = false;
    std::string sFeaturePreset = "";
    int iNumThreads = 0;
    std::string sMatchFilename;
    std::string sIntrinsic_refinement_options = "ADJUST_ALL";
    std::string sSfMInitializer_method = "STELLAR";
    int i_User_camera_model = openMVG::cameras::EINTRINSIC::PINHOLE_CAMERA_RADIAL3;
    bool b_use_motion_priors = false;
    int triangulation_method = static_cast<int>(openMVG::ETriangulationMethod::DEFAULT);

    std::string sMatchFile;
    std::string sPairFile;
    std::string sOutFile = "";
    double dMax_reprojection_error = 4.0;
    unsigned int ui_max_cache_size = 0;

    std::string sGeometricModel = "f";
    float fDistRatio = 0.8f;
    int iMatchingVideoMode = -1;
    std::string sPredefinedPairList = "";
    std::string sNearestMatchingMethod = "AUTO";
    bool bGuided_matching = false;
    int imax_iteration = 2048;
    std::string root_path = "";
};

class OpenMVGReconstructionAgent : public ReconstructionAgent{
    public:
        OpenMVGReconstructionAgent(const std::string& reconstruction_id, CameraIntrinsicsStorage* intrinsics_storage, OpenMVGStorageAdapter* openmvg_storage);
        bool IncrementalSFM(const std::set<std::string>& new_images){}
        bool AddImage(const std::string& image_path);
        bool GenerateMatches(const std::string& image_path);
        bool IncrementalSFM();
        bool ComputeStructure();
        void Load(const std::string& sfm_data);
        void SetConfig(const OpenMVGReconstructionAgentConfig& config);
    private:
        openMVG::Pair_Set _GatherMatchesToCompute(const std::string& new_images);
        bool _GenerateImageFeatures(const std::string& image_path);
        OpenMVGReconstructionAgentConfig _config;
        std::unique_ptr<openMVG::sfm::SfM_Data> _sfm_data;
        CameraIntrinsicsStorage* _intrinsics_storage = nullptr;
        OpenMVGStorageAdapter* _openmvg_storage = nullptr;
        std::string _reconstruction_id;
};