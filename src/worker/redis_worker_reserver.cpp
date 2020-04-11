#include "redis_worker_reserver.h"
#include "exceptions.h"

using namespace sw::redis;
#define CAPACITY_SET_KEY "CAPACITY"
#define USED_KEY "USED"
#define AVAILBLE_KEY "AVAILABLE"

RedisWorkerReserver::RedisWorkerReserver(std::shared_ptr<sw::redis::Redis> redis) : _redis(redis),  
                                                                                    _tx(redis->transaction()){

}

void RedisWorkerReserver::AddNewWorker(const std::string& address, uint32_t cores){
    this->_tx.set(AVAILBLE_KEY+address, std::to_string(cores)).
               set(USED_KEY+address, 0).
               zadd(CAPACITY_SET_KEY, address, 0.0).
               exec();
}

std::string RedisWorkerReserver::ReserverWorker(){
    auto smallest = *this->_redis->zpopmin(CAPACITY_SET_KEY);
    
    std::string address = smallest.first;
    float capacity = smallest.second;

    if(std::get<1>(smallest) == 1.0){
        throw WorkersAtMaxCapacityException();
    }

    auto r = this->_tx.redis();
    while(true){
        try{
            r.watch(AVAILBLE_KEY+address);
            r.get(USED_KEY+address);
            int max_cores = std::stoi(*r.get(AVAILBLE_KEY+address));
            int used_cores = std::stoi(*r.get(USED_KEY+address)) + 1;
            this->_tx.zadd(CAPACITY_SET_KEY, address, float(max_cores) / used_cores)
              .set(USED_KEY+address, std::to_string(used_cores))
              .exec();
            break;
        } catch (const WatchError &err){
            continue;
        }
    }
}

std::string RedisWorkerReserver::ReleaseWorker(const std::string& address){
    auto r = this->_tx.redis();
    while(true){
        try{
            r.watch(AVAILBLE_KEY+address);
            r.get(USED_KEY+address);
            int max_cores = std::stoi(*r.get(AVAILBLE_KEY+address));
            int used_cores = std::stoi(*r.get(USED_KEY+address)) - 1;
            this->_tx.zadd(CAPACITY_SET_KEY, address, float(max_cores) / used_cores)
              .set(USED_KEY+address, std::to_string(used_cores))
              .exec();
        } catch (const WatchError &err){
            continue;
        }
    }
}
