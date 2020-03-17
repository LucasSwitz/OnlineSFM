#include "openmvg_strategy.h"
#include <glog/logging.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/types.h> 
#include <boost/filesystem.hpp>
#include <sys/stat.h> 
#include "config.h"
#include "openmvg.h"


bool DoMVGSFM(const std::string& reconstruction_id,
              const std::string& images_directory, 
              const std::string& output_directory,
              ReconstructionType reconstruction_type,
              SparsePointCloudMetaData& spc_data);

OpenMVGStrategy::OpenMVGStrategy(ReconstructionType reconstruction_type) : _reconstruction_type(reconstruction_type){}

// OpenMVG assumes all of the images are in the same directory. We will just extract the directory from the first image path
int OpenMVGStrategy::DoSFM(const std::vector<ImageMetaData>& images, SparsePointCloudMetaData& spc_data){
    std::string image_directory = images[0].path();
    std::string reconstruction_id = images[0].reconstruction();
    const size_t last_slash_idx = image_directory.rfind('/');

    if (std::string::npos != last_slash_idx)
    {
        image_directory = image_directory.substr(0, last_slash_idx);
    }
    std::string output_directory = image_directory + "/../SFM";
    if(boost::filesystem::exists(output_directory)){
        boost::filesystem::remove_all(output_directory);
    }
    mkdir(output_directory.c_str(), 0777);
    return DoMVGSFM(reconstruction_id, 
                    image_directory, 
                    output_directory, 
                    this->_reconstruction_type, 
                    spc_data);
}

bool DoMVGSFM(const std::string& reconstruction_id,
              const std::string& images_directory, 
              const std::string& output_directory,
              ReconstructionType reconstruction_type,
              SparsePointCloudMetaData& spc_data){
    std::string matches_path = output_directory;
    std::string sfm_data_json_path = matches_path + "/sfm_data.json";
    std::string reconstruction_path = output_directory;
    std::string sfm_data_bin_path = reconstruction_path +"/sfm_data.bin";
    std::string robust_bin_path = reconstruction_path + "/robust.bin";
    std::string robust_ply_path = reconstruction_path + "/robust.ply";
    std::string mvs_path = reconstruction_path + "/scene.mvs";
    std::string undistored_images_path = reconstruction_path + "/undistorted";

    mkdir(matches_path.c_str(), 0777);
    mkdir(reconstruction_path.c_str(), 0777);
    mkdir(undistored_images_path.c_str(), 0777);

    LOG(INFO) << "Doing Intrinsic Analysis for " << reconstruction_id;
    if(!DoMVGSFMIntrinsicAnalysis(images_directory, matches_path)){
        return false;
    }
    LOG(INFO) << "Doing Feature Computing for " << reconstruction_id;
    if(!DoMVGComputeFeatures(sfm_data_json_path, matches_path)){
        return false;
    }
    LOG(INFO) << "Doing Feature Matching for " << reconstruction_id;
    if(!DoMVGComputeMatches(sfm_data_json_path, matches_path)){
        return false;
    }
    LOG(INFO) << "Doing Sparse Reconstruction for " << reconstruction_id;
    switch(reconstruction_type){
        case ReconstructionType::GLOBAL:
            if(!DoMVGComputeGlobalReconstruction(sfm_data_json_path, matches_path, reconstruction_path)){
                return false;
            }
        break;
        case ReconstructionType::INCREMENTAL:
            if(!DoMVGComputeIncrementalReconstruction(sfm_data_json_path, matches_path, reconstruction_path)){
                return false;
            }
        break;
    }
    LOG(INFO) << "Computing structure for " << reconstruction_id;
    if(!DoMVGComputeStructure(sfm_data_bin_path, matches_path, matches_path+"/matches.e.bin", robust_bin_path)){
        return false;
    }
    LOG(INFO) << "Coloring structure for " << reconstruction_id;
    if(!DoMVGColorizeStructure(robust_bin_path, robust_ply_path)){
        return false;
    }
    LOG(INFO) << "Creating MVS File for " << reconstruction_id;
    if(!ConvertToMVS(sfm_data_bin_path, undistored_images_path, mvs_path)){
        return false;
    }
    spc_data.set_reconstruction(reconstruction_id);
    spc_data.set_mvs_path(mvs_path);
    spc_data.set_ply_path(robust_ply_path);

    return true;
}