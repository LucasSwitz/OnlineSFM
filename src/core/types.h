#pragma once
#include "openMVG/features/descriptor.hpp"
#include <unordered_map>

typedef typename openMVG::features::Descriptor<unsigned char, 128> SIFT_Descriptor;

struct desc_less_than
{
  bool operator()(const SIFT_Descriptor &d1,
                  const SIFT_Descriptor &d2) const
  {
    for (int i = 0; i < 128; i++)
    {
      if (d1[i] == d2[i])
        continue;
      return d1[i] < d2[i];
    }
    return true;
  }
};

typedef typename std::vector<SIFT_Descriptor, Eigen::aligned_allocator<SIFT_Descriptor>> SIFT_Vector;
typedef typename std::unordered_map<SIFT_Descriptor, unsigned int> SIFT_Descriptor_count_map;
typedef typename std::map<SIFT_Descriptor, unsigned int, desc_less_than> SIFT_Descriptor_count_ordered_map;
typedef typename std::unordered_map<SIFT_Descriptor, double> SIFT_Descriptor_weight_map;

#define NULL_RECONSTRUCTION_ID "00000000-0000-0000-0000-000000000000"

namespace std
{

  template <>
  struct hash<SIFT_Descriptor>
  {
    std::size_t operator()(const SIFT_Descriptor &k) const
    {
      return ((std::hash<std::string>()(std::string((char *)k.data(), 128))));
    }
  };

}; // namespace std

float SIFT_Descriptor_weight_map_L2_Distance(const SIFT_Descriptor_weight_map &m1, const SIFT_Descriptor_weight_map &m2);
std::string split_SIFT_Vector(const SIFT_Vector &descs);
SIFT_Descriptor_count_map SIFT_Vector_to_Sparse_Vector(const SIFT_Vector &v);
float SIFT_Descriptor_weight_map_norm_dot(const SIFT_Descriptor_weight_map &m1, const SIFT_Descriptor_weight_map &m2);