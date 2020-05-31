#pragma once
#include "openMVG/third_party/flann/src/cpp/flann/algorithms/kdtree_index.h"
#include "openMVG/third_party/flann/src/cpp/flann/algorithms/kmeans_index.h"
#include "openMVG/features/regions_factory.hpp"
#include "openMVG/features/descriptor.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider.hpp"
#include "types.h"
#include <limits.h>

using namespace flann;
using namespace openMVG::features;
using namespace openMVG::sfm;

typedef typename flann::L1<unsigned char> SIFTDistance;

class VisualVocabularyIndex
{
public:
    VisualVocabularyIndex(std::unique_ptr<NNIndex<SIFTDistance>> index) : _index(std::move(index))
    {
    }

    SIFT_Descriptor Index(const SIFT_Descriptor &d)
    {
        unsigned char data[d.size()];
        size_t indices_arr[1];
        SIFTDistance::ResultType dists_arr[1];
        memcpy(data, d.data(), d.size());
        flann::Matrix<SIFTDistance::ElementType> query(data, 1, 128);
        flann::Matrix<size_t> indices(indices_arr, 1, 1);
        flann::Matrix<SIFTDistance::ResultType> dists(dists_arr, 1, 1);
        SearchParams params;
        params.use_heap = flann::FLANN_True;
        SIFT_Descriptor desc_out;
        if (this->_index->knnSearch(query, indices, dists, 1, params))
        {
            SIFTDistance::ElementType *desc_start = this->_index->getPoint(indices_arr[0]);
            memcpy(&desc_out[0], desc_start, 128);
        }
        else
        {
            throw std::runtime_error("No Neighbor found!");
        }
        return desc_out;
    };

    void Load(const std::string &input)
    {
        FILE *fout = fopen(input.c_str(), "rb");
        this->_index->loadIndex(fout);
        fclose(fout);
    }

private:
    std::unique_ptr<NNIndex<SIFTDistance>> _index;
};

class VisualVocabularyGenerator
{
public:
    VisualVocabularyGenerator()
    {
    }

    void Generate()
    {
        Matrix<SIFTDistance::ElementType> descriptors(this->_descriptors.data(), this->_num_descriptors, 128);
        auto params = KMeansIndexParams(10, 1024, FLANN_CENTERS_RANDOM, 0.2);
        params["save_dataset"] = true;
        this->_index = std::make_unique<KMeansIndex<SIFTDistance>>(descriptors, params);
        //this->_index->addPoints(descriptors);
        this->_index->buildIndex();
    }

    void AddDescriptors(const SIFT_Vector &descs)
    {
        for (auto desc : descs)
        {
            this->_descriptors.insert(this->_descriptors.end(), desc.data(), desc.data() + 128);
            this->_num_descriptors++;
        }
    }

    VisualVocabularyIndex Export()
    {
        return VisualVocabularyIndex(std::move(this->_index));
    }

    void Dump(const std::string &output)
    {
        FILE *fout = fopen(output.c_str(), "wb");
        this->_index->saveIndex(fout);
        fclose(fout);
    }

private:
    std::unique_ptr<KMeansIndex<SIFTDistance>> _index = nullptr;
    std::vector<SIFTDistance::ElementType> _descriptors;
    size_t _num_descriptors = 0;
};