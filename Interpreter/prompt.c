#include <windows.h>
#include <locale.h>
#include <stdio.h>
//#include <stdbool.h>

static char input[2048];

#define true 1

int main(int argc, char **argv)
{
	SetConsoleCP(1251);       // ввод в CP1251
	SetConsoleOutputCP(1251); // вывод в CP1251
	
	setlocale(LC_ALL, ""); 	  // для корректного вывода кириллицы у printf() / scanf()
	
	puts("\n Ctrl+C для выхода\n");
	while (true)
	{
		fputs("/>_ ", stdout);
		if (!fgets(input, sizeof (input), stdin)) break;
		//fgets(input, sizeof (input), stdin);

        size_t len = strlen(input);

		// В конце строки заменяем символ переноса на нулевой
		if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
		//input[strlen(input)-1] = '\0';

        puts("                +-------------------+");
		puts("+---------------| Система счисления |");
        puts("|   №   | ASCII |   16   |   _10    |");
        puts("+-------+-------+--------+----------+");

		for (size_t i = 0; input[i] != '\0'; i++)
        {
            unsigned char c = (unsigned char) input[i];

			// если символ печатный — показываем его, иначе выводим точку
            //char display = (c >= 32 && c < 127) ? c : input[i];
			char display = (c >= 32 && c <= 126) || (c >= 128 && c <= 255) ? c : '·';
            printf("| %5zu |   %-3c |   %02X   |   %03u    |\n", i+1, display, c, c);
        }
		puts("+-------+-------+--------+----------+");
	}
	return 0;
}