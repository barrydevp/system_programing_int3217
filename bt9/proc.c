#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    printf("This process is identified by: \n");
    printf("UID=%d, GID=%d\n", getuid(), getgid());
    printf("EUID=%d, EGID=%d\n", geteuid(), getegid());

    int pid;

    printf("my pid=%d\n", getpid());
    pid = fork();

    if (pid == 0) {
        printf("child[%d], fork ok!\n", 1);
        printf("child process has UID=%d, PID=%d\n", getgid(), getpid());
        char in[100];
        /* scanf("%s", in); */
        /* printf("exit child 1 with input: %s\n", in); */
    } else {
        /* wait(NULL); */
        int pid1 = fork();

        if (pid1 == 0) {
            printf("child[%d], fork ok!\n", 2);
            printf("child process has UID=%d, PID=%d\n", getgid(), getpid());
            char in[100];
            /* scanf("%s", in); */
            /* printf("exit child 2 with input: %s\n", in); */
        } else {
            wait(NULL);
            wait(NULL);
            printf("you are in parent :))\n");
            int s1, s2;
        }
    }

    return 0;
}
