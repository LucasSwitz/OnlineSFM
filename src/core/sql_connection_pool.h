#include "connection_pool.h"
#include "mysql_connection.h"

typedef ConnectionPool<sql::Connection> MySQLConnectionPool;