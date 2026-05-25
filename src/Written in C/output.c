#include <stdio.h>
#include <locale.h>

// Глобальная спартанская память языка C$
int x = 0;
int y = 0;
int z = 0;

void rule()
{
    y = 10;
}

void __main()
{
    x = 5;
    while (x)
    {
        rule();
        __main();
        x--;
    }
}

int main()
{
    setlocale(0, "");
    __main(); // Вызов главной функции
    return 0;
}