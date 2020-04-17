#pragma once

#include <string>

class WorkerReserver {
    public:
        virtual void AddNewWorker(const std::string& address, uint32_t cores) = 0;
        virtual std::string ReserveWorker() = 0;
        virtual void ReleaseWorker(const std::string& address) = 0;
};