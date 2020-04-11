#pragma once
#include "openmvg_storage_adapter.h"
#include "sql_storage.h"

class SQLOpenMVGStorage : public OpenMVGStorageAdapter, public SQLStorage {
    public:
        SQLOpenMVGStorage(sql::Driver* driver, 
                   std::shared_ptr<sql::Connection> con,
                   const std::string& views_table,
                   const std::string& intrinsics_table,
                   const std::string& matches_table,
                   const std::string& meta_table,
                   const std::string& poses_table);
        void StoreViewAndIntrinsic(const std::string& reconstruction_id, 
                                   const std::string& image_id,
                                   const std::shared_ptr<openMVG::sfm::View> view,
                                   const std::shared_ptr<openMVG::cameras::IntrinsicBase> intrinsic);
        openMVG::sfm::Views GetViews(const std::string& reconstruction_id); 
        openMVG::sfm::Intrinsics GetIntrinsics(const std::string& reconstruction_id); 
        void DeleteAll(const std::string& reconstruction_id);        
        void StoreMatches(const std::string& reconstruction_id, 
                          char model,
                          const openMVG::matching::PairWiseMatches& matches);
        std::unique_ptr<openMVG::sfm::SfM_Data> GetSFMData(const std::string& reconstruction_id, 
                                                           int mask);
        openMVG::matching::PairWiseMatches GetMatches(const std::string& reconstruction_id);
        void StoreMeta(const std::string& reconstruction_id, const OpenMVGMetadata& meta);
        OpenMVGMetadata GetMeta(const std::string& recontruction_id);
        void StorePoseUpdateView(const std::string& reconstruction_id,
                                 const std::shared_ptr<openMVG::sfm::View> view, 
                                 const openMVG::geometry::Pose3& pose);
        openMVG::sfm::Poses GetPoses(const std::string& reconstruction_id);
        openMVG::IndexT GetViewIdxByImageID(const std::string& image_id);

    private:
        std::string _views_table;
        std::string _intrinsics_table;
        std::string _matches_table;
        std::string _meta_table;
        std::string _poses_table;
};