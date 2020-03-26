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
        MongoDBConfigurationContainer(bsoncxx::document::value document) : _document(document){};
        std::string get_string(const std::string& key);
        int get_int(const std::string& key);
        long get_long(const std::string& key);
        bool get_bool(const std::string& key);
        double get_double(const std::string& key);
        std::string jsonify();
    private:
        bsoncxx::document::value _document;
};


class MongoDBConfigurationAdapter : public ConfigurationAdapter {
    public:
        MongoDBConfigurationAdapter(const std::string& uri, 
                                    const std::string& db_name, 
                                    const std::string& collection_name,
                                    const std::string& default_collection_name);
        ConfigurationContainerPtr Get(const std::string& reconstruction_id, const std::string& configuration_name);
        ConfigurationContainerPtr GetDefault(const std::string& configuration_name);
        ConfigurationContainerPtr GetOrDefault(const std::string& reconstruction_id, 
                                              const std::string& configuration_name, 
                                              ConfigurationContainerPtr def) override;
        void Set(const std::string& reconstruction_id, 
                                         const std::string& configuration_name, 
                                         ConfigurationContainerPtr config);
    private:
        static mongocxx::instance instance;
        mongocxx::client _mongodb_client;
        mongocxx::database _db;
        std::string _collection_name;
        std::string _default_collection_name;
};
