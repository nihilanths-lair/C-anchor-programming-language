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

void cdlr__main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n Я родился!\n");
}

int main()
{
    cdlr__main();
    return 0;
}