cmake_minimum_required(VERSION 3.10)
project(ReconstructionServer CXX)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)  
include_directories(common/proto/gen/cpp src /home/gumbo00/OnlineSFM/depends/openMVG/src)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(glog REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(OpenMVG REQUIRED)
file(GLOB SERVER_SOURCES
    src/server/*.cpp
    common/proto/gen/cpp/*.cc
    common/proto/gen/cpp/*.cpp
    )

add_executable(server ${SERVER_SOURCES})
set_property(TARGET server PROPERTY CXX_STANDARD 14)
target_link_libraries(server  ${Protobuf_LIBRARIES} ${GLOG_LIBRARIES} vlsift gRPC::grpc++ mysqlcppconn Boost::filesystem redis++ hiredis      
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