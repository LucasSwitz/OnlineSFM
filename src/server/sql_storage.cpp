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
    sql::PreparedStatement *stmt;
    sql::ResultSet* res;
    try{
        con = this->_driver->connect(this->_address, this->_user, this->_pass);
        con->setSchema(this->_db);
        stmt = con->prepareStatement(q);
        modifier(stmt);
        res =  stmt->executeQuery();   
        delete stmt;
        delete con;

        return res;
    }catch(sql::SQLException e){
        LOG(ERROR) << "SQL Error: "<< e.what() << e.getSQLStateCStr() << " " << e.getErrorCode();
        return nullptr;
    }
}

void SQLStorage::IssueUpdate(const std::string& u, 
                             std::function<void(sql::PreparedStatement *stmt)> modifier){
    sql::Connection* con;
    sql::PreparedStatement *stmt;
    try{
        con = this->_driver->connect(this->_address, this->_user, this->_pass);
        con->setSchema(this->_db);
        stmt = con->prepareStatement(u);
        modifier(stmt);
        stmt->execute();   
        delete stmt;
        delete con;
    }catch(sql::SQLException e){
        LOG(ERROR) << "SQL Error: " << e.what() << " " << e.getSQLStateCStr() << " " << e.getErrorCode();
    }
}