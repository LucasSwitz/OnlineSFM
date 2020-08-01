#pragma once

#include "job_cost.h"
#include <unordered_map>

class StaticJobCostProvider : public std::unordered_map<std::string, JobCost>, public JobCostProvider
{
public:
    JobCost GetCost(const std::string &type);
    void Load(const std::string &jobs_file_path);
};