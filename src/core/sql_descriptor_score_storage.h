#pragma once
#include "descriptor_score_storage.h"
#include "sql_storage.h"

class SQLDescriptorScoreStorage : public DescriptorScoreStorage, public SQLStorage
{
public:
    SQLDescriptorScoreStorage(const std::string &table);
    void Store(const std::string &reconstruction_id, const std::string &image_id, const SIFT_Descriptor_weight_map &scores);
    SIFT_Descriptor_weight_map Get(const std::string &reconstruction_id, const std::string &image_id);

private:
    std::string _table;
};