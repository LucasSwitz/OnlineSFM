git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP
cd AMQP-CPP
mkdir build
cd build
cmake .. -DAMQP-CPP_BUILD_SHARED=ON -DAMQP-CPP_LINUX_TCP=ON
sudo cmake --build . --target install