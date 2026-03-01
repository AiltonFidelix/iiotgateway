#!/bin/bash

export DB_TYPE="QSQLITE"
export DB_NAME="/opt/iiotgateway/iiotgateway.db"
export PLATFORM="raspberry"

export LD_LIBRARY_PATH="$(dirname "$0")/lib"
exec "$(dirname "$0")/iiotgateway" "$@"