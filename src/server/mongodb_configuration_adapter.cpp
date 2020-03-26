#include "mongodb_configuration_adapter.h"

#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <glog/logging.h>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using namespace mongocxx;

#define RECONSTRUCTION_ID_KEY "reconstruction_id"
#define CONFIRUATIONS_KEY "configurations"
#define DEFAULT_CONFIGURATION_NAME_KEY "configuration"


mongocxx::instance MongoDBConfigurationAdapter::instance{};

std::string MongoDBConfigurationContainer::get_string(const std::string& key){
    try {
        return this->_document.view()[key].get_utf8().value.to_string();
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what();
    }
}

long MongoDBConfigurationContainer::get_long(const std::string& key){
    try {
        return this->_document.view()[key].get_int64().value;
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what();
    }
}

int MongoDBConfigurationContainer::get_int(const std::string& key){
    try {
        return this->_document.view()[key].get_int32().value;
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what();
    }
}

double MongoDBConfigurationContainer::get_double(const std::string& key){
    try {
        return this->_document.view()[key].get_double().value;
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what();
    }
}

bool MongoDBConfigurationContainer::get_bool(const std::string& key){
    try {
        return this->_document.view()[key].get_bool().value;
    }catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what();
    }
}

std::string MongoDBConfigurationContainer::jsonify(){
    try {
        return bsoncxx::to_json(this->_document);
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what();
    }
}

MongoDBConfigurationAdapter::MongoDBConfigurationAdapter(const std::string& uri, 
                                    const std::string& db_name, 
                                    const std::string& collection_name,
                                    const std::string& default_collection_name) : _mongodb_client(mongocxx::uri(uri)),
                                                                          _collection_name(collection_name),
                                                                          _default_collection_name(default_collection_name){
    this->_db = this->_mongodb_client[db_name];
}

ConfigurationContainerPtr MongoDBConfigurationAdapter::Get(const std::string& reconstruction_id, const std::string& configuration_name){
    collection col = this->_db[this->_collection_name];
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        col.find_one(document{} << RECONSTRUCTION_ID_KEY << reconstruction_id << finalize);
    if(maybe_result){
        bsoncxx::document::view v = maybe_result->view();
        return std::make_unique<MongoDBConfigurationContainer>(bsoncxx::document::value((maybe_result->view()[CONFIRUATIONS_KEY][configuration_name].get_document().value)));
    }
    return nullptr;
}

ConfigurationContainerPtr MongoDBConfigurationAdapter::GetDefault(const std::string& configuration_name){
    collection col = this->_db[this->_default_collection_name];
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        col.find_one(document{} << DEFAULT_CONFIGURATION_NAME_KEY << configuration_name << finalize);
    if(maybe_result){
        bsoncxx::document::view v = maybe_result->view();
        return std::make_unique<MongoDBConfigurationContainer>(bsoncxx::document::value(v));
    }
    return nullptr;
}

ConfigurationContainerPtr MongoDBConfigurationAdapter::GetOrDefault(const std::string& reconstruction_id, 
                                              const std::string& configuration_name, 
                                              ConfigurationContainerPtr def){
    ConfigurationContainerPtr config = nullptr;
    if((config = this->Get(reconstruction_id, configuration_name)) || (config = this->GetDefault(configuration_name))){
        return config;
    }
    return def;
}

void MongoDBConfigurationAdapter::Set(const std::string& reconstruction_id, const std::string& configuration_name, ConfigurationContainerPtr config){
    collection configs = this->_db[this->_collection_name];
    bsoncxx::v_noabi::document::value config_value = bsoncxx::from_json(config->jsonify());
    configs.insert_one(bsoncxx::document::view_or_value(config_value));
}