/*#pragma once

#include "openMVS/libs/MVS.h"

class DistributedScene : public MVS::Scene {
    public:
        bool LoadFromeRemoteScene(const std::string& reconstruction_id);
        //bool ComputeDepthMaps(DenseDepthMapData& data) override;
    protected:
        template <typename TYPE>
        bool SerializeLoad(TYPE& obj, std::istringstream& fs, ARCHIVE_TYPE type, unsigned flags=0)
        {
            try {
                switch (type) {
                case ARCHIVE_TEXT: {
                    boost::archive::text_iarchive ar(fs, flags);
                    ar >> obj;
                    break; }
                case ARCHIVE_BINARY: {
                    boost::archive::binary_iarchive ar(fs, flags);
                    ar >> obj;
                    break; }
                case ARCHIVE_BINARY_ZIP: {
                    namespace io = boost::iostreams;
                    io::filtering_streambuf<io::input> ffs;
                    ffs.push(io::zlib_decompressor());
                    ffs.push(fs);
                    boost::archive::binary_iarchive ar(ffs, flags);
                    ar >> obj;
                    break; }
                default:
                    return false;
                }
            }
            catch (const std::exception& e) {
                return false;
            }
            return true;
        }
};*/