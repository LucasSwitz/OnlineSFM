#pragma once
#include "openMVG/features/descriptor.hpp"

typedef typename openMVG::features::Descriptor<unsigned char, 128> SIFT_Descriptor;
typedef typename std::vector<SIFT_Descriptor, Eigen::aligned_allocator<SIFT_Descriptor>> SIFT_Vector;