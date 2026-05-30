#include <stdio.h>
#include <windows.h>
#include <stdint.h>

intptr_t token__eof = 0;
intptr_t token__keyword_while = 1;
intptr_t token__identifier = 2;
intptr_t token__number = 3;
intptr_t token__operator = 4;
intptr_t token__keyword_if = 6;

intptr_t source_code = (intptr_t)"// Через эту переменную будем делать самораскрутку компилятора";
intptr_t position = 0;

intptr_t source_code_2[128];

void cdlr__main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    strcpy((char*)source_code_2, "// Исходный код C$ для самораскрутки!");

    while (((char*)source_code)[position] != token__eof)
    {
        printf("\n Текущий символ (%d): '%c'.", position, ((char*)source_code)[position]);
        if (((char*)source_code)[position] ==  )
        {
            printf(" Обнаружен пробельный символ (%d).")
        }
        position = position + 1;
    }
    putchar('\n');
}

int main()
{
    cdlr__main();
    return 0;
}