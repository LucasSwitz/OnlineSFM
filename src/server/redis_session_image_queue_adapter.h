#pragma once

#include "session_image_queue_adapter.h"
#include <sw/redis++/redis++.h>


class RedisSessionImageQueue : public SessionImageQueueAdapter {
    public:
        RedisSessionImageQueue(const std::string& addr, 
                               const std::string& user, 
                               const std::string& pass);
        std::set<std::string> Get(const std::string& session_id);
        void Put(const std::string& session_id, const std::string& img_id);
        void Clear(const std::string& session_id);
    private:
        sw::redis::Redis _redis;
};  