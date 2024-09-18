#!/bin/bash
mkdir -p build
cd build
cmake .. && make -j

cd ..
cp ./build/demo3.cpython-312-x86_64-linux-gnu.so ./demo3/

python setup.py sdist bdist_wheel

pip install ./dist/demo3-0.1-py3-none-any.whl
