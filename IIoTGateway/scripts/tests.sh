#!/bin/bash

FLAG=$1
ARGS=$#

bash build.sh amd64

if [ $ARGS -eq 1 ]; then
    if [ $FLAG == "-m"  ]; then
        valgrind ./build-amd64/Tests/Runner
    fi
else
    ./build-amd64/Tests/Runner
fi

