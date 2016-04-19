export HOMEIO_SITE=photo
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
make
