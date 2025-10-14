#include <windows.h>
#include <locale.h>
#include <stdio.h>
//#include <stdbool.h>

#define false 0
#define true 1

#define runblock {
#define endblock }

#define __TAB__ "    "

#define _1 "128"
#define _2 "130"

unsigned int dec_to_bin(unsigned short dec);

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
		FILE *doc = fopen("prompt.c", "w");
		unsigned short i;

		fprintf(doc, "unsigned char table_ascii[] =\n");
		fprintf(doc, "{\n");
		fprintf(doc, ""__TAB__"'\\0', // №0\n");
		for (i = 1; i < 9; i++) fprintf(doc, ""__TAB__"'%c', // №%d\n", i, i);
		fprintf(doc, ""__TAB__"'\\t', // №9\n");
		fprintf(doc, ""__TAB__"'\\n', // №10\n");
		for (i = 11; i < 13; i++) fprintf(doc, ""__TAB__"'%c', // №%d\n", i, i);
		fprintf(doc, ""__TAB__"'\\r', // №13\n");
		for (i = 14; i < 39; i++) fprintf(doc, ""__TAB__"'%c', // №%d\n", i, i);
		fprintf(doc, ""__TAB__"'\\%c', // №39\n", i);
		for (i = 40; i < 92; i++) fprintf(doc, ""__TAB__"'%c', // №%d\n", i, i);
		fprintf(doc, ""__TAB__"'\\%c', // №92\n", i);
		for (i = 93; i < 255; i++) fprintf(doc, ""__TAB__"'%c', // №%d\n", i, i);
		fprintf(doc, ""__TAB__"'%c' /// №255\n", i);
		fprintf(doc, "};\n");

        fprintf(doc, "unsigned int table_bin[] =\n");
        fprintf(doc, "{\n");
        for (i = 0; i < 255; i++)
		{
			if (i >= 128) fprintf(doc, "/**/%d, // №%d\n", dec_to_bin(i), i);
			else if (i >= 64) fprintf(doc, "/*·*/%d, // №%d\n", dec_to_bin(i), i);
			else if (i >= 32) fprintf(doc, "/*··*/%d, // №%d\n", dec_to_bin(i), i);
			else if (i >= 16) fprintf(doc, "/*···*/%d, // №%d\n", dec_to_bin(i), i);
			else if (i >= 8) fprintf(doc, "/*····*/%d, // №%d\n", dec_to_bin(i), i);
			else if (i >= 4) fprintf(doc, "/*·····*/%d, // №%d\n", dec_to_bin(i), i);
			else if (i >= 2) fprintf(doc, "/*······*/%d, // №%d\n", dec_to_bin(i), i);
			else if (i >= 0) fprintf(doc, "/*·······*/%d, // №%d\n", dec_to_bin(i), i);
		}
		fprintf(doc, "/**/%d /// №255\n", dec_to_bin(i));
        fprintf(doc, "};\n");
		fprintf(doc, "#include <stdio.h>\n");
		//fprintf(doc, "#define _1 255\n");
		//fprintf(doc, "#define _2 255\n");
		fprintf(doc, "unsigned char GetDec(unsigned char ascii);\n");
		fprintf(doc, "unsigned  int GetBin(unsigned char bin);\n");
		fprintf(doc, "unsigned  int AddBin(unsigned char bin_1, unsigned char bin_2);\n");
		fprintf(doc, "unsigned  int SubBin(unsigned char bin_1, unsigned char bin_2);\n");
		fprintf(doc, "unsigned int dec_to_bin(unsigned char dec);\n");
		fprintf(doc, "#define runblock {\n");
		fprintf(doc, "#define endblock }\n");
		fprintf(doc, "int main()\n");
		fprintf(doc, "{\n");
		fprintf(doc, ""__TAB__"unsigned char i = 0;\n");
		//fprintf(doc, ""__TAB__"do\n");
		//fprintf(doc, ""__TAB__"for (unsigned char i = 0; i <= 255; i++)\n");
		//fprintf(doc, ""__TAB__""__TAB__"printf(\"%%d - %%d\\n\", i, table_bin[i]);\n");
		fprintf(doc, ""__TAB__"do printf(\"%%3d - %%8d\\n\", i, table_bin[(unsigned char)i]); // table_bin[i %% 256]\n");
		//fprintf(doc, ""__TAB__""__TAB__"i++;\n");
		fprintf(doc, ""__TAB__"while (i++ != 255);\n");

		fprintf(doc, ""__TAB__"putchar('\\n');\n");

		fprintf(doc, ""__TAB__"printf(\"~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\\n\");\n");
		fprintf(doc, ""__TAB__"printf(\"  BINARY  | DECIMAL\\n\");\n");
		fprintf(doc, ""__TAB__"printf(\" %%08d | %%8d\\n\", table_bin["_1"], table_ascii["_1"]);\n");
		fprintf(doc, ""__TAB__"printf(\"+\t  +\\n\");\n");
		fprintf(doc, ""__TAB__"printf(\" %%08d | %%8d\\n\", table_bin["_2"], table_ascii["_2"]);\n");
		fprintf(doc, ""__TAB__"printf(\"=\t  =\\n\");\n");
		fprintf(doc, ""__TAB__"printf(\" %%08d | %%8d\\n\", GetBin(table_ascii["_1"] + table_ascii["_2"]), GetDec(table_ascii["_1"] + table_ascii["_2"])); // (table_ascii[128] + table_ascii[130]) %% 256\n");
		fprintf(doc, ""__TAB__"printf(\"~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\\n\");\n");
		fprintf(doc, ""__TAB__"putchar('\\n');\n");
		fprintf(doc, ""__TAB__"printf(\"~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\\n\");\n");
		fprintf(doc, ""__TAB__"printf(\"  BINARY  | DECIMAL\\n\");\n");
		fprintf(doc, ""__TAB__"printf(\" %%08d | %%8d\\n\", table_bin["_1"], table_ascii["_1"]);\n");
		fprintf(doc, ""__TAB__"printf(\"+\\t\\t    +\\n\");\n");
		fprintf(doc, ""__TAB__"printf(\" %%08d | %%8d\\n\", table_bin["_2"], table_ascii["_2"]);\n");
		fprintf(doc, ""__TAB__"printf(\"=\\t\\t    =\\n\");\n");
		fprintf(doc, ""__TAB__"printf(\" %%08d | %%8d\\n\", table_bin[(unsigned char)(table_ascii["_1"] + table_ascii["_2"])], (unsigned char)(table_ascii["_1"] + table_ascii["_2"])); // (table_ascii[128] + table_ascii[130]) %% 256\n");
		fprintf(doc, ""__TAB__"printf(\"~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\\n\");\n");
		fprintf(doc, ""__TAB__"return 0;\n");
		fprintf(doc, "}\n");

		fprintf(doc, "unsigned char GetDec(unsigned char ascii                     ) { return ascii;                                     }\n");
		fprintf(doc, "unsigned  int GetBin(unsigned char bin                       ) { return table_bin[(unsigned char) bin           ]; } // table_bin[bin %% 256]\n");
		fprintf(doc, "unsigned  int AddBin(unsigned char bin_1, unsigned char bin_2) { return table_bin[(unsigned char)(bin_1 + bin_2)]; } // table_bin[(bin_1 + bin_2) %% 256]\n");
		fprintf(doc, "unsigned  int SubBin(unsigned char bin_1, unsigned char bin_2) { return table_bin[(unsigned char)(bin_1 - bin_2)]; } // table_bin[(bin_1 - bin_2) %% 256]\n");
		fprintf(doc, "// decimal to binary | десятичное в двоичное\n");
		fprintf(doc, "unsigned int dec_to_bin(unsigned char dec)\n");
		fprintf(doc, "{\n");
		fprintf(doc, "	unsigned int factor = 1;\n");
		fprintf(doc, "	unsigned int bin = 0;\n");
		fprintf(doc, "	\n");
		fprintf(doc, "	for (short i = 7; i >= 0; i--)\n");
		fprintf(doc, "	{\n");
		fprintf(doc, "		switch (dec %% 2)\n");
		fprintf(doc, "		runblock\n");
		fprintf(doc, "		case 0: bin += (0 * factor); break;\n");
		fprintf(doc, "		default: bin += (1 * factor); break;\n");
		fprintf(doc, "		endblock\n");
		fprintf(doc, "		dec = (dec / 2);\n");
		fprintf(doc, "		//factor *= 2;\n");
		fprintf(doc, "		factor *= 10;\n");
		fprintf(doc, "	}\n");
		fprintf(doc, "	//bin[0] *= 128 + bin[1] *= 64 + bin[2] *= 32 + bin[3] *= 16 + bin[4] *= 8 + bin[5] *= 4 + bin[6] *= 2; //bin[7] *= 1; // оставляем как есть // remainder | остаток\n");
		fprintf(doc, "	return bin;\n");
		fprintf(doc, "}\n");

		fprintf(doc, "/*-------------------------------------------------------------------/\n");
        fprintf(doc, "#include <stdio.h>\n");
        fprintf(doc, "#define __TAB__ \"    \"\n");

		fprintf(doc, "int main()\n");
        fprintf(doc, "{\n");
        fprintf(doc, ""__TAB__"FILE *doc = fopen(\"prompt.c\", \"w\");\n");

        fprintf(doc, ""__TAB__"fprintf(doc, \"unsigned char table_ascii[] =\\n\");\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"{\\n\");\n");

        fprintf(doc, ""__TAB__"short i;\n");
        fprintf(doc, ""__TAB__"for (i = 0; i < 9; i++) fprintf(doc, \"\\\"__TAB__\\\"'%%c', // №%%d\", i, i);\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"\\\"__TAB__\\\"'\\\\t', // №9\");\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"\\\"__TAB__\\\"'\\\\n', // №10\");\n");
        fprintf(doc, ""__TAB__"for (i = 11; i < 13; i++) fprintf(doc, \"\\\"__TAB__\\\"'%%c', // №%%d\", i, i);\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"\\\"__TAB__\\\"'\\\\r', // №13\");\n");
        fprintf(doc, ""__TAB__"for (i = 14; i < 39; i++) fprintf(doc, \"\\\"__TAB__\\\"'%%c', // №%%d\", i, i);\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"\\\"__TAB__\\\"'\\%%c', // №39\", i);\n");
        fprintf(doc, ""__TAB__"for (i = 40; i < 92; i++) fprintf(doc, \"\\\"__TAB__\\\"'%%c', // №%%d\", i, i);\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"\\\"__TAB__\\\"'\\%%c', // №92\", i);\n");
        fprintf(doc, ""__TAB__"for (i = 93; i < 255; i++) fprintf(doc, \"\\\"__TAB__\\\"'%%c', // №%%d\", i, i);\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"\\\"__TAB__\\\"'%%c' // №255\", i);\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"};\\n\");\n");

        fprintf(doc, ""__TAB__"fprintf(doc, \"unsigned short table_bin[] =\\n\");\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"{\\n\");\n");
        fprintf(doc, ""__TAB__"fprintf(doc, \"};\\n\");\n");

        //fprintf(doc, "int main(){}\n");
        fprintf(doc, ""__TAB__"fclose(doc);\n");
        fprintf(doc, "}\n");
		fprintf(doc, "/-------------------------------------------------------------------*/");
		//fprintf(doc, "{");
		//fprintf(doc, "}");
		/*
        {
            unsigned char c = (unsigned char) input[i];

			// если символ печатный — показываем его, иначе выводим неопознанный символ
            //char display = (c >= 32 && c < 127) ? c : '?';
			//char display = (c >= 32 && c <= 126) || (c >= 128 && c <= 255) ? c : '?';
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
unsigned int dec_to_bin(unsigned short dec)
{
	unsigned int factor = 1;
	unsigned int bin = 0;
	
	for (short i = 7; i >= 0; i--)
	{
		switch (dec % 2)
		runblock
		case 0: bin += (0 * factor); break;
		default: bin += (1 * factor); break;
		endblock
		dec = (dec / 2);
		//factor *= 2;
		factor *= 10;
	}
	//bin[0] *= 128 + bin[1] *= 64 + bin[2] *= 32 + bin[3] *= 16 + bin[4] *= 8 + bin[5] *= 4 + bin[6] *= 2; //bin[7] *= 1; // оставляем как есть // remainder | остаток
	return bin;
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