#pragma once
#include "server.pb.h"
#include "sfm_strategy.h"
#include "mvs_strategy.h"

class ReconstructionStrategy {
    public:
        ReconstructionStrategy(SFMStrategy* sfm, MVSStrategy* mvs);
        int Reconstruct(const std::vector<ImageMetaData>& images, SparsePointCloudMetaData& spc_data, OBJMetaData& obj_data);
    private:
        SFMStrategy* _sfm_strategy;
        MVSStrategy* _mvs_strategy;
};