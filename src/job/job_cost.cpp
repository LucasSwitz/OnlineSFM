#include "job_cost.h"

bool operator<(const JobCost &c1, const JobCost &c2)
{
    return c1.ram_usage < c2.ram_usage &&
           c1.exclusive_cores < c2.exclusive_cores;
}

bool operator>(const JobCost &c1, const JobCost &c2)
{
    return c1.ram_usage > c2.ram_usage &&
           c1.exclusive_cores > c2.exclusive_cores;
}

bool operator>=(const JobCost &c1, const JobCost &c2)
{
    return c1.ram_usage >= c2.ram_usage &&
           c1.exclusive_cores >= c2.exclusive_cores;
}