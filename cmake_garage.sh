export HOMEIO_SITE=garage
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++-13 ..
make
