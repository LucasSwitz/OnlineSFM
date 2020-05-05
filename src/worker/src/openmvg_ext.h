#pragma once
#include <openMVG/sfm/pipelines/sfm_regions_provider.hpp>
#include <openMVG/sfm/pipelines/sfm_features_provider.hpp>
#include <openMVG/features/regions.hpp>
#include "regions_storage.h"
#include "openmvg_storage_adapter.h"

namespace openMVG{
namespace sfm {
    template<typename T>
    struct RegionsProviderFromStorage : public Regions_Provider {
        public:
            RegionsProviderFromStorage(std::shared_ptr<RegionsStorage<T>> regions_storage, 
                                       std::shared_ptr<OpenMVGStorageAdapter> openmvg_storage,
                                       std::unique_ptr<openMVG::features::Regions> regions_type) : _regions_storage(regions_storage),
                                                                                _openmvg_storage(openmvg_storage){
                    this->region_type_ = std::move(regions_type);
            }
            void load_from_storage(const std::string& reconstruction_id){
                 auto regions_map = this->_regions_storage->GetAllRegionsReconstruction(reconstruction_id);
                 std::vector<std::string> image_ids;
                 std::for_each(regions_map.begin(), regions_map.end(), [&image_ids](auto e) mutable{
                    image_ids.push_back(e.first);
                 });
                auto images_to_view_idx = this->_openmvg_storage->GetAllViewIdxByImageID(image_ids);
                for(auto e : *images_to_view_idx){
                    this->cache_[e.second] = std::move(regions_map[e.first]);
                }
            }
        private:
            std::shared_ptr<RegionsStorage<T>> _regions_storage;
            std::shared_ptr<OpenMVGStorageAdapter> _openmvg_storage;

    };

    template<typename T>
    struct FeaturesProviderFromStorage : public Features_Provider {
        public:
            FeaturesProviderFromStorage(std::shared_ptr<RegionsStorage<T>> regions_storage, 
                                       std::shared_ptr<OpenMVGStorageAdapter> openmvg_storage) : _regions_storage(regions_storage),
                                                                                                 _openmvg_storage(openmvg_storage){
            }
            void load_from_storage(const std::string& reconstruction_id){
                 auto regions_map = this->_regions_storage->GetAllRegionsReconstruction(reconstruction_id);
                 std::vector<std::string> image_ids;
                 std::for_each(regions_map.begin(), regions_map.end(), [&image_ids](auto e) mutable{
                    image_ids.push_back(e.first);
                 });
                auto images_to_view_idx = this->_openmvg_storage->GetAllViewIdxByImageID(image_ids);
                for(auto e : *images_to_view_idx){
                    this->feats_per_view[e.second] = regions_map[e.first]->GetRegionsPositions();
                }
            }
        private:
            std::shared_ptr<RegionsStorage<T>> _regions_storage;
            std::shared_ptr<OpenMVGStorageAdapter> _openmvg_storage;

    };
 }
}