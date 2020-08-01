#include "static_job_cost_provider.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

static JobCost ParseJobFromJson(const json &j)
{
    JobCost jc;
    jc.exclusive_cores = j["exclusive_cores"];
    jc.ram_usage = j["ram_usage"];
    return jc;
}

JobCost StaticJobCostProvider::GetCost(const std::string &type)
{
    return this->at(type);
}

void StaticJobCostProvider::Load(const std::string &job_file_path)
{
    json j;
    std::ifstream ifs(job_file_path);
    ifs >> j;

    for (json job_json : j["jobs"])
    {
        this->insert({job_json["name"], ParseJobFromJson(job_json)});
    }
}