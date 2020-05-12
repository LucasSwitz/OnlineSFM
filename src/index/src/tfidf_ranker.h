#pragma once

#include "ranker.h"
#include "descriptor_storage.h"
#include "image_storage.h"
#include <memory>

class TFIDFRanker : public Ranker{
    public:
        TFIDFRanker(std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> descriptor_storage, 
                    std::shared_ptr<ImageStorageAdapter> image_storage) : 
        _image_storage(image_storage),
        _descriptor_storage(descriptor_storage){

        }
        std::vector<std::tuple<std::string, float>> Rank(const std::string& reconstruction_id,
                                      const std::string& query_image_id, 
                                      const std::set<std::string>& similar){
            // will be thes
            this->_query_word_frequencies = this->_descriptor_storage->GetAllDescriptors(reconstruction_id, query_image_id);
            // will the size of the number of words for this image
            this->_global_word_frequencies = this->_descriptor_storage->GetGlobalDescriptorFrequenciesByImage(query_image_id); 
            this->_number_of_documents = this->_image_storage->GetImageCount(reconstruction_id);

            SIFT_Descriptor_weight_map query_weight_map = ComputeTFID(this->_query_word_frequencies);
            std::unordered_map<std::string, float> scores;
            for(auto id : similar){
                auto map = this->_descriptor_storage->GetAllDescriptors(reconstruction_id, id);
                scores[id] = SIFT_Descriptor_weight_map_L2_Distance(query_weight_map, ComputeTFID(map));
            }
            std::vector<std::string> results;
            std::vector<std::tuple<std::string, float>> tuple_elems(scores.begin(), scores.end());
            int size = tuple_elems.size();
            std::sort(tuple_elems.begin(), tuple_elems.end(), [this](const auto& e1, const auto& e2) -> bool{
                return std::get<1>(e1) > std::get<1>(e2);
            });
            return tuple_elems;
        }
           
    private:
        SIFT_Descriptor_weight_map ComputeTFID(SIFT_Descriptor_count_map& in_map){
            SIFT_Descriptor_weight_map out_map;
            // total number of words for this document
            int total_num_words = std::accumulate(in_map.begin(), in_map.end(), 0, [in_map](int sum, auto e) -> int{
                return sum + e.second;
            });
            for(auto e : in_map){
                auto word = e.first;
                double count = e.second;
                double tf = count / total_num_words;
                double word_frequency = this->_global_word_frequencies[word];
                double idf = log(this->_number_of_documents / word_frequency);
                out_map[word] = tf*idf;
            }
            return out_map;
        }

        SIFT_Descriptor_count_map _global_word_frequencies;
        SIFT_Descriptor_count_map _query_word_frequencies;
        std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> _descriptor_storage = nullptr;
        std::shared_ptr<ImageStorageAdapter> _image_storage = nullptr;
        unsigned int _number_of_documents = 0;
};