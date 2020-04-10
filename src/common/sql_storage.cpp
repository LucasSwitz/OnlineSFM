#include "sql_storage.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <glog/logging.h>

SQLStorage::SQLStorage(sql::Driver* driver, 
                       std::shared_ptr<sql::Connection> connection):  _driver(driver),
                                                                     _con(connection){
};

sql::ResultSet* SQLStorage::IssueQuery(const std::string& q, 
                                       std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::ResultSet* res;
    try{
        res = this->IssueQuery(q, this->_con, modifier);
        return res;
    }catch(const sql::SQLException& e){
        LOG(ERROR) << "SQL Error: "<< e.what() << e.getSQLStateCStr() << " " << e.getErrorCode();
        return nullptr;
    }
}

sql::ResultSet* SQLStorage::IssueQuery(const std::string& q, 
                                       std::shared_ptr<sql::Connection> con,
                                       std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::PreparedStatement *stmt;
    sql::ResultSet* res;
    try{
        stmt = this->_con->prepareStatement(q);
        modifier(stmt);
        res =  stmt->executeQuery();   
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
        this->IssueUpdate(u, this->_con, modifier);
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
        stmt = this->_con->prepareStatement(u);
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
        Execute(ex, this->_con, modifier);
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
    try{
        this->_con->setAutoCommit(false);
        t(this->_con);
        this->_con->commit();
        this->_con->setAutoCommit(true);
    }catch(const sql::SQLException& e){
        this->_con->setAutoCommit(true);
        throw;
    }
}

SQLStorage::~SQLStorage(){
    if(!this->_con->isClosed())
    this->_con->close();
}