#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include "types.h"

template <typename T>
class DescriptorStorage
{
public:
    virtual void Store(const std::string &reconstruction_id, const std::string &image_id, SIFT_Descriptor_count_map &descriptors) = 0;
    virtual std::unordered_map<T, unsigned int> GetAllDescriptors(const std::string &reconstruction_id, const std::string &image_id) = 0;
    virtual void DeleteAllDescriptors(const std::string &image_id) = 0;
    virtual std::set<std::string> GetAllWithCommonDescriptors(const std::string &reconstruction_id, const std::string &image_id, int page_size = 0, std::string *page_start = nullptr) = 0;
    virtual unsigned int GetImageCount(const std::string &reconstruction_id) = 0;
    virtual std::shared_ptr<std::unordered_map<T, unsigned int>> GetGlobalDescriptorFrequencies(const std::string &reconstruction_id) = 0;
    virtual ~DescriptorStorage(){};
};