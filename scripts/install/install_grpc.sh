apt-get install -y build-essential autoconf libtool pkg-config libssl-dev libc-ares-dev python3-pip
python3 -m pip install grpcio_tools

git clone -b lts_2020_02_25 https://github.com/abseil/abseil-cpp.git
cd abseil-cpp
mkdir build && cd build
cmake ..
make 
make install

mkdir grpc_dl
cd grpc_dl
git clone -b v1.27.x https://github.com/grpc/grpc
cd grpc
git submodule update --init
mkdir -p cmake/build
cd cmake/build
cmake ../.. -DgRPC_BUILD_CSHARP_EXT=OFF -DgRPC_INSTALL=ON -DCMAKE_BUILD_TYPE=Release -DgRPC_ABSL_PROVIDER=package -DgRPC_CARES_PROVIDER=package  -DgRPC_PROTOBUF_PROVIDER=package  -DgRPC_SSL_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package
make
make install