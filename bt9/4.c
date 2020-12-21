#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef enum { false, true } bool;

extern char **environ;

const char PROMPT_STR[] = "barrydevp-> ";
const char SHITS[] = "\t\n\v\f\r ";

char *ltrim(char *command) {
    int lastShitIndex = strspn(command, SHITS);

    /* printf("last shit: %d\n", lastShitIndex); */
    if (lastShitIndex > 0) {
        int len = strlen(command);

        if (lastShitIndex == len) {
            command[0] = '\0';
        } else {
            strcpy(command, command + lastShitIndex);
        }
    }

    return command;
}

char *rtrim(char *command) {

    int len = strlen(command) - 1;

    while (len > 0 && strchr(SHITS, command[len]) != NULL) {
        command[len] = 0;
        len--;
    }

    return command;
}

char *trim(char *command) { return rtrim(ltrim(command)); }

char **parse_command_argvs(char *command) {
    trim(command);

    if (strlen(command) <= 0)
        return NULL;

    char *cpCommand = malloc(strlen(command) * sizeof(char));
    strcpy(cpCommand, command);

    int numOfToken = 0;
    char *curToken = strtok(cpCommand, SHITS);

    while (curToken != NULL) {
        /* printf("%s\n", curToken); */
        numOfToken++;
        curToken = strtok(NULL, SHITS);
    }

    /* printf("%d\n", numOfToken); */

    if (numOfToken <= 0)
        return NULL;

    // plus one for the NULL terminated
    char **argvs = (char **)malloc(sizeof(char *) * (numOfToken + 1));

    curToken = strtok(command, SHITS);
    numOfToken = 0;
    while (curToken != NULL) {
        /* printf("%s\n", curToken); */
        argvs[numOfToken] = curToken;
        numOfToken++;
        curToken = strtok(NULL, SHITS);
    }

    argvs[numOfToken] = NULL;

    return argvs;
}

int parse_and_redirect_in(char *command) {
    if (strlen(command) <= 0)
        return 0;

    char *infile = strchr(command, '<');
    if (infile == NULL)
        return 0;
    *infile = 0;
    infile++;
    trim(infile);
    /* printf("%s\n", infile); */

    int infd;

    if ((infd = open(infile, O_RDONLY)) == -1)
        return -1;

    if (dup2(infd, STDIN_FILENO) == -1) {
        int tmpErrno = errno;
        close(infd);
        errno = tmpErrno;

        return -1;
    }

    return 0;
}

int parse_and_redirect_out(char *command) {
    if (strlen(command) <= 0)
        return 0;

    char *outfile = strchr(command, '>');
    if (outfile == NULL)
        return 0;

    *outfile = 0;
    outfile++;
    trim(outfile);

    int outfd;

    if ((outfd = open(outfile, O_WRONLY)) == -1)
        return -1;

    if (dup2(outfd, STDOUT_FILENO) == -1) {
        int tmpErrno = errno;
        close(outfd);
        errno = tmpErrno;

        return -1;
    }

    if (dup2(outfd, STDERR_FILENO) == -1) {
        int tmpErrno = errno;
        close(outfd);
        errno = tmpErrno;

        return -1;
    }

    return close(outfd);

    /* return 0; */
}
int exc_command(char *command) {
    char *cpCommand = (char *)malloc(sizeof(char) * strlen(command));
    strcpy(cpCommand, command);
    trim(cpCommand);

    int pid = fork();
    if (pid == 0) {
        /* printf("%d\n", getpid()); */
        /* child process */
        /* printf("We are going to execute command %s\n", command); */

        if (parse_and_redirect_out(cpCommand) == -1) {
            perror("redirect_out error");
            exit(EXIT_FAILURE);

            return -1;
        }
        /* printf("%s\n", cpCommand); */

        if (parse_and_redirect_in(cpCommand) == -1) {
            perror("redirect_in error");
            exit(EXIT_FAILURE);

            return -1;
        }
        /* printf("%s\n", cpCommand); */

        char **argvs = parse_command_argvs(cpCommand);

        if (execvpe(cpCommand, argvs, environ) < 0) {
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
        return 1;
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
        /* printf("echo: %s\n", lastCommand); */
        int callOk = exc_command(lastCommand);

        if (callOk) {
            printf("Error when call %s\n", lastCommand);
        }
    }

    return 1;
}

char *simpleTest(char *str) { return strchr(str, '<'); }

int main() {
    printf("This process has pid: %d, identified by: \n", getpid());
    printf("UID=%d, GID=%d\n", getuid(), getgid());
    printf("EUID=%d, EGID=%d\n", geteuid(), getegid());
    printf("---CONSOLE--------------------\n\n\n");

    char *command = (char *)calloc(128, sizeof(char));
    int lastIndex = 0;

    while (listen_and_exec(command, &lastIndex)) {
    }

    printf("\n\n");
    printf("---BYE BYE <3\n");

    return 0;
}
