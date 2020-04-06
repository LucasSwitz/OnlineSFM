#pragma once
#include "sql_storage.h"
#include "descriptor_storage.h"
#include "types.h"
#include "util.h"

#define SQL_GET_DESCRIPTORS(t) "SELECT DESCRIPTOR, COUNT(*) FROM " + t + " WHERE IMAGE_ID = ? GROUP BY DESCRIPTOR"
#define SQL_DELETE_DESCRIPTORS(t) "DELETE FROM "+ t + " WHERE IMAGE_ID = ?"
#define SQL_STORE_DESCRIPTOR(t) "INSERT INTO " + t +  " VALUES (?,?)"
#define SQL_GET_ALL_WITH_DESCRIPTOR(t) "SELECT DISTINCT IMAGE_ID FROM " + t +" WHERE DESCRIPTOR = ?"
#define SQL_GET_DESCRIPTOR_FREQUENCY(t) "SELECT DESCRIPTOR, COUNT(*) FROM " + t + " WHERE DESCRIPTOR IN (?) GROUP BY DESCRIPTOR"

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

        SIFT_Vector GetAllDescriptors(const std::string& image_id){
            sql::ResultSet* res = this->IssueQuery(SQL_GET_DESCRIPTORS(this->_table), 
                [image_id](sql::PreparedStatement *stmt){
                    stmt->setString(1, image_id);
                });
            SIFT_Vector descriptors;
            while(res->next()){
                SIFT_Descriptor descriptor;
                std::string desc_str = res->getString("DESCRIPTOR");
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                descriptors.push_back(descriptor);
            }
            return descriptors;
        }

        std::vector<std::string> GetImagesWithDescriptor(const SIFT_Descriptor& desc){

        }

        std::vector<unsigned long> GetDescriptorFrequencies(const std::vector<SIFT_Descriptor> desc){

        }

        void DeleteAllDescriptors(const std::string& image_id){
            
        }
    private:
        std::string _table;
};