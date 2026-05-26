#include <stdio.h>
#include <windows.h>
#include <stdint.h>

// Глобальная спартанская память языка C$
intptr_t __[100000]; // Единая лента памяти на 100k ячеек
// Пользовательские переменные (выделяем спартанский пул)
intptr_t i = 0, res = 0, flag = 0, cond = 0;

void __main__()
{
    printf("\n ");
    __['$'] = (intptr_t)"C$ is awesome!";
    res = __['$'];
    printf((char*) res);
    putchar('\n');
}

int main()
{
    // Нативное и безопасное переключение кодировки консоли Windows без system()
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    __main__(); // Вызов главной функции
    return 0;
}