#pragma once
#include "worker_reserver.h"
#include <sw/redis++/redis++.h>
#include <memory>
#include <worker_reserver.h>

class RedisWorkerReserver : public WorkerReserver{
    public:
        RedisWorkerReserver(std::shared_ptr<sw::redis::Redis> redis);
        void AddNewWorker(const std::string& address, uint32_t cores);
        void RemoveWorker(const std::string& address);
        std::string ReserveWorker();
        void ReleaseWorker(const std::string& address);
    private:
        sw::redis::Transaction _tx;
        std::shared_ptr<sw::redis::Redis> _redis;
};