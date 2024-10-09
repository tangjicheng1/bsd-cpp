#!/bin/bash
sudo apt-get install -y libelf-dev libdwarf-dev
g++ -std=c++11 get_symbols.cpp -o get_symbols -lelf
