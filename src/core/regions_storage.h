#pragma once

#include <openMVG/features/regions.hpp>
#include <memory>
#include <unordered_map>

template <typename T>
class RegionsStorage
{
public:
        typedef typename std::unordered_map<std::string, std::shared_ptr<T>> image_region_map;
        typedef typename T::DescsT DescsT;
        virtual void Store(const std::string &reconstruction_id,
                           const std::string &image_id,
                           std::shared_ptr<T> regions) = 0;
        virtual std::shared_ptr<T> GetAllRegions(const std::string &reconstruction_id,
                                                 const std::string &image_id) = 0;
        virtual image_region_map GetAllRegionsReconstruction(const std::string &reconstruction_id) = 0;
        virtual DescsT GetAllDescriptors(const std::string &reconstruction_id, const std::string &image_id) = 0;
        virtual image_region_map GetAllRegionsReconstructionForMatches(const std::string &reconstruction_id,
                                                                       const std::string &views_table,
                                                                       const std::string &matches_table) = 0;
};