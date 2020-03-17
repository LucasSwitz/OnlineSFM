#include "reconstruction_strategy.h"
#include <glog/logging.h>
#include "reconstruction.h"

ReconstructionStrategy::ReconstructionStrategy(SFMStrategy* sfm, MVSStrategy* mvs): _sfm_strategy(sfm), _mvs_strategy(mvs){}

int ReconstructionStrategy::Reconstruct(const std::vector<ImageMetaData>& images, SparsePointCloudMetaData& spc_data, OBJMetaData& obj_data){
    //TODO: Return a different error code depending on which step failed. This will let us store SFM even if MVS fails.
    if(!this->_sfm_strategy->DoSFM(images, spc_data)){
        LOG(ERROR) << "SFM Failure for " << images[0].reconstruction();
        return false;
    }
    /*if(!this->_mvs_strategy->DoMVS(spc_data.reconstruction(),  ,obj_data)){
        return false;
    };*/
    return true;
}
