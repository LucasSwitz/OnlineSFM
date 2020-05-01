#include "image_indexer.h"

#include "sql_descriptor_storage.h"
#include "config.h"
#include "types.h"
#include <cppconn/driver.h>
#include "sql_regions_storage.h"

std::unique_ptr<ImageIndexer> ImageIndexerFactory::GetImageIndexer(std::shared_ptr<VisualVocabularyIndex> index){
    return std::make_unique<ImageIndexer>(
        std::make_shared<SQLRegionsStorage<openMVG::features::SIFT_Anatomy_Image_describer::Regions_type>>(
                                               CONFIG_GET_STRING("sql.regions_table")),
        std::make_shared<SQLDescriptorStorage>(
                                               CONFIG_GET_STRING("sql.words_table")),
        index
    );
}

ImageIndexer::ImageIndexer(RegionsStoragePtr regions_storage,
                           DescriptorStorePtr word_storage,
                           std::shared_ptr<VisualVocabularyIndex> index) :  _regions_storage(regions_storage),
                                                                            _word_storage(word_storage),
                                                                            _index(index){

}

void ImageIndexer::Index(const std::string& reconstruction_id, const std::string& image_id){
    SIFT_Vector descriptors = this->_regions_storage->GetAllDescriptors(reconstruction_id, image_id);
    SIFT_Descriptor_count_map sparse_descriptors = SIFT_Vector_to_Sparse_Vector(descriptors);
    SIFT_Vector words;

    //transform(descriptors.begin(), descriptors.end(), back_inserter(words), std::bind(&VisualVocabularyIndex::Index, this->_index.get(), SIFT_Descriptor()));
    SIFT_Descriptor_count_map word_sparse;
    for(auto desc : sparse_descriptors){
        auto word = this->_index->Index(desc.first);
        if(word_sparse.find(word) == word_sparse.end()){
            word_sparse[word] = 0;
        }
        word_sparse[word]+=desc.second;
    }
    this->_word_storage->Store(reconstruction_id, image_id, word_sparse);
}

std::unordered_map<std::string, SIFT_Descriptor_count_map> ImageIndexer::GetSimilar(const std::string& image_id){
    return this->_word_storage->GetAllWithCommonDescriptors(image_id);
}