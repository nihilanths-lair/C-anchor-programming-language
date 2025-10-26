#include <stdio.h>
//#define __void_filler
#define runblock {
#define endblock }
//unsigned char _iter[2] = {0};
unsigned char cell;
unsigned char memory[0x100];
unsigned char ascii[] =
{"\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\
\0\
ЂЃ‚ѓ„…†‡€‰Љ‹ЊЌЋЏђ‘’“”•–—�™љ›њќћџ ЎўЈ¤Ґ¦§Ё©Є«¬­®Ї°±Ііґµ¶·ё№є»јЅѕїАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя\
"};
/*
unsigned char ascii[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};
*/
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
void OutputToConsole()
{
    SetValue(memory);
    puts("--------+----------------------------------------------------------------+----------------+");
    puts(" Offset | 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F |     ASCII      |");
    puts("--------+----------------------------------------------------------------+----------------+");
    for (unsigned char iter_0 = 0; iter_0 <= 0xF; iter_0++)
    {
        printf("     %2X |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            printf("[%2X]", memory[cell + iter_1] & 0xFF);
        }
        putchar('|');
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            //if (memory[cell + iter_1] == '\t' || memory[cell + iter_1] == '\n' || memory[cell + iter_1] == '\r') memory[cell + iter_1] = ' ';
            //if (memory[cell + iter_1] < ' ' || memory[cell + iter_1] == 0x7F || memory[cell + iter_1] == 0x98) memory[cell + iter_1] = ' ';
            printf("%c", memory[cell + iter_1]);
        }
        puts("|");
    }
    puts("--------+----------------------------------------------------------------+----------------+");
    SetValue(memory);
    puts("--------+--------------------------------------------------------------------------------+----------------+");
    puts(" Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |");
    puts("--------+--------------------------------------------------------------------------------+----------------+");
    for (unsigned char iter_0 = 0; iter_0 <= 0xF; iter_0++)
    {
        printf("    %3d |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            printf("[%3d]", memory[cell + iter_1]);
        }
        putchar('|');
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            //if (memory[cell + iter_1] == '\t' || memory[cell + iter_1] == '\n' || memory[cell + iter_1] == '\r') memory[cell + iter_1] = ' ';
            //if (memory[cell + iter_1] < ' ' || memory[cell + iter_1] == 0x7F || memory[cell + iter_1] == 0x98) memory[cell + iter_1] = ' ';
            printf("%c", memory[cell + iter_1]);
        }
        puts("|");
    }
    puts("--------+--------------------------------------------------------------------------------+----------------+");
}
void OutputToFile(const char *name)
{
    FILE *doc = fopen(name, "w");
    SetValue(memory);
    fprintf(doc, "--------+----------------------------------------------------------------+----------------+\n");
    fprintf(doc, " Offset | 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F |     ASCII      |\n");
    fprintf(doc, "--------+----------------------------------------------------------------+----------------+\n");
    for (unsigned char iter_0 = 0; iter_0 <= 0xF; iter_0++)
    {
        fprintf(doc, "     %2X |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%2X]", memory[cell + iter_1] & 0xFF);
        }
        fprintf(doc, "|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            //if (memory[cell + iter_1] == '\t' || memory[cell + iter_1] == '\n' || memory[cell + iter_1] == '\r') memory[cell + iter_1] = ' ';
            //if (memory[cell + iter_1] < ' ' || memory[cell + iter_1] == 0x7F || memory[cell + iter_1] == 0x98) memory[cell + iter_1] = ' ';
            fprintf(doc, "%c", memory[cell + iter_1]);
        }
        fprintf(doc, "|\n");
    }
    fprintf(doc, "--------+----------------------------------------------------------------+----------------+\n");
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, " Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |\n");
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, "      0 |");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        memory[iter_0] = iter_0;
        fprintf(doc, "[%3d]", memory[iter_0]);
        memory[iter_0] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        fprintf(doc, "%c", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    fprintf(doc, "     16 |");
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        fprintf(doc, "[%3d]", memory[iter_0]);
        memory[iter_0] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        fprintf(doc, "%c", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    for (unsigned char iter_0 = 0x2; iter_0 < 0xF; iter_0++)
    {
        fprintf(doc, "    %3d |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%3d]", memory[cell + iter_1]);
        }
        fprintf(doc, "|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            switch (cell + iter_1)
            runblock
            case 0x7F: memory[cell+iter_1] = ' '; break;
            case 0x80: memory[cell+iter_1] = ' '; break;
            case 0x81: memory[cell+iter_1] = ' '; break;
            case 0x82: memory[cell+iter_1] = ' '; break;
            case 0x83: memory[cell+iter_1] = ' '; break;
            case 0x84: memory[cell+iter_1] = ' '; break;
            case 0x85: memory[cell+iter_1] = ' '; break;
            case 0x86: memory[cell+iter_1] = ' '; break;
            case 0x87: memory[cell+iter_1] = ' '; break;
            endblock
            fprintf(doc, "%c", memory[cell + iter_1]);
        }
        fprintf(doc, "|\n");
    }
    fprintf(doc, "\n# # # #\n");
    fprintf(doc, "unsigned char ascii[] =\n{\n");
    cell = 0;
    for (unsigned char iter_0 = 0x0; iter_0 < 0xF; iter_0++)
    {
        //cell = iter_0 * 0x10;
        fprintf(doc, "    ");
        for (unsigned char iter_1 = 0; iter_1 < 0xF; iter_1++)
        {
            fprintf(doc, "0x%02X, ", cell++);
        }
        fprintf(doc, "0x%02X,\n", cell++);
        /*
        if (iter_0 < 0x10) { fprintf(doc, "0x%02X, ", iter_0); continue; }
        if (iter_0 % 0x10 == 0) { fprintf(doc, "\n    ", iter_0); continue; }
        if (iter_0 < 0x20) { fprintf(doc, "0x%X, ", iter_0); continue; }
        if (iter_0 == '\'' || iter_0 == '\\') { fprintf(doc, "'\\%c', ", iter_0); continue; }
        //if (iter_0 == '\\') { fprintf(doc, "'\\%c', ", iter_0); continue; }
        if (iter_0 > 0x7E && iter_0 < 0xA8 || iter_0 > 0xA8 && iter_0 < 0xB8 || iter_0 > 0xB9 && iter_0 < 0xC0) { fprintf(doc, "0x%X,", iter_0); continue; }
        //if (iter_0 > 0xA8 && iter_0 < 0xB8) { fprintf(doc, "0x%X, ", iter_0); continue; }
        fprintf(doc, "'%c', ", iter_0);
        */
    }
    fprintf(doc, "    ");
    for (unsigned char iter_0 = 0x0; iter_0 < 0xF; iter_0++)
    {
        fprintf(doc, "0x%02X, ", cell++);
    }
    fprintf(doc, "0x%02X\n};", cell++);
    //fprintf(doc, "'%c'\n};", 0xFF);
    fprintf(doc, "\n# # # #\n");
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
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+");
    fclose(doc);
}
int main()
{
    OutputToConsole();
    OutputToFile("0.logged");
    return 0;
}