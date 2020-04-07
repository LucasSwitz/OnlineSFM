#include "reconstruction.h"
#include "sql_image_storage.h"
#include "sql_recontruction_storage.h"
#include "sql_sparse_storage.h"
#include "sql_camera_intrinsics_storage.h"
#include "openmvs_strategy.h"
#include "util.h"
#include "sql_obj_storage.h"
#include "config.h"
#include "openmvg_reconstruction_agent.h"
#include "redis_sfm_backlog.h"
#include "mongodb_configuration_adapter.h"
#include "image_filesystem_storer.h"
#include <thread>
#include "index_helpers.h"

std::shared_ptr<Reconstruction> ReconstructionFetcher::Fetch(const std::string& id){
    try{
        return std::make_shared<Reconstruction>(id, new SQLReconstructionStorage(CONFIG_GET_STRING("sql.address"), 
                                                                CONFIG_GET_STRING("sql.user"), 
                                                                CONFIG_GET_STRING("sql.password"), 
                                                                CONFIG_GET_STRING("sql.db"), 
                                                                CONFIG_GET_STRING("sql.reconstruction_table")), 
                                    std::make_shared<SQLImageStorage>(new FileSystemImageDataStorage(),
                                                        CONFIG_GET_STRING("sql.address"), 
                                                        CONFIG_GET_STRING("sql.user"), 
                                                        CONFIG_GET_STRING("sql.password"), 
                                                        CONFIG_GET_STRING("sql.db"), 
                                                        CONFIG_GET_STRING("sql.views_table")), 
                                    new SQLSparseStorage(CONFIG_GET_STRING("sql.address"), 
                                                        CONFIG_GET_STRING("sql.user"), 
                                                        CONFIG_GET_STRING("sql.password"), 
                                                        CONFIG_GET_STRING("sql.db"), 
                                                        CONFIG_GET_STRING("sql.sparse_table")), 
                                    new SQLOBJStorage(CONFIG_GET_STRING("sql.address"), 
                                                        CONFIG_GET_STRING("sql.user"), 
                                                        CONFIG_GET_STRING("sql.password"),
                                                        CONFIG_GET_STRING("sql.db"), 
                                                        CONFIG_GET_STRING("sql.obj_table")),
                                    new SQLCameraIntrinsicsStorage(CONFIG_GET_STRING("sql.address"), 
                                                        CONFIG_GET_STRING("sql.user"), 
                                                        CONFIG_GET_STRING("sql.password"),
                                                        CONFIG_GET_STRING("sql.db"), 
                                                        CONFIG_GET_STRING("sql.intrinsics_table")),
                                    new RedisSFMBacklog(CONFIG_GET_STRING("redis.address"),
                                                        CONFIG_GET_STRING("redis.user"),
                                                        CONFIG_GET_STRING("redis.password")),
                                    std::make_shared<MongoDBConfigurationAdapter>(CONFIG_GET_STRING("mongodb.uri"),
                                                    CONFIG_GET_STRING("mongodb.db"),
                                                    CONFIG_GET_STRING("mongodb.agents_collection"),
                                                    CONFIG_GET_STRING("mongodb.default_agents_collection"),
                                                    CONFIG_GET_STRING("mongodb.reconstructions_collections")));
    }catch(const std::exception& e){
        throw;
    }
}

void ReconstructionFetcher::Store(const ReconstructionData& reconstruction){
    SQLReconstructionStorage(CONFIG_GET_STRING("sql.address"), 
                            CONFIG_GET_STRING("sql.user"), 
                            CONFIG_GET_STRING("sql.password"), 
                            CONFIG_GET_STRING("sql.db"), 
                            CONFIG_GET_STRING("sql.reconstruction_table")).Store(reconstruction);
}

