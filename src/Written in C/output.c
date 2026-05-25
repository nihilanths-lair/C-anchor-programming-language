#include <stdio.h>
#include <locale.h>

int main()
{
    setlocale(0, "");
    // Спартанская память
    int x = 0;
    int y = 0;
    int z = 0;
    /***/
    putchar(4);
    x = 5;
    while (x)
    {
        putchar(45);
        x--;
    }
    putchar(4);
    x = 5;
    y = 53;
    putchar(10);
    while (x)
    {
        putchar(32);
        putchar(67);
        putchar(36);
        putchar(58);
        putchar(32);
        putchar(y);
        y--;
        int __flag_0 = x;
        while (__flag_0)
        {
            putchar(10);
            __flag_0 = 0;
        }
        x--;
    }
    putchar(4);
    x = 5;
    while (x)
    {
        putchar(45);
        x--;
    }
    z = 1;
    int __flag_1 = z;
    while (__flag_1)
    {
        putchar(4);
        putchar(32);
        __flag_1 = 0;
    }
    /***/
    return 0;
}