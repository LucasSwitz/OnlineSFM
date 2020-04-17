#pragma once

#include <string>
#include <set>

class ReconstructionAgent{
    public:
        virtual bool IncrementalSFM() = 0;
        virtual bool AddImage(const std::string& image_id) = 0;
        virtual bool ComputeFeatures(const std::set<std::string>& image_ids) = 0;
        virtual bool ComputeMatches(const  std::set<std::string>& image_ids) = 0;
        virtual bool ComputeStructure() = 0;
};