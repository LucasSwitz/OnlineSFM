#pragma once

#include "mysql_connection.h"
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

class SQLStorage {
    public:
        SQLStorage(sql::Driver* driver, 
                   std::shared_ptr<sql::Connection> connection);
        sql::ResultSet* IssueQuery(const std::string& q, 
                                   std::function<void(sql::PreparedStatement *stmt)> modifier);
        sql::ResultSet* IssueQuery(const std::string& q, 
                                   std::shared_ptr<sql::Connection> con, 
                                   std::function<void(sql::PreparedStatement *stmt)> modifier);
        void IssueUpdate(const std::string& u, 
                         std::function<void(sql::PreparedStatement *stmt)> modifier);

        void IssueUpdate(const std::string& u, 
                             std::shared_ptr<sql::Connection> con, 
                             std::function<void(sql::PreparedStatement *stmt)> modifier);
        void Execute(const std::string& e, std::function<void(sql::PreparedStatement *stmt)> modifier);
        void Execute(const std::string& ex, std::shared_ptr<sql::Connection> con, std::function<void(sql::PreparedStatement *stmt)> modifier);
        void Transaction(std::function<void(std::shared_ptr<sql::Connection> con)> t);
        virtual ~SQLStorage();
    private:
        sql::Driver* _driver;
        std::shared_ptr<sql::Connection> _con;

};