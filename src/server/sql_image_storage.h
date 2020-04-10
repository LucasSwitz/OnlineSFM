#pragma once
#include "image_storage.h"
#include "sql_storage.h"
#include "image_data_storage.h"
#include <functional>

class SQLImageStorage : public ImageStorageAdapter, public SQLStorage  {
    public:
        SQLImageStorage(sql::Driver* driver, 
                        std::shared_ptr<sql::Connection> con,
                        std::shared_ptr<ImageDataStorage> data_storage,
                        const std::string& table);
        ImageMetaData GetMeta(const std::string& image_id);
        std::vector<ImageMetaData> GetAll(std::string reconstruction_id);
        int Store(const ImageData& image_data);
        int Delete(const std::string& image_id);
        int DeleteByReconstruction(const std::string& reconstruction_id);
        ImageData Get(const std::string& image_id);
    private:
        std::string _table;
        std::shared_ptr<ImageDataStorage> _data_storage = nullptr;
};