#!/bin/bash

set -e

FLAG=$1
ARGS=$#
QT_VERSION="6.8.3"
COMPILER="wasm_singlethread"

if [ $ARGS -eq 1 ]; then
    if [ $FLAG == "-c" ]; then
        rm -rf build-wasm
    fi
fi

source ~/.emsdk/emsdk_env.sh

cmake -S . -B build-wasm \
  -DCMAKE_TOOLCHAIN_FILE=~/Qt/$QT_VERSION/$COMPILER/lib/cmake/Qt6/qt.toolchain.cmake \
  -DCMAKE_PREFIX_PATH=~/Qt/$QT_VERSION/$COMPILER

cd build-wasm && cmake --build .