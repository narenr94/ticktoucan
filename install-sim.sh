if [ ! -d "./build" ]; then
    mkdir build
fi

cd build


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

if [ ! -d "./tt_platform" ]; then
    mkdir tt_platform
fi

cd tt_platform

PKG_CONFIG_PATH=${LOCAL_DEPS_BUILD_DIR}/lib/pkgconfig cmake ../../sim/ -DCMAKE_INSTALL_PREFIX=${LOCAL_DEPS_BUILD_DIR}
make
make install

echo -e 'prefix='$LOCAL_DEPS_BUILD_DIR'/lib \nexec_prefix='$LOCAL_DEPS_BUILD_DIR' \nlibdir='$LOCAL_DEPS_BUILD_DIR'/lib \nincludedir='$LOCAL_DEPS_BUILD_DIR'/include \n \nName: ticktoucanplatform \nDescription: platform implementation of tick toucan \nVersion: 1.0 \nLibs: -L${libdir} -lticktoucanplatform \nCflags: -I${includedir}' > $LOCAL_DEPS_BUILD_DIR/lib/pkgconfig/libticktoucanplatform.pc

cd ..

if [ ! -d "./tt_middleware" ]; then
    mkdir tt_middleware
fi

cd tt_middleware

PKG_CONFIG_PATH=${LOCAL_DEPS_BUILD_DIR}/lib/pkgconfig cmake ../../ -DCMAKE_INSTALL_PREFIX=${LOCAL_DEPS_BUILD_DIR}
make
make install



