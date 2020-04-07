#pragma once

#include "types.h"
#include <vector>
#include <unordered_map>

class Ranker{
    public:
        virtual ~Ranker(){};
        virtual std::vector<std::string> Rank(const std::string& query, 
                                              const std::unordered_map<std::string, SIFT_Descriptor_count_map>& res) = 0;
};