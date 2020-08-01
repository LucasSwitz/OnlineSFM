#include "sql_descriptor_score_storage.h"

#define SQL_STORE_IMAGE_WORD_SCORE(t) "INSERT INTO " + t + " VALUES (?,?,?,?) ON DUPLICATE KEY UPDATE SCORE = ?"
#define SQL_GET_IMAGE_WORD_SCORE(t) "SELECT WORD, SCORE FROM " + t + " WHERE RECONSTRUCTION_ID = ? and IMAGE_ID = ?"

SQLDescriptorScoreStorage::SQLDescriptorScoreStorage(const std::string &table) : _table(table)
{
}
void SQLDescriptorScoreStorage::Store(const std::string &reconstruction_id, const std::string &image_id, const SIFT_Descriptor_weight_map &scores)
{
    this->Transaction([this,
                       reconstruction_id, image_id, &scores](std::shared_ptr<sql::Connection> con) mutable {
        for (auto e : scores)
        {
            auto descriptor = e.first;
            float score = e.second;
            std::string desc_string((char *)descriptor.data(), 128);
            this->Execute(SQL_STORE_IMAGE_WORD_SCORE(this->_table),
                          con,
                          [this, reconstruction_id, image_id, desc_string, score](sql::PreparedStatement *pstmt) {
                              pstmt->setString(1, reconstruction_id);
                              pstmt->setString(2, image_id);
                              pstmt->setString(3, desc_string);
                              pstmt->setDouble(4, score);
                              pstmt->setDouble(5, score);
                          });
        }
    });
}

SIFT_Descriptor_weight_map SQLDescriptorScoreStorage::Get(const std::string &reconstruction_id, const std::string &image_id)
{
    auto connection_loan = this->GetConnection();
    SIFT_Descriptor_weight_map scores;
    sql::ResultSet *res = this->IssueQuery(SQL_GET_IMAGE_WORD_SCORE(this->_table), connection_loan.con,
                                           [reconstruction_id, image_id](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, reconstruction_id);
                                               stmt->setString(2, image_id);
                                           });
    while (res->next())
    {
        SIFT_Descriptor word;
        std::string word_str = res->getString("WORD");
        memcpy((char *)word.data(), word_str.data(), 128);
        scores[word] = res->getDouble("SCORE");
    }
    delete res;
    return scores;
}