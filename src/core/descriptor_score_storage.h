#pragma once
#include "types.h"

class DescriptorScoreStorage
{
public:
    virtual void Store(const std::string &reconstruction_id, const std::string &image_id, const SIFT_Descriptor_weight_map &scores) = 0;
    virtual SIFT_Descriptor_weight_map Get(const std::string &reconstruction_id, const std::string &image_id) = 0;
};