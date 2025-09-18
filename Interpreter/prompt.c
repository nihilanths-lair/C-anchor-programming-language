#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdbool.h>

static char input[2048];

int main(int argc, char **argv)
{
	SetConsoleCP(1251);       	// ввод в CP1251
	SetConsoleOutputCP(1251);	// вывод в CP1251
	
	setlocale(LC_ALL, ""); 		// для printf / scanf
	
	puts("\n>C$ v.0.0.1");
	puts("Нажмите Ctrl+C для выхода.\n");

	while (true)
	{
		fputs("Ввод: ", stdout);
		fgets(input, sizeof (input), stdin);
		printf("Вывод: %s\n", input);
	}
	return 0;
}