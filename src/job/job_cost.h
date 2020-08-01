#pragma once

#include <string>

struct JobCost
{
    double ram_usage;
    int exclusive_cores = 0;
    void operator-=(const JobCost &c)
    {
        this->ram_usage -= c.ram_usage;
        this->exclusive_cores -= c.exclusive_cores;
    }
    void operator+=(const JobCost &c)
    {
        this->ram_usage += c.ram_usage;
        this->exclusive_cores += c.exclusive_cores;
    }
};

bool operator<(const JobCost &c1, const JobCost &c2);

bool operator>(const JobCost &c1, const JobCost &c2);

bool operator>=(const JobCost &c1, const JobCost &c2);
class JobCostProvider
{
public:
    virtual JobCost GetCost(const std::string &type) = 0;
};