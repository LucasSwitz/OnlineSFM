export DEBIAN_FRONTEND=noninteractive
apt install -y git wget cmake libprotobuf-dev protobuf-compiler-grpc liblz4-dev libgtest-dev libgrpc++-dev autoconf libmysqlcppconn-dev libgoogle-glog-dev libmongoclient-dev tzdata libgoogle-glog-dev
ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime
dpkg-reconfigure --frontend noninteractive tzdata