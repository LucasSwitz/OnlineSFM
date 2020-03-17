#pragma once 
#include <string>
#include <thread>
#include "reconstruction_storage.h"
#include "image_storage.h"
#include "obj_storage.h"
#include "sparse_storage.h"
#include "reconstruction_strategy.h"

class Reconstruction;

class ReconstructionFetcher {
    public:
        Reconstruction* Fetch(const std::string& id);
        void Create(const std::string& id, const std::string& path);
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
                       ReconstructionStrategy* reconstruction_strategy);
        ~Reconstruction();
        std::string AddImage(ImageData& image);
        int  Reconstruct();
        bool IncrementalSFM();
        void SetupRelocalization();
        bool Relocalize();
        void AddImageToRelocalization(const std::string& image_id);
        void SetupMVS();
        void MVS();
        bool HasReconstructedOnce();
        std::vector<Image> GetImages();
        Image GetImage(const std::string& id);
        SparseReconstruction GetSparse();
        OBJ GetOBJ();
        void Delete();
    private:
        std::string _id;
        bool _running_mvs = false;
        std::thread* _mvs_thread = nullptr;
        std::string _working_mvs_dir;
        ReconstructionStorageAdapter* _reconstruction_storage = nullptr;
        ImageStorageAdapter* _image_storage = nullptr;
        OBJStorageAdapter* _obj_storage = nullptr;
        SparseStorageAdapter* _sparse_storage = nullptr;
        ReconstructionStrategy* _reconstruction_strategy = nullptr;
        void _MVS();
        void _ExportWorkingMVS();
        void ExportLocalization();
        void CleanupLocalization();
};