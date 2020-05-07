#include "util.h"
#include <fstream>
#include <elasticlient/bulk.h>
#include "config.h"



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
    root["duration"] = desc.duration/1000;
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
    auto descs = ToJson(desc);
    std::for_each(descs.begin(), descs.end(), [](const auto& e){
        ELASTIC_DUMP(CONFIG_GET_STRING("elastic.timers_index"), 
                     CONFIG_GET_STRING("elastic.timer_doc_type"),
                     e);
     }
    );
}

#include <elasticlient/logging.h>
#include <glog/logging.h>
void logCallback(elasticlient::LogLevel logLevel, const std::string &msg) {
    if(logLevel != elasticlient::LogLevel::DEBUG)
        LOG(INFO) << (unsigned) logLevel << ": " << msg << std::endl;
}

std::shared_ptr<ElasticDumper> ElasticDumper::_instance;
ElasticDumper::ElasticDumper(const std::vector<std::string>& hosts,
                             unsigned int interval) : _client(std::make_shared<elasticlient::Client>(hosts)),  _interval(interval){
    elasticlient::setLogFunction(logCallback);
}

void ElasticDumper::Init(const std::vector<std::string>& hosts, 
          unsigned int interval){
    if(!ElasticDumper::_instance.get()){
        ElasticDumper::_instance = std::make_shared<ElasticDumper>(hosts, interval);
    }
}

std::shared_ptr<ElasticDumper> ElasticDumper::Instance(){
    if(!ElasticDumper::_instance.get()){
        throw std::runtime_error("TimerDumper not initialized");
    }
    return ElasticDumper::_instance;
}

void ElasticDumper::Dump(const std::string& index, const std::string& doc_type, const std::string& doc){    
    std::lock_guard<std::mutex> l(this->_docs_mutex);
    if(this->_docs.find(index) == this->_docs.end()){
        this->_docs[index] = std::vector<std::tuple<std::string, std::string>>();
    }
    this->_docs[index].push_back({doc_type, doc});
}

void ElasticDumper::Start(){
    this->_dump_thread = std::make_shared<std::thread>(&ElasticDumper::_Dump, this);
}

#include <cpr/response.h>
#include <glog/logging.h>
void ElasticDumper::_Dump(){
    this->_running = true;
    while(this->_running){
        std::unique_lock<std::mutex> l(this->_docs_mutex);
        if(this->_docs.size()){
            for(auto indexes : this->_docs){
                auto index = indexes.first;
                auto& docs_type = indexes.second;
                elasticlient::Bulk bulk_indexer(this->_client);
                elasticlient::SameIndexBulkData bulk(index, this->_docs.size());
                for(auto& doc_type : docs_type){
                    auto type = std::get<0>(doc_type);
                    auto doc = std::get<1>(doc_type);
                    bulk.indexDocument(type, GetUUID(), doc);
                }
                auto errors = bulk_indexer.perform(bulk);
                bulk.clear();
            }
        }
        this->_docs.clear();
        l.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->_interval));
    }
}

ElasticDumper::~ElasticDumper(){
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