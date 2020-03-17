#pragma once
#include "sfm_strategy.h"

typedef enum ReconstructionType{
    INCREMENTAL,
    GLOBAL
};

class OpenMVGStrategy : public SFMStrategy{
    public:
        OpenMVGStrategy(ReconstructionType reconstruction_type);
        int DoSFM(const std::vector<ImageMetaData>& images, SparsePointCloudMetaData& spc_data);
    private:
        ReconstructionType _reconstruction_type;
        void ParseSparsePointCloudData(const std::string& output_dir, SparsePointCloudMetaData& spc_data);
};