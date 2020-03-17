#pragma once
#include <string>
#include "server.pb.h"

class ReconstructionStorageAdapter {
    public:
        virtual void CreateIfNew(const std::string& id, const std::string& path) = 0;
        virtual ReconstructionData Get(const std::string& id) = 0;
        virtual void Delete(const std::string& id) = 0;
};