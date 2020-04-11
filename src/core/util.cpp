#include "util.h"

PrecisionTimer::PrecisionTimer(const std::string& name){
    this->start = std::chrono::high_resolution_clock::now();
    this->name = name;
}

PrecisionTimer::~PrecisionTimer(){
    auto end =  std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - this->start).count();
    std::cout << this->name << ": " << duration << std::endl;
}

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