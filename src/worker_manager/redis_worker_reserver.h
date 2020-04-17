#pragma once
#include "worker_reserver.h"
#include <sw/redis++/redis++.h>
#include <memory>

class RedisWorkerReserver {
    public:
        RedisWorkerReserver(std::shared_ptr<sw::redis::Redis> redis);
        void AddNewWorker(const std::string& address, uint32_t cores);
        std::string ReserveWorker();
        void ReleaseWorker(const std::string& address);
    private:
        sw::redis::Transaction _tx;
        std::shared_ptr<sw::redis::Redis> _redis;
};