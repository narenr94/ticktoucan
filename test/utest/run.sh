#setup file structure
if [ ! -d "./build" ]; then
    mkdir build
fi
cd build

if [ ! -d "./libs" ]; then
    mkdir libs
fi

cd libs

if [ ! -d "./lib" ]; then
    mkdir lib
fi

cd lib

if [ ! -d "./pkgconfig" ]; then
    mkdir pkgconfig
fi

cd ..

if [ ! -d "./include" ]; then
    mkdir include
fi

declare LOCAL_DEPS_BUILD_DIR
LOCAL_DEPS_BUILD_DIR=$(pwd)

#setup gtest
if [ -d "./googletest" ]; then
    echo "googletest directory already exists!!!"
elif [ -d "../../../../../googletest" ]; then
    echo "local googletest found ... copying ..."
    cp -r ../../../../../googletest ./
else
    echo "Installing googletest..."
    git clone https://github.com/google/googletest
fi

cd ./googletest
echo "Building googletest"

mkdir ./build
cd build
env PKG_CONFIG_PATH=${LOCAL_DEPS_BUILD_DIR}/lib/pkgconfig cmake .. -DCMAKE_INSTALL_PREFIX=${LOCAL_DEPS_BUILD_DIR}
make
make install

cd ../../../

#make/build tests
PKG_CONFIG_PATH=${LOCAL_DEPS_BUILD_DIR}/lib/pkgconfig cmake .. -DCMAKE_INSTALL_PREFIX=${LOCAL_DEPS_BUILD_DIR} -DCMAKE_LIBRARY_PATH=${LOCAL_DEPS_BUILD_DIR}/lib
make

#run tests
cmake_version=$(cmake --version | head -n 1 | awk '{print $3}')
major_version=$(echo "$cmake_version" | cut -d. -f1)
minor_version=$(echo "$cmake_version" | cut -d. -f2)
if [[ "$major_version" -gt 3 ]] || [[ "$major_version" -eq 3 && "$minor_version" -ge 21 ]]; then
  CT_TESTDIR="" 
else
  CT_TESTDIR="--testdir build" 
    
fi

ctest -V -j 4 --output-on-failure --no-compress-output -T Test $CT_TESTDIR --output-junit ctest-results.xml
