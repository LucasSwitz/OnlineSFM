#include "sql_camera_intrinsics_storage.h"

#define SQL_GET_CAMERA_INTRINSIC(t) "SELECT * FROM " + t + "WHERE MODEL = ?"
#define SQL_STORE_CAMERA_INTRINSIC(t) "INSERT INTO " + t + " VALUES (?,?,?,?)"
#define SQL_DELETE_CAMERA_INTRINSIC(t) "DELETE FROM " + t + " WHERE MODEL = ?"


SQLCameraIntrinsicsStorage::SQLCameraIntrinsicsStorage(const std::string& address, 
                                                       const std::string& user, 
                                                       const std::string& pass, 
                                                       const std::string& db,
                                                       const std::string& table) : SQLStorage(address, user, pass, db),
                                                                                    _table(table){
    
}

void SQLCameraIntrinsicsStorage::Store(const CameraIntrinsics& camera_instrinsics){

}

void SQLCameraIntrinsicsStorage::Delete(const std::string& model){

}

CameraIntrinsics SQLCameraIntrinsicsStorage::Get(const std::string& model){
    
}