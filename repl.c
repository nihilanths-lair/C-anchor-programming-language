#include <stdio.h>
#include <string.h>
#include <windows.h>

#define SIZE_INPUT 2048
//int GetConstructionID(){return ;}
// Добавляет конструкцию, ID автоматически инкрементирует
short AddConstruction(const char *s1, const char *s2)
{
    static char l_construction[sizeof (short)][SIZE_INPUT];
    static char r_construction[sizeof (short)][SIZE_INPUT];
    //char construction[sizeof (short)][2048][2048];
    static int count = 0;

    printf("l_construction[%d] = %s\n", count, l_construction[count]);
    printf("r_construction[%d] = %s\n", count, r_construction[count]);

    strcpy(l_construction[count], s1);
    strcpy(r_construction[count], s2);

    printf("l_construction[%d] = %s\n", count, l_construction[count]);
    printf("r_construction[%d] = %s\n", count, r_construction[count]);
    count ++;
    printf("count = %d\n", count);
}
int main()
{
    SetConsoleCP(1251);       // ввод в CP1251
	SetConsoleOutputCP(1251); // вывод в CP1251
	//setlocale(0, "");         // для корректного вывода кириллицы у printf/scanf

    //---------------[Начало блока - 1]---------------//
    short construction_id = 0;
    construction_id = AddConstruction("Число", "5");
    //---------------[Конец блока - 1]----------------//

    char input[SIZE_INPUT];
	puts("\n Ctrl+C для выхода\n");
	while (!0)
	{
		fputs("/>_ ", stdout);
		if (!fgets(input, sizeof (input), stdin)) break;
		//fgets(input, sizeof (input), stdin);

		size_t len = strlen(input);

		// В конце строки заменяем символ переноса на нулевой
		if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
		//input[strlen(input)-1] = '\0';

        // для логирования
        puts(input);
	}
}