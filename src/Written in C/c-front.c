#include <stdio.h>
#include <windows.h>
#include <stdint.h>

intptr_t token__eof = 0;
intptr_t token__keyword_while = 1;
intptr_t token__identifier = 2;
intptr_t token__number = 3;
intptr_t token__operator = 4;
intptr_t token__keyword_if = 6;

intptr_t source_code = (intptr_t)"// Исходный код C$ для самораскрутки!";
intptr_t position = 0;

void cdlr__main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    while (source_code[position] != token__eof)
    {
        printf("\n Текущий символ: '%d'.", source_code[position]);
        position = position + 1;
    }
    putchar('\n');
}

int main()
{
    cdlr__main();
    return 0;
}