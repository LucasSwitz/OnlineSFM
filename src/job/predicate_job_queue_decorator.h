#pragma once
#include "job.h"

class PredicateJobQueueDecorator : public JobQueue
{
public:
    PredicateJobQueueDecorator(std::shared_ptr<JobQueue> queue);
    virtual bool TryAcceptJob(const std::string &type) = 0;
    virtual void CancelConsume(const std::string &type);
    virtual void OnRejectJob(const std::string &job_type_name);
    virtual void _Consume(const std::string &type_name, ConsumeFunction);
    virtual void _PostResult(JobResult &jr) = 0;
    virtual void _SendJob(const std::string &type, const std::string &job_id, const std::string &msg, std::function<void(JobResult &)>);

protected:
    std::shared_ptr<JobQueue> _queue;
};