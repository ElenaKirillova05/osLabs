#include <stdio.h>
#include "gcd_euclid_and_binary.h"

int main()
{
    int command;
    printf("Введите команду: ");
    while (scanf("%d", &command) != EOF)
    {
        if (command == 1)
        {
            int a, b;
            printf("Введите два числа: ");
            scanf("%d %d", &a, &b);
            printf("НОД: %d\n", GCD(a, b));
        }
        else if (command == 2)
        {
            long x;
            printf("Введите число: ");
            scanf("%ld", &x);
            printf("Результат перевода: %s\n", translation(x));
        }
        else
        {
            printf("Неверная команда.\n");
        }
        printf("Введите команду: ");
    }
    return 0;
}
