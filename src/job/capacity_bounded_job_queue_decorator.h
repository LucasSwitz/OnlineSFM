#pragma once

#include "predicate_job_queue_decorator.h"
#include "job_cost.h"
#include <set>
#include <tuple>
#include <mutex>

class CapacityBoundedJobQueueDecorator : public PredicateJobQueueDecorator
{
public:
    CapacityBoundedJobQueueDecorator(std::shared_ptr<JobQueue> queue,
                                     std::shared_ptr<JobCostProvider> cost_provider,
                                     JobCost availabe_resources);
    bool TryAcceptJob(const std::string &type);
    void _Consume(const std::string &type_name, ConsumeFunction consumef);
    void _PostResult(JobResult &jr);
    void OnRejectJob(const std::string &type_name);
    void OnAcceptJob(const std::string &type_name);

private:
    std::set<std::tuple<JobCost, std::string>> _subscriptions_halted;
    std::shared_ptr<JobCostProvider> _cost_provider;
    JobCost _available_resources;
    std::unordered_map<std::string, ConsumeFunction> _consume_functions;
    std::mutex _resources_mutex;
};
