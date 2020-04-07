#pragma once
#include "sql_storage.h"
#include "descriptor_storage.h"
#include "types.h"
#include "util.h"

#define SQL_GET_DESCRIPTORS(t) "SELECT DESCRIPTOR, COUNT(*) as TOTAL FROM " + t + " WHERE IMAGE_ID = ? GROUP BY DESCRIPTOR"
#define SQL_DELETE_DESCRIPTORS(t) "DELETE FROM "+ t + " WHERE IMAGE_ID = ?"
#define SQL_STORE_DESCRIPTOR(t) "INSERT INTO " + t +  " VALUES (?,?)"
#define SQL_GET_ALL_WITH_DESCRIPTORS(t) "SELECT DISTINCT IMAGE_ID FROM " + t +" WHERE DESCRIPTOR IN (?)"
#define SQL_GET_DESCRIPTOR_FREQUENCY(t) "SELECT DESCRIPTOR, COUNT(*) as TOTAL FROM " + t + " WHERE DESCRIPTOR IN (?) GROUP BY DESCRIPTOR"
#define SQL_GET_ALL_WITH_DESCRIPTORS_COMMON(t) "SELECT IMAGE_ID, DESCRIPTOR, COUNT(*) as TOTAL FROM " + t +" as T1 WHERE EXISTS (SELECT * FROM " + t + " as T2 WHERE T2.IMAGE_ID = (?) AND T1.IMAGE_ID <> T2.IMAGE_ID AND T1.DESCRIPTOR = T2.DESCRIPTOR) GROUP BY (T1.IMAGE_ID, T1.DESCRIPTOR)"
#define SQL_GET_UNIQUE_IMAGE_COUNT(t) "SELECT COUNT(DISTINCT IMAGE_ID) as TOTAL FROM " + t
#define SQL_GET_GLOBAL_DESCRIPTOR_FREQUENCIES_BY_IMAGE(t) "SELECT DESCRIPTOR, COUNT(*) as TOTAL FROM " + t + " as T1 WHERE EXISTS (SELECT * FROM " + t +  "as T2 WHERE T2.IMAGE_ID = (?) AND T2.DESCRIPTOR = T1.DESCRIPTOR) GROUP_BY T1.DESCRIPTOR"

class SQLDescriptorStorage : public SQLStorage, 
                             public DescriptorStorage<SIFT_Descriptor> {
    public:
        SQLDescriptorStorage(
                   const std::string& address, 
                   const std::string& user, 
                   const std::string& pass,
                   const std::string& db,
                   const std::string& table) : SQLStorage(address, user, pass, db), 
                                               _table(table){
                    
        }

        void StoreAll(const std::string& image_id, const SIFT_Vector& descriptors){
            this->Transaction([this, 
                     image_id, descriptors](sql::Connection* con){
                for(auto descriptor : descriptors){
                    std::string desc_string((char*) descriptor.data(), 128);
                    int len = desc_string.size();
                    this->Execute(SQL_STORE_DESCRIPTOR(this->_table),
                        con, 
                        [this, image_id, desc_string](sql::PreparedStatement* pstmt){
                            pstmt->setString(1, image_id);
                            pstmt->setString(2, desc_string);
                        });
                }
            });
        }

        void Store(const std::string& image_id, SIFT_Descriptor descriptor){
            std::string desc_string((char*) descriptor.data(), 128);
            this->IssueUpdate(SQL_STORE_DESCRIPTOR(this->_table), 
                [this, image_id, desc_string](sql::PreparedStatement* pstmt){
                pstmt->setString(1, image_id);
                pstmt->setString(2, desc_string);
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
                descs[descriptor] = res->getUInt64("TOTAL");
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
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                map[descriptor] = res->getUInt64("TOTAL");
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
            while(res->next()){
                std::string image_id = res->getString("IMAGE_ID");
                if(map.find(image_id) == map.end()){
                    map[image_id] = SIFT_Descriptor_count_map();
                }
                SIFT_Descriptor descriptor;
                std::string desc_str = res->getString("DESCRIPTOR");
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                map[image_id][descriptor] = res->getUInt64("TOTAL");
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
                map[descriptor] = res->getUInt64("TOTAL");
            }
            delete res;
            return map;
        }


        void DeleteAllDescriptors(const std::string& image_id){
            
        }
    private:
        std::string _table;
};