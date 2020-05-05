
#include <string>
#include "sql_camera_intrinsics_storage.h"
#include "openMVG/exif/sensor_width_database/ParseDatabase.hpp"
#include <cppconn/driver.h>
#include "config.h"

/**
 * ./upload_intrinsics_tool $OPENMVG_SENSOR_DB tcp://127.0.0.1:3306 root password  reconstruction CAMERA_INTRINSICS
 */
int main(int argc, char** argv){
    std::string path_to_db = argv[1];
    std::string address = argv[2];
    std::string user = argv[3];
    std::string pass = argv[4];
    std::string db = argv[5];
    std::string table = argv[6];
    json j;
    j["sql"] = json();
    j["sql"]["address"] = address;
    j["sql"]["user"] = user;
    j["sql"]["db"] = db;
    j["sql"]["password"] = pass;
    j["sql"]["instrinsics_table"] = table;
    ConfigManager::Instance()->Set(j);
    SQLStorage::InitConnectionPool(10);
    SQLCameraIntrinsicsStorage intrinsics_storage(table);

    std::vector<Datasheet> vec_database;
    std::cout << "Loading DB from: " << path_to_db;
    if (!parseDatabase(path_to_db, vec_database))
    {
      std::cerr
       << "\nInvalid input database: " << path_to_db
       << ", please specify a valid file." << std::endl;
      return 1;
    }
    for(Datasheet d : vec_database){
        CameraIntrinsics intrinsic;
        std::cout << "Uploading " << d.model_ << std::endl;
        intrinsic.set_model(d.model_);
        intrinsic.set_sensor_size(d.sensorSize_);
        intrinsics_storage.Store(intrinsic);
    }
    std::cout << "Added " << vec_database.size() << " sensors to DB";
    return 0;
}