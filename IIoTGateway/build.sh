#!/bin/bash

ARCH=$1
ARCHS=("amd64" "aarch64")
QT_VERSION="6.8.3"
DOCKER_IMAGE="qt-$ARCH:$QT_VERSION"

options() {
    echo "Usage ./build.sh [OPTION]"
    echo "  Options: ${ARCHS[@]}"
    exit 0
}

if [ $# -eq 0 ]; then
    options
elif [[ ! ${ARCHS[@]} =~ $ARCH ]]; then
    options
fi

if [ $ARCH == "aarch64" ]; then
    rm -rf build-$ARCH && mkdir build-$ARCH 

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
    rm -rf build-$ARCH && mkdir build-$ARCH && cd build-$ARCH

    $HOME/Qt/$QT_VERSION/gcc_64/bin/qt-cmake ..
    cmake --build . --parallel 12
fi
