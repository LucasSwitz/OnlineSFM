#include "capacity_bounded_job_queue_decorator.h"
#include <glog/logging.h>

CapacityBoundedJobQueueDecorator::CapacityBoundedJobQueueDecorator(std::shared_ptr<JobQueue> queue,
                                                                   std::shared_ptr<JobCostProvider> cost_provider,
                                                                   JobCost available_resources) : PredicateJobQueueDecorator(queue),
                                                                                                  _cost_provider(cost_provider),
                                                                                                  _available_resources(available_resources)
{
}

void CapacityBoundedJobQueueDecorator::_Consume(const std::string &type_name, ConsumeFunction callback)
{
    this->_consume_functions[type_name] = callback;
    PredicateJobQueueDecorator::_Consume(type_name, callback);
}

bool CapacityBoundedJobQueueDecorator::TryAcceptJob(const std::string &type)
{
    std::lock_guard<std::mutex> l(this->_resources_mutex);
    bool can_accept = this->_available_resources >= this->_cost_provider->GetCost(type);
    if (can_accept)
    {
        LOG(INFO) << "Accepting consumption " << type;
        this->_available_resources -= this->_cost_provider->GetCost(type);
    }
    return can_accept;
}

void CapacityBoundedJobQueueDecorator::OnRejectJob(const std::string &type_name)
{
    JobCost c = this->_cost_provider->GetCost(type_name);
    std::unique_lock<std::mutex> l(this->_resources_mutex);
    auto t = std::tuple<JobCost, std::string>(c, type_name);
    if (this->_subscriptions_halted.find(t) == this->_subscriptions_halted.end())
    {
        this->_subscriptions_halted.insert(t);
        LOG(INFO) << "Canceling consumption " << type_name;
        this->_queue->CancelConsume(type_name);
    }
    l.unlock();
}

void CapacityBoundedJobQueueDecorator::_PostResult(JobResult &jr)
{
    JobCost c = this->_cost_provider->GetCost(jr.type);
    std::unique_lock<std::mutex> l(this->_resources_mutex);
    auto cut = std::upper_bound(this->_subscriptions_halted.begin(),
                                this->_subscriptions_halted.end(),
                                std::tuple<JobCost, std::string>(c, jr.type));
    for (auto it = this->_subscriptions_halted.begin(); it != cut; it++)
    {
        std::string type_name = std::get<1>(*it);
        LOG(INFO) << "Finished " << jr.type;
        LOG(INFO) << "Re-consuming" << type_name;
        this->_Consume(type_name, this->_consume_functions[type_name]);
    }
    this->_subscriptions_halted = std::set<std::tuple<JobCost, std::string>>(cut, this->_subscriptions_halted.end());
    this->_available_resources += c;
    l.unlock();
    PredicateJobQueueDecorator::_PostResult(jr);
}