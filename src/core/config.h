#pragma once
#define CONFIG_GET_STRING(path) ConfigManager::Instance()->Get<std::string>(path)
#define CONFIG_GET_BOOL(path) ConfigManager::Instance()->Get<bool>(path)
#define CONFIG_GET_DOUBLE(path) ConfigManager::Instance()->Get<double>(path)
#define CONFIG_GET_INT(path) ConfigManager::Instance()->Get<int>(path)
#define CONFIG_GET_LIST(path) ConfigManager::Instance()->Get<std::vector<std::string>>(path)

#define CONFIG_LOAD(path) ConfigManager::Instance()->Load(path)

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static int get_next_first_object(json *parent, const std::string &path, json **object)
{
    int pos;
    if ((pos = path.find(".")) != std::string::npos)
    {
        std::string k = path.substr(0, pos);
        if (parent->find(k) == parent->end())
            throw std::runtime_error("Config element not found: " + path);
        *object = &(*parent)[k];
        return pos + 1;
    }
    *object = parent;
    return -1;
}

class ConfigManager
{
public:
    static ConfigManager *Instance();
    std::string GetString(const std::string &path);
    bool Load(const std::string &path);
    void Set(json &j)
    {
        _config = j;
    }
    template <class T>
    T Get(const std::string &path)
    {
        std::string path_cp(path);
        json *object = nullptr;
        json *parent = &this->_config;
        int last_index;
        while ((last_index = get_next_first_object(parent, path_cp, &object)) != -1)
        {
            path_cp = path_cp.substr(last_index, path_cp.size());
            parent = object;
        }
        if (object->find(path_cp) == object->end())
            throw std::runtime_error("Config element not found: " + path);
        return (*object)[path_cp].get<T>();
    }

private:
    ConfigManager(){};
    static ConfigManager *_instance;
    json _config;
};