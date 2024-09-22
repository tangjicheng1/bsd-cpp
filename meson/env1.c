#include <stdio.h>

int main(int argc, char** argv, char** envp) {
    printf("命令行参数:\n");
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    printf("\n环境变量:\n");
    for (char** env = envp; *env != 0; env++) {
        printf("%s\n", *env);
    }

#ifdef __STDC_VERSION__
    printf("C语言标准版本: %ld\n", __STDC_VERSION__);
#else
    printf("C语言标准版本: 未定义\n");
#endif

    return 0;
}
