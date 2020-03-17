#pragma once
#include "image_storage.h"
#include "filesystem_storer.h"
#include "sql_storage.h"
#include <functional>

class SQLImageStorage : public ImageStorageAdapter, public FileSystemStorer<ImageData>, public SQLStorage  {
    public:
        SQLImageStorage(const std::string& address, 
                                 const std::string& user, 
                                 const std::string& pass, 
                                 const std::string& db,
                                 const std::string& table);
        ImageMetaData GetMeta(const std::string& image_id);
        std::vector<ImageMetaData> GetAll(std::string reconstruction_id);
        int Store(const ImageData& image_data);
        int Delete(const std::string& image_id);
        int DeleteByReconstruction(const std::string& reconstruction_id);
        ImageData Get(const std::string& image_id);
    private:
        std::string _table;
};