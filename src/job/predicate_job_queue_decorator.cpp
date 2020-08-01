#include "predicate_job_queue_decorator.h"

PredicateJobQueueDecorator::PredicateJobQueueDecorator(std::shared_ptr<JobQueue> queue) : _queue(queue)
{
}

void PredicateJobQueueDecorator::_Consume(const std::string &type_name, ConsumeFunction consume_f)
{
    this->_queue->_Consume(type_name, [this, type_name, consume_f](std::string data) {
        // this might need to be done atomically so that we accept and decrement resources at same time
        if (this->TryAcceptJob(type_name))
        {
            return consume_f(data);
        }
        else
        {
            this->OnRejectJob(type_name);
            return false;
        }
    });
}

void PredicateJobQueueDecorator::_PostResult(JobResult &jr)
{
    this->_queue->_PostResult(jr);
}

void PredicateJobQueueDecorator::_SendJob(const std::string &type, const std::string &job_id, const std::string &msg, std::function<void(JobResult &)> callback)
{
    this->_queue->_SendJob(type, job_id, msg, callback);
}

void PredicateJobQueueDecorator::CancelConsume(const std::string &type)
{
    this->_queue->CancelConsume(type);
}

void PredicateJobQueueDecorator::OnRejectJob(const std::string &type_name)
{
    //nothing by default
}