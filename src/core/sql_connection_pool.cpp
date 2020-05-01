#include "sql_connection_pool.h"
#include "config.h"
#include <cppconn/driver.h>
#include <glog/logging.h>
  
/*
ExpotentialBackoff("MySQL Connector", [&connection, driver]()mutable{
            try{
                connection = std::shared_ptr<sql::Connection>(driver->connect(CONFIG_GET_STRING("sql.address"), 
                                                                    CONFIG_GET_STRING("sql.user"), 
                                                                    CONFIG_GET_STRING("sql.password")));
                return connection.get() != nullptr;
            } catch(...){
                return false;
            }
        }, 
        100);
*/

MySQLConnectionLoan::MySQLConnectionLoan(std::shared_ptr<sql::Connection> con, MySQLConnectionPool* pool){
    this->con = con;
    this->_pool = pool;
}

MySQLConnectionLoan::MySQLConnectionLoan(MySQLConnectionLoan&& l){
    this->con = l.con;
    this->_pool = l._pool;
    l.con = nullptr;
}

void MySQLConnectionLoan::operator=(MySQLConnectionLoan&& l){
    this->con = l.con;
    this->_pool = l._pool;
    l.con = nullptr;
}

MySQLConnectionLoan::~MySQLConnectionLoan(){
    if(this->con){
        this->_pool->Return(this->con);
    }
}

MySQLConnectionPool::MySQLConnectionPool(unsigned int pool_size){
    this->_pool_size = pool_size;
    sql::Driver* driver(get_driver_instance());
    while(pool_size--){
        auto connection = std::shared_ptr<sql::Connection>(driver->connect(CONFIG_GET_STRING("sql.address"), 
                                                      CONFIG_GET_STRING("sql.user"), 
                                                      CONFIG_GET_STRING("sql.password")));
        connection->setSchema(CONFIG_GET_STRING("sql.db"));
        this->_connections.push_back(connection);
    }
}

MySQLConnectionLoan MySQLConnectionPool::Get(){
    std::unique_lock<std::mutex> lck(this->_mtx);
    while(this->_used_connections == this->_pool_size){
        this->_cv.wait(lck);
    }
    auto con = this->_connections[this->_connections.size() - 1];
    this->_connections.pop_back();
    this->_used_connections++; 
    return MySQLConnectionLoan(con, this);
}

void MySQLConnectionPool::Return(std::shared_ptr<sql::Connection> con){
    std::unique_lock<std::mutex> lck(this->_mtx);
    this->_connections.push_back(con);
    this->_used_connections--;
    this->_cv.notify_one();
}

MySQLConnectionPool::~MySQLConnectionPool(){
    for(auto con : this->_connections){
        con->close();
    }
}
