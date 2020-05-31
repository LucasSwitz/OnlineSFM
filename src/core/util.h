#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <elasticlient/client.h>

#define ELASTIC_DUMP(i, t, d) ElasticDumper::Instance()->Dump(i, t, d)

struct TimerDescriptor
{
    TimerDescriptor(const std::string &name) : name(name) {}
    void AddChild(TimerDescriptor timer)
    {
        children.push_back(timer);
    }
    std::string name;
    long duration;
    long start;
    float percent = 100.0;
    std::vector<TimerDescriptor> children;
};

typedef typename std::unordered_map<std::thread::id, std::vector<TimerDescriptor>> TimerStackMap;

class PrecisionTimer
{
public:
    PrecisionTimer(const std::string &name);
    ~PrecisionTimer();

protected:
    static void Export(TimerDescriptor &desc);

private:
    std::chrono::_V2::high_resolution_clock::time_point start;
    std::string name;
    static TimerStackMap timer_stacks;
    static std::mutex _stacks_mutex;
};

class ElasticDumper
{
public:
    void Dump(const std::string &index, const std::string &doc_type, const std::string &doc);
    void Start();
    ~ElasticDumper();
    static void Init(const std::vector<std::string> &hosts, unsigned int interval);
    static std::shared_ptr<ElasticDumper> Instance();
    ElasticDumper(const std::vector<std::string> &hosts, unsigned int interval);

private:
    std::unordered_map<std::string, std::vector<std::tuple<std::string, std::string>>> _docs;
    std::shared_ptr<elasticlient::Client> _client;
    std::shared_ptr<std::thread> _dump_thread;
    std::mutex _docs_mutex;
    static std::shared_ptr<ElasticDumper> _instance;
    unsigned int _interval;
    bool _running = false;
    void _Dump();
};

std::string GetUUID();
void DeleteIfExists(const std::string &path);
void DeleteDirIfExists(const std::string &path);
void CleanAndMakeDir(const std::string &path);