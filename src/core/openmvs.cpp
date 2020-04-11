
#include <glog/logging.h>
#include <unistd.h>  
#include <stdlib.h> 
#include <errno.h>   
#include <sys/wait.h> 
#include "config.h"
#include "openmvs.h"

#define cc(s) const_cast<char*>(s.c_str())

bool ExecuteMVSBinary(const std::string& binary_name, char** argv){
    LOG(INFO) << "Forking: " << binary_name;
    std::string openmvs_binaries_path = std::getenv("OPENMVS_BIN");
    std::string binary_path = openmvs_binaries_path+"/"+binary_name;
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
                LOG(ERROR) << "OpenMVS termianted with non-zero code";
            }               
        } 
        else{ 
            LOG(ERROR) << "OpenMVS termianted with non-zero code";
        }
    }  
    return false;
}

bool OpenMVSDensifyPointCloud(const std::string& working_dir, const std::string& mvs_path, int fusion_mode){
    std::string threads = std::to_string(CONFIG_GET_INT("reconstruction.threads"));
    char* argv[] = {"DensifyPointCloud",
                    "-w", cc(working_dir),
                    "--max-threads", cc(threads),
                    cc(mvs_path),
                    NULL};
    return ExecuteMVSBinary("DensifyPointCloud", argv);
}

bool OpenMVSReconstructMesh(const std::string& working_dir, const std::string& dense_mvs_path){
    std::string threads = std::to_string(CONFIG_GET_INT("reconstruction.threads"));
    char* argv[] = {"ReconstructMesh",
                    "-w", cc(working_dir),
                    "--max-threads", cc(threads),
                    cc(dense_mvs_path),
                    NULL};
    return ExecuteMVSBinary("ReconstructMesh", argv);
}

bool OpenMVSRefineMesh(const std::string& working_dir, const std::string& dense_mesh_mvs_path, int max_face_area){
    std::string face_area = std::to_string(max_face_area);
    std::string threads = std::to_string(CONFIG_GET_INT("reconstruction.threads"));
    char* argv[] = {"RefineMesh",
                    "-w", cc(working_dir),
                    "--max-threads", cc(threads),
                    "--max-face-area", cc(face_area),
                    cc(dense_mesh_mvs_path),
                    NULL};
    return ExecuteMVSBinary("RefineMesh", argv);
}

bool OpenMVSTextureMesh(const std::string& working_dir, const std::string& dense_mesh_refine_mvs_path){
    std::string threads = std::to_string(CONFIG_GET_INT("reconstruction.threads"));
    char* argv[] = {"TextureMesh",
                    "-w", cc(working_dir),
                    "--max-threads", cc(threads),
                    "--export-type", "obj",
                    cc(dense_mesh_refine_mvs_path),
                    NULL};
    return ExecuteMVSBinary("TextureMesh", argv);
}
