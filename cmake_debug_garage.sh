export HOMEIO_SITE=debug_garage
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++-13 ..
make
