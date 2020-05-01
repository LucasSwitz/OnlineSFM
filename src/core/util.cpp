#include "util.h"
#include <fstream>

TimerStackMap PrecisionTimer::timer_stacks;
PrecisionTimer::PrecisionTimer(const std::string& name){
    this->name = name;
    std::thread::id thread_id = std::this_thread::get_id();
    if(timer_stacks.find(thread_id) == timer_stacks.end()){
        timer_stacks[thread_id] = std::vector<TimerDescriptor>();
   }
   TimerDescriptor t(name);
   timer_stacks[thread_id].push_back(t);
   this->start = std::chrono::high_resolution_clock::now();
}

PrecisionTimer::~PrecisionTimer(){
    auto end =  std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - this->start).count();
    std::thread::id thread_id = std::this_thread::get_id();
    auto this_descriptor = timer_stacks[thread_id].back();
    timer_stacks[thread_id].pop_back();
    this_descriptor.duration = duration;
    if(timer_stacks[thread_id].size() > 0){
        timer_stacks[thread_id].back().AddChild(this_descriptor);
    }else{
        Export(this_descriptor);
    }
}

#include <nlohmann/json.hpp>
using json = nlohmann::json;

json ToJson(const TimerDescriptor& desc){
    json root;
    root["name"] = desc.name;
    root["duration"] = desc.duration;
    root["percent"] = desc.percent;
    std::vector<json> children;
    for(auto c : desc.children){
        children.push_back(ToJson(c));
    }
    root["children"] = children;
    return root;
}

void FillPercent(TimerDescriptor& desc){  
    double total = desc.duration;
    for(auto& c : desc.children){
        c.percent = c.duration/total;
        FillPercent(c);
    }
}

void PrecisionTimer::Export(TimerDescriptor& desc){
    FillPercent(desc);
    json report = ToJson(desc);
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string time_str(std::ctime(&time));
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thread_id = ss.str();
    std::string report_name =  desc.name + "_" + thread_id + + "_" + time_str + ".json";
    std::ofstream o(report_name);
    o << std::setw(4) << report << std::endl;
    o.close();
}

#include "util.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

using namespace boost::uuids;

std::string GetUUID(){
    random_generator gen;
    uuid id = gen();
    return to_string(id);
}

void DeleteIfExists(const std::string& path){
    if(boost::filesystem::exists(path)){
        boost::filesystem::remove(path);
    }
}

void DeleteDirIfExists(const std::string& path){
    if(boost::filesystem::exists(path)){
        boost::filesystem::remove_all(path);
    }
}

void CleanAndMakeDir(const std::string& path){
    DeleteDirIfExists(path);
    boost::filesystem::create_directory(path);
}