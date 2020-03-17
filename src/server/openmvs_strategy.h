#pragma once

#include "mvs_strategy.h"

class OpenMVSStrategy : public MVSStrategy {
    public:
        int DoMVS(const std::string& reconstruction_id, 
                  const std::string& mvs_path, 
                  OBJMetaData& obj_data);
};