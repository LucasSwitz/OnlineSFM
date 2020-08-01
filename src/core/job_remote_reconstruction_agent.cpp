#include "job_remote_reconstruction_agent.h"
#include <grpcpp/grpcpp.h>
#include "worker.pb.h"
#include <glog/logging.h>
#include "config.h"
#include "grpc_service_provider.h"
#include "jobs.h"

using namespace grpc;

JobRemoteReconstructionAgent::JobRemoteReconstructionAgent(const std::string &reconstruction_id,
                                                           const std::shared_ptr<JobQueue> job_queue) : _reconstruction_id(reconstruction_id),
                                                                                                        _job_queue(job_queue)
{
}

bool JobRemoteReconstructionAgent::ComputeFeatures(const std::set<std::string> &image_ids)
{
    std::vector<ComputeFeaturesJob> jobs;
    std::transform(image_ids.begin(), image_ids.end(), back_inserter(jobs), [this](const std::string &image_id) {
        ComputeFeaturesJob j;
        j.reconstruction_id = this->_reconstruction_id;
        j.image_id = image_id;
        return j;
    });
    auto results = this->_job_queue->QueueAndWait(jobs);
    return std::all_of(results.begin(), results.end(), [](auto result) {
        return result.status;
    });
}

bool JobRemoteReconstructionAgent::AddImage(const std::string &image_id)
{
    AddImageJob job;
    job.reconstruction_id = this->_reconstruction_id;
    job.image_id = image_id;
    auto results = this->_job_queue->QueueAndWait(job);
    return results.status;
}

bool JobRemoteReconstructionAgent::ComputeMatches(const std::set<std::string> &image_ids)
{
    std::vector<ComputeMatchesJob> jobs;
    std::transform(image_ids.begin(), image_ids.end(), back_inserter(jobs), [this](const std::string &image_id) {
        ComputeMatchesJob j;
        j.reconstruction_id = this->_reconstruction_id;
        j.image_id = image_id;
        return j;
    });
    auto results = this->_job_queue->QueueAndWait(jobs);
    return std::all_of(results.begin(), results.end(), [](auto result) {
        return result.status;
    });
}

bool JobRemoteReconstructionAgent::IncrementalSFM()
{
    SparseReconstructionJob job;
    job.reconstruction_id = this->_reconstruction_id;
    auto results = this->_job_queue->QueueAndWait(job);
    return results.status;
}

bool JobRemoteReconstructionAgent::ComputeStructure()
{
    ComputeStructureJob job;
    job.reconstruction_id = this->_reconstruction_id;
    auto results = this->_job_queue->QueueAndWait(job);
    return results.status;
}

bool JobRemoteReconstructionAgent::MVS()
{
    MVSJob job;
    job.reconstruction_id = this->_reconstruction_id;
    auto results = this->_job_queue->QueueAndWait(job);
    return results.status;
}