export HOMEIO_SITE=wind
[ -d build ] || mkdir build
cd build
cmake ..
make
