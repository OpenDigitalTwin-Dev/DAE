#!/bin/sh

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/../../install/sundials_install
make -j4
make install
