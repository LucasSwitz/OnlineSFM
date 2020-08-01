#pragma once

#include "ranker.h"
#include "descriptor_storage.h"
#include "image_storage.h"
#include "index.pb.h"
#include "util.h"
#include "descriptor_score_storage.h"
#include <memory>

class TFIDFRanker : public Ranker
{
public:
    TFIDFRanker(std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> word_storage,
                std::shared_ptr<ImageStorageAdapter> image_storage,
                std::shared_ptr<DescriptorScoreStorage> score_storage) : _image_storage(image_storage),
                                                                         _word_storage(word_storage),
                                                                         _score_storage(score_storage)
    {
    }

    void Score(const std::string &reconstruction_id, const std::string &image_id)
    {
        {
            PrecisionTimer t("TFIDRanker.Score.GetAllDescriptors");
            this->_query_word_frequencies = this->_word_storage->GetAllDescriptors(reconstruction_id, image_id);
        }
        {
            PrecisionTimer t("TFIDRanker.Score.GetGlobalDescriptorFrequencies");
            this->_global_word_frequencies = this->_word_storage->GetGlobalDescriptorFrequencies(reconstruction_id);
        }
        {
            PrecisionTimer t("TFIDRanker.Score.GetImageCount");
            this->_number_of_documents = this->_image_storage->GetImageCount(reconstruction_id);
        }
        {
            PrecisionTimer t("TFIDRanker.Score.ScoreImage");
            auto image_descs = this->_word_storage->GetAllDescriptors(reconstruction_id, image_id);
            auto score_vec = ComputeTFIDF(image_descs);
            this->_score_storage->Store(reconstruction_id, image_id, score_vec);
        }
    }

    std::vector<std::tuple<std::string, float>>
    Rank(const std::string &reconstruction_id,
         const std::string &query_image_id,
         const std::set<std::string> &similar,
         ClosestNDetails *details = nullptr)
    {
        auto query_weight_map = this->_score_storage->Get(reconstruction_id, query_image_id);
        std::unordered_map<std::string, float> scores;
        for (auto id : similar)
        {
            double score = 0.0;
            auto tfidf_score_vec = this->_score_storage->Get(reconstruction_id, id);
            if (details)
            {
                ClosestNDetails_ClosestNImageDetails image_details;
                score = SIFT_Descriptor_weight_map_norm_dot(query_weight_map, tfidf_score_vec);
                image_details.set_score(score);
                (*details->mutable_image_details())[id] = image_details;
            }
            else
            {
                score = SIFT_Descriptor_weight_map_norm_dot(query_weight_map, tfidf_score_vec);
            }
            scores[id] = score;
        }
        std::vector<std::string> results;
        std::vector<std::tuple<std::string, float>> tuple_elems(scores.begin(), scores.end());
        int size = tuple_elems.size();
        std::sort(tuple_elems.begin(), tuple_elems.end(), [this](const auto &e1, const auto &e2) -> bool {
            return std::get<1>(e1) > std::get<1>(e2);
        });
        return tuple_elems;
    }

private:
    SIFT_Descriptor_weight_map ComputeTFIDF(SIFT_Descriptor_count_map &in_map)
    {
        SIFT_Descriptor_weight_map out_map;
        // total number of words for this document
        int total_num_words = std::accumulate(in_map.begin(), in_map.end(), 0, [in_map](int sum, auto e) -> int {
            return sum + e.second;
        });
        for (auto e : in_map)
        {
            auto word = e.first;
            double count = e.second;
            double tf = count / total_num_words;
            unsigned int word_frequency = this->_global_word_frequencies->at(word);
            double idf = log(double(this->_number_of_documents) / word_frequency);
            double score = tf * idf;
            assert(score != std::numeric_limits<double>::infinity());
            out_map[word] = score;
        }
        return out_map;
    }

    std::shared_ptr<SIFT_Descriptor_count_map> _global_word_frequencies;
    SIFT_Descriptor_count_map _query_word_frequencies;
    std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> _word_storage = nullptr;
    std::shared_ptr<ImageStorageAdapter> _image_storage = nullptr;
    std::shared_ptr<DescriptorScoreStorage> _score_storage;
    unsigned int _number_of_documents = 0;
};