#pragma once

#include "reconstruction_agent.h"
#include "worker.grpc.pb.h"

class RemoteReconstructionAgent : public ReconstructionAgent
{
public:
    std::shared_ptr<ReconstructionAgent> MakeDefault();
    RemoteReconstructionAgent(const std::string &reconstruction_id,
                              const std::string &address);
    bool AddImage(const std::string &image_id);
    bool ComputeFeatures(const std::set<std::string> &image_paths);
    bool ComputeMatches(const std::set<std::string> &image_paths);
    bool IncrementalSFM();
    bool ComputeStructure();
    bool MVS();

private:
    std::string _reconstruction_id;
    std::string _address;
};