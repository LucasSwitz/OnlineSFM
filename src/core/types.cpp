#include "types.h"
#include <boost/algorithm/string/join.hpp>
#include <functional>
#include <numeric>

float SIFT_Descriptor_weight_map_L2_Distance(const SIFT_Descriptor_weight_map &m1, const SIFT_Descriptor_weight_map &m2)
{
    float distance_2 = 0.0f;
    for (auto e : m1)
    {
        auto desc = e.first;
        float weight_1 = e.second;
        float weight_2 = 0.0;
        if (m2.find(desc) != m2.end())
        {
            weight_2 = m2.at(desc);
        }
        distance_2 += (weight_1 - weight_2) * (weight_1 - weight_2);
    }
    for (auto e : m2)
    {
        auto desc = e.first;
        auto weight = e.second;
        if (m1.find(desc) == m1.end())
        {
            distance_2 += weight * weight;
        }
    }
    return distance_2;
}

float SIFT_Descriptor_weight_map_dot(const SIFT_Descriptor_weight_map &m1, const SIFT_Descriptor_weight_map &m2)
{
    float dot = 0.0f;
    auto &smaller = m1.size() < m2.size() ? m1 : m2;
    auto &larger = m2.size() > m1.size() ? m2 : m1;
    for (auto e : smaller)
    {
        auto desc = e.first;
        float weight_1 = e.second;
        float weight_2 = 0.0;
        if (larger.find(desc) != larger.end())
        {
            weight_2 = larger.at(desc);
        }
        dot += weight_1 * weight_2;
    }
    return dot;
}

float SIFT_Descriptor_weight_map_mag(const SIFT_Descriptor_weight_map &m1)
{
    float mag = 0.0f;
    for (auto e : m1)
    {
        mag += e.second * e.second;
    }
    return sqrt(mag);
}

float SIFT_Descriptor_weight_map_norm_dot(const SIFT_Descriptor_weight_map &m1, const SIFT_Descriptor_weight_map &m2)
{
    return SIFT_Descriptor_weight_map_dot(m1, m2) / (SIFT_Descriptor_weight_map_mag(m1) * SIFT_Descriptor_weight_map_mag(m2));
}

std::string split_SIFT_Vector(const SIFT_Vector &descs)
{
    std::vector<std::string> desc_strs;
    transform(descs.begin(), descs.end(), desc_strs.begin(), [](const SIFT_Descriptor &d) -> std::string {
        return std::string((char *)d.data(), 128);
    });
    return boost::algorithm::join(desc_strs, ",");
}

SIFT_Descriptor_count_map SIFT_Vector_to_Sparse_Vector(const SIFT_Vector &v)
{
    SIFT_Descriptor_count_map cm;
    for (auto d : v)
    {
        if (cm.find(d) == cm.end())
        {
            cm[d] = 0;
        }
        cm[d]++;
    }
    return cm;
}