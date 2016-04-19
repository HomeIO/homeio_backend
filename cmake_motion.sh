export HOMEIO_SITE=motion
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
make
