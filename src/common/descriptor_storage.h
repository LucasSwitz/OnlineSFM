#pragma once

#include <vector>
#include <unordered_map>
#include "types.h"

template<typename T>
class DescriptorStorage {
    public:
        virtual void Store(const std::string& image_id, T feature) = 0;
        virtual std::unordered_map<T, unsigned long> GetAllDescriptors(const std::string& image_id) = 0;
        virtual void DeleteAllDescriptors(const std::string& image_id) = 0;
        virtual void StoreAll(const std::string& image_id, const std::vector<T, Eigen::aligned_allocator<T>>& descriptors) = 0;
        virtual std::vector<std::string> GetImagesWithDescriptors(const std::vector<T, Eigen::aligned_allocator<T>>& desc) = 0;
        virtual std::unordered_map<T, unsigned long> GetDescriptorFrequencies(const std::vector<T, Eigen::aligned_allocator<T>>& desc) = 0;
        virtual std::unordered_map<std::string, std::unordered_map<T, unsigned long>> GetAllWithCommonDescriptors(const std::string& image_id) = 0;
        virtual unsigned int GetImageCount() = 0;
        virtual std::unordered_map<T, unsigned long> GetGlobalDescriptorFrequenciesByImage(const std::string& image) = 0;
};