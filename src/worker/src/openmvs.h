
#pragma once
#include <string>

bool ExecuteMVSBinary(const std::string &binary_name, char **argv);
bool OpenMVSDensifyPointCloud(const std::string &working_dir, const std::string &mvs_path, int fusion_mode);
bool OpenMVSReconstructMesh(const std::string &working_dir, const std::string &dense_mvs_path);
bool OpenMVSRefineMesh(const std::string &working_dir, const std::string &dense_mesh_mvs_path, int max_face_area);
bool OpenMVSTextureMesh(const std::string &working_dir, const std::string &dense_mesh_refine_mvs_path);
