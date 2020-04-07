#include "image_indexer.h"

#include "sql_descriptor_storage.h"
#include "config.h"
#include "types.h"

std::unique_ptr<ImageIndexer> ImageIndexerFactory::GetImageIndexer(std::shared_ptr<VisualVocabularyIndex> index){
    return std::make_unique<ImageIndexer>(
        std::make_shared<SQLDescriptorStorage>(CONFIG_GET_STRING("sql.address"), 
                                               CONFIG_GET_STRING("sql.user"), 
                                               CONFIG_GET_STRING("sql.password"), 
                                               CONFIG_GET_STRING("sql.db"), 
                                               CONFIG_GET_STRING("sql.descriptors_table")),
        std::make_shared<SQLDescriptorStorage>(CONFIG_GET_STRING("sql.address"), 
                                               CONFIG_GET_STRING("sql.user"), 
                                               CONFIG_GET_STRING("sql.password"), 
                                               CONFIG_GET_STRING("sql.db"), 
                                               CONFIG_GET_STRING("sql.words_table")),
        index
    );
}

ImageIndexer::ImageIndexer(DescriptorStorePtr desc_storage,
                           DescriptorStorePtr word_storage,
                           std::shared_ptr<VisualVocabularyIndex> index) : _descriptor_storage(desc_storage),
                                                                            _word_storage(word_storage),
                                                                            _index(index){

}

void ImageIndexer::Index(const std::string& image_id){
    SIFT_Descriptor_count_map descriptors = this->_descriptor_storage->GetAllDescriptors(image_id);
    SIFT_Vector words;
    //transform(descriptors.begin(), descriptors.end(), back_inserter(words), std::bind(&VisualVocabularyIndex::Index, this->_index.get(), SIFT_Descriptor()));
    for(auto desc : descriptors){
        words.push_back(this->_index->Index(desc.first));
    }
    this->_word_storage->StoreAll(image_id, words);
}

std::unordered_map<std::string, SIFT_Descriptor_count_map> ImageIndexer::GetSimilar(const std::string& image_id){
    return this->_word_storage->GetAllWithCommonDescriptors(image_id);
}