#include "sql_openmvg_storage.h"
#include "sql_descriptor_storage.h"
Reconstruction::Reconstruction(const std::string& id,
                       ReconstructionStorageAdapter* reconstruction_storage,
                       std::shared_ptr<ImageStorageAdapter> image_storage,
                       SparseStorageAdapter* sparse_storage,
                       OBJStorageAdapter* obj_storage,
                       CameraIntrinsicsStorage* intrinsics_storage,
                       SFMBacklogCounter* backlog_monitor,
                       std::shared_ptr<ConfigurationAdapter> config_adapter) : 
    _id(id), 
    _reconstruction_storage(reconstruction_storage),
    _image_storage(image_storage), 
    _obj_storage(obj_storage), 
    _sparse_storage(sparse_storage),
    _intrinsics_storage(intrinsics_storage),
    _session_backlog(backlog_monitor),
    _config_adapter(config_adapter),
    reconstruction_agent(id,
                        _intrinsics_storage, 
                        new SQLOpenMVGStorage(CONFIG_GET_STRING("sql.address"),
                                            CONFIG_GET_STRING("sql.user"), 
                                            CONFIG_GET_STRING("sql.password"),
                                            CONFIG_GET_STRING("sql.db"), 
                                            CONFIG_GET_STRING("sql.openmvg_views_table"),
                                            CONFIG_GET_STRING("sql.openmvg_intrinsics_table"),
                                            CONFIG_GET_STRING("sql.openmvg_matches_table"),
                                            CONFIG_GET_STRING("sql.openmvg_meta_table"),
                                            CONFIG_GET_STRING("sql.openmvg_poses_table")),
                                            config_adapter,
                        std::make_shared<SQLDescriptorStorage>(CONFIG_GET_STRING("sql.address"),
                                            CONFIG_GET_STRING("sql.user"), 
                                            CONFIG_GET_STRING("sql.password"),
                                            CONFIG_GET_STRING("sql.db"),
                                            CONFIG_GET_STRING("sql.descriptors_table")),
                                            image_storage){
    this->_data = this->_reconstruction_storage->Get(this->_id);
    OpenMVGReconstructionAgentConfig config;
    config.features_dir = this->_data.features_path();
    config.sfm_dir = this->_data.features_path();
    config.root_path = this->_data.images_path();
    config.matches_dir = this->_data.matches_path();
    config.sOutFile =  this->_data.sfm_path() + "/robust.bin";
    this->reconstruction_agent.SetConfig(config);
}

const ReconstructionData& Reconstruction::Data(){
    return this->_data;
}

std::string Reconstruction::StoreImage(ImageData& image){
    std::string image_id = GetUUID();
    image.mutable_metadata()->set_id(image_id);
    this->_image_storage->Store(image);
    return image_id;
}

std::vector<Image> Reconstruction::GetImages(){
    std::vector<ImageMetaData> img_metas = this->_image_storage->GetAll(this->_id);
    std::vector<Image> out;
    for(ImageMetaData meta : img_metas){
        out.push_back(Image(meta.id(), this->_image_storage));
    }
    return out;
}

Image Reconstruction::GetImage(const std::string& id){
    return Image(id, this->_image_storage);
}

SparseReconstruction Reconstruction::GetSparse(){
    SparsePointCloudMetaData sparse = this->_sparse_storage->GetMetaByReconstruction(this->_id);
    return SparseReconstruction(sparse.id(), this->_sparse_storage);
}

OBJ Reconstruction::GetOBJ(){
    std::vector<OBJMetaData> objs = this->_obj_storage->GetAll(this->_id);
    return OBJ(objs[0].id(), this->_obj_storage);
}

void Reconstruction::Delete(){
    this->_image_storage->DeleteByReconstruction(this->_id);
    this->_sparse_storage->DeleteByReconstruction(this->_id);
    this->_reconstruction_storage->Delete(this->_id);
}

Image::Image(const std::string& id, std::shared_ptr<ImageStorageAdapter> img_storage) : _id(id), 
                                                                                        _storage_adapter(img_storage){
    
}

