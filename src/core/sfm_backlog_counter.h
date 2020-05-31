#pragma once
#include <set>
#include <string>

class SFMBacklogCounter
{
public:
    virtual void Incr(const std::string &session_id, size_t incr = 1) = 0;
    virtual uint64_t CheckCounter(const std::string &session_id) = 0;
    virtual void Clear(const std::string &session_id) = 0;
};