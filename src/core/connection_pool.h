#pragma onc
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>

template <typename T>
class ConnectionPool;

template <typename T>
class ConnectionLoan
{
public:
    ConnectionLoan(std::shared_ptr<T> con, ConnectionPool<T> *pool)
    {
        this->con = con;
        this->_pool = pool;
    }
    ConnectionLoan(ConnectionLoan &&l)
    {
        this->con = l.con;
        this->_pool = l._pool;
        l.con = nullptr;
    }
    void operator=(ConnectionLoan<T> &&l)
    {
        this->con = l.con;
        this->_pool = l._pool;
        l.con = nullptr;
    }
    ~ConnectionLoan()
    {
        if (this->con)
        {
            this->_pool->Return(this->con);
        }
    }
    ConnectionLoan(const ConnectionLoan &src) = delete;
    void operator=(const ConnectionLoan &src) = delete;
    std::shared_ptr<T> con;

private:
    ConnectionPool<T> *_pool;
};

template <typename T>
class ConnectionPool
{
    friend class ConnectionLoan<T>;

public:
    ConnectionPool(unsigned int pool_size)
    {
        this->_pool_size = pool_size;
        while (pool_size--)
        {
            auto connection = this->Create();
            this->Add(connection);
        }
    }
    virtual ~ConnectionPool()
    {
        for (auto con : this->_connections)
        {
            this->Close(con);
        }
    }
    ConnectionLoan<T> Get()
    {
        std::unique_lock<std::mutex> lck(this->_mtx);
        while (this->_connections.empty())
        {
            this->_cv.wait(lck);
        }
        auto con = this->_connections[this->_connections.size() - 1];
        this->_connections.pop_back();
        this->_used_connections++;
        return ConnectionLoan<T>(con, this);
    }
    void Replace(std::shared_ptr<T> *con)
    {
        std::unique_lock<std::mutex> lck(this->_mtx);
        this->Close(*con);
        con->reset();
        *con = std::move(this->Create());
    }
    void Return(std::shared_ptr<T> con)
    {
        std::unique_lock<std::mutex> lck(this->_mtx);
        this->_connections.push_back(con);
        this->_used_connections--;
        this->_cv.notify_one();
    }

protected:
    std::shared_ptr<T> Create();
    void Close(std::shared_ptr<T> con);
    void Add(std::shared_ptr<T> con)
    {
        this->_pool_size++;
        this->_connections.push_back(con);
    }

private:
    int _pool_size;
    unsigned int _used_connections = 0;
    std::vector<std::shared_ptr<T>> _connections;
    std::mutex _mtx;
    std::condition_variable _cv;
};