#!/bin/bash
luastatic ./fibonacci.lua /usr/local/lib/libluajit-5.1.a -I/usr/local/include -o fib_jit

sudo luarocks install luasocket
sudo luarocks install luastatic
sudo luarocks install lua-cjson

sudo luarocks install luasql-sqlite3
sudo luarocks install luasql-postgres
sudo luarocks install luasql-mysql
sudo luarocks install luasql-sqlite
sudo luarocks install luasql-odbc
