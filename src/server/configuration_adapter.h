#pragma once
#include <string>
#include <memory>

class ConfigurationContainer{
    public:
        virtual std::string get_string(const std::string& key) = 0;
        virtual int get_int(const std::string& key) = 0;
        virtual bool get_bool(const std::string& key) = 0;
        virtual double get_double(const std::string& key) = 0;
        virtual std::string jsonify() = 0;
};

typedef std::unique_ptr<ConfigurationContainer> ConfigurationContainerPtr;

class ConfigurationAdapter {
    public:
        ConfigurationAdapter(){}
        virtual ConfigurationContainerPtr Get(const std::string& reconstruction_id, const std::string& configuration_name) = 0;
        virtual ConfigurationContainerPtr GetOrDefault(const std::string& reconstruction_id, 
                                                                     const std::string& configuration_name, 
                                                                     ConfigurationContainerPtr def);
        virtual void Set(const std::string& reconstruction_id, const std::string& configuration_name, ConfigurationContainerPtr config) = 0;
};