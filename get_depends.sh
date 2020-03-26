apt install -y git libprotobuf-dev protobuf-compiler-grpc libgrpc++-dev autoconf libmysqlcppconn-dev nlohmann-json-dev libmongoclient-dev

#openMVG  https://github.com/openMVG/openMVG/blob/master/BUILD.md#linux
apt-get install -y libpng-dev libjpeg-dev libtiff-dev libxxf86vm1 libxxf86vm-dev libxi-dev libxrandr-dev
git clone --recursive https://github.com/openMVG/openMVG.git
mkdir openMVG_Build && cd openMVG_Build
cmake -DCMAKE_BUILD_TYPE=RELEASE ../openMVG/src/
cmake --build . --target install

# Then need to build protobuf
apt-get install -y autoconf automake libtool curl make g++ unzip
git clone https://github.com/protocolbuffers/protobuf.git
cd protobuf
git submodule update --init --recursive
./autogen.sh
./configure
make
make install
ldconfig

# Need to build GRPC
apt-get install -y build-essential autoconf libtool pkg-config
git clone -b RELEASE_TAG_HERE https://github.com/grpc/grpc
cd grpc
git submodule update --init
mkdir -p cmake/build
cd cmake/build
cmake ../.. 
            -DBUILD_SHARED_LIBS=ON           \
            -DgRPC_INSTALL=ON                \
            -DCMAKE_BUILD_TYPE=Release       \
            -DgRPC_ABSL_PROVIDER=package     \
            -DgRPC_CARES_PROVIDER=package    \
            -DgRPC_PROTOBUF_PROVIDER=package \
            -DgRPC_SSL_PROVIDER=package      \
            -DgRPC_ZLIB_PROVIDER=package
make
make install

#MONGO DB C
wget https://github.com/mongodb/mongo-c-driver/releases/download/1.16.2/mongo-c-driver-1.16.2.tar.gz
tar xzf mongo-c-driver-1.16.2.tar.gz
cd mongo-c-driver-1.16.2
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
make install ..

cmake ..                                \
    -DCMAKE_BUILD_TYPE=Release          \
    -DCMAKE_INSTALL_PREFIX=/usr/local

#MONGO DB C++
http://mongocxx.org/mongocxx-v3/installation/

#boost 1.66
wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
tar -xf boost_1_66_0.tar.gz
cd boost_1_66_0
./bootstrap.sh
./b2 install

# MongoDB server  https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/
git clone https://github.com/mongodb/mongo-cxx-driver.git \
    --branch releases/stable --depth 1
cd mongo-cxx-driver/build
cmake ..                                \
    -DCMAKE_BUILD_TYPE=Release          \
    -DCMAKE_INSTALL_PREFIX=/usr/local
make && make install

#Redis
git clone https://github.com/redis/hiredis.git
cd hiredis
make
make install

#Prepare and empty machine for building:
apt-get update -qq -y && apt-get install -qq -y
apt-get -y install build-essential git mercurial cmake libpng-dev libjpeg-dev libtiff-dev libglu1-mesa-dev libxmu-dev libxi-dev
main_path=`pwd`

#Eigen (Required)
hg clone https://bitbucket.org/eigen/eigen#3.2
mkdir eigen_build && cd eigen_build
cmake . ../eigen
make && make install
cd ..

#Boost (Required)
apt-get -y install libboost-iostreams-dev libboost-program-options-dev libboost-system-dev libboost-serialization-dev

#OpenCV (Required)
apt-get -y install libopencv-dev

#CGAL (Required)
apt-get -y install libcgal-dev libcgal-qt5-dev

#VCGLib (Required)
git clone https://github.com/cdcseacave/VCG.git vcglib

#Ceres (Required)
apt-get -y install libatlas-base-dev libsuitesparse-dev
git clone https://ceres-solver.googlesource.com/ceres-solver ceres-solver
mkdir ceres_build && cd ceres_build
cmake . ../ceres-solver/ -DMINIGLOG=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF
make -j2 && make install
cd ..

#GLFW3 (Optional)
apt-get -y install freeglut3-dev libglew-dev libglfw3-dev

#OpenMVS
git clone https://github.com/cdcseacave/openMVS.git openMVS
mkdir openMVS_build && cd openMVS_build
cmake . ../openMVS -DCMAKE_BUILD_TYPE=Release -DVCG_ROOT="$main_path/vcglib"

#If you want to use OpenMVS as shared library, add to the CMake command:
-DBUILD_SHARED_LIBS=ON

#Install OpenMVS library (optional):
make -j2 && make install