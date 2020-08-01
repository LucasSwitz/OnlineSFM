#pragma once
#include "sql_storage.h"
#include "descriptor_storage.h"
#include "types.h"
#include "util.h"
#include <glog/logging.h>

#define SQL_GET_DESCRIPTORS(t) "SELECT DESCRIPTOR, N FROM " + t + " WHERE RECONSTRUCTION_ID = ? AND IMAGE_ID = ?"
#define SQL_DELETE_DESCRIPTORS(t) "DELETE FROM " + t + " WHERE IMAGE_ID = ?"
#define SQL_STORE_DESCRIPTOR(t) "INSERT INTO " + t + " VALUES (?,?,?,?)"
#define SQL_GET_DESCRIPTOR_FREQUENCY(t) "SELECT DESCRIPTOR, SUM(N) as N FROM " + t + " WHERE DESCRIPTOR IN (?) GROUP BY DESCRIPTOR"
#define SQL_GET_ALL_WITH_DESCRIPTORS_COMMON(t) "SELECT T1.IMAGE_ID FROM " + t + " as T1 WHERE T1.RECONSTRUCTION_ID = ? AND T1.IMAGE_ID != ? AND exists(SELECT * FROM IMAGE_WORDS as T2 WHERE T2.IMAGE_ID = ? AND T1.DESCRIPTOR = T2.DESCRIPTOR)"
#define SQL_GET_UNIQUE_IMAGE_COUNT(t) "SELECT COUNT(DISTINCT IMAGE_ID) as TOTAL FROM " + t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_GET_GLOBAL_DESCRIPTOR_FREQUENCIES(t) "SELECT DESCRIPTOR, SUM(N) as N FROM " + t + " WHERE RECONSTRUCTION_ID = ? GROUP BY DESCRIPTOR"
#define SQL_GET_ALL_WITH_DESCRIPTORS_COMMON_PAGED(t) "SELECT T2.IMAGE_ID, T2.DESCRIPTOR, T2.N FROM " + t + " as T1, " + t + " as T2 WHERE T1.IMAGE_ID = (?) AND T1.DESCRIPTOR > ? AND T1.IMAGE_ID != T2.IMAGE_ID AND T1.RECONSTRUCTION_ID = T2.RECONSTRUCTION_ID AND T1.DESCRIPTOR = T2.DESCRIPTOR ORDER BY T2.DESCRIPTOR ASC LIMIT ?"
class SQLDescriptorStorage : public SQLStorage,
                             public DescriptorStorage<SIFT_Descriptor>
{
public:
    SQLDescriptorStorage(
        const std::string &table) : _table(table)
    {
    }

    virtual void Store(const std::string &reconstruction_id, const std::string &image_id, SIFT_Descriptor_count_map &descriptors)
    {
        this->Transaction([this,
                           reconstruction_id, image_id, descriptors](std::shared_ptr<sql::Connection> con) {
            for (auto e : descriptors)
            {
                auto descriptor = e.first;
                unsigned int count = e.second;
                std::string desc_string((char *)descriptor.data(), 128);
                int len = desc_string.size();
                this->Execute(SQL_STORE_DESCRIPTOR(this->_table),
                              con,
                              [this, reconstruction_id, image_id, desc_string, count](sql::PreparedStatement *pstmt) {
                                  pstmt->setString(1, reconstruction_id);
                                  pstmt->setString(2, image_id);
                                  pstmt->setString(3, desc_string);
                                  pstmt->setInt64(4, count);
                              });
            }
        });
    }

    virtual SIFT_Descriptor_count_map GetAllDescriptors(const std::string &reconstruction_id, const std::string &image_id)
    {
        auto connection_loan = this->GetConnection();
        SIFT_Descriptor_count_map descs;
        sql::ResultSet *res = this->IssueQuery(SQL_GET_DESCRIPTORS(this->_table), connection_loan.con,
                                               [image_id, reconstruction_id](sql::PreparedStatement *stmt) {
                                                   stmt->setString(1, reconstruction_id);
                                                   stmt->setString(2, image_id);
                                               });
        while (res->next())
        {
            SIFT_Descriptor descriptor;
            std::string desc_str = res->getString("DESCRIPTOR");
            memcpy((char *)descriptor.data(), desc_str.data(), 128);
            descs[descriptor] = res->getUInt64("N");
        }
        delete res;
        return descs;
    }

    virtual std::set<std::string> GetAllWithCommonDescriptors(const std::string &reconstruction_id,
                                                              const std::string &image_id,
                                                              int page_size,
                                                              std::string *page_start)
    {
        auto connection_loan = this->GetConnection();
        std::set<std::string> set;

        sql::ResultSet *res = nullptr;

        if (!page_size)
        {
            res = this->IssueQuery(SQL_GET_ALL_WITH_DESCRIPTORS_COMMON(this->_table), connection_loan.con,
                                   [reconstruction_id, image_id](sql::PreparedStatement *stmt) {
                                       stmt->setString(1, reconstruction_id);
                                       stmt->setString(2, image_id);
                                       stmt->setString(3, image_id);
                                   });
        }
        else if (!page_start)
        {
            throw std::runtime_error("You must provide a page start");
        }
        else
        {
            res = this->IssueQuery(SQL_GET_ALL_WITH_DESCRIPTORS_COMMON_PAGED(this->_table), connection_loan.con,
                                   [image_id, page_start, page_size](sql::PreparedStatement *stmt) {
                                       std::string desc_string((char *)page_start->data(), 128);
                                       stmt->setString(1, image_id);
                                       stmt->setString(2, desc_string);
                                       stmt->setInt(3, page_size);
                                   });
        }
        if (!res)
        {
            LOG(ERROR) << "Failed to retrieve results";
            return set;
        }

        while (res->next())
        {
            std::string image_id = res->getString("IMAGE_ID");
            set.insert(image_id);
        }
        delete res;
        return set;
    }

    virtual unsigned int GetImageCount(const std::string &reconstruction_id)
    {
        auto connection_loan = this->GetConnection();
        int total;
        sql::ResultSet *res = this->IssueQuery(SQL_GET_UNIQUE_IMAGE_COUNT(this->_table), connection_loan.con,
                                               [reconstruction_id](sql::PreparedStatement *stmt) {
                                                   stmt->setString(1, reconstruction_id);
                                               });
        if (res->next())
        {
            total = res->getUInt64("TOTAL");
        }
        delete res;
        return total;
    }

    virtual std::shared_ptr<SIFT_Descriptor_count_map> GetGlobalDescriptorFrequencies(const std::string &reconstruction_id)
    {
        auto connection_loan = this->GetConnection();
        std::shared_ptr<SIFT_Descriptor_count_map> map = std::make_shared<SIFT_Descriptor_count_map>();
        sql::ResultSet *res = this->IssueQuery(SQL_GET_GLOBAL_DESCRIPTOR_FREQUENCIES(this->_table), connection_loan.con,
                                               [reconstruction_id](sql::PreparedStatement *stmt) {
                                                   stmt->setString(1, reconstruction_id);
                                               });
        while (res->next())
        {
            SIFT_Descriptor descriptor;
            std::string desc_str = res->getString("DESCRIPTOR");
            memcpy((char *)descriptor.data(), desc_str.data(), 128);
            (*map)[descriptor] = res->getInt("N");
        }
        delete res;
        return map;
    }

    virtual void DeleteAllDescriptors(const std::string &image_id)
    {
    }

protected:
    std::string _table;
};