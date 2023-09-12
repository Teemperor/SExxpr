#!/bin/bash

set -e

cd "$(dirname "$0")"
cd ..

rm -rf build-fuzz
mkdir build-fuzz
cd build-fuzz

cmake -DCMAKE_BUILD_TYPE=RelWithDbgInfo -GNinja ..
ninja
./fuzz/parse-fuzzer -print_pcs=1 -timeout=10 -max_len=20000
