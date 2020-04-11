#include "image_filesystem_storer.h"
#include "config.h"

FileSystemImageDataStorage::FileSystemImageDataStorage() : FileSystemStorer(CONFIG_GET_STRING("storage.root")){

}

int FileSystemImageDataStorage::Get(const std::string& path, std::vector<char>& buf){
    return FileSystemStorer::Read(path, buf);
}

std::string FileSystemImageDataStorage::Store(const ImageData& img, const std::string& path){
    return FileSystemStorer::Store(img.data(), path);
}

void FileSystemImageDataStorage::Delete(const std::string& path){
    this->DeleteItem(path);
}