#include "sql_recontruction_storage.h"
#include <glog/logging.h>
#include <boost/filesystem.hpp>

#define SQL_INSERT_RECONSTRUCTION(t) "INSERT IGNORE INTO " + t + " VALUES (?,?) ON DUPLICATE KEY UPDATE PATH = ?"
#define SQL_GET_RECONSTRUCTION(t) "SELECT * FROM " + t + " WHERE ID = ?"
#define SQL_DELETE_RECONSTRUCTION(t) "DELETE FROM " + t + " WHERE ID = ?"

SQLReconstructionStorage::SQLReconstructionStorage(const std::string& address, 
                                                        const std::string& user, 
                                                        const std::string& pass, 
                                                        const std::string& db,
                                                        const std::string& table) : SQLStorage(address, user, pass, db),
                                                                                    _table(table){
    
}

void SQLReconstructionStorage::CreateIfNew(const std::string& id, const std::string& path){
    LOG(INFO) << "Creating reconstruction " << id << "if it does not exist";
    this->IssueUpdate(SQL_INSERT_RECONSTRUCTION(this->_table), [this, id, path](sql::PreparedStatement* pstmt){
        pstmt->setString(1, id);
        pstmt->setString(2, path);
        pstmt->setString(3, path);
    });
}

ReconstructionData SQLReconstructionStorage::Get(const std::string& id){
    sql::ResultSet* res = this->IssueQuery(SQL_GET_RECONSTRUCTION(this->_table), 
        [id](sql::PreparedStatement *stmt){
            stmt->setString(1, id);
    });
    ReconstructionData reconstruction_data;
    if(res->next()){
        reconstruction_data.set_id(res->getString("ID"));
        reconstruction_data.set_path(res->getString("PATH"));
    }
    return reconstruction_data;
}

void SQLReconstructionStorage::Delete(const std::string& id){
    LOG(INFO) << "Deleting Reconstruction " << id;
    ReconstructionData reconstruction_data = Get(id);
    boost::filesystem::remove_all(reconstruction_data.path());
    this->IssueUpdate(SQL_DELETE_RECONSTRUCTION(this->_table), 
        [id](sql::PreparedStatement *stmt){
            stmt->setString(1, id);
    });
}
