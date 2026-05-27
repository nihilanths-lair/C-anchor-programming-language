#include <stdio.h>
#include <windows.h>
#include <stdint.h>

intptr_t token_type;
intptr_t token_value;
intptr_t token_text[64];

void print()
{
    printf("\n MVP-компилятор мета-языка C$.\n");
}

void __main__()
{
    print();
}

int main()
{
    // Нативное и безопасное переключение кодировки консоли Windows без system()
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    __main__(); // Вызов главной функции
    return 0;
}