#pragma once

#include "job.h"

class JobIndexService
{
public:
    JobIndexService();
    bool Index(const std::string &reconstruction_id, const std::string &image_id);
    bool Score(const std::string &reconstruction_id, const std::vector<std::string> &images);
    bool Score(const std::string &reconstruction_id, const std::string &image_id);

private:
    std::shared_ptr<JobQueue> _job_queue;
};