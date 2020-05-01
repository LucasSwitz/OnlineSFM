#include "redis_worker_reserver.h"
#include "exceptions.h"
#include <chrono>
#include <thread>
#include <glog/logging.h>


using namespace sw::redis;
#define CAPACITY_SET_KEY "CAPACITY"
#define USED_KEY "USED"
#define AVAILBLE_KEY "AVAILABLE"

RedisWorkerReserver::RedisWorkerReserver(std::shared_ptr<sw::redis::Redis> redis) : _redis(redis),  
                                                                                    _tx(redis->transaction()){

}

void RedisWorkerReserver::AddNewWorker(const std::string& address, uint32_t cores){
    try {   
        this->_tx.set(AVAILBLE_KEY+address, std::to_string(cores)).
                set(USED_KEY+address, "0").
                zadd(CAPACITY_SET_KEY, address, 0.0).
                exec();
    } catch(const std::exception& e){
        this->_tx = this->_redis->transaction();
    }
}

std::string RedisWorkerReserver::ReserveWorker(int num_cores){
    std::string address;
    auto r = this->_tx.redis();
    while(true){
        try{
            r.watch(CAPACITY_SET_KEY);
            std::unordered_map<std::string, double> addr_dict;
            this->_redis->zrange(CAPACITY_SET_KEY, 0, 0, std::inserter(addr_dict, addr_dict.begin()));
            if(addr_dict.size() == 0){
                LOG(ERROR) << "No Workers available. Trying again in 1 second";
                using namespace std::chrono_literals; 
                std::this_thread::sleep_for(1s);
                return this->ReserveWorker();
            }
            address = addr_dict.begin()->first;
            float capacity =  addr_dict.begin()->second;
            if(capacity >= 1.0){
                LOG(ERROR) << "Workers at max capacity. Trying again in 1 second";
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1s);
                return this->ReserveWorker();
            }
            r.watch(AVAILBLE_KEY+address);
            r.watch(USED_KEY+address);
            int max_cores = std::stoi(*r.get(AVAILBLE_KEY+address));
            num_cores = num_cores == -1 ? max_cores : num_cores;
            int used_cores = std::stoi(*r.get(USED_KEY+address)) + num_cores;
            float capacity_used = float(used_cores) / max_cores;
            this->_tx.zadd(CAPACITY_SET_KEY, address, capacity_used)
                     .set(USED_KEY+address, std::to_string(used_cores))
                     .exec();
            break;
        } catch (const WatchError &err){
            this->_tx = this->_redis->transaction();
            r = this->_tx.redis();
            continue;
        } catch(const std::exception& e){
            throw;
        }
    }
    return address;
}

void RedisWorkerReserver::ReleaseWorker(const std::string& address, int num_cores){
    auto r = this->_tx.redis();
    while(true){
        try{
            r.watch(CAPACITY_SET_KEY);
            r.watch(AVAILBLE_KEY+address);
            r.watch(USED_KEY+address);
            int max_cores = std::stoi(*r.get(AVAILBLE_KEY+address));
            num_cores = num_cores == -1 ? max_cores : num_cores;
            int used_cores = std::stoi(*r.get(USED_KEY+address)) - num_cores;
            float capacity_used = float(used_cores) / max_cores;
            this->_tx.zadd(CAPACITY_SET_KEY, address, capacity_used)
              .set(USED_KEY+address, std::to_string(used_cores))
              .exec();
            break;
        } catch (const WatchError &err){
            this->_tx = this->_redis->transaction();
            r = this->_tx.redis();
            continue;
        } catch(const std::exception& e){
            throw;
        }
    }
}

void RedisWorkerReserver::RemoveWorker(const std::string& address){
    auto r = this->_tx.redis();
    while(true){
        try{
            r.watch(CAPACITY_SET_KEY);
            r.watch(AVAILBLE_KEY+address);
            r.watch(USED_KEY+address);
            int max_cores = std::stoi(*r.get(AVAILBLE_KEY+address));
            int used_cores = std::stoi(*r.get(USED_KEY+address)) - 1;
            float capacity_used = float(used_cores) / max_cores;
            this->_tx.zrem(CAPACITY_SET_KEY, address)
              .del(USED_KEY+address)
              .del(AVAILBLE_KEY+address)
              .exec();
            break;
        } catch (const WatchError &err){
            this->_tx = this->_redis->transaction();
            r = this->_tx.redis();
            continue;
        } catch(const std::exception& e){
            throw;
        }
    }
}
