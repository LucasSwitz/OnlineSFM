#pragma once
#include "configuration_adapter.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

class MongoDBConfigurationContainer : public ConfigurationContainer{
    public:
        MongoDBConfigurationContainer(bsoncxx::document::value document);
        std::string get_string(const std::string& key);
        int get_int(const std::string& key);
        long get_long(const std::string& key);
        bool get_bool(const std::string& key);
        double get_double(const std::string& key);
        void patch(const std::string& json);
        std::string jsonify();
    private:
        bsoncxx::document::value _document;
};

class MongoDBConfigurationAdapter : public ConfigurationAdapter {
    public:
        MongoDBConfigurationAdapter(const std::string& uri, 
                                    const std::string& db_name, 
                                    const std::string& agents_collection_name,
                                    const std::string& default_agents_collection_name,
                                    const std::string& recontructions_collection_name);
        ConfigurationContainerPtr GetAgentConfig(const std::string& reconstruction_id, const std::string& configuration_name);
        ConfigurationContainerPtr GetDefault(const std::string& configuration_name);
        ConfigurationContainerPtr GetReconstructionConfig(const std::string& reconstruction_id);
        void SetReconstructionConfig(const std::string& reconstruction_id, const std::string& json);
        ConfigurationContainerPtr GetAgentConfigOrDefault(const std::string& reconstruction_id, 
                                              const std::string& configuration_name, 
                                              ConfigurationContainerPtr def) override;
        void SetAgentConfig(const std::string& reconstruction_id, 
                                         const std::string& configuration_name, 
                                         const std::string& config_json);
    private:
        static mongocxx::instance instance;
        mongocxx::client _mongodb_client;
        mongocxx::database _db;
        std::string _agents_collection;
        std::string _reconstruction_configs_collection;
        std::string _default_agents_collection;
};
