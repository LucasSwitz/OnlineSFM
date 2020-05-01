#include "sql_storage.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <glog/logging.h>


SQLStorage::SQLStorage(){};

std::unique_ptr<MySQLConnectionPool> SQLStorage::_connection_pool;

void SQLStorage::InitConnectionPool(unsigned int size){
    SQLStorage::_connection_pool = std::make_unique<MySQLConnectionPool>(size);
}

sql::ResultSet* SQLStorage::IssueQuery(const std::string& q, 
                                       std::shared_ptr<sql::Connection> con,
                                       std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::PreparedStatement *stmt;
    sql::ResultSet* res;
    try{
        stmt = con->prepareStatement(q);
        modifier(stmt);
        res = stmt->executeQuery();   
        delete stmt;
        return res;
    }catch(const sql::SQLException& e){
        LOG(ERROR) << "SQL Error: "<< e.what() << e.getSQLStateCStr() << " " << e.getErrorCode();
        return nullptr;
    }
}
#include "util.h"

void SQLStorage::IssueUpdate(const std::string& u, 
                             std::function<void(sql::PreparedStatement *stmt)> modifier){
    try{
        auto con_loan = this->_connection_pool->Get();
        this->IssueUpdate(u, con_loan.con, modifier);
    }catch(const sql::SQLException& e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
        throw;
    }
}

void SQLStorage::IssueUpdate(const std::string& u,
                             std::shared_ptr<sql::Connection> con, 
                             std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::PreparedStatement *stmt;
    try{
        stmt = con->prepareStatement(u);
        modifier(stmt);
        stmt->execute();   
        delete stmt;
    }catch(const sql::SQLException& e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
        throw;
    }
}


void SQLStorage::Execute(const std::string& ex, std::function<void(sql::PreparedStatement *stmt)> modifier){
    try{
        auto con_loan = this->_connection_pool->Get();
        Execute(ex, con_loan.con, modifier);
    }catch(const sql::SQLException& e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
        throw;
    }
}

void SQLStorage::Execute(const std::string& ex, std::shared_ptr<sql::Connection> con, std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::PreparedStatement *stmt;
    try{
        stmt = con->prepareStatement(ex);
        modifier(stmt);
        stmt->execute();
        delete stmt;
    }catch(const sql::SQLException& e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
        throw;
    }
}

void SQLStorage::Transaction(std::function<void(std::shared_ptr<sql::Connection> con)> t){
    auto con_loan = this->_connection_pool->Get();
    try{
        con_loan.con->setAutoCommit(false);
        t(con_loan.con);
        con_loan.con->commit();
        con_loan.con->setAutoCommit(true);
    }catch(const sql::SQLException& e){
        con_loan.con->setAutoCommit(true);
        throw;
    }
}

MySQLConnectionLoan SQLStorage::GetConnection(){
    return this->_connection_pool->Get();
}

SQLStorage::~SQLStorage(){
    
}