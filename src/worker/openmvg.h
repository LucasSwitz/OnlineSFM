#pragma once
#include <string>

bool DoMVGSFMIntrinsicAnalysis(const std::string& images_directory, 
                               const std::string& output_directory);
bool DoMVGComputeFeatures(const std::string& sfm_data_path, 
                          const std::string& output_directory);
bool DoMVGComputeMatches(const std::string& sfm_data_path, 
                         const std::string& output_directory);
bool DoMVGComputeGlobalReconstruction(const std::string& sfm_data_path, 
                                const std::string& matches_directory, 
                                const std::string& output_directory);
bool DoMVGComputeIncrementalReconstruction(const std::string& sfm_data_path, 
                                const std::string& matches_directory, 
                                const std::string& output_directory);
bool DoMVGComputeStructure(const std::string& sfm_data_bin_path,
                           const std::string& matches_directory,
                           const std::string& matches_file,
                           const std::string& robust_bin_file_path);
bool DoMVGColorizeStructure(const std::string& robust_bin_path,
                            const std::string& output_ply_file_path);
bool ConvertToMVS(const std::string& robust_data_bin_path, 
                  const std::string& undistored_images_path, 
                  const std::string& mvs_output_path);
bool DoOpenMVGLocalizeImages(const std::string& sfm_data_path, 
                           const std::string& matches_path,
                           const std::string& output_path,
                           const std::string& query_images_path);
bool DoOpenMVGIncrementalSFM2(const std::string& sfm_data_path, 
                              const std::string& matches_path, 
                              const std::string& output_directory);