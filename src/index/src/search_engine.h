#pragma once

#include "image_indexer.h"
#include "tfidf_ranker.h"
#include "types.h"

class SearchEngine{
    public:
        SearchEngine(std::unique_ptr<ImageIndexer> index, std::unique_ptr<Ranker> ranker) : _index(std::move(index)), _ranker(std::move(ranker)){ 

        }

        std::vector<std::string> Search(const std::string& image_id, unsigned int result_count = 100){
            auto matches = this->_index->GetSimilar(image_id);
            auto ranked = this->_ranker->Rank(image_id, matches);
            return std::vector<std::string>(ranked.begin(), ranked.begin() + std::min((size_t)result_count+1, ranked.size()));
        }

    private:
        std::unique_ptr<ImageIndexer> _index = nullptr; 
        std::unique_ptr<Ranker> _ranker;;

};