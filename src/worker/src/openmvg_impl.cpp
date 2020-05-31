#include "openmvg_reconstruction_agent.h"
#include "sql_regions_storage.h"
#include "sql_openmvg_storage.h"
#include "sql_image_storage.h"
#include "sql_recontruction_storage.h"
#include "sql_sparse_storage.h"
#include "sql_obj_storage.h"
#include "sql_camera_intrinsics_storage.h"
#include "mongodb_configuration_adapter.h"
#include "config.h"
#include "reconstruction.h"
#include <mutex>

static std::mutex sql_driver_mux;

std::shared_ptr<ReconstructionContext> OpenMVGReconstructionContext(const std::string &id)
{
    try
    {
        auto intrinsics_storage = std::make_shared<SQLCameraIntrinsicsStorage>(CONFIG_GET_STRING("sql.intrinsics_table"));
        auto config_adapter = std::make_shared<MongoDBConfigurationAdapter>(
            CONFIG_GET_STRING("mongodb.uri"),
            CONFIG_GET_STRING("mongodb.db"),
            CONFIG_GET_STRING("mongodb.agents_collection"),
            CONFIG_GET_STRING("mongodb.default_agents_collection"),
            CONFIG_GET_STRING("mongodb.reconstructions_collections"));
        auto remote_storage = std::make_shared<RemoteStorageAdapter>(CONFIG_GET_STRING("storage.address"));
        auto image_storage = std::make_shared<SQLImageStorage>(remote_storage,
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
        auto reconstruction_agent = std::make_shared<OpenMVGReconstructionAgent>(
            id,
            intrinsics_storage,
            std::make_shared<SQLOpenMVGStorage>(
                CONFIG_GET_STRING("sql.openmvg_views_table"),
                CONFIG_GET_STRING("sql.openmvg_intrinsics_table"),
                CONFIG_GET_STRING("sql.openmvg_matches_table"),
                CONFIG_GET_STRING("sql.openmvg_meta_table"),
                CONFIG_GET_STRING("sql.openmvg_poses_table"),
                CONFIG_GET_STRING("sql.openmvg_landmarks_table")),
            config_adapter,
            std::make_shared<SQLRegionsStorage<openMVG::features::SIFT_Regions>>(
                CONFIG_GET_STRING("sql.regions_table")),
            image_storage,
            sparse_storage,
            obj_storage);

        return std::make_shared<ReconstructionContext>(id, connection_context, reconstruction_agent);
    }
    catch (const std::exception &e)
    {
        throw;
    }
}