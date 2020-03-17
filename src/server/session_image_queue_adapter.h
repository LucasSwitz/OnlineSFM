#pragma once
#include <set>
#include <string>

class SessionImageQueueAdapter {
    public:
        virtual std::set<std::string> Get(const std::string& session_id) = 0;
        virtual void Put(const std::string& session_id, const std::string& img_id) = 0;
};