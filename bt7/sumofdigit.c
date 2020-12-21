#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int number, right_digit;
    int sum = 0;
    if (argc == 2)
        number = atoi(argv[1]);
    else
        printf(
            "Enter your number (Keep in mind the max length of a long int). "),
            scanf("%i", &number);

    do {
        right_digit = number % 10;
        number = number / 10;
        sum += right_digit;
    } while (number > 0);

    printf("The sum of the digits of your number is %i \n", sum),
        printf("hello %i", 1);
    return 0;
}
