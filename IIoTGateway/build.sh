#!/bin/bash

ARCH=$1
FLAG=$2
ARGS=$#
ARCHS=("amd64" "aarch64")
QT_VERSION="6.8.3"
DOCKER_IMAGE="qt-$ARCH:$QT_VERSION"

verifyBuildfolder() {
    if [ $ARGS -eq 2 ]; then
        if [ $FLAG == "-c" ]; then
            rm -rf build-$ARCH
        fi
    fi

    if [ ! -d "build-$ARCH" ]; then
        mkdir build-$ARCH 
    fi
}

options() {
    echo "Usage ./build.sh [OPTION]"
    echo "  Options: ${ARCHS[@]}"
    exit 0
}

if [ $ARGS -eq 0 ]; then
    options
elif [[ ! ${ARCHS[@]} =~ $ARCH ]]; then
    options
fi

if [ $ARCH == "aarch64" ]; then
    verifyBuildfolder

    docker run --rm \
        -v $(pwd):/src \
        -w /src/build-$ARCH \
        $DOCKER_IMAGE \
        qt-cmake ..

    docker run --rm \
        -v $(pwd):/src \
        -w /src/build-$ARCH \
        $DOCKER_IMAGE \
        cmake --build . --parallel 12

elif [ $ARCH == "amd64" ]; then
    # TODO: create a docker image for amd64 build system and refactor script

    verifyBuildfolder
    cd build-$ARCH

    $HOME/Qt/$QT_VERSION/gcc_64/bin/qt-cmake ..
    cmake --build . --parallel 12
fi
