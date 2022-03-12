#! /usr/bin/env bash
set -x

[[ -d build ]] || mkdir build -v

cd build

[[ -f Makefile ]] && make clean

cmake ..
make
./tcp_server "$@"
