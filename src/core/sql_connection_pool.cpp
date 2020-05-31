#include "sql_connection_pool.h"
#include <mutex>
#include <condition_variable>
#include <vector>
#include "config.h"
#include <cppconn/driver.h>
#include <glog/logging.h>

template <>
std::shared_ptr<sql::Connection> MySQLConnectionPool::Create()
{
    sql::Driver *driver(get_driver_instance());
    auto connection = std::shared_ptr<sql::Connection>(driver->connect(CONFIG_GET_STRING("sql.address"),
                                                                       CONFIG_GET_STRING("sql.user"),
                                                                       CONFIG_GET_STRING("sql.password")));
    connection->setSchema(CONFIG_GET_STRING("sql.db"));
    return connection;
}

template <>
void MySQLConnectionPool::Close(std::shared_ptr<sql::Connection> con)
{
    con->close();
}