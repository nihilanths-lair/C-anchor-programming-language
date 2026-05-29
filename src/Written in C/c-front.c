#include <stdio.h>
#include <windows.h>
#include <stdint.h>

intptr_t token_type;
intptr_t token_value;
intptr_t token_text[64];

void print()
{
    printf(" Call: print.");
}

void cdlr__main()
{
    putchar('\n');
    print();
    putchar('\n');
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cdlr__main(); // Вызов главной функции языка C$
    return 0;
}