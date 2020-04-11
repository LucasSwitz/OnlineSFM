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

#include "configuration_adapter.h"
#include "descriptor_storage.h"
#include "image_storage.h"

#include "types.h"

typedef struct OpenMVGReconstructionAgentConfig {
    std::string features_dir = "",
    sfm_dir = "",
    matches_dir = "",
    root_path = "",
    sOutFile = "";
};

class OpenMVGReconstructionAgent : public ReconstructionAgent{
    public:
        OpenMVGReconstructionAgent(const std::string& reconstruction_id, 
                                   std::shared_ptr<CameraIntrinsicsStorage> intrinsics_storage, 
                                   std::shared_ptr<OpenMVGStorageAdapter> openmvg_storage,
                                   std::shared_ptr<ConfigurationAdapter> configuration_adapter,
                                   std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> descriptor_storage,
                                   std::shared_ptr<ImageStorageAdapter> image_storage);
        bool IncrementalSFM(const std::set<std::string>& new_images){}
        bool AddImage(const std::string& image_path);
        bool ComputeMatches(const std::set<std::string>& image_pathes);
        bool ComputeFeatures(const std::set<std::string>& image_id);
        bool IncrementalSFM();
        bool ComputeStructure();
        void Load(const std::string& sfm_data);
        void SetConfig(void* config);
        ~OpenMVGReconstructionAgent();
    private:
        openMVG::Pair_Set _GatherMatchesToCompute(const std::set<std::string>& new_image_paths);
        bool _GenerateImageFeatures(const std::string& image_path);
        OpenMVGReconstructionAgentConfig _config;
        std::unique_ptr<openMVG::sfm::SfM_Data> _sfm_data;
        std::shared_ptr<ConfigurationAdapter> _configuration_adapter;
        std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> _descriptor_storage;
        std::shared_ptr<ImageStorageAdapter> _image_storage;
        std::shared_ptr<CameraIntrinsicsStorage> _intrinsics_storage = nullptr;
        std::shared_ptr<OpenMVGStorageAdapter> _openmvg_storage = nullptr;
        std::string _reconstruction_id;
};