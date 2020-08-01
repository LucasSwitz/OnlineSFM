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
#include "job_remote_reconstruction_agent.h"
#include "capacity_bounded_job_queue_decorator.h"
#include "ampq_job_queue.h"
#include "static_job_cost_provider.h"
#include "expotential_backoff.h"
#include <cppconn/driver.h>
#include <mutex>
#include "event2/event.h"

static std::mutex sql_driver_mux;

std::shared_ptr<ReconstructionContext> RemoteReconstructionContext(const std::string &id)
{
    try
    {
        auto intrinsics_storage = std::make_shared<SQLCameraIntrinsicsStorage>(
            CONFIG_GET_STRING("sql.intrinsics_table"));
        auto config_adapter = std::make_shared<MongoDBConfigurationAdapter>(
            CONFIG_GET_STRING("mongodb.uri"),
            CONFIG_GET_STRING("mongodb.db"),
            CONFIG_GET_STRING("mongodb.agents_collection"),
            CONFIG_GET_STRING("mongodb.default_agents_collection"),
            CONFIG_GET_STRING("mongodb.reconstructions_collections"));
        auto remote_storage = std::make_shared<RemoteStorageAdapter>(CONFIG_GET_STRING("storage.address"));
        auto image_storage = std::make_shared<SQLImageStorage>(
            remote_storage,
            CONFIG_GET_STRING("sql.views_table"));
        auto sparse_storage = std::make_shared<SQLSparseStorage>(
            CONFIG_GET_STRING("sql.sparse_table"),
            remote_storage);
        auto reconstruction_storage = std::make_shared<SQLReconstructionStorage>(
            CONFIG_GET_STRING("sql.reconstruction_table"));
        auto obj_storage = std::make_shared<SQLOBJStorage>(
            CONFIG_GET_STRING("sql.obj_table"),
            remote_storage);
        auto connection_context = std::make_shared<ReconstructionConnectionContext>(intrinsics_storage,
                                                                                    config_adapter,
                                                                                    remote_storage,
                                                                                    remote_storage,
                                                                                    image_storage,
                                                                                    sparse_storage,
                                                                                    reconstruction_storage,
                                                                                    obj_storage);
        //auto reconstruction_agent = std::make_shared<RemoteReconstructionAgent>(id, CONFIG_GET_STRING("worker_pool.address"));
        auto cost_provider = std::make_shared<StaticJobCostProvider>();
        cost_provider->Load(CONFIG_GET_STRING("jobs.cost_file"));
        auto job_queue = std::make_shared<CapacityBoundedJobQueueDecorator>(std::make_shared<AMQPJobExchange>(event_base_new(),
                                                                                                              "amqp://user:bitnami@localhost:5672",
                                                                                                              "job_exchange",
                                                                                                              "my_key"),
                                                                            cost_provider,
                                                                            JobCost());
        auto reconstruction_agent = std::make_shared<JobRemoteReconstructionAgent>(id, job_queue);
        return std::make_shared<ReconstructionContext>(id, connection_context, reconstruction_agent);
    }
    catch (const std::exception &e)
    {
        throw;
    }
}