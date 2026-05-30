#include <stdio.h>
#include <windows.h>
#include <stdint.h>
intptr_t token__type_eof = 0;
intptr_t token__while = 1;
intptr_t token__identifier = 2;
intptr_t token__number = 3;
intptr_t token__operator = 4;
intptr_t token__keyword_if = 6;
intptr_t token__text[64];
intptr_t i = 6;

void cdlr__main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    while (i != 0)
    {
        printf("\n Я родился!");
        i--;
    }
    putchar('\n');
}

int main()
{
    cdlr__main();
    return 0;
}