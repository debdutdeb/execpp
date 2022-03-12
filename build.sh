#! /usr/bin/env bash
set -e
set -o pipefail

[[ -d build ]] || mkdir build -v

cd build

[[ -f Makefile ]] && make clean

cmake ..
make
./exec "$@"
