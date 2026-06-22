#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>

#include "em86.txt"

int main()
{
    setlocale(0, "");
    //output_registers();
    executor();
    putchar('\n');
    return 0;
}