#pragma once
#include "vocab_index.h"
#include "descriptor_storage.h"

typedef typename std::shared_ptr<DescriptorStorage<SIFT_Descriptor>> DescriptorStorePtr;

class ImageIndexer {
    public:
        ImageIndexer(DescriptorStorePtr desc_storage,
                     DescriptorStorePtr word_storage,
                     std::shared_ptr<VisualVocabularyIndex> index);
        virtual void Index(const std::string& reconstruction_id, const std::string& image_id);
        virtual std::unordered_map<std::string, SIFT_Descriptor_count_map> GetSimilar(const std::string& image_id);
    private:
        std::shared_ptr<VisualVocabularyIndex> _index = nullptr;
        DescriptorStorePtr _descriptor_storage = nullptr;
        DescriptorStorePtr _word_storage = nullptr;
};

class ImageIndexerFactory{
    public:
        static std::unique_ptr<ImageIndexer> GetImageIndexer(std::shared_ptr<VisualVocabularyIndex> index);
};

