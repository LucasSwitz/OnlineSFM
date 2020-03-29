#openMVG  https://github.com/openMVG/openMVG/blob/master/BUILD.md#linux
apt-get install -y libpng-dev libjpeg-dev libtiff-dev libxxf86vm1 libxxf86vm-dev libxi-dev libxrandr-dev
git clone --recursive https://github.com/openMVG/openMVG.git
mkdir openMVG_Build && cd openMVG_Build
cmake -DCMAKE_BUILD_TYPE=RELEASE ../openMVG/src/
cmake --build . --target install