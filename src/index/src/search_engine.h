#pragma once

#include "image_indexer.h"
#include "tfidf_ranker.h"
#include "types.h"
#include "index.pb.h"
#include "util.h"

#define SIFT_DESCRIPTOR_PAGE_MIN()
class SearchEngine
{
public:
    SearchEngine(std::unique_ptr<ImageIndexer> index, std::unique_ptr<Ranker> ranker) : _index(std::move(index)), _ranker(std::move(ranker))
    {
    }

    std::vector<std::string> Search(const std::string &reconstruction_id,
                                    const std::string &image_id,
                                    unsigned int result_count = 100,
                                    ClosestNDetails *details = nullptr)
    {
        std::set<std::string> matches;
        {
            PrecisionTimer t("ClosestN.SearchEngine.GetSimilar");
            matches = this->_index->GetSimilar(reconstruction_id, image_id, nullptr);
        }
        std::vector<std::tuple<std::string, float>> ranked;
        {
            PrecisionTimer t("ClosestN.SearchEngine.Rank");
            ranked = this->_ranker->Rank(reconstruction_id, image_id, matches, details);
        }
        auto current_results = std::vector<std::tuple<std::string, float>>(ranked.begin(),
                                                                           ranked.begin() + std::min((size_t)result_count + 1,
                                                                                                     ranked.size()));
        std::vector<std::string> out;
        std::for_each(current_results.begin(), current_results.end(), [&out](auto e) mutable {
            out.push_back(std::get<0>(e));
        });
        return out;
    }

private:
    std::unique_ptr<ImageIndexer> _index = nullptr;
    std::unique_ptr<Ranker> _ranker;
    ;
};