#!/bin/bash

mkdir -p build && cd build
cmake ..
make
# Name of executable (I'm lazy)
./run_tests