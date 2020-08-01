#include "ampq_job_queue.h"
#include <glog/logging.h>

std::string AMQPJobExchange::TagName(const std::string &type)
{
    return this->_tag + "|" + type;
}

JobResult AMQPJobExchange::ParseJobResult(const AMQP::Message &message)
{
    std::string b(message.body(), message.bodySize());
    return JobQueue::ParseJobResult(b);
}

AMQPJobExchange::AMQPJobExchange(event_base *eb,
                                 const std::string &address,
                                 const std::string &exchange,
                                 const std::string &key) : _ev_handler(eb),
                                                           _tcp_connection(&_ev_handler, AMQP::Address(address)),
                                                           _channel(&_tcp_connection),
                                                           _exchange(exchange),
                                                           _tag(key),
                                                           _eb(eb)
{
    this->_eb_thread = std::make_shared<std::thread>(event_base_dispatch, eb);
    this->_channel.declareExchange(exchange, AMQP::direct);
}

void AMQPJobExchange::_SendJob(const std::string &type,
                               const std::string &job_id,
                               const std::string &msg,
                               std::function<void(JobResult &)> callback)
{
    if (this->_declared_queues.find(type) == this->_declared_queues.end())
    {
        this->_channel.declareQueue(type).onError([](const std::string &err) {
            LOG(ERROR) << err;
        });
        ;
        this->_channel.bindQueue(this->_exchange, type, type).onError([](const std::string &err) {
            LOG(ERROR) << err;
        });
        ;
        this->_declared_queues.insert(type);
    }
    this->_channel.declareQueue(job_id).onError([](const std::string &err) {
        LOG(ERROR) << err;
    });
    ;
    this->_channel.bindQueue(this->_exchange, job_id, job_id).onError([](const std::string &err) {
        LOG(ERROR) << err;
    });
    ;
    this->_channel.consume(job_id).onReceived([this, callback](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
                                      this->_channel.ack(deliveryTag);
                                      JobResult jr = AMQPJobExchange::ParseJobResult(message);
                                      callback(jr);
                                  })
        .onError([](const std::string &err) {
            LOG(ERROR) << err;
        });
    AMQP::Envelope e(msg.c_str(), msg.size());
    e.setContentType("application/json");
    this->_channel.publish(this->_exchange, type, e);
}

void AMQPJobExchange::_Consume(const std::string &type_name, ConsumeFunction consume_f)
{
    if (this->_declared_queues.find(type_name) == this->_declared_queues.end())
    {
        this->_channel.declareQueue(type_name).onError([](const std::string &err) {
            LOG(ERROR) << err;
        });
        ;
        this->_channel.bindQueue(this->_exchange, type_name, type_name).onError([](const std::string &err) {
            LOG(ERROR) << err;
        });
        ;
        this->_declared_queues.insert(type_name);
    }

    this->_channel.consume(type_name, TagName(type_name)).onReceived([this, consume_f, type_name](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
                                                             std::string b(message.body(), message.bodySize());
                                                             std::thread t([this, consume_f, b, deliveryTag, redelivered]() {
                                                                 if (consume_f(b))
                                                                 {
                                                                     this->_channel.ack(deliveryTag);
                                                                 }
                                                                 else
                                                                 {
                                                                     this->_channel.reject(deliveryTag, AMQP::requeue);
                                                                 }
                                                             });

                                                             t.detach();
                                                         })
        .onError([](const std::string &err) {
            LOG(ERROR) << err;
        })
        .onSuccess([this, type_name]() {
            LOG(INFO) << "Successfully setup consumer " << this->TagName(type_name) << " for " << type_name;
        });
    ;
    ;
}

void AMQPJobExchange::CancelConsume(const std::string &type_name)
{
    std::condition_variable cv;
    std::unique_lock<std::mutex> lk(_cancel_mutex);
    this->_channel.cancel(TagName(type_name)).onFinalize([this, &cv]() {
                                                 std::unique_lock<std::mutex> lk(this->_cancel_mutex);
                                                 cv.notify_one();
                                             })
        .onError([](const std::string &err) {
            LOG(ERROR) << err;
        });
    cv.wait(lk);
}

void AMQPJobExchange::_PostResult(JobResult &jr)
{
    this->_channel.publish(this->_exchange, jr.job_id, SerializeJobResult(jr));
}

AMQPJobExchange::~AMQPJobExchange()
{
    event_base_loopbreak(this->_eb);
    this->_eb_thread->detach();
}