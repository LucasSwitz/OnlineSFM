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
#include "regions_storage.h"
#include "image_storage.h"

#include "types.h"

#include <openMVG/features/regions_factory.hpp>
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer.hpp"
#include "image_storage_openmvg_adapter.h"
#include "sparse_storage.h"
#include "obj_storage.h"
#include <cppconn/driver.h>

class OpenMVGReconstructionAgent : public ReconstructionAgent
{
public:
    OpenMVGReconstructionAgent(const std::string &reconstruction_id,
                               std::shared_ptr<CameraIntrinsicsStorage> intrinsics_storage,
                               std::shared_ptr<OpenMVGStorageAdapter> openmvg_storage,
                               std::shared_ptr<ConfigurationAdapter> configuration_adapter,
                               std::shared_ptr<RegionsStorage<openMVG::features::SIFT_Anatomy_Image_describer::Regions_type>> regions_storage,
                               std::shared_ptr<ImageStorageAdapter> image_storage,
                               std::shared_ptr<SparseStorageAdapter> sparse_storage,
                               std::shared_ptr<OBJStorageAdapter> obj_storage);
    bool IncrementalSFM(const std::set<std::string> &new_images) {}
    bool AddImage(const std::string &image_path);
    bool ComputeMatches(const std::set<std::string> &image_pathes);
    bool ComputeFeatures(const std::set<std::string> &image_id);
    bool IncrementalSFM();
    bool ComputeStructure();
    void DensifyPointCloud(const std::string &reconstruction_id);
    bool ExportToMVS(std::vector<char> &mvs_raw);
    bool ComputeDepthMap(const std::string image_id);
    ~OpenMVGReconstructionAgent();
    bool MVS();

private:
    bool ExportToMVS(const std::string &path);
    openMVG::Pair_Set _GatherMatchesToCompute(const std::set<std::string> &new_image_paths, std::vector<std::string> &relevant_matches);
    bool _GenerateImageFeatures(const std::string &image_path);
    std::unique_ptr<openMVG::sfm::SfM_Data> _sfm_data;
    std::shared_ptr<ConfigurationAdapter> _configuration_adapter;
    std::shared_ptr<RegionsStorage<openMVG::features::SIFT_Anatomy_Image_describer::Regions_type>> _regions_storage;
    std::shared_ptr<ImageStorageAdapter> _image_storage;
    std::shared_ptr<CameraIntrinsicsStorage> _intrinsics_storage = nullptr;
    std::shared_ptr<OpenMVGStorageAdapter> _openmvg_storage = nullptr;
    std::shared_ptr<SparseStorageAdapter> _sparse_storage = nullptr;
    std::shared_ptr<OBJStorageAdapter> _obj_storage = nullptr;
    ImageStorageOpenMVGAdapter _openmvg_images_storage;
    std::string _reconstruction_id;
};