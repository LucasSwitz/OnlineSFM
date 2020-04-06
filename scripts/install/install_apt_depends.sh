export DEBIAN_FRONTEND=noninteractive
apt install -y git wget cmake libprotobuf-dev protobuf-compiler-grpc liblz4-dev libgrpc++-dev autoconf libmysqlcppconn-dev nlohmann-json-dev libmongoclient-dev tzdata libgoogle-glog-dev
ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime
dpkg-reconfigure --frontend noninteractive tzdata