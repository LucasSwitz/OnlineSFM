#pragma once

#include "ranker.h"
#include "descriptor_storage.h"
#include <memory>

class TFIDFRanker : public Ranker{
    public:
        TFIDFRanker(std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> descriptor_storage) : _descriptor_storage(descriptor_storage){

        }
        std::vector<std::string> Rank(const std::string& query_image_id, 
                                      const std::unordered_map<std::string, SIFT_Descriptor_count_map>& res){
            this->_query_word_frequencies = this->_descriptor_storage->GetAllDescriptors(query_image_id);
            this->_global_word_frequencies = this->_descriptor_storage->GetGlobalDescriptorFrequenciesByImage(query_image_id);
            this->_number_of_documents = this->_descriptor_storage->GetImageCount();

            std::vector<std::tuple<std::string, SIFT_Descriptor_count_map>> tuple_elems(res.begin(), res.end());

            std::sort(tuple_elems.begin(), tuple_elems.end(), [this, res](auto e1, auto e2) -> bool{
                auto map1 = std::get<1>(e1);
                auto map2 = std::get<1>(e2);

                auto query_weight_map = ComputeTFID(this->_query_word_frequencies);
                double dist1 = SIFT_Descriptor_weight_map_L2_Distance(query_weight_map, ComputeTFID(map1));
                double dist2 = SIFT_Descriptor_weight_map_L2_Distance(query_weight_map, ComputeTFID(map2));

                return  dist1 > dist2;
            });

            std::vector<std::string> results;
            std::transform(tuple_elems.begin(), tuple_elems.end(), std::back_inserter(results), [](auto e) {return std::get<0>(e);});
            return results;
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
                auto count = e.second;
                double tf = count / total_num_words;
                double idf = log(this->_number_of_documents / this->_global_word_frequencies[word]);
                out_map[word] = tf*idf;
            }
            return out_map;
        }

        SIFT_Descriptor_count_map _global_word_frequencies;
        SIFT_Descriptor_count_map _query_word_frequencies;
        std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> _descriptor_storage = nullptr;
        unsigned int _number_of_documents = 0;
};