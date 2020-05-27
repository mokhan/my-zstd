#!/bin/bash

set -ex

rm -rf build
mkdir build
pushd build

conan install --build zstd ..

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
bin/example "../logo.png"
