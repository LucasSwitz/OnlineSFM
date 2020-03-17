#include "config.h"
#include <fstream>

ConfigManager* ConfigManager::_instance = nullptr;

ConfigManager* ConfigManager::Instance(){
    if(!_instance){
        ConfigManager::_instance = new ConfigManager();
    }
    return ConfigManager::_instance;
}

bool ConfigManager::Load(const std::string& path){
    std::ifstream i(path);
    i >> this->_config;
}

