#pragma once

#include "regions_storage.h"
#include "sql_storage.h"
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer_io.hpp"
#include <cereal/archives/binary.hpp>

#define SQL_INSERT_REGION(t) "INSERT INTO " + t + " VALUES (?,?,?,?)"
#define SQL_GET_ALL_REGIONS_RECONSTRUCTION(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_GET_ALL_REGIONS_IMAGE(t) "SELECT * FROM " + t + "WHERE RECONSTRUCTION_ID = ? AND IMAGE_ID = ?"
#define SQL_GET_ALL_DESCRIPTORS(t) "SELECT DESCRIPTOR FROM " + t + " WHERE RECONSTRUCTION_ID = ? AND IMAGE_ID = ?"

template<typename T>
class SQLRegionsStorage : public SQLStorage, public RegionsStorage<T> {
    public:

        typedef typename T::DescriptorT DescriptorT;
        typedef typename T::FeatureT FeatureT; 
        typedef typename T::DescsT DescsT;
        typedef typename RegionsStorage<T>::image_region_map image_region_map;
        
        SQLRegionsStorage(
                       sql::Driver* driver, 
                       std::shared_ptr<sql::Connection> connection,
                       const std::string& table) : SQLStorage(driver, connection), _table(table){}

    
        void Store(const std::string& reconstruction_id, 
               const std::string& image_id, 
               std::shared_ptr<T> regions){
            std::vector<std::istringstream*> ifeature_stream_list(regions->RegionCount());
            for(int i = 0; i < regions->RegionCount(); i++){
                std::ostringstream ostream;
                auto feature = regions->Features()[i];
                {
                    cereal::BinaryOutputArchive archive_out(ostream);
                    archive_out(feature);
                }
                ifeature_stream_list[i] = new std::istringstream(ostream.str());
            }
            this->Transaction([this, 
                               reconstruction_id, 
                               image_id, 
                               ifeature_stream_list,
                               regions](std::shared_ptr<sql::Connection> con)mutable{
                for(int i = 0; i < regions->RegionCount(); i++){
                    DescriptorT descriptor;
                    memcpy(&descriptor, regions->DescriptorRawData() + i*sizeof(DescriptorT), sizeof(DescriptorT));
                    std::string desc_string((char*) descriptor.data(), 128);
                    this->Execute(SQL_INSERT_REGION(this->_table),
                        con, 
                        [this, i, reconstruction_id, image_id, desc_string, ifeature_stream_list](sql::PreparedStatement* pstmt) mutable{
                            pstmt->setString(1, reconstruction_id);
                            pstmt->setString(2, image_id);
                            pstmt->setString(3, desc_string);
                            pstmt->setBlob(4, ifeature_stream_list[i]);
                    });
                }
            });
        }

        std::shared_ptr<T> GetAllRegions(const std::string& reconstruction_id, 
                        const std::string& image_id){
            std::shared_ptr<T> regions = std::make_shared<T>();
            sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_REGIONS_IMAGE(this->_table), 
                [reconstruction_id, image_id](sql::PreparedStatement *stmt){
                    stmt->setString(1, reconstruction_id);
                    stmt->setString(2, image_id);
                });
            while(res->next()){
                DescriptorT descriptor;
                FeatureT feature;
                std::string desc_str = res->getString("DESCRIPTOR");
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                std::stringstream is(res->getString("FEATURE"));
                {
                    cereal::BinaryInputArchive archive_in(is);
                    archive_in(feature);
                }
                regions->Descriptors().push_back(descriptor);
                regions->Features().push_back(feature);
            }
            delete res;
            return regions;
        }

        image_region_map GetAllRegionsReconstruction(const std::string& reconstruction_id){
            image_region_map regions_map;
            sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_REGIONS_RECONSTRUCTION(this->_table), 
                [reconstruction_id](sql::PreparedStatement *stmt){
                    stmt->setString(1, reconstruction_id);
                });
            while(res->next()){
                DescriptorT descriptor;
                FeatureT feature;
                std::string desc_str = res->getString("DESCRIPTOR");
                std::string image_id = res->getString("IMAGE_ID");
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                std::stringstream is(res->getString("FEATURE"));
                {
                    cereal::BinaryInputArchive archive_in(is);
                    archive_in(feature);
                }
                if(regions_map.find(image_id) == regions_map.end()){
                    regions_map[image_id] = std::make_shared<T>();
                }
                regions_map[image_id]->Descriptors().push_back(descriptor);
                regions_map[image_id]->Features().push_back(feature);
            }
            delete res;
            return regions_map;
        }

       DescsT GetAllDescriptors(const std::string& reconstruction_id, const std::string& image_id){
            DescsT descriptors;
            sql::ResultSet* res = this->IssueQuery(SQL_GET_ALL_DESCRIPTORS(this->_table), 
                [reconstruction_id, image_id](sql::PreparedStatement *stmt){
                    stmt->setString(1, reconstruction_id);
                    stmt->setString(2, image_id);
                });
            while(res->next()){
                DescriptorT descriptor;
                FeatureT feature;
                std::string desc_str = res->getString("DESCRIPTOR");
                memcpy((char *)descriptor.data(), desc_str.data(), 128);
                descriptors.push_back(descriptor);
            }
            delete res;
            return descriptors;
        }

    private:
        std::string _table;
};