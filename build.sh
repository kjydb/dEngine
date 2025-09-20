#!/usr/bin/env bash

# rm -rf build
# mkdir -p build
# cd build
# cmake .. -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build .

set -e
SCRIPT_DIR=$(dirname ${BASH_SOURCE})

INITIAL_BUILD=false
for arg in "$@"; do
  if [ "$arg" == "--initial" ]; then
    INITIAL_BUILD=true
    break
  fi
done

if [ "$INITIAL_BUILD" == "true" ]; then
  rm -rf $SCRIPT_DIR/build
  mkdir $SCRIPT_DIR/build
fi

cd $SCRIPT_DIR/build
cmake .. -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
