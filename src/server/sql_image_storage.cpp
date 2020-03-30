#include "sql_image_storage.h"
#include "config.h"

#include <glog/logging.h>

#define SQL_GET_IMAGE(t) "SELECT * FROM " + t + " WHERE ID = ? "
#define SQL_GET_ALL_IMAGES(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_INSERT_IMAGE(t) "INSERT INTO " + t +  " VALUES (?,?,?,?) ON DUPLICATE KEY UPDATE PATH = ?, FORMAT = ?"
#define SQL_DELETE_IMAGE(t) "DELETE FROM "+ t + " WHERE ID = ?"
#define SQL_DELETE_ALL_IMAGES(t) "DELETE FROM "+ t + " WHERE RECONSTRUCTION_ID = ?"

SQLImageStorage::SQLImageStorage(ImageDataStorage* data_storage,
                                    const std::string& address, 
                                    const std::string& user, 
                                    const std::string& pass, 
                                    const std::string& db,
                                    const std::string& table) : 
                                                            _data_storage(data_storage),
                                                             SQLStorage(address,
                                                                        user,
                                                                        pass,
                                                                        db),
                                                             _table(table){
}

ImageMetaData SQLImageStorage::GetMeta(const std::string& image_id){
    LOG(INFO) << "Retrieving Image " << image_id;
    ImageMetaData image;
    sql::ResultSet* res = this->IssueQuery(SQL_GET_IMAGE(this->_table), 
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
    }
    return image;
}

std::vector<ImageMetaData> SQLImageStorage::GetAll(std::string reconstruction_id){
    LOG(INFO) << "Retrieving all image for reconstruction " << reconstruction_id;
    sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_IMAGES(this->_table), 
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
    std::string path = this->_data_storage->Store(image_data, rel_path);

    // Store to DB
    this->IssueUpdate(SQL_INSERT_IMAGE(this->_table), 
        [this, image_data, path](sql::PreparedStatement *stmt){
            stmt->setString(1, image_data.metadata().id());
            stmt->setString(2, image_data.metadata().reconstruction());
            stmt->setString(3, path);
            stmt->setString(4, image_data.metadata().format());
            stmt->setString(5, path);
            stmt->setString(6, image_data.metadata().format());
    });
    //TODO return a status code or something
}

int SQLImageStorage::Delete(const std::string& image_id){
    ImageMetaData img_meta = GetMeta(image_id);
    this->_data_storage->Delete(img_meta.path());
    this->IssueUpdate(SQL_DELETE_IMAGE(this->_table), 
        [this, image_id](sql::PreparedStatement *stmt){
            stmt->setString(1, image_id);
    });
    //TODO return a status code or something
}

int SQLImageStorage::DeleteByReconstruction(const std::string& reconstruction_id){
    std::vector<ImageMetaData> images = GetAll(reconstruction_id);
    for(ImageMetaData i : images){
        this->_data_storage->Delete(i.path());
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
    if(this->_data_storage->Get(meta.path(), raw_data)){
        char data_arr[raw_data.size()];
        std::copy(raw_data.begin(), raw_data.end(), data_arr);
        img_data.set_data(data_arr);
    }
    return img_data;
}
