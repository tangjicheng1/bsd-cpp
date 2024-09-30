#!/bin/bash
gcc -c print1.c -o print1.o
gcc -c print2.c -o print2.o

ar rcs liball2.a print2.o print1.o

gcc main.c liball.a -o main_exe

gcc -c my_export.c -o my_export.o

# for shared library
gcc -shared -fPIC -o libmy_export.so my_export.c

gcc -L. -lmy_export my_export_main.c -o my_export_exe

LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./ ./my_export_exe

# for static library
gcc -c my_export.c -o my_export.o
ar rcs libmy_export.a my_export.o

gcc my_export_main.c libmy_export.a -o my_export2_exe
