#pragma once 
#include <string>
#include <thread>
#include <memory>
#include "reconstruction_storage.h"
#include "image_storage.h"
#include "obj_storage.h"
#include "sparse_storage.h"
#include "reconstruction_agent.h"
#include "camera_intrinsics_storage.h"
#include "configuration_adapter.h"
#include "remote_storage_adapter.h"
#include "config.h"

class Reconstruction;

struct ReconstructionConnectionContext {
    ReconstructionConnectionContext(
        std::shared_ptr<CameraIntrinsicsStorage> intrinsics_storage_,
        std::shared_ptr<ConfigurationAdapter> config_adapter_,
        std::shared_ptr<ImageDataStorage> image_data_storage_,
        std::shared_ptr<SPCDataStorage> spc_data_storage_,
        std::shared_ptr<ImageStorageAdapter> image_storage_,
        std::shared_ptr<SparseStorageAdapter> sparse_storage_,
        std::shared_ptr<ReconstructionStorageAdapter> reconstruction_storage_,
        std::shared_ptr<OBJStorageAdapter> obj_storage_) : intrinsics_storage(intrinsics_storage_),
                                                          config_adapter(config_adapter_),
                                                          image_data_storage(image_data_storage_),
                                                          spc_data_storage(spc_data_storage_),
                                                          image_storage(image_storage_),
                                                          sparse_storage(sparse_storage_),
                                                          reconstruction_storage(reconstruction_storage_),
                                                          obj_storage(obj_storage_){}
    std::shared_ptr<CameraIntrinsicsStorage> intrinsics_storage;
    std::shared_ptr<ConfigurationAdapter> config_adapter;
    std::shared_ptr<ImageDataStorage> image_data_storage;
    std::shared_ptr<SPCDataStorage> spc_data_storage;
    std::shared_ptr<ImageStorageAdapter> image_storage;
    std::shared_ptr<SparseStorageAdapter> sparse_storage;
    std::shared_ptr<ReconstructionStorageAdapter> reconstruction_storage;
    std::shared_ptr<OBJStorageAdapter> obj_storage;
};

struct ReconstructionContext {
    ReconstructionContext(std::string id_, 
                          std::shared_ptr<ReconstructionConnectionContext> connection_context_,
                          std::shared_ptr<ReconstructionAgent> reconstruction_agent_) : id(id_),
                                                                                       connection_context(connection_context_),
                                                                                       reconstruction_agent(reconstruction_agent_){}
    std::string id;
    std::shared_ptr<ReconstructionConnectionContext> connection_context;
    std::shared_ptr<ReconstructionAgent> reconstruction_agent;
};

class ReconstructionFetcher {
    public:
        std::shared_ptr<Reconstruction> Fetch(std::shared_ptr<ReconstructionContext> ctx);
};

/*These represent the remote objects and should implement delete and update functions etc.*/
/*Upon construction, each will fetch its remote data.
TODO: Create move constructors
*/
class Image {
    public:
        Image() = default;
        Image(const std::string& id, std::shared_ptr<ImageStorageAdapter> storage_adapter);
        ImageData Data();
        void Delete();
        std::string ID();
    private:
        std::string _id;
        std::shared_ptr<ImageStorageAdapter> _storage_adapter;
};

class SparseReconstruction {
    public:
        SparseReconstruction() = default;
        SparseReconstruction(const std::string& id, std::shared_ptr<SparseStorageAdapter> storage_adapter);
        SparsePointCloudData Data(); 
        void Delete();
    private:
        std::string _id;
        std::shared_ptr<SparseStorageAdapter> _storage_adapter;
};

class OBJ{
    public:
        OBJ() = default;
        OBJ(const std::string& id, std::shared_ptr<OBJStorageAdapter> storage_adapter);
        OBJData Data();
        void Delete();
    private:
        std::string _id;
        std::shared_ptr<OBJStorageAdapter> _storage_adapter;
};


class DefaultReconstructionConfig : public ConfigurationContainer{
    std::string get_string(const std::string& key);
    int get_int(const std::string& key);
    bool get_bool(const std::string& key);
    double get_double(const std::string& key);
    void patch(const std::string& json);
    std::string jsonify();
    ConfigurationContainerPtr get_container(const std::string& key);
};

class Reconstruction {
    public:
        Reconstruction(const std::string& id,
                       std::shared_ptr<ReconstructionStorageAdapter> reconstruction_staroage,
                       std::shared_ptr<ImageStorageAdapter> image_storage,
                       std::shared_ptr<SparseStorageAdapter> sparse_storage,
                       std::shared_ptr<OBJStorageAdapter> obj_storage,
                       std::shared_ptr<CameraIntrinsicsStorage> intrinsics_storages,
                       std::shared_ptr<ConfigurationAdapter> config_adapter,
                       std::shared_ptr<ReconstructionAgent> reconstruction_agent);
        ~Reconstruction();
        std::string StoreImage(ImageData& image);
        bool ComputeFeatures(const std::set<std::string>& images);
        bool ComputeMatches(const std::set<std::string>& images);
        bool AddImage(const std::string& image_id, bool index=false);
        bool SparseReconstruct();
        void StoreData(const ReconstructionData& data);
        bool MVS();
        std::vector<Image> GetImages();
        bool ComputeStructure();
        Image GetImage(const std::string& id);
        SparseReconstruction GetSparse();
        OBJ GetOBJ();
        bool IsRunningMVS();
        const ReconstructionData& Data();
        void Delete();
        void SetAgentConfigFields(const std::string& agent_name, const std::string& config_json);
        void SetConfigFields( const std::string& config_json);
        ConfigurationContainerPtr GetConfig();
        ConfigurationContainerPtr GetAgentConfig(const std::string& agent_name);
    private:
        std::string _id;
        bool _running_mvs = false;
        std::thread* _mvs_thread = nullptr;
        std::string _working_mvs_dir;
        std::shared_ptr<ReconstructionStorageAdapter> _reconstruction_storage = nullptr;
        std::shared_ptr<ImageStorageAdapter> _image_storage = nullptr;
        std::shared_ptr<OBJStorageAdapter> _obj_storage = nullptr;
        std::shared_ptr<SparseStorageAdapter> _sparse_storage = nullptr;
        std::shared_ptr<CameraIntrinsicsStorage> _intrinsics_storage = nullptr;
        std::shared_ptr<ConfigurationAdapter> _config_adapter = nullptr;
        std::shared_ptr<ReconstructionAgent> reconstruction_agent;
        ReconstructionData _data;
        void _SetupMVS();
        bool _MVS();
        void _ExportWorkingMVS();
};