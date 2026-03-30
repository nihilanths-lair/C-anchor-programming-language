//C:\Users\Глеб\Documents\github.com\nihilanths-lair\C-anchor\src\Written in C>gcc codegen.c -o _
#include <locale.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    setlocale(0, "");
    //
	printf("\n argc = %d", argc);
    for (int i = 0; i < argc; i++) printf("\n argv[%d] = \"%s\"", i, argv[i]);
    putchar('\n');
    //
    return 0;
}