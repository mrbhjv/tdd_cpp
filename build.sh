#!/bin/bash

mkdir -p build && cd build
cmake .. || exit 1
make || exit 2

# Name of executable (I'm lazy)
./run_tests