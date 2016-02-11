export HOMEIO_SITE=wind
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
make
echo "gdb build/bin/homeio_wind"
