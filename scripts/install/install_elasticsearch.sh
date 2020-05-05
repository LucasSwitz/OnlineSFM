#https://github.com/seznam/elasticlient
git clone https://github.com/seznam/elasticlient
cd elasticlient
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
pip3 install elasticsearch