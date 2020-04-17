#include "sql_obj_storage.h"
#include "config.h"

#define SQL_INSERT_OBJ(t) "INSERT INTO " +  t + " VALUES (?,?,?,?,?) ON DUPLICATE KEY UPDATE PATH = ?, TEXTURE_PATH = ?, MTL_PATH = ?"
#define SQL_GET_OBJ(t) "SELECT * FROM " + t + " WHERE ID = ?"
#define SQL_GET_ALL_OBJ(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_DELETE_OBJ(t) "DELETE FROM " + t + " WHERE ID = ?"
#define SQL_DELETE_ALL_OBJS(t) "DELETE FROM " + t + " WHERE RECONSTRUCTION_ID = ?"

SQLOBJStorage::SQLOBJStorage(sql::Driver* driver, 
                             std::shared_ptr<sql::Connection> con,
                             const std::string& table) : FileSystemStorer(CONFIG_GET_STRING("storage.root")),
                                                         SQLStorage(driver, con),
                                                         _table(table){

}

OBJMetaData SQLOBJStorage::GetMeta(const std::string& obj_id){
    sql::ResultSet* res = this->IssueQuery(SQL_GET_OBJ(this->_table), 
        [obj_id](sql::PreparedStatement *stmt){
            stmt->setString(1, obj_id);
    });
    OBJMetaData obj_meta;
    if(res->next()){
        obj_meta.set_id(res->getString("ID"));
        obj_meta.set_reconstruction(res->getString("RECONSTRUCTION_ID"));
        obj_meta.set_path(res->getString("PATH"));
        obj_meta.set_texture_path(res->getString("TEXTURE_PATH"));
        obj_meta.set_mtl_path(res->getString("MTL_PATH"));
    }
    delete res;
    return obj_meta;
} 

OBJData SQLOBJStorage::Get(const std::string& obj_id){
    LOG(INFO) << "Getting OBJ " << obj_id;
    OBJMetaData meta = this->GetMeta(obj_id);
    OBJData obj_data;
    obj_data.mutable_metadata()->CopyFrom(meta);
    std::vector<char> raw_data;
    if(this->Read(meta.path(), raw_data)){
        LOG(INFO) << "Read OBJ of " << raw_data.size() << " bytes";
        std::string data_str(raw_data.begin(), raw_data.end()); 
        obj_data.set_obj_data(data_str);
    }else{
        LOG(ERROR) << "Failed to read OBJ file of " << obj_id;
    }
    raw_data.clear();
    if(this->Read(meta.texture_path(), raw_data)){
        LOG(INFO) << "Read Texture of" << raw_data.size() << " bytes";
        std::string data_str(raw_data.begin(), raw_data.end()); 
        obj_data.set_texture_data(data_str);
    }else{
        LOG(ERROR) << "Failed to read Texture of  " << obj_id;
    }
    raw_data.clear();
    if(this->Read(meta.mtl_path(), raw_data)){
        LOG(INFO) << "Read MTL of " << raw_data.size() << " bytes";
        std::string data_str(raw_data.begin(), raw_data.end()); 
        obj_data.set_mtl_data(data_str);
    }else{
        LOG(ERROR) << "Failed to read MTL of  " << obj_id;
    }
    return obj_data;
}

std::vector<OBJMetaData> SQLOBJStorage::GetAll(const std::string& reconstruction_id){
    LOG(INFO) << "Retrieving all OBJs for reconstruction " << reconstruction_id;
    sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_OBJ(this->_table), 
        [this, reconstruction_id](sql::PreparedStatement *stmt){
            stmt->setString(1, reconstruction_id);
    });

    if(!res){
        LOG(ERROR) << "Failure to retrieve results";
        return std::vector<OBJMetaData>();
    }

    std::vector<OBJMetaData> objs;
    while(res->next()){
        OBJMetaData obj;
        obj.set_id(res->getString("ID"));
        obj.set_reconstruction(res->getString("RECONSTRUCTION_ID"));
        obj.set_path(res->getString("PATH"));
        obj.set_texture_path(res->getString("TEXTURE_PATH"));
        obj.set_mtl_path(res->getString("MTL_PATH"));
        objs.push_back(obj);
    }

    delete res;
    return objs;
}


int SQLOBJStorage::Store(const OBJMetaData& obj_data){
    LOG(INFO) << "Storing OBJ Data for " << obj_data.reconstruction();
    this->IssueUpdate(SQL_INSERT_OBJ(this->_table), 
        [this, obj_data](sql::PreparedStatement *stmt){
            stmt->setString(1, obj_data.id());
            stmt->setString(2, obj_data.reconstruction());
            stmt->setString(3, obj_data.path());
            stmt->setString(4, obj_data.texture_path());
            stmt->setString(5, obj_data.mtl_path());
            stmt->setString(6, obj_data.path());
            stmt->setString(7, obj_data.texture_path());
            stmt->setString(8, obj_data.mtl_path());
    });
}

int SQLOBJStorage::Delete(const std::string& obj_id){
    LOG(INFO) << "Deleting OBJ Data " << obj_id;
    OBJMetaData obj_meta = GetMeta(obj_id);
    FileSystemStorer::DeleteItem(obj_meta.path());
    this->IssueUpdate(SQL_DELETE_OBJ(this->_table), 
        [obj_id](sql::PreparedStatement *stmt){
            stmt->setString(1, obj_id);
    });
}

int SQLOBJStorage::DeleteByReconstruction(const std::string& reconstruction_id){
    std::vector<OBJMetaData> objs = GetAll(reconstruction_id);
    for(OBJMetaData obj: objs){
        FileSystemStorer::DeleteItem(obj.path());
    }
    this->IssueUpdate(SQL_DELETE_ALL_OBJS(this->_table), 
        [this, reconstruction_id](sql::PreparedStatement *stmt){
            stmt->setString(1, reconstruction_id);
    });
}