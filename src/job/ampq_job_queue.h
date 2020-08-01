#pragma once
#include "job.h"
#include <set>
#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>
#include <amqpcpp/libevent.h>
#include <thread>

class NoHeartbeatLibEventHandler : public AMQP::LibEventHandler
{
public:
    NoHeartbeatLibEventHandler(struct event_base *evbase) : AMQP::LibEventHandler(evbase) {}
    uint16_t onNegotiate(AMQP::TcpConnection *connection, uint16_t interval)
    {
        return 0;
    }
};

class AMQPJobExchange : public JobQueue
{
public:
    AMQPJobExchange(event_base *eb,
                    const std::string &address,
                    const std::string &exchange,
                    const std::string &key);
    virtual ~AMQPJobExchange();
    void Unsubscribe(const std::string &type_name);
    void CancelConsume(const std::string &type_name);

protected:
    void _Consume(const std::string &type_name, ConsumeFunction consume_f);
    void _PostResult(JobResult &jr);
    void _SendJob(const std::string &type, const std::string &job_id, const std::string &msg, std::function<void(JobResult &)>);

private:
    std::string TagName(const std::string &type_name);

private:
    NoHeartbeatLibEventHandler _ev_handler;
    AMQP::TcpConnection _tcp_connection;
    AMQP::TcpChannel _channel;
    std::set<std::string> _declared_queues;
    std::string _exchange;
    std::string _tag;
    std::mutex _cancel_mutex;
    std::shared_ptr<std::thread> _eb_thread;
    event_base *_eb;

    static JobResult ParseJobResult(const AMQP::Message &message);
};