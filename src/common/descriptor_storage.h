#pragma once

#include <vector>

template<typename T>
class DescriptorStorage {
    public:
        virtual void Store(const std::string& image_id, T feature) = 0;
        virtual std::vector<T, Eigen::aligned_allocator<T>> GetAllDescriptors(const std::string& image_id) = 0;
        virtual void DeleteAllDescriptors(const std::string& image_id) = 0;
        virtual void StoreAll(const std::string& image_id, const std::vector<T, Eigen::aligned_allocator<T>>& descriptors) = 0;
        virtual std::vector<std::string> GetImagesWithDescriptor(const T& desc) = 0;
        virtual std::vector<unsigned long> GetDescriptorFrequencies(const std::vector<T> desc) = 0;
};