#!/bin/bash

## prepare
pip install nanobind
python -m nanobind --cmake_dir
python -m nanobind --include

## build
mkdir build
cd build
cmake ..
make

## run
cp ./my.py ./build
python my.py

##
python -m site --user-site

pip install scikit-build cmake setuptools wheel nanobind
