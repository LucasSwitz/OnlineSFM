#pragma once

#include <chrono>
#include <thread>
#include <exception>
#include <glog/logging.h>

class ExpotentialBackoffFailure : public std::runtime_error{
    public:
        ExpotentialBackoffFailure(const std::string& name, unsigned int retry_count) : std::runtime_error("Failed to connect to " + name + " after " + std::to_string(retry_count) + " tries"){

        }
};

template<typename T>
void ExpotentialBackoff(const std::string& name, T f, unsigned int max_retry){
    using namespace std::chrono_literals;
    auto delay = 1s;
    do{
        if(!f()){
            LOG(ERROR) << "Failed to connect to: " << name << ". Trying again in " << delay.count() << " seconds";
            std::this_thread::sleep_for(delay);
            delay *= 2;
            continue;
        }
        return;
    }while(max_retry--);
    throw ExpotentialBackoffFailure(name, max_retry);

}