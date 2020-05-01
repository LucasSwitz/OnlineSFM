#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>

struct TimerDescriptor{
    TimerDescriptor(const std::string& name) : name(name){}
    void AddChild(TimerDescriptor timer){
        children.push_back(timer);
    }
    std::string name;
    std::vector<TimerDescriptor> children;
    double duration;
    float percent = 100.0;
};

typedef typename std::unordered_map<std::thread::id, std::vector<TimerDescriptor>> TimerStackMap;

class PrecisionTimer{
    public:
        PrecisionTimer(const std::string& name);
        ~PrecisionTimer();
    protected:
        static void Export(TimerDescriptor& desc);
    private:
        std::chrono::_V2::high_resolution_clock::time_point start;
        std::string name;
        static TimerStackMap timer_stacks;
};

std::string GetUUID();
void DeleteIfExists(const std::string& path);
void DeleteDirIfExists(const std::string& path);
void CleanAndMakeDir(const std::string& path);