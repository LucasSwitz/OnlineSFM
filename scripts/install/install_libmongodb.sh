#MONGO DB C++ http://mongocxx.org/mongocxx-v3/installation/

apt-get install -y python3
git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/v3.5 --depth 1
cd mongo-cxx-driver/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make && make install