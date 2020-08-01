#pragma once
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>
#include <sstream>
#include <condition_variable>
#include "util.h"

using json = nlohmann::json;

typedef typename std::function<bool(std::string)> ConsumeFunction;

class Job
{
public:
    const std::string &id()
    {
        return this->_id;
    }

protected:
    void ParseHeader(const json &j)
    {
        this->_id = j["id"];
    }

private:
    std::string _type;
    std::string _id;
};

template <typename T>
std::string SerializeJob(const std::string &job_id, T &job)
{
    json j;
    j["type"] = T::Type();
    j["id"] = job_id;
    j["job"] = T::Serialize(job);
    std::stringstream ss;
    return j.dump();
}

struct JobResult
{
    JobResult(const std::string &job_id, const std::string &type) : job_id(job_id), type(type) {}
    bool status;
    std::string type;
    std::string info;
    std::string job_id;
};

class JobQueue
{
public:
    template <typename T>
    void Queue(T &job, std::function<void(JobResult &)> callback)
    {
        std::string job_id = GetUUID();
        std::string serialized = SerializeJob(job_id, job);
        this->_SendJob(T::Type(), job_id, serialized, callback);
    }

    template <typename T>
    JobResult QueueAndWait(T &job)
    {
        std::condition_variable cv;
        JobResult jr_out(job.id(), T::Type());
        this->Queue(job, [&cv, &jr_out](JobResult &jr) mutable {
            cv.notify_one();
            jr_out = jr;
        });
        std::mutex m;
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
        return jr_out;
    }

    template <typename T>
    void Queue(std::vector<T> &job_collection, std::function<void(std::vector<JobResult> &)> callback)
    {
        int count = 0;
        int total = job_collection.size();
        std::vector<JobResult> jrs;
        for (auto job : job_collection)
        {
            this->Queue(job, [total, &count, &jrs, callback](JobResult &jr) mutable {
                count++;
                jrs.push_back(jr);
                if (count == total)
                {
                    callback(jrs);
                }
            });
        }
    }

    template <typename T>
    std::vector<JobResult> QueueAndWait(std::vector<T> &job_collection)
    {
        std::vector<JobResult> jrs;
        std::condition_variable cv;
        this->Queue(job_collection, [&jrs, &cv](std::vector<JobResult> &job_results) mutable {
            jrs = job_results;
            cv.notify_one();
        });
        std::mutex m;
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
        return jrs;
    }

    template <typename T>
    void Consume(std::function<bool(T &, JobResult &)> callback)
    {
        this->_Consume(T::Type(), [this, callback](const std::string &data) {
            json j = json::parse(data);
            T job = T::Parse(j);
            JobResult jr(job.id(), T::Type());
            if (callback(job, jr))
            {
                this->_PostResult(jr);
                return true;
            }
            else
            {
                return false;
            }
        });
    }

    virtual void CancelConsume(const std::string &type_name) = 0;
    static JobResult ParseJobResult(const std::string &data);

    /*These are left public so that decorators can call them, but not should be used otherwise.*/
    virtual void _Consume(const std::string &type_name, ConsumeFunction) = 0;
    virtual void _PostResult(JobResult &jr) = 0;
    virtual void _SendJob(const std::string &type, const std::string &job_id, const std::string &msg, std::function<void(JobResult &)>) = 0;
};

std::string SerializeJob(const std::string &job_id, std::shared_ptr<Job> job);
std::string SerializeJobResult(const JobResult &j);