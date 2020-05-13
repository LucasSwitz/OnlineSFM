#pragma once
#include "flann/algorithms/kdtree_index.h"
#include "openMVG/features/regions_factory.hpp"
#include "openMVG/features/descriptor.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider.hpp"
#include "types.h"

using namespace flann;
using namespace openMVG::features;
using namespace openMVG::sfm;



struct SIFTDistance {
    typedef bool is_kdtree_distance;
    typedef unsigned char ElementType;
    typedef unsigned char ResultType;

    template <typename Iterator1, typename Iterator2>
    ResultType operator()(Iterator1 a, Iterator2 b, size_t size, 
                        ResultType /*worst_dist*/ = -1) const
    {
        ResultType result = ResultType();
        ResultType diff;
        for(size_t i = 0; i < size; ++i ){
            diff = *a++ - *b++;
            result += diff*diff;
        }
        return result;
    }
    template <typename U, typename V>
    inline ResultType accum_dist(const U& a, const V& b, int) const
    {
        return (a-b)*(a-b);
    }
};

class VisualVocabularyIndex {
    public:
        VisualVocabularyIndex(std::unique_ptr<NNIndex<SIFTDistance>> index) :  _index(std::move(index)){

        }

        SIFT_Descriptor Index(const SIFT_Descriptor& d){
            unsigned char data[d.size()];
            size_t indices_arr[1];
            SIFTDistance::ResultType dists_arr[1];
            memcpy(data, d.data(), d.size());
            flann::Matrix<SIFTDistance::ElementType> query(data, 1, 128);
            flann::Matrix<size_t> indices(indices_arr, 1, 1);
            flann::Matrix<SIFTDistance::ResultType> dists(dists_arr, 1, 1);
            SearchParams params;
            this->_index->knnSearch(query, indices, dists, 1, params);
            SIFTDistance::ElementType* desc_start = this->_index->getPoint(*indices[0]);
            SIFT_Descriptor desc_out;
            memcpy(&desc_out[0], desc_start, 128);
            return desc_out;
        };        
        
        void Load(const std::string& input){
            FILE* fout = fopen(input.c_str(), "rb");
            this->_index->loadIndex(fout);
            fclose(fout);
        }

    private:
        std::unique_ptr<NNIndex<SIFTDistance>> _index;
};

class VisualVocabularyGenerator {
    public:
        VisualVocabularyGenerator(){
            auto params = KDTreeIndexParams(10);
            params["save_dataset"] = true;
            this->_index = std::make_unique<KDTreeIndex<SIFTDistance>>(Matrix<SIFTDistance::ElementType>(nullptr, 0, 128), params);
        }

        void Generate(){
            Matrix<SIFTDistance::ElementType> descriptors(this->_descriptors.data(), this->_num_descriptors, 128);
            this->_index->addPoints(descriptors);
            this->_index->buildIndex();
        }

        void AddDescriptors(const SIFT_Vector& descs){
            for(auto desc : descs){
                this->_descriptors.insert(this->_descriptors.end(), desc.data(), desc.data() + 128);
                this->_num_descriptors++;
            }
        } 

        VisualVocabularyIndex Export(){
            return VisualVocabularyIndex(std::move(this->_index));
        }

        void Dump(const std::string& output){
            FILE* fout = fopen(output.c_str(), "wb");
            this->_index->saveIndex(fout);
            fclose(fout);
        }

    private:
        std::unique_ptr<KDTreeIndex<SIFTDistance>> _index = nullptr;
        std::vector<SIFTDistance::ElementType> _descriptors;
        size_t _num_descriptors = 0;
};      