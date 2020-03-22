#pragma once

#include "openMVG/sfm/sfm_view.hpp"
#include "openMVG/cameras/Camera_Intrinsics.hpp"
#include "openMVG/matching/indMatch.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/geometry/pose3.hpp"

#include <memory>


typedef struct OpenMVGMetadataT{
    std::string root_path;
} OpenMVGMetadata;

class OpenMVGStorageAdapter {
    public:
        virtual openMVG::sfm::Views GetViews(const std::string& reconstruction_id) = 0; 
        virtual openMVG::sfm::Intrinsics GetIntrinsics(const std::string& reconstruction_id) = 0; 
        virtual void DeleteAll(const std::string& reconstruction_id) = 0;
        virtual void StoreViewAndIntrinsic(const std::string& reconstruction_id, 
                                           const std::shared_ptr<openMVG::sfm::View> view,
                                           const std::shared_ptr<openMVG::cameras::IntrinsicBase> intrinsic) = 0;
        virtual void StoreMatches(const std::string& reconstruction_id, char model, const openMVG::matching::PairWiseMatches& matches) = 0;
        virtual std::unique_ptr<openMVG::sfm::SfM_Data> GetSFMData(const std::string& reconstruction_id, 
                                                                   int mask) = 0;
        virtual openMVG::matching::PairWiseMatches GetMatches(const std::string& reconstruction_id) = 0;
        virtual void StoreMeta(const std::string& reconstruction_id, const OpenMVGMetadata& meta) = 0;
        virtual OpenMVGMetadata GetMeta(const std::string& reconstruction_id) = 0;
        virtual void StorePoseUpdateView(const std::string& reconstruction_id,
                                         const std::shared_ptr<openMVG::sfm::View> view, 
                                         const openMVG::geometry::Pose3& pose) = 0;
        virtual openMVG::sfm::Poses GetPoses(const std::string& reconstruction_id) = 0;
};