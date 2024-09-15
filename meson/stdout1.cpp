#include <unistd.h>
#include <cstdio>

int main() {
    if (isatty(fileno(stdout))) {
        printf("stdout is line buffered (typically)\n");
    } else {
        printf("stdout is fully buffered (typically)\n");
    }

    printf("stdout is %d\n", fileno(stdout));

    return 0;
}
