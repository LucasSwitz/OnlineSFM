#include "ampq_job_queue.h"
#include <event2/event.h>
#include "test_job.h"
#include <iostream>
int main(int argc, char **argv)
{
    event_base *eb = event_base_new();
    AMQPJobExchange ex(eb, "amqp://user:bitnami@localhost:5672", "job_exchange", "test_job");
    TestJob j;
    ex.Queue(
        j, [eb](JobResult &jr) {
            assert(jr.status);
            std::cout << "Job finished successfully"
                      << "\n";
            event_base_loopbreak(eb);
        });
    event_base_dispatch(eb);
}