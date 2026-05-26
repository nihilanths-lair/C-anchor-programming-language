#include <stdio.h>
#include <windows.h>

// Глобальная спартанская память языка C$
int __[100000]; // Единая лента памяти на 100k ячеек
// Пользовательские переменные (выделяем спартанский пул)
int i = 0, res = 0, flag = 0, cond = 0;

void __main__()
{
    i = 0;
    cond = 'A';
    __[i] = cond;
    res = __[0];
    putchar('\n');
    putchar(res);
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