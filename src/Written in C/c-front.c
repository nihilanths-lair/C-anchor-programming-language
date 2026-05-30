#include <stdio.h>
#include <windows.h>
#include <stdint.h>

intptr_t token__eof = 0;
intptr_t token__keyword_while = 1;
intptr_t token__identifier = 2;
intptr_t token__number = 3;
intptr_t token__operator = 4;
intptr_t token__keyword_if = 6;

intptr_t source_code = (intptr_t)"Я C$ строка инициализированная как глобальная";

intptr_t i = 0;

void cdlr__main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    while (i != 6)
    {
        printf("\n Я родился %d!", i);
        i = i + 1;
    }
    putchar('\n');
}

int main()
{
    cdlr__main();
    return 0;
}