#include <stdio.h>
#include <stdlib.h>

int GCD(int A, int B)
{
    while (B != 0)
    {
        int temp = B;
        B = A % B;
        A = temp;
    }
    return A;
}

char *translation(long x)
{
    static char result[64];
    int i = 63;
    result[i--] = '\0';
    do
    {
        result[i--] = (x % 2) + '0';
        x /= 2;
    } while (x > 0);
    return &result[i + 1];
}
