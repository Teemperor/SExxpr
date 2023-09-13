#!/bin/bash

set -e

cd "$(dirname "$0")"
cd ..

rm -rf build-fuzz
mkdir build-fuzz
cd build-fuzz

cmake -DCMAKE_BUILD_TYPE=RelWithDbgInfo -GNinja ..
ninja parse-fuzzer
./fuzz/parse-fuzzer -timeout=60 -max_len=20000 "$@"
