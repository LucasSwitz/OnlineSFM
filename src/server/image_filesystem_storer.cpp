#include "image_filesystem_storer.h"
#include "config.h"

FileSystemImageDataStorage::FileSystemImageDataStorage() : FileSystemStorer(CONFIG_GET_STRING("storage.root")){

}

int FileSystemImageDataStorage::Get(const std::string& path, std::vector<char>& buf){
    return this->Get(path, buf);
}

std::string FileSystemImageDataStorage::Store(const ImageData& img, const std::string& path){
    this->Store(img, path);
}

void FileSystemImageDataStorage::Delete(const std::string& path){
    this->DeleteItem(path);
}