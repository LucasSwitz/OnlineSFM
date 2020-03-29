#pragma once
#include <string>
#include <ostream>
#include <glog/logging.h>
#include <fstream>
#include <iostream> 
#include <sys/types.h> 
#include <boost/filesystem.hpp>

template<typename T>
class FileSystemStorer{
    public:
        FileSystemStorer(const std::string& storage_root) : _storage_root(storage_root){};
        std::string Store(T pb, const std::string& path){
            std::filebuf fb;
            std::string store_location = this->_storage_root + "/" + path;
            fb.open(store_location , std::ios::out|std::ios::binary);
            std::ostream os(&fb);
            os << pb.data();
            fb.close();
            LOG(INFO) << "Finished Writing file to disk " << store_location; 
            return store_location;
        }
        void Delete(const std::string& reconstruction_id, const std::string& subpath, const std::string& item_name){
            std::string store_location = this->_storage_root + "/" + reconstruction_id + "/" + subpath + "/" + item_name;
            if(!boost::filesystem::exists(store_location)) return;
            if(boost::filesystem::remove(store_location)){
                LOG(INFO) << "Deleted file" << store_location;
            }else{
                LOG(ERROR) << "Failed to delete file " << store_location;
            }
        }
        void DeleteDir(const std::string& path){
            if(!boost::filesystem::exists(path)) return;
            if(boost::filesystem::remove_all(path)){
                LOG(INFO) << "Deleted file" << path;
            }else{
                LOG(ERROR) << "Failed to delete file " << path;
            }
        }
        void DeleteItem(const std::string& path){
            if(!boost::filesystem::exists(path)) return;
            if(boost::filesystem::remove(path)){
                LOG(INFO) << "Deleted file" << path;
            }else{
                LOG(ERROR) << "Failed to delete file " << path;
            }
        }
        bool Read(const std::string& path, std::vector<char>& data){
            if(!boost::filesystem::exists(path)){
                LOG(ERROR) << "File does not exist " << path;
                return false;
            }
            std::ifstream file(path, std::ios::binary);
            file.unsetf(std::ios::skipws);
            std::streampos fileSize;
            file.seekg(0, std::ios::end);
            fileSize = file.tellg();
            LOG(INFO) << "Reading " << path << " of size " << fileSize;
            file.seekg(0, std::ios::beg);
            data.reserve(fileSize);
            data.insert(data.begin(),
               std::istream_iterator<char>(file),
               std::istream_iterator<char>());
            return data.size() != 0;
    }
    private:
        std::string _storage_root;
};