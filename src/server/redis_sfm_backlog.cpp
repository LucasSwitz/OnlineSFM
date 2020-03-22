#include "redis_sfm_backlog.h"

using namespace sw::redis;

RedisSFMBacklog::RedisSFMBacklog(const std::string& addr, 
                                               const std::string& user, 
                                               const std::string& pass): _redis(addr){

}

void RedisSFMBacklog::Clear(const std::string& session_id){
    this->_redis.set(session_id, 0);
}

void RedisSFMBacklog::Incr(const std::string& session_id, size_t incr){
    this->_redis.incrby(session_id, incr);
}

uint64_t RedisSFMBacklog::CheckCounter(const std::string& session_id){
    sw::redis::OptionalString res = this->_redis.get(session_id);
    return std::stoi(*res);
}