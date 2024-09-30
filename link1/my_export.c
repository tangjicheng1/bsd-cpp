#include <stdio.h>

__attribute__((visibility("hidden"))) void print1();
__attribute__((visibility("default"))) void print2();

void print1() {
    printf("call print1\n");
}

void print2() {
    printf("call print2\n");
}
