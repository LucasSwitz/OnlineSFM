#pragma once
#include "server.pb.h"

/*Will want to fork and exec to the openMVS and openMVG binaries */
class MVSStrategy
{
public:
    virtual int DoMVS(const std::string &reconstruction_id,
                      const std::string &mvs_path,
                      OBJMetaData &obj_data) = 0;
};