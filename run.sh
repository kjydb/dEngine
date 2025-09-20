#!/usr/bin/env bash

# ./build/samples/samples

set -e
SCRIPT_DIR=$(dirname ${BASH_SOURCE})

DEBUG=false
for arg in "$@"; do
  if [ "$arg" == "--debug" ]; then
    DEBUG=true
    break
  fi
done

if [ "$DEBUG" == "true" ]; then
  gdb $SCRIPT_DIR/build/samples/samples
else
  $SCRIPT_DIR/build/samples/samples
fi