ImageData Image::Data(){
    // Maybe cache this for a certain amount of time?
    return this->_storage_adapter->Get(this->_id);
}

void Image::Delete(){
    this->_storage_adapter->Delete(this->_id);
}

OBJ::OBJ(const std::string& id, OBJStorageAdapter* obj_storage) : _id(id), _storage_adapter(obj_storage){

}


OBJData OBJ::Data(){
    // Maybe cache this for a certain amount of time?
    return this->_storage_adapter->Get(this->_id);
}

void OBJ::Delete(){
    this->_storage_adapter->Delete(this->_id);
}

SparseReconstruction::SparseReconstruction(const std::string& id, SparseStorageAdapter* storage_adapter) : 
                                                                                     _id(id), 
                                                                                     _storage_adapter(storage_adapter){

}

SparsePointCloudData SparseReconstruction::Data(){
    if(!this->_id.empty())
        return this->_storage_adapter->Get(this->_id);
    else
        return SparsePointCloudData();
}

#include "openmvg.h"
#include "openmvs_strategy.h"

bool Reconstruction::HasReconstructedOnce(){
    return !this->_sparse_storage->GetMetaByReconstruction(this->_id).reconstruction().compare(this->_id);
}

void Reconstruction::_SetupMVS(){
    SparsePointCloudMetaData spc_data = this->_sparse_storage->GetMetaByReconstruction(this->_id);
    std::string reconstruction_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id;
    std::string mvs_dir = reconstruction_dir + "/working_MVS";
    CleanAndMakeDir(mvs_dir);
    boost::filesystem::copy_file(spc_data.mvs_path(), mvs_dir + "/scene.mvs");
    boost::filesystem::copy_directory(reconstruction_dir + "/SFM/undistorted", mvs_dir + "/undistorted");
    this->_working_mvs_dir = mvs_dir;
}

bool Reconstruction::MVS(bool block){
    if(!this->_running_mvs){
        this->_SetupMVS();
        this->_running_mvs = true;
        if(!block){
            this->_mvs_thread = new std::thread(&Reconstruction::_MVS, this);    
            return true;
        }else{
            return this->_MVS();
        }
    }
}

bool Reconstruction::_MVS(){
    OpenMVSStrategy mvs;
    std::vector<OBJMetaData> old =  this->_obj_storage->GetAll(this->_id);
    OBJMetaData obj_meta;
    if(old.empty()){
        obj_meta.set_reconstruction(this->_id);
        obj_meta.set_id(GetUUID());
    }else{
        obj_meta = old[0];
    }
    if(mvs.DoMVS(this->_id, this->_working_mvs_dir, obj_meta)){
        this->_obj_storage->Store(obj_meta);
        LOG(INFO) << "MVS Finished successfully for " << this->_id;
        _ExportWorkingMVS();
        return true;
    }else{
        LOG(ERROR) << "MVS Failed for " << this->_id;
        return false;
    }
    this->_running_mvs = false;
}

#include "openMVG/types.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"

bool Reconstruction::SparseReconstruct(){
    LOG(INFO) << "Doing Reconstruct for " << this->_id;
    std::string reconstruction_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id;
    std::string reconstruction_reconstruction_dir = reconstruction_dir + "/SFM";
    std::string robust_bin_path = reconstruction_reconstruction_dir + "/robust.bin";
    std::string robust_ply_path = reconstruction_reconstruction_dir + "/robust.ply";
    std::string mvs_path = reconstruction_reconstruction_dir + "/scene.mvs";
    std::string undistored_images_path = reconstruction_reconstruction_dir + "/undistorted";

    LOG(INFO) << "Starting Incremental SFM for " << this->_id;
    if(!reconstruction_agent.IncrementalSFM()){
         return false;
    }
    LOG(INFO) << "Starting compute structure for " << this->_id;
    if(!reconstruction_agent.ComputeStructure()){
        return false;
    }
    LOG(INFO) << "Creating MVS File for " << this->_id;
    if(!ConvertToMVS(robust_bin_path, undistored_images_path, mvs_path)){
        return false;
    }

    SparsePointCloudMetaData spc_data;
    OBJMetaData obj_data;
    spc_data.set_id(GetUUID());
    spc_data.set_reconstruction(this->_id);
    spc_data.set_mvs_path(mvs_path);
    spc_data.set_ply_path(robust_ply_path);
    this->_sparse_storage->Store(spc_data);
    return true;
}

