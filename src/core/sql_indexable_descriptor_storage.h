#pragma once
#include "sql_descriptor_storage.h"
#include "types.h"

#define SQL_INCREMENT_FREQUENCY(t) "INSERT INTO " + t + " VALUES (?,?,1) ON DUPLICATE KEY UPDATE N = N + 1"
#define SQL_GET_INDEXABLE_GLOBAL_DESCRIPTOR_FREQUENCY(t) "SELECT WORD, N FROM " + t + " WHERE RECONSTRUCTION_ID = ?"

#define STORE_CHUNK_SIZE 60000
class SQLIndexableDescriptorStorage : public SQLDescriptorStorage
{
public:
    SQLIndexableDescriptorStorage(const std::string &descriptor_tables, const std::string &frequency_table) : SQLDescriptorStorage(descriptor_tables)
    {
        this->_frequency_table = frequency_table;
    }

    void Store(const std::string &reconstruction_id, const std::string &image_id, SIFT_Descriptor_count_map &descriptors) override
    {
        SIFT_Descriptor_count_ordered_map ordered_desc(descriptors.begin(), descriptors.end());
        auto it = ordered_desc.begin();
        int n_chunks = ceil((float)(ordered_desc.size()) / STORE_CHUNK_SIZE);
        while (n_chunks--)
        {
            this->Transaction([this,
                               reconstruction_id, image_id, &ordered_desc, &it](std::shared_ptr<sql::Connection> con) mutable {
                int n = STORE_CHUNK_SIZE;
                while (n-- && it != ordered_desc.end())
                {
                    auto e = *it;
                    auto descriptor = e.first;
                    unsigned int count = e.second;
                    std::string desc_string((char *)descriptor.data(), 128);
                    this->Execute(SQL_INCREMENT_FREQUENCY(this->_frequency_table),
                                  con,
                                  [this, reconstruction_id, desc_string, count](sql::PreparedStatement *pstmt) {
                                      pstmt->setString(1, reconstruction_id);
                                      pstmt->setString(2, desc_string);
                                  });
                    this->Execute(SQL_STORE_DESCRIPTOR(this->_table),
                                  con,
                                  [this, reconstruction_id, image_id, desc_string, count](sql::PreparedStatement *pstmt) {
                                      pstmt->setString(1, reconstruction_id);
                                      pstmt->setString(2, image_id);
                                      pstmt->setString(3, desc_string);
                                      pstmt->setInt64(4, count);
                                  });
                    std::advance(it, 1);
                }
            },
            sql::enum_transaction_isolation::TRANSACTION_READ_COMMITTED);
        }
    }

    SIFT_Descriptor_count_map
    GetGlobalDescriptorFrequencies(const std::string &reconstruction_id) override
    {
        auto connection_loan = this->GetConnection();
        SIFT_Descriptor_count_map map;
        sql::ResultSet *res = this->IssueQuery(SQL_GET_INDEXABLE_GLOBAL_DESCRIPTOR_FREQUENCY(this->_frequency_table), connection_loan.con,
                                               [reconstruction_id](sql::PreparedStatement *stmt) {
                                                   stmt->setString(1, reconstruction_id);
                                               });
        while (res->next())
        {
            SIFT_Descriptor descriptor;
            std::string desc_str = res->getString("WORD");
            memcpy((char *)descriptor.data(), desc_str.data(), 128);
            map[descriptor] = res->getInt("N");
            assert(map[descriptor] != 0);
        }
        delete res;
        return map;
    }

private:
    std::string _frequency_table;
};