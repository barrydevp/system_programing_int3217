#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef enum { false, true } bool;

extern char **environ;

const char PROMPT_STR[] = "barrydevp-> ";

extern char *test;

int exc_command(char *command) {
    int pid = fork();
    if (pid == 0) {
        /* child process */
        /* printf("We are going to execute command %s\n", command); */

        char *argvs[] = {command, NULL};

        if (execvpe(command, argvs, environ) < 0) {
            /* printf("%s", strerror(1)); */
            perror("cannot execute command");
            exit(EXIT_FAILURE);

            return -1;
        }

    } else {
        if (wait(NULL) < 0) {
            perror("cannot wait child process");

            return -1;
        }
    }

    return 0;
}

int listen_and_exec(char *lastCommand, int *lastIndex) {
    /* printf("pid: %d\n", getpid()); */
    if (*lastIndex == 0) {
        write(STDOUT_FILENO, PROMPT_STR, strlen(PROMPT_STR));
    }

    int commandSize =
        read(STDIN_FILENO, lastCommand + *lastIndex, 128 - *lastIndex);

    if (commandSize == -1) {
        perror("cannot read from stdin");
    }

    if (commandSize == 0) {
        return 1;
    }

    lastCommand[*lastIndex + commandSize - 1] = 0;

    if (strcmp("exit", lastCommand) == 0) {
        return 0;
    }

    if (lastCommand[*lastIndex + commandSize - 2] == '\\') {
        *lastIndex = *lastIndex + commandSize - 2;
    } else {
        *lastIndex = 0;
        int callOk = exc_command(lastCommand);

        if (callOk) {
            printf("Error when call %s\n", lastCommand);
        }
    }

    return 1;
}

int main() {
    printf("This process has pid: %d, identified by: \n", getpid());
    printf("UID=%d, GID=%d\n", getuid(), getgid());
    printf("EUID=%d, EGID=%d\n", geteuid(), getegid());
    printf("---CONSOLE--------------------\n\n\n");

    char *command = (char *)calloc(128, sizeof(char));
    int lastIndex = 0;

    while (listen_and_exec(command, &lastIndex)) {
    }

    return 0;
}
