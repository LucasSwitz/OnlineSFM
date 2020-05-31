#pragma once
#include <string>
#include "server.pb.h"

class ReconstructionStorageAdapter
{
public:
    virtual void Store(const ReconstructionData &data) = 0;
    virtual ReconstructionData Get(const std::string &id) = 0;
    virtual void Delete(const std::string &id) = 0;
    virtual ~ReconstructionStorageAdapter(){};
};