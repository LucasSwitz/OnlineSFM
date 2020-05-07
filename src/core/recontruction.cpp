#include "reconstruction.h"
//#include "openmvs_strategy.h"
#include "util.h"
#include "image_filesystem_storer.h"
#include <thread>
#include "index_helpers.h"


 std::shared_ptr<Reconstruction> ReconstructionFetcher::Fetch(std::shared_ptr<ReconstructionContext> ctx){
    return std::make_shared<Reconstruction>(ctx->id,  
            ctx->connection_context->reconstruction_storage, 
            ctx->connection_context->image_storage, 
            ctx->connection_context->sparse_storage, 
            ctx->connection_context->obj_storage,
            ctx->connection_context->intrinsics_storage,
            ctx->connection_context->config_adapter,
            ctx->reconstruction_agent);
}

Reconstruction::Reconstruction(const std::string& id,
                       std::shared_ptr<ReconstructionStorageAdapter> reconstruction_storage,
                       std::shared_ptr<ImageStorageAdapter> image_storage,
                       std::shared_ptr<SparseStorageAdapter> sparse_storage,
                       std::shared_ptr<OBJStorageAdapter> obj_storage,
                       std::shared_ptr<CameraIntrinsicsStorage> intrinsics_storage,
                       std::shared_ptr<ConfigurationAdapter> config_adapter,
                       std::shared_ptr<ReconstructionAgent> reconstruction_agent) : 
    _id(id), 
    _reconstruction_storage(reconstruction_storage),
    _image_storage(image_storage), 
    _obj_storage(obj_storage), 
    _sparse_storage(sparse_storage),
    _intrinsics_storage(intrinsics_storage),
    _config_adapter(config_adapter),
    reconstruction_agent(reconstruction_agent){
        this->_data = this->_reconstruction_storage->Get(this->_id);
}

void Reconstruction::StoreData(const ReconstructionData& data){
    this->_reconstruction_storage->Store(data);
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

std::string Image::ID(){
    return this->_id;
}

OBJ::OBJ(const std::string& id, std::shared_ptr<OBJStorageAdapter> obj_storage) : _id(id), _storage_adapter(obj_storage){

}


OBJData OBJ::Data(){
    // Maybe cache this for a certain amount of time?
    return this->_storage_adapter->Get(this->_id);
}

void OBJ::Delete(){
    this->_storage_adapter->Delete(this->_id);
}

SparseReconstruction::SparseReconstruction(const std::string& id,  std::shared_ptr<SparseStorageAdapter> storage_adapter) : 
                                                                                     _id(id), 
                                                                                     _storage_adapter(storage_adapter){

}

SparsePointCloudData SparseReconstruction::Data(){
    if(!this->_id.empty())
        return this->_storage_adapter->Get(this->_id);
    else
        return SparsePointCloudData();
}

void Reconstruction::_SetupMVS(){
    /*SparsePointCloudMetaData spc_data = this->_sparse_storage->GetMetaByReconstruction(this->_id);
    std::string reconstruction_dir = CONFIG_GET_STRING("storage.root") + "/" + this->_id;
    std::string mvs_dir = reconstruction_dir + "/working_MVS";
    CleanAndMakeDir(mvs_dir);
    boost::filesystem::copy_file(spc_data.mvs_path(), mvs_dir + "/scene.mvs");
    boost::filesystem::copy_directory(reconstruction_dir + "/SFM/undistorted", mvs_dir + "/undistorted");
    this->_working_mvs_dir = mvs_dir;*/
}

bool Reconstruction::MVS(){
    LOG(INFO) << "Starting MVS for " << this->_id;
    return reconstruction_agent->MVS();
}

bool Reconstruction::SparseReconstruct(){
    LOG(INFO) << "Starting Incremental SFM for " << this->_id;
    return reconstruction_agent->IncrementalSFM();
}

bool Reconstruction::ComputeStructure(){
    LOG(INFO) << "Starting compute structure for " << this->_id;
    std::vector<char> mvs_raw;
    return reconstruction_agent->ComputeStructure();
}

bool Reconstruction::AddImage(const std::string& image_id, bool index){
    LOG(INFO) << "Adding image: " << image_id;
    if(index){
        auto indexing_client = GetIndexingClient(CONFIG_GET_STRING("index.address"));
        //TODO: Wrap this shit
        IndexImageRequest req;
        req.set_image_id(image_id);
        req.set_reconstruction_id(this->_id);
        IndexImageResponse resp;
        grpc::ClientContext context;
        LOG(INFO) << "Indexing image: " << image_id;
        auto status = indexing_client->IndexImage(&context, req, &resp);
        if(!status.ok()){
            LOG(ERROR) << "Failed to index image: " << image_id;
            return false;
        }
    }
    return reconstruction_agent->AddImage(image_id);
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

bool Reconstruction::ComputeFeatures(const std::set<std::string>& images){
    return this->reconstruction_agent->ComputeFeatures(images);
}

bool Reconstruction::ComputeMatches(const std::set<std::string>& images){
    return this->reconstruction_agent->ComputeMatches(images);
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

ConfigurationContainerPtr DefaultReconstructionConfig::get_container(const std::string& key){
    return nullptr;
}

std::string DefaultReconstructionConfig::jsonify(){
    return "{\"reconstruction_agent\":\"openmvg\"}";
}
