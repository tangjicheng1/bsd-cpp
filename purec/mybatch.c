#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CMD_LENGTH 2048
#define MAX_ARGS 256

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s command.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Can NOT open command file");
        return EXIT_FAILURE;
    }

    char command[MAX_CMD_LENGTH];
    pid_t pids[MAX_ARGS];
    int count = 0;

    while (fgets(command, sizeof(command), file) != NULL) {
        command[strcspn(command, "\n")] = 0;

        if (strlen(command) == 0) {
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Can NOT fork process");
            fclose(file);
            return EXIT_FAILURE;
        } else if (pid == 0) {
            char* args[MAX_ARGS];
            char* token = strtok(command, " ");
            int i = 0;

            while (token != NULL && i < MAX_ARGS - 1) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            execvp(args[0], args);
            perror("Execute command failed");
            printf("Failed command: %s\n", command);
            exit(EXIT_FAILURE);
        } else {
            pids[count++] = pid;
        }
    }

    fclose(file);

    for (int i = 0; i < count; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return EXIT_SUCCESS;
}
