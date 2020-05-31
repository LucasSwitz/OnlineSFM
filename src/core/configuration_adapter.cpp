#include "configuration_adapter.h"


ConfigurationContainerPtr  ConfigurationAdapter::GetReconstructionConfigOrDefault(const std::string& reconstruction_id, 
                                                                           ConfigurationContainerPtr def){
    ConfigurationContainerPtr config = this->GetReconstructionConfig(reconstruction_id);
    if(!config)
        return def;
    return config;
}

void ConfigurationAdapter::SetReconstructionConfig(const std::string& reconstruction_id, ConfigurationContainerPtr config){
    this->SetReconstructionConfig(reconstruction_id, config->jsonify());
}

ConfigurationContainerPtr ConfigurationAdapter::GetAgentConfigOrDefault(const std::string& reconstruction_id, 
                                                                     const std::string& configuration_name, 
                                                                     ConfigurationContainerPtr def){
    ConfigurationContainerPtr config = this->GetAgentConfig(reconstruction_id, configuration_name);
    if(!config)
        return def;
    return config;
}

void ConfigurationAdapter::SetAgentConfig(const std::string& reconstruction_id, const std::string& configuration_name, ConfigurationContainerPtr config){
    this->SetAgentConfig(reconstruction_id, configuration_name, config->jsonify());
 }