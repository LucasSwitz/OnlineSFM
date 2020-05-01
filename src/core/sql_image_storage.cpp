#include "sql_image_storage.h"
#include "config.h"

#include <glog/logging.h>

#define SQL_GET_IMAGE(t) "SELECT * FROM " + t + " WHERE ID = ? "
#define SQL_GET_ALL_IMAGES(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_INSERT_IMAGE(t) "INSERT INTO " + t +  " VALUES (?,?,?,?,?) ON DUPLICATE KEY UPDATE PATH = ?, FORMAT = ?"
#define SQL_DELETE_IMAGE(t) "DELETE FROM "+ t + " WHERE ID = ?"
#define SQL_DELETE_ALL_IMAGES(t) "DELETE FROM "+ t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_INSERT_UNDISTORTED(t) "UPDATE " + t + " SET UNDISTORTED_PATH = ? WHERE ID = ?"

SQLImageStorage::SQLImageStorage(
                                 std::shared_ptr<ImageDataStorage> data_storage,
                                 const std::string& table) : 
                                                            _data_storage(data_storage),
                                                            _table(table){
}

ImageMetaData SQLImageStorage::GetMeta(const std::string& image_id){
    auto connection_loan = this->GetConnection();
    ImageMetaData image;
    sql::ResultSet* res = this->IssueQuery(SQL_GET_IMAGE(this->_table), connection_loan.con,
        [this, image_id](sql::PreparedStatement *stmt){
            stmt->setString(1, image_id);
    });

    if(!res){
        LOG(ERROR) << "Failed to retrieve metadata for image " << image_id;
        return image;
    }

    if(res->next()){
        image.set_id(res->getString("ID"));
        image.set_reconstruction(res->getString("RECONSTRUCTION_ID"));
        image.set_path(res->getString("PATH"));
        image.set_undistorted_path(res->getString("UNDISTORTED_PATH"));
        image.set_format(res->getString("FORMAT"));
    }
    delete res;
    return image;
}

std::vector<ImageMetaData> SQLImageStorage::GetAll(std::string reconstruction_id){
    auto connection_loan = this->GetConnection();
    LOG(INFO) << "Retrieving all image metadata for reconstruction " << reconstruction_id;
    sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_IMAGES(this->_table), connection_loan.con,
        [this, reconstruction_id](sql::PreparedStatement *stmt){
            stmt->setString(1, reconstruction_id);
    });

    if(!res){
        LOG(ERROR) << "Failure to retrieve results";
        return std::vector<ImageMetaData>();
    }

    std::vector<ImageMetaData> images;
    while(res->next()){
        ImageMetaData img;
        img.set_id(res->getString("ID"));
        img.set_reconstruction(res->getString("RECONSTRUCTION_ID"));
        img.set_path(res->getString("PATH"));
        img.set_format(res->getString("FORMAT"));
        images.push_back(img);
    }
    delete res;
    return images;
}

int SQLImageStorage::Store(const ImageData& image_data){
    LOG(INFO) << "Storing image " << image_data.metadata().id() << " for reconstruction " << image_data.metadata().reconstruction();
    // Store to filesystem
    std::string rel_path = image_data.metadata().reconstruction() + 
                                                  "/images/" + 
                                                  image_data.metadata().id() + "." + image_data.metadata().format();
    std::string path = this->_data_storage->StoreImage(image_data, rel_path);

    // Store to DB
    this->IssueUpdate(SQL_INSERT_IMAGE(this->_table), 
        [this, image_data, path](sql::PreparedStatement *stmt){
            stmt->setString(1, image_data.metadata().id());
            stmt->setString(2, image_data.metadata().reconstruction());
            stmt->setString(3, path);
            stmt->setString(4, image_data.metadata().format());
            stmt->setString(5, "");
            stmt->setString(6, path);
            stmt->setString(7, image_data.metadata().format());
    });
    //TODO return a status code or something
}

int SQLImageStorage::Delete(const std::string& image_id){
    ImageMetaData img_meta = GetMeta(image_id);
    this->_data_storage->DeleteImage(img_meta.path());
    this->IssueUpdate(SQL_DELETE_IMAGE(this->_table), 
        [this, image_id](sql::PreparedStatement *stmt){
            stmt->setString(1, image_id);
    });
    //TODO return a status code or something
}

int SQLImageStorage::DeleteByReconstruction(const std::string& reconstruction_id){
    std::vector<ImageMetaData> images = GetAll(reconstruction_id);
    for(ImageMetaData i : images){
        this->_data_storage->DeleteImage(i.path());
    }
    this->IssueUpdate(SQL_DELETE_ALL_IMAGES(this->_table), 
        [this, reconstruction_id](sql::PreparedStatement *stmt){
            stmt->setString(1, reconstruction_id);
    });
}

ImageData SQLImageStorage::Get(const std::string& image_id){
    ImageMetaData meta = this->GetMeta(image_id);
    ImageData img_data;
    img_data.mutable_metadata()->CopyFrom(meta);
    std::vector<char> raw_data;
    if(this->_data_storage->GetImage(meta.path(), raw_data)){
        std::string data_str(raw_data.begin(), raw_data.end());
        img_data.set_data(data_str);
    }
    return img_data;
}

void SQLImageStorage::StoreUndistorted(const std::string& image_id, ImageData& undistored_data){
    LOG(INFO) << "Storing undistorted image " << undistored_data.metadata().id() 
              << " for reconstruction " << undistored_data.metadata().reconstruction();
    // Store to filesystem
    std::string rel_path = undistored_data.metadata().reconstruction() + 
                                                  "/images/" + 
                                                  undistored_data.metadata().id() + "." + undistored_data.metadata().format();
    std::string path = this->_data_storage->StoreImage(undistored_data, rel_path);
    // Store to DB
    this->IssueUpdate(SQL_INSERT_UNDISTORTED(this->_table), 
        [this, image_id, path](sql::PreparedStatement *stmt){
            stmt->setString(1, path);
            stmt->setString(2, image_id);
    });
}

ImageData SQLImageStorage::GetUndistorted(const std::string& image_id){
    ImageMetaData meta = this->GetMeta(image_id);
    ImageData img_data;
    std::vector<char> raw_data;
    if(this->_data_storage->GetImage(meta.undistorted_path(), raw_data)){
        std::string data_str(raw_data.begin(), raw_data.end());
        img_data.set_data(data_str);
    }
    return img_data;
}