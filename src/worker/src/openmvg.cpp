#include <glog/logging.h>
#include <unistd.h>  
#include <stdlib.h> 
#include <errno.h>   
#include <sys/wait.h> 
#include "config.h"
#include "openmvg.h"
#include "util.h"

#define cc(s) const_cast<char*>(s.c_str())

static bool ExecuteMVGBinary(const std::string& binary_name, char** argv){
    LOG(INFO) << "Forking: " << binary_name;
    std::string openmvg_binaries_path = std::getenv("OPENMVG_BIN");
    std::string binary_path = openmvg_binaries_path+"/"+binary_name;
    int status; 
    pid_t pid = fork();
    if(pid == -1){ 
        LOG(ERROR) << "Failed to fork " << binary_name;
    }else if(pid == 0){
        execv(const_cast<char*>(binary_path.c_str()), argv);
        LOG(ERROR) << "Unknown command: " << binary_path;
        exit(1);
    }else{
        pid_t tpid = wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status))  {
            return true;              
        } else if (WIFEXITED(status) && WEXITSTATUS(status)) { 
            if (WEXITSTATUS(status) == 127) { 
                LOG(ERROR) << "execv failed";
            } 
            else {
                LOG(ERROR) << "OpenMVG termianted with non-zero code";
            }               
        } 
        else{ 
            LOG(ERROR) << "OpenMVG termianted with non-zero code";
        }
    }  
    return false;
}

bool DoMVGSFMIntrinsicAnalysis(const std::string& images_directory, 
                               const std::string& output_directory){
    /*
        1. Takes input dir of images
        2. Creates an output dir with sfm_data.json 
    */
    std::string sensor_db = std::getenv("OPENMVG_SENSOR_DB");
    sensor_db += + "/sensor_width_camera_database.txt";
    char* argv[] = {"openMVG_main_SfMInit_ImageListing",
                    "-i", cc(images_directory),
                    "-o", cc(output_directory),
                    "-d", cc(sensor_db),
                    NULL};
    return ExecuteMVGBinary("openMVG_main_SfMInit_ImageListing", argv);
}

bool DoMVGComputeFeatures(const std::string& sfm_data_path, 
                          const std::string& output_directory){
    /*
        1. Takes input sfm_data.json and a feature type
        2. outputs to sfm_data.json
    */
    std::string threads = std::to_string(CONFIG_GET_INT("reconstruction.threads"));
    char* argv[] = {"openMVG_main_ComputeFeatures",
                    "-i", cc(sfm_data_path),
                    "-o", cc(output_directory),
                    "--numThreads", cc(threads),
                    "-m", "SIFT",
                    NULL};
    return ExecuteMVGBinary("openMVG_main_ComputeFeatures", argv);
}

bool DoMVGComputeMatches(const std::string& sfm_data_path, 
                         const std::string& output_directory){
    /*
        1. Takes input sfm_data.json and a feature type
        2. Creates an output dir with sfm_data.json 
    */
   //DeleteIfExists(output_directory +"/matches.e.bin");
   //DeleteIfExists(output_directory +"/matches.putative.bin");
   DeleteIfExists(output_directory +"/puntative_matches");
   DeleteIfExists(output_directory +"/geometric_matches");
   char* argv[] = {"openMVG_main_ComputeMatches",
                   "-i", cc(sfm_data_path),
                   "-o", cc(output_directory),
                   "-g", "e",
                   NULL};
   return ExecuteMVGBinary("openMVG_main_ComputeMatches", argv);
}

bool DoMVGComputeGlobalReconstruction(const std::string& sfm_data_path, 
                                const std::string& matches_directory, 
                                const std::string& output_directory){
    /*
        1. Takes input sfm_data.json
        2. outputs to reconstruction_dir
    */
   char* argv[] = {"openMVG_main_GlobalSfM",
                   "-i", cc(sfm_data_path),
                   "-m", cc(matches_directory),
                   "-o", cc(output_directory),
                   NULL};
   return ExecuteMVGBinary("openMVG_main_GlobalSfM", argv);
}

bool DoMVGComputeIncrementalReconstruction(const std::string& sfm_data_path, 
                                const std::string& matches_directory, 
                                const std::string& output_directory){
    /*
        1. Takes input sfm_data.json
        2. outputs to reconstruction_dir
    */
   char* argv[] = {"openMVG_main_IncrementalSfM",
                   "-i", cc(sfm_data_path),
                   "-m", cc(matches_directory),
                   "-o", cc(output_directory),
                   NULL};
   return ExecuteMVGBinary("openMVG_main_IncrementalSfM", argv);
}

bool DoMVGComputeStructure(const std::string& sfm_data_bin_path,
                           const std::string& matches_directory,
                           const std::string& matches_file,
                           const std::string& robust_bin_file_path){
    /*
     * Takes in reconstruction_dir/sfm_data.bin
     * Takes in matches_dir/matches.e.bin
     * Outputs robust.bin
    */
   char* argv[] = {"openMVG_main_ComputeStructureFromKnownPoses",
                   "-i", cc(sfm_data_bin_path),
                   "-m", cc(matches_directory),
                   "-f", cc(matches_file),
                   "-o", cc(robust_bin_file_path),
                   NULL};
   return ExecuteMVGBinary("openMVG_main_ComputeStructureFromKnownPoses", argv);
}

bool DoMVGColorizeStructure(const std::string& robust_bin_path,
                            const std::string& output_ply_file_path){
    /*
    * 1. Takes input from reconstruction_dir/robust.bin
    * 2. writes to reconstruction_dir/robust_colorized.ply
    */
   char* argv[] = {"openMVG_main_ComputeSfM_DataColor",
                   "-i", cc(robust_bin_path),
                   "-o", cc(output_ply_file_path),
                   NULL};
   return ExecuteMVGBinary("openMVG_main_ComputeSfM_DataColor", argv);
}

bool ConvertToMVS(const std::string& robust_data_bin_path, 
                  const std::string& undistored_images_path, 
                  const std::string& mvs_output_path){
    char* argv[] = {"openMVG_main_openMVG2openMVS",
                    "-i", cc(robust_data_bin_path),
                    "-d", cc(undistored_images_path),
                    "-o", cc(mvs_output_path),
                    NULL};
    return ExecuteMVGBinary("openMVG_main_openMVG2openMVS", argv);
}

bool DoOpenMVGLocalizeImages(const std::string& sfm_data_path, 
                           const std::string& matches_path,
                           const std::string& output_path,
                           const std::string& query_images_path){
    char* argv[] = {"openMVG_main_SfM_Localization", 
                    "-i", cc(sfm_data_path),
                    "-m", cc(matches_path),
                    "-o", cc(output_path),
                    "-q", cc(query_images_path),
                    NULL};
    return ExecuteMVGBinary("openMVG_main_SfM_Localization", argv);
}

bool DoOpenMVGIncrementalSFM2(const std::string& sfm_data_path, 
                              const std::string& matches_path, 
                              const std::string& output_directory){
    DeleteIfExists(output_directory +"/global_relative_rotation_view_graph");
    DeleteIfExists(output_directory +"/global_relative_rotation_pose_graph_final");
    DeleteIfExists(output_directory +"/global_relative_rotation_pose_graph");
    std::string m = matches_path +"/matches.e.bin";
    char* argv[] = {"openMVG_main_IncrementalSfM2",
                   "-i", cc(sfm_data_path),
                   "-m", cc(matches_path),
                   "-o", cc(output_directory),
                   "-M", cc(m),
                   "-S", "EXISTING_POSE",
                   NULL};
   return ExecuteMVGBinary("openMVG_main_IncrementalSfM2", argv);
}