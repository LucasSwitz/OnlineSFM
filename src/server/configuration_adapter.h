#pragma once
#include <string>
#include <memory>

class ConfigurationContainer{
    public:
        virtual std::string get_string(const std::string& key) = 0;
        virtual int get_int(const std::string& key) = 0;
        virtual bool get_bool(const std::string& key) = 0;
        virtual double get_double(const std::string& key) = 0;
        virtual void patch(const std::string& json) = 0;
        virtual std::string jsonify() = 0;
};

typedef std::unique_ptr<ConfigurationContainer> ConfigurationContainerPtr;

class ConfigurationAdapter {
    public:
        ConfigurationAdapter(){}
        virtual ConfigurationContainerPtr GetReconstructionConfig(const std::string& reconstruction_id) = 0;
        virtual void SetReconstructionConfig(const std::string& reconstruction_id, const std::string& json) = 0;
        virtual ConfigurationContainerPtr GetReconstructionConfigOrDefault(const std::string& reconstruction_id, 
                                                                           ConfigurationContainerPtr def);
        virtual void SetReconstructionConfig(const std::string& reconstruction_id, ConfigurationContainerPtr config);


        virtual ConfigurationContainerPtr GetAgentConfig(const std::string& reconstruction_id, const std::string& configuration_name) = 0;
        virtual void SetAgentConfig(const std::string& reconstruction_id, const std::string& configuration_name, const std::string& json) = 0;
        virtual ConfigurationContainerPtr GetAgentConfigOrDefault(const std::string& reconstruction_id, 
                                                                  const std::string& configuration_name, 
                                                                  ConfigurationContainerPtr def);
        virtual void SetAgentConfig(const std::string& reconstruction_id, const std::string& configuration_name, ConfigurationContainerPtr config);
};