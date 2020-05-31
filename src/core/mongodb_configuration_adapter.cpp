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
#define AGENT_NAME_KEY "agent_name"

mongocxx::instance MongoDBConfigurationAdapter::instance{};

MongoDBConfigurationContainer::MongoDBConfigurationContainer(bsoncxx::document::value v) : _document(v){
    //strip out the _id field
    auto doc = document{};
    for(auto it =  this->_document.view().begin(); it != this->_document.view().end(); it++){
        if(it->key().compare("_id") != 0){
            doc << it->key() << it->get_value();
        }
    }
    this->_document = doc.extract();
}

std::string MongoDBConfigurationContainer::get_string(const std::string& key){
    try {
        return this->_document.view()[key].get_utf8().value.to_string();
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what() << " " << key;
    }
}

long MongoDBConfigurationContainer::get_long(const std::string& key){
    try {
        return this->_document.view()[key].get_int64().value;
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what() << " " << key;
    }
}

int MongoDBConfigurationContainer::get_int(const std::string& key){
    try {
        return this->_document.view()[key].get_int32().value;
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what() << " " << key;
    }
}

double MongoDBConfigurationContainer::get_double(const std::string& key){
    try {
        return this->_document.view()[key].get_double().value;
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what() << " " << key;
    }
}

bool MongoDBConfigurationContainer::get_bool(const std::string& key){
    try {
        return this->_document.view()[key].get_bool().value;
    }catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what() << " " << key;
    }
}

ConfigurationContainerPtr MongoDBConfigurationContainer::get_container(const std::string& key){
    try {
        return std::make_unique<MongoDBConfigurationContainer>(bsoncxx::document::value(this->_document.view()[key].get_document().view()));
    }catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON parse error: " << e.what() << " " << key;
    }
}

std::string MongoDBConfigurationContainer::jsonify(){
    try {
        return bsoncxx::to_json(this->_document);
    } catch(bsoncxx::exception& e){
        LOG(ERROR) << "BSON jsonify error: " << e.what();
        return "{}";
    }
}

void MongoDBConfigurationContainer::patch(const std::string& json){
    auto new_bson = bsoncxx::from_json(json);
    auto doc = document{};
    for(auto it =  this->_document.view().begin(); it != this->_document.view().end(); it++){
        if(new_bson.view().find(it->key()) == new_bson.view().end()){
            doc << it->key() << it->get_value();
        }
    }
    for(auto it = new_bson.view().begin(); it != new_bson.view().end(); it++){
            doc << it->key() << it->get_value();
    }
    this->_document = doc.extract();
}

MongoDBConfigurationAdapter::MongoDBConfigurationAdapter(const std::string& uri, 
                                    const std::string& db_name, 
                                    const std::string& agents_collection_name,
                                    const std::string& default_agents_collection_name,
                                    const std::string& reconstructions_collection_name) : _mongodb_client(mongocxx::uri(uri)),
                                                                          _agents_collection(agents_collection_name),
                                                                          _reconstruction_configs_collection(reconstructions_collection_name),
                                                                          _default_agents_collection(default_agents_collection_name){
    this->_db = this->_mongodb_client[db_name];
}

ConfigurationContainerPtr MongoDBConfigurationAdapter::GetAgentConfig(const std::string& reconstruction_id, const std::string& agent_name){
    collection col = this->_db[this->_agents_collection];
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        col.find_one(document{} << RECONSTRUCTION_ID_KEY << reconstruction_id <<  AGENT_NAME_KEY << agent_name << finalize);
    
    if(maybe_result){
        return std::make_unique<MongoDBConfigurationContainer>(bsoncxx::document::value((maybe_result->view())));
    }
    return nullptr;
}

ConfigurationContainerPtr MongoDBConfigurationAdapter::GetDefault(const std::string& agent_name){
    collection col = this->_db[this->_default_agents_collection];
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        col.find_one(document{} << AGENT_NAME_KEY << agent_name << finalize);
    if(maybe_result){
        bsoncxx::document::view v = maybe_result->view();
        return std::make_unique<MongoDBConfigurationContainer>(bsoncxx::document::value(v));
    }
    LOG(ERROR) << "Unable to get default agent config in MongoDB for " << agent_name;
    return nullptr;
}

ConfigurationContainerPtr MongoDBConfigurationAdapter::GetAgentConfigOrDefault(const std::string& reconstruction_id, 
                                              const std::string& agent_name, 
                                              ConfigurationContainerPtr def){
    ConfigurationContainerPtr config = nullptr;
    if((config = this->GetAgentConfig(reconstruction_id, agent_name)) || (config = this->GetDefault(agent_name))){
        return config;
    }
    return def;
}

void MongoDBConfigurationAdapter::SetAgentConfig(const std::string& reconstruction_id, const std::string& agent_name, const std::string& config_json){
    collection configs = this->_db[this->_agents_collection];
    bsoncxx::v_noabi::document::value config_value = bsoncxx::from_json(config_json);
    options::replace options;
    options.upsert(true);

    auto filter = document{} << RECONSTRUCTION_ID_KEY << reconstruction_id << AGENT_NAME_KEY << agent_name << finalize;

    // Add the reconstruction id if its not already there
    if(config_value.view().find(RECONSTRUCTION_ID_KEY) == config_value.view().end()){
        auto doc = document{};
        doc << RECONSTRUCTION_ID_KEY << reconstruction_id;
        for(auto it =  config_value.view().begin(); it != config_value.view().end(); it++){
                doc << it->key() << it->get_value();
        }
        auto json = bsoncxx::to_json(doc);
        configs.replace_one(bsoncxx::document::view_or_value(doc.view()), 
                           bsoncxx::document::view_or_value(doc.view()),
                           options);
    }else{
        configs.replace_one(bsoncxx::document::view_or_value(filter.view()), 
                           bsoncxx::document::view_or_value(config_value.view()),
                           options);
    }
}


ConfigurationContainerPtr MongoDBConfigurationAdapter::GetReconstructionConfig(const std::string& reconstruction_id){
    collection col = this->_db[this->_reconstruction_configs_collection];
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        col.find_one(document{} << RECONSTRUCTION_ID_KEY << reconstruction_id << finalize);
    if(maybe_result){
        return std::make_unique<MongoDBConfigurationContainer>(bsoncxx::document::value((maybe_result->view())));
    }
    return nullptr;
}

void MongoDBConfigurationAdapter::SetReconstructionConfig(const std::string& reconstruction_id, const std::string& json){
    collection configs = this->_db[this->_reconstruction_configs_collection];
    bsoncxx::v_noabi::document::value config_value = bsoncxx::from_json(json);
    auto filter = document{} << RECONSTRUCTION_ID_KEY << reconstruction_id << finalize;
    options::replace options;
    options.upsert(true);
    if(config_value.view().find(RECONSTRUCTION_ID_KEY) == config_value.view().end()){
        auto doc = document{};
        for(auto it =  config_value.view().begin(); it != config_value.view().end(); it++){
                doc << it->key() << it->get_value();
        }
        doc << RECONSTRUCTION_ID_KEY << reconstruction_id;
        configs.replace_one(bsoncxx::document::view_or_value(doc.view()), 
                           bsoncxx::document::view_or_value(doc.view()),
                           options);
    }else{
        configs.replace_one(bsoncxx::document::view_or_value(filter.view()), 
                           bsoncxx::document::view_or_value(config_value.view()),
                           options);
    }
}