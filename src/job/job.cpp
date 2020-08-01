#include "job.h"

JobResult JobQueue::ParseJobResult(const std::string &data)
{
    json j = json::parse(data);
    JobResult jr(j["id"], j["type"]);
    jr.status = j["status"];
    jr.info = j["info"];
    return jr;
}

std::string SerializeJobResult(const JobResult &jr)
{
    json j;
    j["id"] = jr.job_id;
    j["status"] = jr.status;
    j["info"] = jr.info;
    j["type"] = jr.type;
    std::stringstream ss;
    ss << j;
    return ss.str();
}