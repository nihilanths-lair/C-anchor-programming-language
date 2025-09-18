#include <stdio.h>
#include <locale.h>

/*
<&> Declare a buffer for user input of size 2048
<&> Объявить буфер для пользовательского ввода размером 2048
*/
static char input[2048];

int main(int argc, char **argv)
{
	setlocale(0, "");
	
	/*
	<&> Print Version and Exit Information
	<&> Версия для печати и информация о выходе
	*/
	puts("\nC$ Version 0.0.1");
	puts("Press Ctrl+C to Exit\n");

	/*
	<&> In a never ending loop
	<&> В бесконечном цикле
	*/
	while (1)
	{
		/*
		<&> Output our prompt
		<&> Выведите нашу подсказку
		*/
		fputs("Ввод: ", stdout);

		/*
		<&> Read a line of user input of maximum size 2048
		<&> Прочитать строку пользовательского ввода максимального размера 2048
		*/
		fgets(input, sizeof (input), stdin);

		/*
		<&> Echo input back to user
		<&> Возвращает ввод пользователю
		*/
		printf("Вывод: %s", input);
	}
	return 0;
}