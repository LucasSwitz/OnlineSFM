#include "image_indexer.h"

#include "sql_descriptor_storage.h"
#include "config.h"
#include "types.h"
#include <cppconn/driver.h>
#include "sql_regions_storage.h"
#include "sql_indexable_descriptor_storage.h"

std::unique_ptr<ImageIndexer> ImageIndexerFactory::GetImageIndexer(std::shared_ptr<VisualVocabularyIndex> index)
{
    return std::make_unique<ImageIndexer>(
        std::make_shared<SQLRegionsStorage<openMVG::features::SIFT_Anatomy_Image_describer::Regions_type>>(
            CONFIG_GET_STRING("sql.regions_table")),
        std::make_shared<SQLIndexableDescriptorStorage>(
            CONFIG_GET_STRING("sql.words_table"),
            CONFIG_GET_STRING("sql.word_frequencies_table")),
        index);
}

ImageIndexer::ImageIndexer(RegionsStoragePtr regions_storage,
                           DescriptorStorePtr word_storage,
                           std::shared_ptr<VisualVocabularyIndex> index) : _regions_storage(regions_storage),
                                                                           _word_storage(word_storage),
                                                                           _index(index)
{
}

void ImageIndexer::Index(const std::string &reconstruction_id, const std::string &image_id)
{
    SIFT_Vector descriptors = this->_regions_storage->GetAllDescriptors(reconstruction_id, image_id);
    SIFT_Descriptor_count_map sparse_descriptors = SIFT_Vector_to_Sparse_Vector(descriptors);

    SIFT_Descriptor_count_map word_sparse;
    {
        PrecisionTimer t("Index.ImageIndexer.Index.IndexAllDescriptors");
        for (auto desc : sparse_descriptors)
        {
            auto word = this->_index->Index(desc.first);
            if (word_sparse.find(word) == word_sparse.end())
            {
                word_sparse[word] = 0;
            }
            word_sparse[word] += desc.second;
        }
    }
    {
        PrecisionTimer t("Index.ImageIndexer.Index.StoreWords");
        this->_word_storage->Store(reconstruction_id, image_id, word_sparse);
    }
}

std::set<std::string> ImageIndexer::GetSimilar(const std::string &reconstruction_id, const std::string &image_id, std::string *page_start, int page_size)
{
    return this->_word_storage->GetAllWithCommonDescriptors(reconstruction_id, image_id, page_size, page_start);
}