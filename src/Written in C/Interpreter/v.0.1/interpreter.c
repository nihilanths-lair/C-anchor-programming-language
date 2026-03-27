// <!-- Interpreter C$ v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    setlocale(0, "");
    //
	printf("argc = %d", argc);
	for (int i = 0; i < argc; i++) printf("\nargv[%d] = \"%s\"", i, argv[i]);
    //
    return 0;
}