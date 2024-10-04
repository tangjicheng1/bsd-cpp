#!/bin/bash
luastatic ./fibonacci.lua /usr/local/lib/libluajit-5.1.a -I/usr/local/include -o fib_jit
