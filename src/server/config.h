#pragma once
#define CONFIG_GET_STRING(path) ConfigManager::Instance()->Get<std::string>(path)
#define CONFIG_GET_BOOL(path) ConfigManager::Instance()->Get<bool>(path)
#define CONFIG_GET_DOUBLE(path) ConfigManager::Instance()->Get<double>(path)
#define CONFIG_GET_INT(path) ConfigManager::Instance()->Get<int>(path)
#define CONFIG_LOAD(path) ConfigManager::Instance()->Load(path)

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static int get_next_first_object(json* parent, const std::string& path, json** object){  
    int pos;
    if((pos = path.find(".")) != std::string::npos){
        *object = &(*parent)[path.substr(0, pos)];
        return pos+1;
    }
    *object = parent;
    return -1;
}

class ConfigManager {
    public:
        static ConfigManager* Instance();
        std::string GetString(const std::string& path);
        bool Load(const std::string& path);

        template <class T>
        T Get(const std::string& path){
            std::string path_cp(path);
            json* object = nullptr;
            json* parent = &this->_config;
            int last_index;
            while((last_index = get_next_first_object(parent, path_cp, &object)) != -1){
                path_cp = path_cp.substr(last_index, path_cp.size());
                parent = object;
            }
            return (*object)[path_cp].get<T>();
        }  

    private:
        ConfigManager(){};
        static ConfigManager* _instance;
        json _config;
};