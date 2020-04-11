#pragma once 
#include <string>
#include <thread>
#include "reconstruction_storage.h"
#include "image_storage.h"
#include "obj_storage.h"
#include "sparse_storage.h"
#include "openMVG/types.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openmvg_reconstruction_agent.h"
#include "camera_intrinsics_storage.h"
#include "sfm_backlog_counter.h"
#include <memory>

class Reconstruction;

class ReconstructionFetcher {
    public:
        std::shared_ptr<Reconstruction> Fetch(const std::string& id);
        void Store(const ReconstructionData& reconstruction);
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
                       SFMBacklogCounter* session_backlog,
                       std::shared_ptr<ConfigurationAdapter> config_adapter,
                       std::shared_ptr<ReconstructionAgent> reconstruction_agent);
        ~Reconstruction();
        void ComputeFeatures(const std::set<std::string>& images);
        void ComputeMatches(const std::set<std::string>& images);
        void AddImage(const std::string& image_id);
        std::string StoreImage(ImageData& image);
        bool SparseReconstruct();
        bool MVS(bool block = false);
        bool Reconstruct(const std::set<std::string>& new_images);
        bool HasReconstructedOnce();
        std::vector<Image> GetImages();
        Image GetImage(const std::string& id);
        SparseReconstruction GetSparse();
        OBJ GetOBJ();
        void Delete();
        bool IsRunningMVS();
        const ReconstructionData& Data();
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
        SFMBacklogCounter* _session_backlog = nullptr;
        void _SetupMVS();
        bool _MVS();
        void _ExportWorkingMVS();
};