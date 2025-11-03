//--------------------------------------------------------//
#include <stdio.h>
#include <locale.h>
//#include <windows.h>
//--------------------------------------------------------//
#define runblock {
#define endblock }
//--------------------------------------------------------//
//unsigned char _iter[2] = {0};
unsigned char cell;
unsigned char memory[0x100];
//--------------------------------------------------------//
/*
unsigned char ascii[] =
{"\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\
\0\
ЂЃ‚ѓ„…†‡€‰Љ‹ЊЌЋЏђ‘’“”•–—�™љ›њќћџ ЎўЈ¤Ґ¦§Ё©Є«¬­®Ї°±Ііґµ¶·ё№є»јЅѕїАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя\
"};
*/
//--------------------------------------------------------//
unsigned char ext_ascii[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 'Ё', 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 'ё', '№', 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П',
    'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я',
    'а', 'б', 'в', 'г', 'д', 'е', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п',
    'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я'
};
//--------------------------------------------------------//
void SetValue(unsigned char *memory)//, const unsigned char value
{
    /*
    for (unsigned char iter_0 = '\0'; iter_0 <= 31; iter_0++)
    {
        memory[iter_0] = ' '; //' ';
        //if (iter_0 % 16 == 0) putchar('\n');
        //printf("%02X", memory[iter_0]&0xFF);
        //printf(" %3d", iter_0);
    }
    */
    for (unsigned char iter_0 = '\0'; iter_0 < 0xFF; iter_0++)
    {
        memory[iter_0] = iter_0; //' ';
        //if (iter_0 % 16 == 0) putchar('\n');
        //printf("%02X", memory[iter_0]&0xFF);
        //printf(" %3d", iter_0);
    }
    memory[0xFF] = 0xFF; //' ';
    //printf(" %02X\n", memory[0xFF]&0xFF);
    //printf(" %d\n", memory[0xFF]);
    //memory[0x6] = ' ';
    //memory['\0'] = ' '; // 0
    //memory['\a'] = ' '; // 7
    //memory['\b'] = ' '; // 8
    //memory['\t'] = ' '; // 9
    //memory['\n'] = ' '; // 10
    //memory['\r'] = ' '; // 13
}
//--------------------------------------------------------//
void OutputToConsole()
{
    //SetConsoleCP(1251);       // ввод в CP1251
	//SetConsoleOutputCP(1251); // вывод в CP1251
    setlocale(0, "");
    printf("+--------+----------------------------------------------------------------+----------------+\n");
    printf("| Offset | 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F |     ASCII      |\n");
    printf("+--------+----------------------------------------------------------------+----------------+\n");
    printf("|      0 |"); // Первый ряд
    for (unsigned char iter_0 = 0x0; iter_0 <= 0xF; iter_0++)
    {
        printf("[%02X]", memory[iter_0] = iter_0); // Код (00-0F)
    }
    printf("|");
    for (unsigned char iter_0 = 0x0; iter_0 <= 0xF; iter_0++)
    {
        memory[iter_0] = ' ';
        printf("%c", memory[iter_0]); // ASCII (00-0F)
    }
    printf("|\n");
    printf("|     %2X |", 0x10); // Второй ряд
    for (unsigned char iter_0 = 0x10; iter_0 <= 0x1F; iter_0++)
    {
        printf("[%02X]", memory[iter_0] = iter_0); // Код (10-1F)
        memory[iter_0] = ' ';
    }
    printf("|");
    for (unsigned char iter_0 = 0x10; iter_0 <= 0x1F; iter_0++)
    {
        printf("%c", memory[iter_0]); // ASCII (10-1F)
    }
    printf("|\n");
    for (unsigned char iter_0 = 0x2; iter_0 <= 0xF; iter_0++)
    {
        printf("|     %02X |", cell = iter_0*0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            printf("[%02X]", memory[cell+iter_1] = cell+iter_1);
        }
        printf("|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            if (cell+iter_1 > 0x7E && cell+iter_1 < 0xA8) memory[cell+iter_1] = ' ';
            else if (cell+iter_1 > 0xA8 && cell+iter_1 < 0xB8) memory[cell+iter_1] = ' ';
            else if (cell+iter_1 > 0xB8 && cell+iter_1 < 0xC0) memory[cell+iter_1] = ' ';
            printf("%c", memory[cell+iter_1]);
        }
        printf("|\n");
    }
    printf("+--------+----------------------------------------------------------------+----------------+\n");
    printf("+--------+--------------------------------------------------------------------------------+----------------+\n");
    printf("| Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |\n");
    printf("+--------+--------------------------------------------------------------------------------+----------------+\n");
    printf("|      0 |"); // Первый ряд
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        printf("[%03d]", memory[iter_0] = iter_0);
    }
    printf("|");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        memory[iter_0] = ' ';
        printf("%c", memory[iter_0]);
    }
    printf("|\n");
    printf("|     %d |", 16); // Второй ряд
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        printf("[%03d]", memory[iter_0]);
        memory[iter_0] = ' ';
    }
    printf("|");
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        printf("%c", memory[iter_0]);
    }
    printf("|\n");
    for (unsigned char iter_0 = 0x2; iter_0 <= 0xF; iter_0++)
    {
        printf("|    %3d |", cell = iter_0*0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            printf("[%03d]", memory[cell+iter_1]);
        }
        printf("|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            if (cell+iter_1 > 0x7E && cell+iter_1 < 0xA8) memory[cell+iter_1] = ' ';
            else if (cell+iter_1 > 0xA8 && cell+iter_1 < 0xB8) memory[cell+iter_1] = ' ';
            else if (cell+iter_1 > 0xB8 && cell+iter_1 < 0xC0) memory[cell+iter_1] = ' ';
            printf("%c", memory[cell+iter_1]);
        }
        printf("|\n");
    }
    printf("+--------+--------------------------------------------------------------------------------+----------------+");
}
//--------------------------------------------------------//
void OutputToFile(const char *name)
{
    FILE *doc = fopen(name, "w");
    fprintf(doc, "+--------+----------------------------------------------------------------+----------------+\n");
    fprintf(doc, "| Offset | 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F |     ASCII      |\n");
    fprintf(doc, "+--------+----------------------------------------------------------------+----------------+\n");
    fprintf(doc, "|      0 |");
    for (unsigned char iter_0 = 0; iter_0 <= 0xF; iter_0++)
    {
        fprintf(doc, "[%02X]", memory[iter_0] = iter_0);
        memory[iter_0] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 0x0; iter_0 <= 0xF; iter_0++)
    {
        fprintf(doc, "%c", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    fprintf(doc, "|     %2X |", 0x10);
    for (unsigned char iter_0 = 0x10; iter_0 <= 0x1F; iter_0++)
    {
        fprintf(doc, "[%02X]", memory[iter_0] = iter_0);
        memory[iter_0] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 0x10; iter_0 <= 0x1F; iter_0++)
    {
        fprintf(doc, "%c", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    for (unsigned char iter_0 = 0x2; iter_0 <= 0xF; iter_0++)
    {
        fprintf(doc, "|     %02X |", cell = iter_0*0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%02X]", memory[cell+iter_1] = cell+iter_1);
        }
        fprintf(doc, "|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            if (cell+iter_1 > 0x7E && cell+iter_1 < 0xA8) memory[cell+iter_1] = ' ';
            else if (cell+iter_1 > 0xA8 && cell+iter_1 < 0xB8) memory[cell+iter_1] = ' ';
            else if (cell+iter_1 > 0xB8 && cell+iter_1 < 0xC0) memory[cell+iter_1] = ' ';
            fprintf(doc, "%c", memory[cell+iter_1]);
        }
        fprintf(doc, "|\n");
    }
    fprintf(doc, "+--------+----------------------------------------------------------------+----------------+\n");
    fprintf(doc, "+--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, "| Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |\n");
    fprintf(doc, "+--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, "|      0 |");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        memory[iter_0] = iter_0;
        fprintf(doc, "[%03d]", memory[iter_0] = iter_0);
        memory[iter_0] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        fprintf(doc, "%c", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    fprintf(doc, "|     %d |", 16);
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        fprintf(doc, "[%03d]", memory[iter_0] = iter_0);
        memory[iter_0] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        fprintf(doc, "%c", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    for (unsigned char iter_0 = 0x2; iter_0 <= 0xF; iter_0++)
    {
        fprintf(doc, "|    %3d |", cell = iter_0*0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%03d]", memory[cell+iter_1] = cell+iter_1);
        }
        fprintf(doc, "|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            if (cell+iter_1 > 0x7E && cell+iter_1 < 0xA8) memory[cell+iter_1] = ' ';
            else if (cell+iter_1 > 0xA8 && cell+iter_1 < 0xB8) memory[cell+iter_1] = ' ';
            else if (cell+iter_1 > 0xB8 && cell+iter_1 < 0xC0) memory[cell+iter_1] = ' ';
            fprintf(doc, "%c", memory[cell+iter_1]);
        }
        fprintf(doc, "|\n");
    }
    /*
    fprintf(doc, "\n# # # #\n");
    fprintf(doc, "unsigned char ext_ascii[] =\n{\n");
    cell = 0;
    for (unsigned char iter_0 = 0x0; iter_0 < 0xF; iter_0++)
    {
        //cell = iter_0 * 0x10;
        fprintf(doc, "    ");
        for (unsigned char iter_1 = 0; iter_1 < 0xF; iter_1++)
        {
            //if (cell < 0x20) { fprintf(doc, "0x%02X, ", cell++); continue; }
            if (cell < 0x1F || cell > 0x7E && cell < 0xA8 || cell > 0xA8 && cell < 0xB8 || cell > 0xB9 && cell < 0xC0) fprintf(doc, "0x%02X, ", cell++);
            else if (cell == '\'' || cell == '\\') fprintf(doc, "'\\%c', ", cell++);
            else fprintf(doc, "'%c', ", cell++);
        }
        switch (cell)
        runblock
        case 0x0F: case 0x1F: case 0x7F: case 0x8F: case 0x9F: case 0xAF: case 0xBF: fprintf(doc, "0x%02X,\n", cell++); break;
        default: fprintf(doc, "'%c',\n", cell++);
        endblock
        //if (iter_0 % 0x10 == 0) { fprintf(doc, "\n    ", iter_0); continue; }
    }
    fprintf(doc, "    ");
    for (unsigned char iter_0 = 0x0; iter_0 < 0xF; iter_0++)
    {
        fprintf(doc, "'%c', ", cell++);
    }
    fprintf(doc, "'%c'\n};", cell++);
    //fprintf(doc, "'%c'\n};", 0xFF);
    fprintf(doc, "\n# # # #\n");
    */
    /*
    for (unsigned char iter_0 = 0x7; iter_0 < 0x8; iter_0++)
    {
        fprintf(doc, "    %3d |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%3d]", memory[cell + iter_1]);
        }
        fprintf(doc, "|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "%c", memory[cell + iter_1]);
        }
        fprintf(doc, "|\n");
    }
    */
    fprintf(doc, "+--------+--------------------------------------------------------------------------------+----------------+");
    /*
    fprintf(doc, "void *case_[] =\n{\n");
    for (unsigned char iter_0 = '\0'; iter_0 < 0xF; iter_0++)
    {
        fprintf(doc, "    ");
        for (unsigned char iter_1 = '\0'; iter_1 < 0xF; iter_1++)
        {
            fprintf(doc, "&&_0x%02X, ", iter_0*0xF+iter_1);
        }
        fprintf(doc, "\n");
    }
    fprintf(doc, "};\n");
    //fprintf(doc, "//_0x__:
    goto *case_[cell+iter_1];");
    for (unsigned char iter_0 = '\0'; iter_0 < 0xF; iter_0++)
    {
        for (unsigned char iter_1 = '\0'; iter_1 < 0xF; iter_1++)
        {
            fprintf(doc, "_0x%02X:", iter_0*0xF+iter_1);
        }
        fprintf(doc, "\n");
    }
    */
    fclose(doc);
}
//--------------------------------------------------------//
int main()
{
    OutputToConsole();
    OutputToFile("0.txt");
    return 0;
}
//--------------------------------------------------------//