#include <stdio.h>
#include <locale.h>

// Глобальная спартанская память языка C$
int __[100000]; // Единая лента памяти на 100k ячеек
int x = 0, y = 0, z = 0;

void __main()
{
    __[0] = 5;
    x = __[0];
    while (x != 0)
    {
        putchar(65);
        x--;
        __[0] = x;
    }
}

int main()
{
    setlocale(0, "");
    __main(); // Вызов главной функции
    return 0;
}
