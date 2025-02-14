#include <stdio.h>
#include <stdlib.h>

int GCD(int A, int B)
{
    int gcd = 1;
    for (int i = 1; i <= A && i <= B; i++)
    {
        if (A % i == 0 && B % i == 0)
            gcd = i;
    }
    return gcd;
}

char *translation(long x)
{
    static char result[64];
    int i = 63;
    result[i--] = '\0';
    do
    {
        result[i--] = (x % 3) + '0';
        x /= 3;
    } while (x > 0);
    return &result[i + 1];
}