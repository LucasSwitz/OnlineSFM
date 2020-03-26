#include "configuration_adapter.h"

ConfigurationContainerPtr ConfigurationAdapter::GetOrDefault(const std::string& reconstruction_id, 
                                                                     const std::string& configuration_name, 
                                                                     ConfigurationContainerPtr def){
    ConfigurationContainerPtr config = this->Get(reconstruction_id, configuration_name);
    if(!config)
        return def;
    return config;
}