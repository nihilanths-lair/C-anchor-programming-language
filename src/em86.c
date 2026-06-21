#include <stdio.h>
#include <locale.h>
#include <stdint.h>

#include "em86.txt"

int main()
{
    setlocale(0, "");
    output_registers();
    putchar('\n');
    return 0;
}