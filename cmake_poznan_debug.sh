export HOMEIO_SITE=poznan
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
make
echo "gdb bin/homeio_poznan"
