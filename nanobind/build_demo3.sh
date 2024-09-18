#!/bin/bash
mkdir -p build
cd build
cmake .. && make -j

cd ..
cp ./build/demo3.cpython-312-x86_64-linux-gnu.so ./demo3_package/demo3_package/

cd demo3_package
python setup.py sdist bdist_wheel
