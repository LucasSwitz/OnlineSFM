#include "redis_session_image_queue_adapter.h"

using namespace sw::redis;

RedisSessionImageQueue::RedisSessionImageQueue(const std::string& addr, 
                                               const std::string& user, 
                                               const std::string& pass): _redis(addr){

}

std::set<std::string> RedisSessionImageQueue::Get(const std::string& session_id){
    std::set<std::string> img_ids;
    QueuedReplies replies = this->_redis.transaction().lrange(session_id, 0, -1)
                        .del(session_id)
                        .exec();
    redisReply imgs = replies.get(0);
    for(int i = 0; i < imgs.elements; i++){
        img_ids.insert(imgs.element[i]->str);
    }
    return img_ids;
}

void RedisSessionImageQueue::Put(const std::string& session_id, const std::string& img_id){
    this->_redis.lpush(session_id, img_id);
}