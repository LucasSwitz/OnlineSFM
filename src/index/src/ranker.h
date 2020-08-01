#pragma once

#include "types.h"
#include <vector>
#include <set>
#include "index.pb.h"

class Ranker
{
public:
    virtual ~Ranker(){};
    virtual std::vector<std::tuple<std::string, float>> Rank(const std::string &reconstruction_id,
                                                             const std::string &query,
                                                             const std::set<std::string> &res,
                                                             ClosestNDetails *details = nullptr) = 0;
    virtual void Score(const std::string &reconstruction_id, const std::string &image_id) = 0;
};