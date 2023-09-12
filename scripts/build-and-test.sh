#!/bin/bash

set -e

cd "$(dirname "$0")"
cd ..

rm -rf build
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-sanitize-recover=all" -GNinja ..
ninja
ctest . --output-on-failure
