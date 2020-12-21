#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char *cwd;
    cwd = getcwd(NULL, 0);

    printf("cwd: %s\n", cwd);

    /*     if (chdir("./test") == -1) { */
    /*         perror("chdir"); */
    /*         exit(EXIT_FAILURE); */
    /*     } */

    free(cwd);

    cwd = getcwd(NULL, 0);

    printf("cwd: %s\n", cwd);

    /* assert(cwd == NULL); */

    /* printf("ok\n"); */

    /* if (chmod(cwd, 0) == -1) { */
    /*     perror("chmod"); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    /* system("ls -ld"); */
    struct stat buf;

    if (lstat(cwd, &buf) < 0) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    printf("%d\n", (int)buf.st_dev);
    printf("%d\n", buf.st_mode);
    if (S_ISDIR(buf.st_mode)) {
        printf("directory.\n");
    }

    int fd = open(strcat(cwd, "/in.in"), O_RDONLY | O_WRONLY);

    printf("fd: %d\n", fd);

    char str[5] = "alo";
    /* printf("str: %s\n", str); */
    /* printf("str: %d\n", str[4]); */
    FILE *file = fdopen(fd, "w");

    if (file == NULL) {
        perror("cannot open");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "test %s\n", str);
    fprintf(file, "test2 %s\n", str);

    fclose(file);

    file = fdopen(fd, "r");

    /* int size = read(fd, str, 3); */
    /* /1* str[size - 1] = 0; *1/ */
    /* printf("str: %s\n", str); */
    /* size = read(fd, str, 4); */
    /* str[size - 1] = 0; */
    printf("str: %s\n", str);

    close(fd);

    return 0;
}
