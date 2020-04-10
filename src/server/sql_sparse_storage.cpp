#include "sql_sparse_storage.h"
#include <glog/logging.h>
#include "config.h"

#define SQL_INSERT_SPARSE(t) "INSERT INTO " +  t + " VALUES (?,?,?,?) ON DUPLICATE KEY UPDATE PLY_PATH = ?, MVS_PATH = ?"
#define SQL_GET_SPARSE(t) "SELECT * FROM " + t + " WHERE ID = ?"
#define SQL_GET_ALL_SPARSE(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_DELETE_SPARSE(t) "DELETE FROM " + t + " WHERE ID = ?"
#define SQL_DELETE_ALL_SPARSES(t) "DELETE FROM " + t + " WHERE RECONSTRUCTION_ID = ?"

SQLSparseStorage::SQLSparseStorage(sql::Driver* driver, 
                                   std::shared_ptr<sql::Connection> con,
                                   const std::string& table) : FileSystemStorer(CONFIG_GET_STRING("storage.root")),
                                                               SQLStorage(driver, con),
                                                               _table(table){
                                    
}

SparsePointCloudData SQLSparseStorage::Get(const std::string& sparse_id){
    SparsePointCloudMetaData meta = this->GetMeta(sparse_id);
    SparsePointCloudData sparse_data;
    sparse_data.mutable_metadata()->CopyFrom(meta);
    std::vector<char> raw_data;
    if(this->Read(meta.ply_path(), raw_data)){
        LOG(INFO) << "Read Sparse of " << raw_data.size() << " bytes";
        std::string data_str(raw_data.begin(), raw_data.end()); 
        sparse_data.set_data(data_str);
    }else{
        LOG(ERROR) << "Failed to read Sparse file of " << sparse_id;
    }
    return sparse_data;
}


SparsePointCloudMetaData SQLSparseStorage::GetMeta(const std::string& sparse_id){
    sql::ResultSet* res = this->IssueQuery(SQL_GET_SPARSE(this->_table), 
        [sparse_id](sql::PreparedStatement *stmt){
            stmt->setString(1, sparse_id);
    });
    SparsePointCloudMetaData sparse_meta;
    if(res->next()){
        sparse_meta.set_id(res->getString("ID"));
        sparse_meta.set_reconstruction(res->getString("RECONSTRUCTION_ID"));
        sparse_meta.set_ply_path(res->getString("PLY_PATH"));
        sparse_meta.set_mvs_path(res->getString("MVS_PATH"));
    }
    return sparse_meta;
}

void SQLSparseStorage::Store(const SparsePointCloudMetaData& sparse_data){
    LOG(INFO) << "Storing Sparse Data " << sparse_data.id() <<  " for " << sparse_data.reconstruction();
    this->IssueUpdate(SQL_INSERT_SPARSE(this->_table), 
        [this, sparse_data](sql::PreparedStatement *stmt){
            stmt->setString(1, sparse_data.id());
            stmt->setString(2, sparse_data.reconstruction());
            stmt->setString(3, sparse_data.ply_path());
            stmt->setString(4, sparse_data.mvs_path());
            stmt->setString(5, sparse_data.ply_path());
            stmt->setString(6, sparse_data.mvs_path());
    });
}

void SQLSparseStorage::Delete(const std::string& sparse_id){
    LOG(INFO) << "Deleting Sparse Data " << sparse_id;
    SparsePointCloudMetaData sparse_meta = GetMeta(sparse_id);
    FileSystemStorer::DeleteItem(sparse_meta.ply_path());
    FileSystemStorer::DeleteItem(sparse_meta.mvs_path());
    this->IssueUpdate(SQL_DELETE_SPARSE(this->_table), 
        [sparse_id](sql::PreparedStatement *stmt){
            stmt->setString(1, sparse_id);
    });
}

std::vector<SparsePointCloudMetaData> SQLSparseStorage::GetAll(const std::string& reconstruction_id){
    LOG(INFO) << "Retrieving all Sparses for reconstruction " << reconstruction_id;
    sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_SPARSE(this->_table), 
        [this, reconstruction_id](sql::PreparedStatement *stmt){
            stmt->setString(1, reconstruction_id);
    });

    if(!res){
        LOG(ERROR) << "Failure to retrieve results";
        return std::vector<SparsePointCloudMetaData>();
    }

    std::vector<SparsePointCloudMetaData> sparses;
    while(res->next()){
        SparsePointCloudMetaData sparse;
        sparse.set_id(res->getString("ID"));
        sparse.set_reconstruction(res->getString("RECONSTRUCTION_ID"));
        sparse.set_ply_path(res->getString("PLY_PATH"));
        sparse.set_mvs_path(res->getString("MVS_PATH"));
        sparses.push_back(sparse);
    }

    delete res;
    return sparses;
}

int SQLSparseStorage::DeleteByReconstruction(const std::string& reconstruction_id){
    std::vector<SparsePointCloudMetaData> sparses = GetAll(reconstruction_id);
    for(SparsePointCloudMetaData sparse : sparses){
        FileSystemStorer::DeleteItem(sparse.ply_path());
        FileSystemStorer::DeleteItem(sparse.mvs_path());
    }
    this->IssueUpdate(SQL_DELETE_ALL_SPARSES(this->_table), 
        [this, reconstruction_id](sql::PreparedStatement *stmt){
            stmt->setString(1, reconstruction_id);
    });
}

SparsePointCloudMetaData SQLSparseStorage::GetMetaByReconstruction(const std::string& reconstruction_id){
    std::vector<SparsePointCloudMetaData> all = this->GetAll(reconstruction_id);
    return all.empty() ? SparsePointCloudMetaData() : all[0];
}