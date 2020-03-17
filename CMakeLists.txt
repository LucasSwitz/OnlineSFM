cmake_minimum_required(VERSION 3.10)
project(ReconstructionServer CXX)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)  
include_directories(common/proto/gen/cpp src)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(glog REQUIRED)
find_package(gRPC CONFIG REQUIRED)

file(GLOB SERVER_SOURCES
    src/server/*.cpp
    common/proto/gen/cpp/*.cc
    common/proto/gen/cpp/*.cpp
    )

add_executable(server ${SERVER_SOURCES})
target_link_libraries(server ${Protobuf_LIBRARIES} ${GLOG_LIBRARIES} gRPC::grpc++ mysqlcppconn Boost::filesystem redis++ hiredis)