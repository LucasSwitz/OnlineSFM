cmake_minimum_required(VERSION 3.10)
project(ReconstructionServer CXX)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)  
include_directories(common/proto/gen/cpp 
                    src
                    /usr/local/include/openMVG_dependencies
                    /usr/local/include/mongocxx/v_noabi
                    /usr/local/include/libmongoc-1.0
                    /usr/local/include/openMVG
                    /usr/local/include/bsoncxx/v_noabi
                    /usr/local/include/libbson-1.0)
find_package(Protobuf REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(glog REQUIRED)
find_package(gRPC REQUIRED)
find_package(OpenMVG REQUIRED)
file(GLOB SERVER_SOURCES
    src/server/*.cpp
    common/proto/gen/cpp/*.cc
    common/proto/gen/cpp/*.cpp
)

file(GLOB UPLOAD_INTRINISICS_SOURCES
    tools/upload_intrinsics_db_to_mysql.cpp
    src/server/sql_camera_intrinsics_storage.cpp
    src/server/sql_storage.cpp
    common/proto/gen/cpp/*.cc
    common/proto/gen/cpp/*.cpp
)

add_executable(server ${SERVER_SOURCES})
set_property(TARGET server PROPERTY CXX_STANDARD 14)
target_link_libraries(server 
                      mongocxx 
                      bsoncxx 
                      ${Protobuf_LIBRARIES} 
                      vlsift 
                      gRPC::grpc++
                      glog
                      mysqlcppconn Boost::filesystem 
                      redis++ 
                      hiredis      
                      OpenMVG::openMVG_camera
                      OpenMVG::openMVG_exif
                      OpenMVG::openMVG_features
                      OpenMVG::openMVG_geodesy
                      OpenMVG::openMVG_geometry
                      OpenMVG::openMVG_graph
                      OpenMVG::openMVG_image
                      OpenMVG::openMVG_linearProgramming
                      OpenMVG::openMVG_matching
                      OpenMVG::openMVG_matching_image_collection
                      OpenMVG::openMVG_multiview
                      OpenMVG::openMVG_numeric
                      OpenMVG::openMVG_robust_estimation
                      OpenMVG::openMVG_sfm
                      OpenMVG::openMVG_system)

add_executable(upload_intrinsics_tool ${UPLOAD_INTRINISICS_SOURCES})
set_property(TARGET upload_intrinsics_tool PROPERTY CXX_STANDARD 14)
target_link_libraries(upload_intrinsics_tool gRPC::grpc++ glog ${Protobuf_LIBRARIES} mysqlcppconn redis++ hiredis
    OpenMVG::openMVG_exif
)