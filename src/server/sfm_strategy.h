#pragma once
#include <string>
#include "server.pb.h"

class SFMStrategy{
    public:
        virtual int DoSFM(const std::vector<ImageMetaData>& images, SparsePointCloudMetaData& spc_data) = 0;
};