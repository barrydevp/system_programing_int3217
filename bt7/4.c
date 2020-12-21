#include <stdio.h>
#include <stdlib.h>

#define stringify(x) #x
#define concat(a, b) a##b

int main(int argc, char *argv[]) {
    char ab[] = "test";

    printf("%s", concat(a, b));

    printf("\n");

    int *a = (int *)malloc(1);

    *(a - 1) = 3;

    printf("%d\n", *a);
    printf("%d\n", a[-1]);

    return 0;
}
