#include "openmvs_strategy.h"
#include <glog/logging.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include "util.h"
#include "config.h"
#include "openmvs.h"

#define cc(s) const_cast<char *>(s.c_str())

bool DoOpenMVS(const std::string reconstruction_id,
               const std::string &mvs_path,
               OBJMetaData &obj_data);

int OpenMVSStrategy::DoMVS(const std::string &reconstruction_id,
                           const std::string &mvs_path,
                           OBJMetaData &obj_data)
{
    return DoOpenMVS(reconstruction_id, mvs_path, obj_data);
}

bool DoOpenMVS(const std::string reconstruction_id,
               const std::string &mvs_path,
               OBJMetaData &obj_data)
{
    std::string scene_path = mvs_path + "/scene.mvs";
    std::string dense_scene_path = mvs_path + "/scene_dense.mvs";
    std::string dense_mesh_scene_path = mvs_path + "/scene_dense_mesh.mvs";
    std::string dense_mesh_refined_scene_path = mvs_path + "/scene_dense_mesh_refine.mvs";
    std::string dense_mesh_refined_textured_path = mvs_path + "/scene_dense_mesh_refine_texture.mvs";
    std::string obj_path = mvs_path + "/scene_dense_mesh_refine_texture.obj";
    std::string texture_path = mvs_path + "/scene_dense_mesh_refine_texture_material_0_map_Kd.jpg";
    std::string mtl_path = mvs_path + "/scene_dense_mesh_refine_texture.mtl";
    LOG(INFO) << "Creating Dense Point Cloud for " << reconstruction_id;
    if (!OpenMVSDensifyPointCloud(mvs_path, scene_path, 1))
    {
        return false;
    }
    LOG(INFO) << "Reconstructing Mesh for " << reconstruction_id;
    if (!OpenMVSReconstructMesh(mvs_path, dense_scene_path))
    {
        return false;
    }
    LOG(INFO) << "Refining Mesh for " << reconstruction_id;
    if (!OpenMVSRefineMesh(mvs_path, dense_mesh_scene_path, 16))
    {
        return false;
    }
    LOG(INFO) << "Texturing Mesh for " << reconstruction_id;
    if (!OpenMVSTextureMesh(mvs_path, dense_mesh_refined_scene_path))
    {
        return false;
    }
    obj_data.set_reconstruction(reconstruction_id);
    obj_data.set_path(obj_path);
    obj_data.set_texture_path(texture_path);
    obj_data.set_mtl_path(mtl_path);
    return true;
}