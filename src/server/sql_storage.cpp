#include "sql_storage.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <glog/logging.h>

SQLStorage::SQLStorage(const std::string& address, 
                                 const std::string& user, 
                                 const std::string& pass,
                                 const std::string& db):  _address(address),
                                                             _user(user),
                                                             _pass(pass),
                                                             _db(db){
    this->_driver = get_driver_instance();
};

sql::ResultSet* SQLStorage::IssueQuery(const std::string& q, 
                                       std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::Connection* con;
    sql::ResultSet* res;
    try{
        con = this->_driver->connect(this->_address, this->_user, this->_pass);
        con->setSchema(this->_db);
        res = this->IssueQuery(q, con, modifier);
        delete con;
        return res;
    }catch(sql::SQLException e){
        LOG(ERROR) << "SQL Error: "<< e.what() << e.getSQLStateCStr() << " " << e.getErrorCode();
        return nullptr;
    }
}

sql::ResultSet* SQLStorage::IssueQuery(const std::string& q, 
                                       sql::Connection* con,
                                       std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::PreparedStatement *stmt;
    sql::ResultSet* res;
    try{
        stmt = con->prepareStatement(q);
        modifier(stmt);
        res =  stmt->executeQuery();   
        delete stmt;
        return res;
    }catch(sql::SQLException e){
        LOG(ERROR) << "SQL Error: "<< e.what() << e.getSQLStateCStr() << " " << e.getErrorCode();
        return nullptr;
    }
}

void SQLStorage::IssueUpdate(const std::string& u, 
                             std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::Connection* con;
    try{
        con = this->_driver->connect(this->_address, this->_user, this->_pass);
        con->setSchema(this->_db);
        this->IssueUpdate(u, con, modifier);
        delete con;
    }catch(const sql::SQLException& e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
        throw;
    }
}

void SQLStorage::IssueUpdate(const std::string& u,
                             sql::Connection* con, 
                             std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::PreparedStatement *stmt;
    try{
        con->setSchema(this->_db);
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
    sql::Connection* con;
    try{
        con = this->_driver->connect(this->_address, this->_user, this->_pass);
        con->setSchema(this->_db);
        Execute(ex, con, modifier);
        delete con;
    }catch(sql::SQLException e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
    }
}

void SQLStorage::Execute(const std::string& ex, sql::Connection* con, std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::PreparedStatement *stmt;
    try{
        stmt = con->prepareStatement(ex);
        modifier(stmt);
        stmt->execute();
        delete stmt;
    }catch(sql::SQLException e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
    }
}

void SQLStorage::Transaction(std::function<void(sql::Connection *con)> t){
    sql::Connection* con;
    try{
        con = this->_driver->connect(this->_address, this->_user, this->_pass);
        con->setSchema(this->_db);
        con->setAutoCommit(false);
        t(con);
        con->commit();
        delete con;
    }catch(sql::SQLException e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
    }
}