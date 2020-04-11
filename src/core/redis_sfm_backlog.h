#pragma once

#include "sfm_backlog_counter.h"
#include <sw/redis++/redis++.h>


class RedisSFMBacklog : public SFMBacklogCounter {
    public:
        RedisSFMBacklog(const std::string& addr, 
                               const std::string& user, 
                               const std::string& pass);
        void Incr(const std::string& session_id, size_t incr = 1);
        uint64_t CheckCounter(const std::string& session_id);
        void Clear(const std::string& session_id);
    private:
        sw::redis::Redis _redis;
};  