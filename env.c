#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("current ALO: %s\n", getenv("ALO"));

    setenv("ALO", "IN_C", 1);

    printf("after set ALO: %s\n", getenv("ALO"));

    return 0;
}
