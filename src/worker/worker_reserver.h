#pragma once

#include <string>

class WorkerReserver {
    public:
        virtual void AddNewWorker(const std::string& address, uint32_t cores) = 0;
        virtual std::string ReserverWorker() = 0;
        virtual std::string ReleaseWorker(const std::string& address) = 0;
};