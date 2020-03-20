#include "sql_recontruction_storage.h"
#include <glog/logging.h>
#include <boost/filesystem.hpp>

#define SQL_INSERT_RECONSTRUCTION(t) "INSERT INTO " + t + " VALUES (?,?,?,?,?,?,?) ON DUPLICATE KEY UPDATE IMAGES_PATH = ?, FEATURES_PATH = ?, MATCHES_PATH = ?, SFM_PATH = ?, MVS_PATH = ?"
#define SQL_GET_RECONSTRUCTION(t) "SELECT * FROM " + t + " WHERE ID = ?"
#define SQL_DELETE_RECONSTRUCTION(t) "DELETE FROM " + t + " WHERE ID = ?"

SQLReconstructionStorage::SQLReconstructionStorage(const std::string& address, 
                                                        const std::string& user, 
                                                        const std::string& pass, 
                                                        const std::string& db,
                                                        const std::string& table) : SQLStorage(address, user, pass, db),
                                                                                    _table(table){
    
}

void SQLReconstructionStorage::Store(const ReconstructionData& data){
    LOG(INFO) << "Storing reconstruction " << data.id();
    this->IssueUpdate(SQL_INSERT_RECONSTRUCTION(this->_table), [this, data](sql::PreparedStatement* pstmt){
        pstmt->setString(1, data.id());
        pstmt->setString(2, data.root());
        pstmt->setString(3, data.images_path());
        pstmt->setString(4, data.features_path());
        pstmt->setString(5, data.matches_path());
        pstmt->setString(6, data.sfm_path());
        pstmt->setString(7, data.mvs_path());
        pstmt->setString(8, data.images_path());
        pstmt->setString(9, data.features_path());
        pstmt->setString(10, data.matches_path());
        pstmt->setString(11, data.sfm_path());
        pstmt->setString(12, data.mvs_path());

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
        reconstruction_data.set_root(res->getString("ROOT_PATH"));
        reconstruction_data.set_images_path(res->getString("IMAGES_PATH"));
        reconstruction_data.set_mvs_path(res->getString("MVS_PATH"));
        reconstruction_data.set_sfm_path(res->getString("SFM_PATH"));
        reconstruction_data.set_features_path(res->getString("FEATURES_PATH"));
        reconstruction_data.set_matches_path(res->getString("MATCHES_PATH"));
    }
    return reconstruction_data;
}

void SQLReconstructionStorage::Delete(const std::string& id){
    LOG(INFO) << "Deleting Reconstruction " << id;
    ReconstructionData reconstruction_data = Get(id);
    boost::filesystem::remove_all(reconstruction_data.root());
    this->IssueUpdate(SQL_DELETE_RECONSTRUCTION(this->_table), 
        [id](sql::PreparedStatement *stmt){
            stmt->setString(1, id);
    });
}
