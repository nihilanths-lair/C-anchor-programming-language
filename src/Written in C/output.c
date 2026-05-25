#include <stdio.h>

int main()
{
    // Спартанская память
    int x = 0;
    int y = 0;
    /***/
    x = 1;
    while (x)
    {
        y = 42;
        x = 0;
    }
    /***/
    return 0;
}