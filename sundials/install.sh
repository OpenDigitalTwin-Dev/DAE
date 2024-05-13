#!/bin/sh

sudo apt update
sudo apt -y install cmake make gcc g++

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/../../install/sundials_install
make -j4
make install
