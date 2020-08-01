#pragma once
#include "job.h"

class TestJob : public Job
{
public:
    TestJob()
    {
        this->test_key = "test_value";
    };
    static json Serialize(TestJob &tj)
    {
        json j;
        j["test_key"] = tj.test_key;
        return j;
    }
    static std::string Type()
    {
        return "test";
    }
    static TestJob Parse(const json &j)
    {
        TestJob tj;
        tj.ParseHeader(j);

        json job_json = j["job"];
        tj.test_key = job_json["test_key"];
        return tj;
    }
    std::string test_key;
};