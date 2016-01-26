export HOMEIO_SITE=wind
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
make
