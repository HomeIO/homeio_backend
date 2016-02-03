export HOMEIO_SITE=poznan
[ -d build ] || mkdir build
cd build
echo `pwd`
cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ..
make
