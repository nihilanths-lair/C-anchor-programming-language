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
    __main_block_rule();
}

int rule()
{
    y = 10;
}

int __main_block_rule()
{
    x = 5;
    while (x)
    {
        rule();
        x--;
    }
    /***/
    return 0;
}