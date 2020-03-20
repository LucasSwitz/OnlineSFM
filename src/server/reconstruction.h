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

class Reconstruction;

class ReconstructionFetcher {
    public:
        Reconstruction* Fetch(const std::string& id);
        void Store(const ReconstructionData& reconstruction);
};

/*These represent the remote objects and should implement delete and update functions etc.*/
/*Upon construction, each will fetch its remote data.
TODO: Create move constructors
*/
class Image {
    public:
        Image(const std::string& id, ImageStorageAdapter* storage_adapter);
        ImageData Data();
        void Delete();
    private:
        std::string _id;
        ImageStorageAdapter* _storage_adapter;
};

class SparseReconstruction {
    public:
        SparseReconstruction(const std::string& id, SparseStorageAdapter* storage_adapter);
        SparsePointCloudData Data(); 
        void Delete();
    private:
        std::string _id;
        SparseStorageAdapter* _storage_adapter;
};

class OBJ{
    public:
        OBJ(const std::string& id, OBJStorageAdapter* storage_adapter);
        OBJData Data();
        void Delete();
    private:
        std::string _id;
        OBJStorageAdapter* _storage_adapter;
};

class Reconstruction {
    public:
        Reconstruction(const std::string& id,
                       ReconstructionStorageAdapter* reconstruction_staroage,
                       ImageStorageAdapter* image_storage,
                       SparseStorageAdapter* sparse_storage,
                       OBJStorageAdapter* obj_storage,
                       CameraIntrinsicsStorage* intrinsics_storages);
        ~Reconstruction();
        void AddImage(const std::string& image_id);
        std::string StoreImage(ImageData& image);
        int  Reconstruct();
        void SetupMVS();
        void MVS();
        bool Reconstruct(const std::set<std::string>& new_images);
        bool HasReconstructedOnce();
        std::vector<Image> GetImages();
        Image GetImage(const std::string& id);
        SparseReconstruction GetSparse();
        OBJ GetOBJ();
        void Delete();
        bool IsRunningMVS();
        const ReconstructionData& Data();
    private:
        std::string _id;
        bool _running_mvs = false;
        std::thread* _mvs_thread = nullptr;
        std::string _working_mvs_dir;
        ReconstructionStorageAdapter* _reconstruction_storage = nullptr;
        ImageStorageAdapter* _image_storage = nullptr;
        OBJStorageAdapter* _obj_storage = nullptr;
        SparseStorageAdapter* _sparse_storage = nullptr;
        CameraIntrinsicsStorage* _intrinsics_storage = nullptr;
        OpenMVGReconstructionAgent reconstruction_agent;
        ReconstructionData _data;
        void _MVS();
        void _ExportWorkingMVS();
};