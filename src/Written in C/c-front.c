#include <stdio.h>
#include <windows.h>
#include <stdint.h>

intptr_t token_type;
intptr_t token_value;
intptr_t token_text[64];

void print()
{
    printf("\n MVP-компилятор мета-языка C$.\n");
    token_type = 5;
}


void c_main()
{
    print();
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    c_main();
    return 0;
}