void Reconstruction::AddImage(const std::string& image_id){
    std::string reconstruction_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id;
    LOG(INFO) << "Adding image: " << image_id;

    auto indexing_client = GetIndexingClient(CONFIG_GET_STRING("index.address"));
    
    //TODO: Wrap this shit
    IndexImageRequest req;
    req.set_image_id(image_id);
    IndexImageResponse resp;
    grpc::ClientContext context;
    LOG(INFO) << "Indexing image: " << image_id;
    indexing_client->IndexImage(&context, req, &resp);

    if(!reconstruction_agent.AddImage(image_id)){
        LOG(ERROR) << "Failed to add image " << image_id;
    }else{
        //this->_session_backlog->Incr(this->_id);
    }
}

void Reconstruction::_ExportWorkingMVS(){
    std::string mvs_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id + "/MVS";
    if(boost::filesystem::exists(this->_working_mvs_dir)){
        DeleteDirIfExists(mvs_dir);
        boost::filesystem::copy_directory(this->_working_mvs_dir, mvs_dir);
    }
}

Reconstruction::~Reconstruction(){
    if(this->_mvs_thread){
        this->_mvs_thread->join();
        delete this->_mvs_thread;
    }
}

bool Reconstruction::IsRunningMVS(){
    return this->_running_mvs;
}

void Reconstruction::ComputeFeatures(const std::set<std::string>& images){
    this->reconstruction_agent.ComputeFeatures(images);
}

void Reconstruction::ComputeMatches(const std::set<std::string>& images){
    this->reconstruction_agent.ComputeMatches(images);
}

void Reconstruction::SetAgentConfigFields(const std::string& agent_name, const std::string& config_json){
   auto config = this->_config_adapter->GetAgentConfigOrDefault(this->_id, agent_name, nullptr);
   config->patch(config_json);
   std::string j = config->jsonify();
   this->_config_adapter->SetAgentConfig(this->_id, agent_name, std::move(config));
}

void Reconstruction::SetConfigFields(const std::string& config_json){
    auto config = this->_config_adapter->GetReconstructionConfigOrDefault(this->_id, std::make_unique<DefaultReconstructionConfig>());
    config->patch(config_json);
    this->_config_adapter->SetReconstructionConfig(this->_id, std::move(config));
}

ConfigurationContainerPtr Reconstruction::GetConfig(){
    return this->_config_adapter->GetReconstructionConfigOrDefault(this->_id, std::make_unique<DefaultReconstructionConfig>());
}

ConfigurationContainerPtr Reconstruction::GetAgentConfig(const std::string& agent_name){
    return this->_config_adapter->GetAgentConfigOrDefault(this->_id, agent_name, nullptr);
}

std::string DefaultReconstructionConfig::get_string(const std::string& key){
    if(key.compare("reconstruction_agent") == 0){
        return "openmvg";
    }
    return "";
}

int DefaultReconstructionConfig::get_int(const std::string& key){
    return 0;
}

bool DefaultReconstructionConfig::get_bool(const std::string& key){
    return false;
}

double DefaultReconstructionConfig::get_double(const std::string& key){
    return 0.0;
}

void DefaultReconstructionConfig::patch(const std::string& json){

}

std::string DefaultReconstructionConfig::jsonify(){
    return "{\"reconstruction_agent\":\"openmvg\"}";
}
