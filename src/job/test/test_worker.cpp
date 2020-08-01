#include "ampq_job_queue.h"
#include <event2/event.h>
#include "test_job.h"
#include <iostream>
int main(int argc, char **argv)
{
    event_base *eb = event_base_new();
    AMQPJobExchange ex(eb, "amqp://user:bitnami@localhost:5672", "job_exchange", "test_worker");
    ex.Consume<TestJob>([](TestJob &j, JobResult &r) {
        ;
        std::cout
            << "got job " << j.id() << "\n";
        r.job_id = j.id();
        r.status = true;
        r.info = "test info";
        return true;
    });
    event_base_dispatch(eb);
}