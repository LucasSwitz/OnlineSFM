#pragma once
#include "openMVG/features/descriptor.hpp"
#include <unordered_map>

typedef typename openMVG::features::Descriptor<unsigned char, 128> SIFT_Descriptor;
typedef typename std::vector<SIFT_Descriptor, Eigen::aligned_allocator<SIFT_Descriptor>> SIFT_Vector;
typedef typename std::unordered_map<SIFT_Descriptor, unsigned long> SIFT_Descriptor_count_map;
typedef typename std::unordered_map<SIFT_Descriptor, float> SIFT_Descriptor_weight_map;

namespace std {

  template <>
  struct hash<SIFT_Descriptor>
  {
    std::size_t operator()(const SIFT_Descriptor& k) const
    {
      return ((std::hash<std::string>()(std::string((char*) k.data(), 128))));
    }
  };

};

double SIFT_Descriptor_weight_map_L2_Distance(const SIFT_Descriptor_weight_map& m1, const SIFT_Descriptor_weight_map& m2);
std::string split_SIFT_Vector(const SIFT_Vector& descs);