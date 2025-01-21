#!/bin/bash

# Clean environment
rm -rf build install
mkdir build install
cd build
cmake ..
make
make install