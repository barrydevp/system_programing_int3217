#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char *cwd;
    cwd = getcwd(NULL, 0);

    printf("cwd: %s/n", cwd);

    return 0;
}
