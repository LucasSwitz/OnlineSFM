#include "util.h"
#include <fstream>
#include <elasticlient/bulk.h>

TimerStackMap PrecisionTimer::timer_stacks;
std::mutex PrecisionTimer::_stacks_mutex;
PrecisionTimer::PrecisionTimer(const std::string& name){
    std::lock_guard<std::mutex> l(PrecisionTimer::_stacks_mutex);
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
    std::lock_guard<std::mutex> l(PrecisionTimer::_stacks_mutex);
    auto this_descriptor = timer_stacks[thread_id].back();
    timer_stacks[thread_id].pop_back();

    this_descriptor.duration = duration;
    this_descriptor.start = this->start.time_since_epoch().count()*1000;

    if(timer_stacks[thread_id].size() > 0){
        timer_stacks[thread_id].back().AddChild(this_descriptor);
    }else{
        Export(this_descriptor);
    }
}

#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::vector<std::string> ToJson(const TimerDescriptor& desc){
    json root;
    root["name"] = desc.name;
    root["time"] = desc.start;
    root["duration"] = desc.duration;
    root["percent"] = desc.percent;
    std::vector<std::string> descs;
    for(auto c : desc.children){
        auto child_descs = ToJson(c);
        descs.insert(descs.end(), child_descs.begin(), child_descs.end());
    }

    std::stringstream ss;
    ss << root;
    descs.push_back(ss.str());
    return descs;
}

void Finalize(TimerDescriptor& desc){  
    double total = desc.duration;
    for(auto& c : desc.children){
        c.percent = c.duration/total;
        Finalize(c);
    }
}

void PrecisionTimer::Export(TimerDescriptor& desc){
    //Finalize(desc);
    TimerDumper::Instance()->Dump(desc);
}

#include <elasticlient/logging.h>
#include <glog/logging.h>
void logCallback(elasticlient::LogLevel logLevel, const std::string &msg) {
    if(logLevel != elasticlient::LogLevel::DEBUG)
        LOG(INFO) << (unsigned) logLevel << ": " << msg << std::endl;
}

std::shared_ptr<TimerDumper> TimerDumper::_instance;
TimerDumper::TimerDumper(const std::vector<std::string>& hosts, 
                         const std::string& index, 
                         const std::string& doc_type, 
                         unsigned int interval) : _client(std::make_shared<elasticlient::Client>(hosts)), _index(index), _doc_type(doc_type), _interval(interval){
    elasticlient::setLogFunction(logCallback);
}

void TimerDumper::Init(const std::vector<std::string>& hosts, 
          const std::string& index, 
          const std::string& doc_type, 
          unsigned int interval){
    if(!TimerDumper::_instance.get()){
        TimerDumper::_instance = std::make_shared<TimerDumper>(hosts, index, doc_type, interval);
    }
}

std::shared_ptr<TimerDumper> TimerDumper::Instance(){
    if(!TimerDumper::_instance.get()){
        throw std::runtime_error("TimerDumper not initialized");
    }
    return TimerDumper::_instance;
}

void TimerDumper::Dump(const TimerDescriptor& desc){    
    std::lock_guard<std::mutex> l(this->_docs_mutex);
    auto new_json = ToJson(desc);
    this->_docs.insert(this->_docs.end(), new_json.begin(), new_json.end());
}

void TimerDumper::Start(){
    this->_dump_thread = std::make_shared<std::thread>(&TimerDumper::_Dump, this);
}

#include <cpr/response.h>
#include <glog/logging.h>
void TimerDumper::_Dump(){
    this->_running = true;
    while(this->_running){
        std::unique_lock<std::mutex> l(this->_docs_mutex);
        if(this->_docs.size()){
            elasticlient::Bulk bulk_indexer(this->_client);
            elasticlient::SameIndexBulkData bulk(this->_index, this->_docs.size());
            for(std::string& doc : this->_docs){
                bulk.indexDocument(this->_doc_type, GetUUID(), doc);
            }
            this->_docs.clear();
            auto errors = bulk_indexer.perform(bulk);
            bulk.clear();
        }
        l.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->_interval));
    }
}

TimerDumper::~TimerDumper(){
    this->_running = false;
    if(this->_dump_thread.get()){
        this->_dump_thread->join();
    }
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