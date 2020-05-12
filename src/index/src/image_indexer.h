#pragma once
#include "vocab_index.h"
#include "regions_storage.h"
#include "descriptor_storage.h"
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer.hpp"

typedef typename std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> DescriptorStorePtr;
typedef typename std::shared_ptr<RegionsStorage<openMVG::features::SIFT_Anatomy_Image_describer::Regions_type>> RegionsStoragePtr;
class ImageIndexer {
    public:
        ImageIndexer(RegionsStoragePtr regions_storage,
                     DescriptorStorePtr word_storage,
                     std::shared_ptr<VisualVocabularyIndex> index);
        virtual void Index(const std::string& reconstruction_id, const std::string& image_id);
        virtual std::set<std::string> GetSimilar(const std::string& image_id, 
                                                                                      std::string* page_start,
                                                                                      int page_size = 0);
    private:
        std::shared_ptr<VisualVocabularyIndex> _index = nullptr;
        RegionsStoragePtr _regions_storage = nullptr;
        DescriptorStorePtr _word_storage = nullptr;
};

class ImageIndexerFactory{
    public:
        static std::unique_ptr<ImageIndexer> GetImageIndexer(std::shared_ptr<VisualVocabularyIndex> index);
};

