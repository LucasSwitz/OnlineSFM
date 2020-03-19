#include "reconstruction.h"
#include "sql_image_storage.h"
#include "sql_recontruction_storage.h"
#include "sql_sparse_storage.h"
#include "openmvs_strategy.h"
#include "util.h"
#include "sql_obj_storage.h"
#include "config.h"
#include "openmvg_reconstruction_agent.h"
#include <thread>

Reconstruction* ReconstructionFetcher::Fetch(const std::string& id){
    return new Reconstruction(id, new SQLReconstructionStorage(CONFIG_GET_STRING("sql.address"), 
                                                               CONFIG_GET_STRING("sql.user"), 
                                                               CONFIG_GET_STRING("sql.password"), 
                                                               CONFIG_GET_STRING("sql.db"), 
                                                               CONFIG_GET_STRING("sql.reconstruction_table")), 
                                  new SQLImageStorage(CONFIG_GET_STRING("sql.address"), 
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
                                                    CONFIG_GET_STRING("sql.obj_table")));
}

void ReconstructionFetcher::Create(const std::string& id, const std::string& path){
    try{
        SQLReconstructionStorage(CONFIG_GET_STRING("sql.address"), 
                                CONFIG_GET_STRING("sql.user"), 
                                CONFIG_GET_STRING("sql.password"), 
                                CONFIG_GET_STRING("sql.db"), 
                                CONFIG_GET_STRING("sql.reconstruction_table")).CreateIfNew(id, path);
    }catch(std::exception& e){
        LOG(ERROR) << e.what();
    }
}

Reconstruction::Reconstruction(const std::string& id,
                       ReconstructionStorageAdapter* reconstruction_storage,
                       ImageStorageAdapter* image_storage,
                       SparseStorageAdapter* sparse_storage,
                       OBJStorageAdapter* obj_storage) : _id(id), 
                                                         _reconstruction_storage(reconstruction_storage),
                                                         _image_storage(image_storage), 
                                                         _obj_storage(obj_storage), 
                                                         _sparse_storage(sparse_storage){
    std::string reconstruction_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id;
    std::string reconstruction_reconstruction_dir = reconstruction_dir + "/SFM";
    std::string matches_dir = reconstruction_dir + "/SFM";
    std::string sfm_data_path = matches_dir + "/sfm_data.json";
    std::string sfm_data_bin_path = reconstruction_reconstruction_dir +"/sfm_data.bin";
    std::string robust_bin_path = reconstruction_reconstruction_dir + "/robust.bin";
    std::string robust_ply_path = reconstruction_reconstruction_dir + "/robust.ply";
    std::string mvs_path = reconstruction_reconstruction_dir + "/scene.mvs";
    std::string undistored_images_path = reconstruction_reconstruction_dir + "/undistorted";
    OpenMVGReconstructionAgentConfig config;
    config.sMatchesDirectory = matches_dir;
    config.sGeometricModel = "e";
    config.sMatchesDir = matches_dir;
    config.sOutDir = matches_dir;
    config.sMatchFile = matches_dir+"/matches.e.bin";
    config.sOutFile = robust_bin_path;
    config.sfileDatabase = std::getenv("OPENMVG_SENSOR_DB");
    config.sfileDatabase += "/sensor_width_camera_database.txt";
    config.sOutputDir = matches_dir;
    config.root_path = reconstruction_dir + "/images";
    this->reconstruction_agent.SetConfig(config);
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

Image::Image(const std::string& id, ImageStorageAdapter* img_storage) : _id(id), _storage_adapter(img_storage){
    
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

void Reconstruction::SetupMVS(){
    if(this->_running_mvs) return;
    SparsePointCloudMetaData spc_data = this->_sparse_storage->GetMetaByReconstruction(this->_id);
    std::string reconstruction_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id;
    std::string mvs_dir = reconstruction_dir + "/working_MVS";
    CleanAndMakeDir(mvs_dir);
    boost::filesystem::copy_file(spc_data.mvs_path(), mvs_dir + "/scene.mvs");
    boost::filesystem::copy_directory(reconstruction_dir + "/SFM/undistorted", mvs_dir + "/undistorted");
    this->_working_mvs_dir = mvs_dir;
}

void Reconstruction::MVS(){
    if(!this->_running_mvs){
        this->_running_mvs = true;
        this->_mvs_thread = new std::thread(&Reconstruction::_MVS, this);    
    }
}

void Reconstruction::_MVS(){
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
    }else{
        LOG(ERROR) << "MVS Failed for " << this->_id;
    }
    this->_running_mvs = false;
}

#include "openMVG/types.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"

bool Reconstruction::Reconstruct(const std::set<std::string>& new_images){
    LOG(INFO) << "Doing Reconstruct for " << this->_id;
    std::string reconstruction_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id;
    std::string reconstruction_reconstruction_dir = reconstruction_dir + "/SFM";
    std::string matches_dir = reconstruction_dir + "/SFM";
    std::string sfm_data_path = matches_dir + "/sfm_data.json";
    std::string sfm_data_bin_path = reconstruction_reconstruction_dir +"/sfm_data.bin";
    std::string robust_bin_path = reconstruction_reconstruction_dir + "/robust.bin";
    std::string robust_ply_path = reconstruction_reconstruction_dir + "/robust.ply";
    std::string mvs_path = reconstruction_reconstruction_dir + "/scene.mvs";
    std::string undistored_images_path = reconstruction_reconstruction_dir + "/undistorted";

    std::set<std::string> new_image_paths;

    for(std::string image_id : new_images){
        ImageMetaData meta = this->_image_storage->GetMeta(image_id);
        new_image_paths.insert(meta.path());
    }
    LOG(INFO) << "Gathering Matches for " << this->_id;
    if(!reconstruction_agent.GenerateMatches(new_image_paths)){
        return false;
    }
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
    ImageMetaData img_meta = this->_image_storage->GetMeta(image_id);
    std::string reconstruction_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id;
    LOG(INFO) << "Adding image: " << image_id;
    if(!reconstruction_agent.AddImage(img_meta.path())){
        LOG(ERROR) << "Failed to add image " << image_id;
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