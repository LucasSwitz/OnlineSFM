#pragma once

#include "mysql_connection.h"
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

class SQLStorage {
    public:
        SQLStorage(const std::string& address, 
                   const std::string& user, 
                   const std::string& pass,
                   const std::string& db);
        sql::ResultSet* IssueQuery(const std::string& q, 
                                   std::function<void(sql::PreparedStatement *stmt)> modifier);
        sql::ResultSet* IssueQuery(const std::string& q, 
                                   sql::Connection* con, 
                                   std::function<void(sql::PreparedStatement *stmt)> modifier);
        void IssueUpdate(const std::string& u, 
                         std::function<void(sql::PreparedStatement *stmt)> modifier);

        void IssueUpdate(const std::string& u, 
                             sql::Connection* con, 
                             std::function<void(sql::PreparedStatement *stmt)> modifier);
        void Execute(const std::string& e, std::function<void(sql::PreparedStatement *stmt)> modifier);
        void Execute(const std::string& ex, sql::Connection* con, std::function<void(sql::PreparedStatement *stmt)> modifier);
        void Transaction(std::function<void(sql::Connection *con)> t);
        virtual ~SQLStorage();
    private:
        std::string _db;
        std::string _address;
        std::string _user;
        std::string _pass;
        sql::Driver* _driver;
        sql::Connection* _con;

};