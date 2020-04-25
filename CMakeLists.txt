cmake_minimum_required(VERSION 3.10)
project(ReconstructionServer CXX)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)  
include_directories(common/proto/gen/cpp 
                    depends/openMVS/openMVS_build
                    depends/openMVG/src/software/SfM/export/
                    depends/
                    src/core
                    src/gateway
                    src/index/src
                    src/worker
                    src/worker_manager
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

file(GLOB CORE_SOURCES
    common/proto/gen/cpp/*.cc
    common/proto/gen/cpp/*.cpp
    src/core/*.cpp
)

file(GLOB GATEWAY_SOURCES
    src/gateway/*.cpp
)

file(GLOB UPLOAD_INTRINISICS_SOURCES
    tools/upload_intrinsics_db_to_mysql.cpp
)

file(GLOB BUILD_INDEX_SOURCES
    src/index/src/*.cpp
    src/index/tools/generate_index.cpp
)

file(GLOB INDEX_SERVER_SOURCES
    src/index/src/*.cpp
    src/index/server/*.cpp
)

file(GLOB STORAGE_SOURCES
    src/storage/*.cpp
)

file(GLOB WORKER_POOL_MANAGER_SOURCES
    src/worker_manager/*.cpp
)

file(GLOB WORKER_SOURCES
    src/worker/src/*.cpp
)

add_library(core ${CORE_SOURCES})
set_property(TARGET core PROPERTY CXX_STANDARD 14)
target_link_libraries(core 
                     mongocxx 
                     bsoncxx
                     stlplus3
                     ${Protobuf_LIBRARIES} 
                     vlsift 
                     gRPC::grpc++
                     glog
                     mysqlcppconn Boost::filesystem 
                     redis++ 
                     hiredis      
                     OpenMVG::openMVG_image
                     OpenMVG::openMVG_features
)

add_executable(gateway ${GATEWAY_SOURCES})
set_property(TARGET gateway PROPERTY CXX_STANDARD 14)
target_link_libraries(gateway core)

add_executable(upload_intrinsics_tool ${UPLOAD_INTRINISICS_SOURCES})
set_property(TARGET upload_intrinsics_tool PROPERTY CXX_STANDARD 14)
target_link_libraries(upload_intrinsics_tool core)

add_executable(build_index ${BUILD_INDEX_SOURCES})
set_property(TARGET build_index PROPERTY CXX_STANDARD 14)
target_link_libraries(build_index 
                      core
                      lz4
)

add_executable(indexing_server ${INDEX_SERVER_SOURCES})
set_property(TARGET indexing_server PROPERTY CXX_STANDARD 14)
target_link_libraries(indexing_server glog
                      core
                      lz4
)

add_executable(storage_server ${STORAGE_SOURCES})
set_property(TARGET storage_server PROPERTY CXX_STANDARD 14)
target_link_libraries(storage_server
                      core
)

add_executable(worker_manager ${WORKER_POOL_MANAGER_SOURCES})
set_property(TARGET worker_manager PROPERTY CXX_STANDARD 14)
target_link_libraries(worker_manager
                      core
)

add_executable(worker ${WORKER_SOURCES})
set_property(TARGET worker PROPERTY CXX_STANDARD 14)
target_link_libraries(worker
                      core
                      OpenMVG::openMVG_linearProgramming
                      OpenMVG::openMVG_matching
                      OpenMVG::openMVG_matching_image_collection
                      OpenMVG::openMVG_multiview
                      OpenMVG::openMVG_numeric
                      OpenMVG::openMVG_robust_estimation
                      OpenMVG::openMVG_sfm
                      OpenMVG::openMVG_system
                      OpenMVG::openMVG_camera
                      OpenMVG::openMVG_exif
                      OpenMVG::openMVG_features
                      OpenMVG::openMVG_geodesy
                      OpenMVG::openMVG_geometry
                      OpenMVG::openMVG_graph
)