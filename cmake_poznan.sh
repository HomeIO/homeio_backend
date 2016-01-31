export HOMEIO_SITE=poznan
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
make
