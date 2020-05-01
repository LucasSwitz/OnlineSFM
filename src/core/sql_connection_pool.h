#include "mysql_connection.h"
#include <mutex>
#include <condition_variable>
#include <vector>
class MySQLConnectionPool;
class MySQLConnectionLoan{
    public:
        MySQLConnectionLoan(std::shared_ptr<sql::Connection> con, MySQLConnectionPool* pool);
        MySQLConnectionLoan(MySQLConnectionLoan&& l);
        ~MySQLConnectionLoan();
        MySQLConnectionLoan(const MySQLConnectionLoan& src)= delete;
        void operator=(const MySQLConnectionLoan& src) = delete;
        void operator=(MySQLConnectionLoan&& src);
        std::shared_ptr<sql::Connection> con;
    private:
        MySQLConnectionPool* _pool;
        
};

class MySQLConnectionPool {
    friend class MySQLConnectionLoan;
    public:
        MySQLConnectionPool(unsigned int pool_size);
        ~MySQLConnectionPool();
        MySQLConnectionLoan Get();
    protected:
        void Return(std::shared_ptr<sql::Connection> con);
    private:
        int _pool_size;
        unsigned int _used_connections = 0;
        std::vector<std::shared_ptr<sql::Connection>> _connections;
        std::mutex _mtx;
        std::condition_variable _cv;
};