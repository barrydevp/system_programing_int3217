#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    printf("This process is identified by: \n");
    printf("UID=%d, GID=%d\n", getuid(), getgid());
    printf("EUID=%d, EGID=%d\n", geteuid(), getegid());

    printf("Parent pid=%d\n", getpid());

    int pid;
    pid = fork();

    if (pid == 0) {
        printf("child[%d], fork ok!\n", 1);
        printf("child process has UID=%d, PID=%d\n", getgid(), getpid());

    } else {
        if (argc == 2 && strcmp(argv[1], "wait") == 0) {
            printf("Parent wait.\n");
            wait(NULL);
        }

        printf("Parent out.\n");
    }

    return 0;
}
