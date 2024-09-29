#!/bin/bash
gcc -c print1.c -o print1.o
gcc -c print2.c -o print2.o

ar rcs liball2.a print2.o print1.o

gcc main.c liball.a -o main_exe

gcc -c my_export.c -o my_export.o