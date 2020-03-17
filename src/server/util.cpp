#include "util.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

using namespace boost::uuids;

std::string GetUUID(){
    random_generator gen;
    uuid id = gen();
    return to_string(id);
}

void DeleteIfExists(const std::string& path){
    if(boost::filesystem::exists(path)){
        boost::filesystem::remove(path);
    }
}

void DeleteDirIfExists(const std::string& path){
    if(boost::filesystem::exists(path)){
        boost::filesystem::remove_all(path);
    }
}

void CleanAndMakeDir(const std::string& path){
    DeleteDirIfExists(path);
    boost::filesystem::create_directory(path);
}