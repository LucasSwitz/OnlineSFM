#pragma once

#include "server.pb.h"

class OBJDataStorage
{
public:
    virtual void StoreOBJ(const OBJData &img,
                          std::string *obj_path,
                          std::string *mtl_path,
                          std::string *texture_path) = 0;
    virtual int GetOBJ(const std::string &path, std::vector<char> &buf) = 0;
    virtual void DeleteOBJ(const std::string &path) = 0;
    virtual int GetMTL(const std::string &path, std::vector<char> &buf) = 0;
    virtual void DeleteMTL(const std::string &path) = 0;
    virtual int GetTexture(const std::string &path, std::vector<char> &buf) = 0;
    virtual void DeleteTexture(const std::string &path) = 0;
};