export HOMEIO_SITE=poznan
[ -d build ] || mkdir build
cd build
cmake ..
make
