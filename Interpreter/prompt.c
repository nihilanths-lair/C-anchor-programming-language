#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdbool.h>

static char input[2048];

int main(int argc, char **argv)
{
	SetConsoleCP(1251);       	// ���� � CP1251
	SetConsoleOutputCP(1251);	// ����� � CP1251
	
	setlocale(LC_ALL, ""); 		// ��� printf / scanf
	
	puts("\n>C$ v.0.0.1");
	puts("������� Ctrl+C ��� ������.\n");

	while (true)
	{
		fputs("����: ", stdout);
		fgets(input, sizeof (input), stdin);
		printf("�����: %s\n", input);
	}
	return 0;
}