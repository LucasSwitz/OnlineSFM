#pragma once

#include "reconstruction_agent.h"
#include "worker.grpc.pb.h"
#include "job.h"

class JobRemoteReconstructionAgent : public ReconstructionAgent
{
public:
    JobRemoteReconstructionAgent(const std::string &reconstruction_id,
                                 const std::shared_ptr<JobQueue> job_queue);
    bool AddImage(const std::string &image_id);
    bool ComputeFeatures(const std::set<std::string> &image_paths);
    bool ComputeMatches(const std::set<std::string> &image_paths);
    bool IncrementalSFM();
    bool ComputeStructure();
    bool MVS();

private:
    std::string _reconstruction_id;
    std::shared_ptr<JobQueue> _job_queue;
};