#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    DIR *dp;

    struct dirent *dirp;

    if (argc != 2) {
        errno = ENOTSUP;
        perror("invalid args");
        exit(EXIT_FAILURE);
    }

    if ((dp = opendir(argv[1])) == NULL) {
        perror("cannot opendir");
        exit(EXIT_SUCCESS);
    }

    while ((dirp = readdir(dp)) != NULL) {
        printf("%d:", dirp->d_type);
        printf("%s\t", dirp->d_name);
    }
    /* rewinddir(dp); */
    /* while ((dirp = readdir(dp)) != NULL) { */
    /*     printf("%d:", dirp->d_type); */
    /*     printf("%s\t", dirp->d_name); */
    /* } */

    closedir(dp);

    return 0;
}
