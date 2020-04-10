#pragma once
#include "sql_storage.h"
#include "descriptor_storage.h"
#include "types.h"
#include "util.h"
#include <glog/logging.h>

#define SQL_GET_DESCRIPTORS(t) "SELECT DESCRIPTOR, N FROM " + t + " WHERE IMAGE_ID = ?"
#define SQL_DELETE_DESCRIPTORS(t) "DELETE FROM "+ t + " WHERE IMAGE_ID = ?"
#define SQL_STORE_DESCRIPTOR(t) "INSERT INTO " + t +  " VALUES (?,?,?,?)"
#define SQL_GET_ALL_WITH_DESCRIPTORS(t) "SELECT DISTINCT IMAGE_ID FROM " + t +" WHERE DESCRIPTOR IN (?)"
#define SQL_GET_DESCRIPTOR_FREQUENCY(t) "SELECT DESCRIPTOR, SUM(N) as N FROM " + t + " WHERE DESCRIPTOR IN (?) GROUP BY DESCRIPTOR"
#define SQL_GET_ALL_WITH_DESCRIPTORS_COMMON(t) "SELECT T2.IMAGE_ID, T2.DESCRIPTOR, T2.N FROM "+ t +" as T1, "+ t +" as T2 WHERE T1.IMAGE_ID = (?) AND T1.IMAGE_ID != T2.IMAGE_ID AND T1.RECONSTRUCTION_ID = T2.RECONSTRUCTION_ID AND T1.DESCRIPTOR = T2.DESCRIPTOR"
#define SQL_GET_UNIQUE_IMAGE_COUNT(t) "SELECT COUNT(DISTINCT IMAGE_ID) as TOTAL FROM " + t
#define SQL_GET_GLOBAL_DESCRIPTOR_FREQUENCIES_BY_IMAGE(t) "SELECT T2.DESCRIPTOR, SUM(T2.N) as N FROM "+ t +" as T1, "+ t +" as T2 WHERE T1.IMAGE_ID = (?) AND T1.DESCRIPTOR = T2.DESCRIPTOR GROUP BY T1.DESCRIPTOR"

class SQLDescriptorStorage : public SQLStorage, 
                             public DescriptorStorage<SIFT_Descriptor> {
    public:
        SQLDescriptorStorage(
                   sql::Driver* driver, 
                   std::shared_ptr<sql::Connection> con,
                   const std::string& table) : SQLStorage(driver, con), 
                                               _table(table){
                    
        }

        void Store(const std::string& reconstruction_id, const std::string& image_id, SIFT_Descriptor_count_map& descriptors){
            this->Transaction([this, 
                     reconstruction_id, image_id, descriptors](std::shared_ptr<sql::Connection> con){
                for(auto e : descriptors){
                    auto descriptor = e.first;
                    unsigned int count = e.second;
                    std::string desc_string((char*) descriptor.data(), 128);
                    int len = desc_string.size();
                    this->Execute(SQL_STORE_DESCRIPTOR(this->_table),
                        con, 
                        [this, reconstruction_id, image_id, desc_string, count](sql::PreparedStatement* pstmt){
                            pstmt->setString(1, reconstruction_id);
                            pstmt->setString(2, image_id);
                            pstmt->setString(3, desc_string);
                            pstmt->setInt64(4, count);
                        });
                }
            });
        }

        SIFT_Descriptor_count_map GetAllDescriptors(const std::string& image_id){
            SIFT_Descriptor_count_map descs;
            sql::ResultSet* res = this->IssueQuery(SQL_GET_DESCRIPTORS(this->_table), 
                [image_id](sql::PreparedStatement *stmt){
                    stmt->setString(1, image_id);
                });
            while(res->next()){
                SIFT_Descriptor descriptor;
                std::string desc_str = res->getString("DESCRIPTOR");
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                descs[descriptor] = res->getUInt64("N");
            }
            delete res;
            return descs;
        }

        std::vector<std::string> GetImagesWithDescriptors(const SIFT_Vector& desc){
            std::vector<std::string> image_ids;
            sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_WITH_DESCRIPTORS(this->_table), 
                [desc](sql::PreparedStatement *stmt){
                    stmt->setString(1, split_SIFT_Vector(desc));
                });
            while(res->next()){
                image_ids.push_back(res->getString("IMAGE_ID"));
            }
            delete res;
            return image_ids;
        }

        SIFT_Descriptor_count_map GetDescriptorFrequencies(const SIFT_Vector& desc){
            SIFT_Descriptor_count_map map;
            sql::ResultSet* res = this->IssueQuery(SQL_GET_DESCRIPTOR_FREQUENCY(this->_table), 
                [desc](sql::PreparedStatement *stmt){
                    stmt->setString(1, split_SIFT_Vector(desc));
                });
            while(res->next()){
                SIFT_Descriptor descriptor;
                std::string desc_str = res->getString("DESCRIPTOR");
                memcpy((char *)descriptor.data(), descriptor.data(), 128);
                map[descriptor] = res->getUInt64("N");
            }
            delete res;
            return map;
        }

        std::unordered_map<std::string, SIFT_Descriptor_count_map> GetAllWithCommonDescriptors(const std::string& image_id){
            std::unordered_map<std::string, SIFT_Descriptor_count_map> map;
            sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_WITH_DESCRIPTORS_COMMON(this->_table), 
                [image_id](sql::PreparedStatement *stmt){
                    stmt->setString(1, image_id);
                });
            if(!res){
                LOG(ERROR) << "Failed to retrieve results";
                return map;
            }
            while(res->next()){
                std::string image_id = res->getString("IMAGE_ID");
                if(map.find(image_id) == map.end()){
                    map[image_id] = SIFT_Descriptor_count_map();
                }
                SIFT_Descriptor descriptor;
                std::string desc_str = res->getString("DESCRIPTOR");
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                map[image_id][descriptor] = res->getUInt64("N");
            }
            delete res;
            return map;
        }

        unsigned int GetImageCount(){
            int total;
            sql::ResultSet* res = this->IssueQuery(SQL_GET_UNIQUE_IMAGE_COUNT(this->_table), 
                [](sql::PreparedStatement *stmt){});
            if(res->next()){
                total =  res->getUInt64("TOTAL");
            }
            delete res;
            return total;
        }

        SIFT_Descriptor_count_map GetGlobalDescriptorFrequenciesByImage(const std::string& image_id){
           SIFT_Descriptor_count_map map;
            sql::ResultSet* res = this->IssueQuery(SQL_GET_GLOBAL_DESCRIPTOR_FREQUENCIES_BY_IMAGE(this->_table), 
                [image_id](sql::PreparedStatement *stmt){
                    stmt->setString(1, image_id);
            });
            while(res->next()){
                SIFT_Descriptor descriptor;
                std::string desc_str = res->getString("DESCRIPTOR");
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                map[descriptor] = res->getUInt64("N");
            }
            delete res;
            return map;
        }


        void DeleteAllDescriptors(const std::string& image_id){
            
        }
    private:
        std::string _table;
};