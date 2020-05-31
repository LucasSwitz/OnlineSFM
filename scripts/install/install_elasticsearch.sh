#https://github.com/seznam/elasticlient
sudo apt-get install libmicrohttpd-dev
git clone https://github.com/whoshuu/cpr
cd cpr
mkdir build
cd build
cmake ..
make 
make install
cd ../../ 

# httpmock server did not use the newest version of cpr that is updated to use the new curl. I manually pulled the new cpr.
# had to manually copy the cpr headers into /usr/include

git clone https://github.com/seznam/elasticlient
cd elasticlient
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
make install
pip3 install elasticsearch

#https://github.com/docker-library/elasticsearch/issues/111