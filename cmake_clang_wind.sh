#export CC=/usr/bin/clang
#export CXX=/usr/bin/clang++

export HOMEIO_SITE=wind
[ -d build ] || mkdir build
cd build
echo `pwd`
cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ..
make
