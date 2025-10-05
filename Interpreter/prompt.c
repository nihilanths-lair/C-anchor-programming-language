#include <windows.h>
#include <locale.h>
#include <stdio.h>
//#include <stdbool.h>

#define false 0
#define true 1

#define runblock {
#define endblock }

short dec_to_bin(unsigned short dec);

static char input[2048];

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

		//FILE *doc = fopen("prompt.log", "w");
		/*
		fprintf(doc, "		     +-------------------+\n");
		fprintf(doc, "		     | Система счисления |\n");
		fprintf(doc, "+----+-------+          +--------+\n");
		fprintf(doc, "|  № | ASCII | 16 | 010 |\n");
		fprintf(doc, "+----+-------+----+-----+\n");
		*/
		/*
        puts("		     +-------------------+");
		puts("		     | Система счисления |");
		puts("+----+-------+          +--------+");
		puts("|  № | ASCII | 16 | 010 |");
		puts("+----+-------+----+-----+");
		*/
		//for (size_t i = 0; input[i] != '\0'; i++)
		FILE *doc = fopen("gnt_prompt.c", "w");

		fprintf(doc, "unsigned char table_ascii[] =\n");
		fprintf(doc, "{\n");

		short i;
		for (i = 0; i < 9; i++) fprintf(doc, "    '%c', // №%d\n", i, i);
		fprintf(doc, "    '\\t', // №9\n");
		fprintf(doc, "    '\\n', // №10\n");
		for (i = 11; i < 13; i++) fprintf(doc, "    '%c', // №%d\n", i, i);
		fprintf(doc, "    '\\r', // №13\n");
		for (i = 14; i < 39; i++) fprintf(doc, "    '%c', // №%d\n", i, i);
		fprintf(doc, "    '\\%c', // №39\n", i);
		for (i = 40; i < 92; i++) fprintf(doc, "    '%c', // №%d\n", i, i);
		fprintf(doc, "    '\\%c', // №92\n", i);
		for (i = 93; i < 255; i++) fprintf(doc, "    '%c', // №%d\n", i, i);

		fprintf(doc, "    '%c' // №255\n", i);
		fprintf(doc, "};\n");

		fprintf(doc, "int main(){}\n");
		fprintf(doc, "/*-------------------------------------------------------------------/\n");
        fprintf(doc, "FILE *doc = fopen(\"gnt_prompt.c\", \"w\");\n");

        fprintf(doc, "fprintf(doc, \"unsigned char table_ascii[] =\\n\");\n");
        fprintf(doc, "fprintf(doc, {\\n\");\n");

        fprintf(doc, "short i;\n");
        fprintf(doc, "for (i = 0; i < 9; i++) fprintf(doc, \"    '%%c', // №%%d\", i, i);\n");
        fprintf(doc, "fprintf(doc, \"    '\\\\t', // №9\");\n");
        fprintf(doc, "fprintf(doc, \"    '\\\\n', // №10\");\n");
        fprintf(doc, "for (i = 11; i < 13; i++) fprintf(doc, \"    '%%c', // №%%d\", i, i);\n");
        fprintf(doc, "fprintf(doc, \"    '\\\\r', // №13\");\n");
        fprintf(doc, "for (i = 14; i < 39; i++) fprintf(doc, \"    '%%c', // №%%d\", i, i);\n");
        fprintf(doc, "fprintf(doc, \"    '\\%%c', // №39\", i);\n");
        fprintf(doc, "for (i = 40; i < 92; i++) fprintf(doc, \"    '%%c', // №%%d\", i, i);\n");
        fprintf(doc, "fprintf(doc, \"    '\\%%c', // №92\", i);\n");
        fprintf(doc, "for (i = 93; i < 255; i++) fprintf(doc, \"    '%%c', // №%%d\", i, i);\n");
        fprintf(doc, "fprintf(doc, \"    '%%c' // №255\", i);\n");
        fprintf(doc, "fprintf(doc, \"};\\n\");\n");
        fprintf(doc, "int main(){}\n");
        fprintf(doc, "fclose(doc);\n");
        fprintf(doc, "/-------------------------------------------------------------------*/");
		//fprintf(doc, "{");
		//fprintf(doc, "}");
		/*
        {
            unsigned char c = (unsigned char) input[i];

			// если символ печатный — показываем его, иначе выводим неопознанный символ
            //char display = (c >= 32 && c < 127) ? c : '';
			//char display = (c >= 32 && c <= 126) || (c >= 128 && c <= 255) ? c : '';
            //printf("| %5zu |   %-3c |   %02X   |   %03u    |\n", i+1, display, c, c);
			fprintf(doc, "| %2zu |   %-3c | %02X | %03u |\n", i+1, c, c, c);
        }
		*/
		//fprintf(doc, "+----+-------+----+-----+");
		fclose(doc);
		//puts("+-----+-------+----+-----+");
	}
	return 0;
}
// decimal to binary | десятичное в двоичное
short dec_to_bin(unsigned short dec)
{
    unsigned short bin[8];
    //unsigned short rmnd = dec;
    for (short i = 8; i >= 0; i--)
    {
        switch (dec % 2)
        runblock
        case 0: bin[i] = 0; break; // remainder | остаток
        default: bin[i] = 1; break; // remainder | остаток
        endblock
        dec = (dec / 2);
    }
    bin[0] *= 128;
    bin[1] *= 64;
    bin[2] *= 32;
    bin[3] *= 16;
    bin[4] *= 8;
    bin[5] *= 4;
    bin[6] *= 2;
    //bin[7] *= 1; // оставляем как есть
    return 152;
}
unsigned char bin_to_dec()
{
    return '�';
}
/*
// decimal to binary | десятичное в двоичное
unsigned char *dec_to_bin(unsigned short dec)
{
    return '�';
}
*/