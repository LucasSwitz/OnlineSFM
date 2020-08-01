#include "job_index_service.h"
#include "jobs.h"
#include "ampq_job_queue.h"
#include <iterator>

JobIndexService::JobIndexService() : _job_queue(std::make_shared<AMQPJobExchange>(event_base_new(),
                                                                                  "amqp://user:bitnami@localhost:5672",
                                                                                  "job_exchange",
                                                                                  "my_key"))
{
}

bool JobIndexService::Index(const std::string &reconstruction_id, const std::string &image_id)
{
    IndexImageJob j;
    j.reconstruction_id = reconstruction_id;
    j.image_id = image_id;
    return this->_job_queue->QueueAndWait(j).status;
}

bool JobIndexService::Score(const std::string &reconstruction_id, const std::string &image_id)
{
    ScoreImageJob j;
    j.reconstruction_id = reconstruction_id;
    j.image_id = image_id;
    return this->_job_queue->QueueAndWait(j).status;
}

bool JobIndexService::Score(const std::string &reconstruction_id, const std::vector<std::string> &images)
{
    std::vector<ScoreImageJob> index_jobs;
    std::transform(images.begin(), images.end(), back_inserter(index_jobs), [reconstruction_id](const std::string &id) {
        ScoreImageJob j;
        j.reconstruction_id = reconstruction_id;
        j.image_id = id;
        return j;
    });
    auto results = this->_job_queue->QueueAndWait(index_jobs);
    return std::all_of(results.begin(), results.end(), [](auto &r) { return r.status; });
}
