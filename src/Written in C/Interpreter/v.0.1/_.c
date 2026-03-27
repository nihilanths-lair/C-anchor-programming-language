// <!-- C-style (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    setlocale(0, "");
    //
	printf("2 + 2 * 2 = %d", 2 + 2 * 2);
    //
    return 0;
}