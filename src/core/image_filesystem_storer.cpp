#include "image_filesystem_storer.h"
#include "config.h"

FileSystemImageDataStorage::FileSystemImageDataStorage() : FileSystemStorer(CONFIG_GET_STRING("storage.root")){

}

int FileSystemImageDataStorage::GetImage(const std::string& path, std::vector<char>& buf){
    return FileSystemStorer::Read(path, buf);
}

std::string FileSystemImageDataStorage::StoreImage(const ImageData& img, const std::string& path){
    return FileSystemStorer::Store(img.data(), path);
}

void FileSystemImageDataStorage::DeleteImage(const std::string& path){
    this->DeleteItem(path);
}