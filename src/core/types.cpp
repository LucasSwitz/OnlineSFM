#include "types.h"
#include <boost/algorithm/string/join.hpp>
#include <functional>
#include <numeric>

float SIFT_Descriptor_weight_map_L2_Distance(const SIFT_Descriptor_weight_map& m1, const SIFT_Descriptor_weight_map& m2){
    auto& bigger = m1.size() > m2.size() ? m1 : m2;
    auto& smaller = m1 == bigger ? m2 : m1;
    float distance_2 = 0.0f;
    for(auto e : bigger){
        auto desc = e.first; 
        float weight_1 = e.second;
        float weight_2 = 0.0;
        if(smaller.find(desc) != smaller.end()){
            weight_2 = smaller.at(desc);
        }
        distance_2 += (weight_1-weight_2)*(weight_1-weight_2);
    }
    for(auto e : smaller){
        auto desc = e.first;
        auto weight = e.second;
        if(bigger.find(desc) == bigger.end()){
            distance_2+=weight;
        }
    }
    return sqrt(distance_2);
}

std::string split_SIFT_Vector(const SIFT_Vector& descs){
    std::vector<std::string> desc_strs;
    transform(descs.begin(), descs.end(), desc_strs.begin(), [](const SIFT_Descriptor& d) -> std::string{
            return std::string((char*) d.data(), 128);
    });
    return boost::algorithm::join(desc_strs, ",");
}

SIFT_Descriptor_count_map SIFT_Vector_to_Sparse_Vector(const SIFT_Vector& v){
    SIFT_Descriptor_count_map cm;
    for(auto d : v){
        if(cm.find(d) == cm.end()){
            cm[d] = 0;
        }
        cm[d]++;
    }
    return cm;
}