#include "sql_regions_storage.h"
#include "sql_image_storage.h"
#include "sql_recontruction_storage.h"
#include "sql_sparse_storage.h"
#include "sql_obj_storage.h"
#include "sql_camera_intrinsics_storage.h"
#include "mongodb_configuration_adapter.h"
#include "config.h"
#include "reconstruction.h"
#include "remote_reconstruction_agent.h"
#include "expotential_backoff.h"
#include <cppconn/driver.h>
#include <mutex>

static std::mutex sql_driver_mux;

std::shared_ptr<ReconstructionContext> RemoteReconstructionContext(const std::string& id){
     try{ 
        sql_driver_mux.lock();
        sql::Driver* driver(get_driver_instance());
        sql_driver_mux.unlock();
        std::shared_ptr<sql::Connection> connection;
        ExpotentialBackoff("MySQL Connector", [&connection, driver]()mutable{
            try{
                connection = std::shared_ptr<sql::Connection>(driver->connect(CONFIG_GET_STRING("sql.address"), 
                                                                    CONFIG_GET_STRING("sql.user"), 
                                                                    CONFIG_GET_STRING("sql.password")));
                return connection.get() != nullptr;
            } catch(...){
                return false;
            }
        }, 
        100);
        connection->setSchema(CONFIG_GET_STRING("sql.db"));
        auto intrinsics_storage = std::make_shared<SQLCameraIntrinsicsStorage>(
                                                        driver,
                                                        connection, 
                                                        CONFIG_GET_STRING("sql.intrinsics_table"));
        auto config_adapter = std::make_shared<MongoDBConfigurationAdapter>(
                                                    CONFIG_GET_STRING("mongodb.uri"),
                                                    CONFIG_GET_STRING("mongodb.db"),
                                                    CONFIG_GET_STRING("mongodb.agents_collection"),
                                                    CONFIG_GET_STRING("mongodb.default_agents_collection"),
                                                    CONFIG_GET_STRING("mongodb.reconstructions_collections"));
        auto remote_storage = std::make_shared<RemoteStorageAdapter>(CONFIG_GET_STRING("storage.address"));
        auto image_storage = std::make_shared<SQLImageStorage>( 
                                                        driver,
                                                        connection, 
                                                        remote_storage, 
                                                        CONFIG_GET_STRING("sql.views_table"));
        auto sparse_storage = std::make_shared<SQLSparseStorage>(
                                    driver,
                                    connection, 
                                    CONFIG_GET_STRING("sql.sparse_table"),
                                    remote_storage);
        auto reconstruction_storage = std::make_shared<SQLReconstructionStorage>(
                                driver,
                                connection, 
                                CONFIG_GET_STRING("sql.reconstruction_table"));
        auto obj_storage = std::make_shared<SQLOBJStorage>(
                                    driver,
                                    connection, 
                                    CONFIG_GET_STRING("sql.obj_table"));
        auto connection_context = std::make_shared<ReconstructionConnectionContext>(intrinsics_storage,
                                                                                    config_adapter,
                                                                                    remote_storage,
                                                                                    remote_storage,
                                                                                    image_storage,
                                                                                    sparse_storage,
                                                                                    reconstruction_storage,
                                                                                    obj_storage);
        auto reconstruction_agent = std::make_shared<RemoteReconstructionAgent>(id, CONFIG_GET_STRING("worker_pool.address"));
        return std::make_shared<ReconstructionContext>(id, connection_context, reconstruction_agent);
     }catch(const std::exception& e){
         throw;
     }
        
}