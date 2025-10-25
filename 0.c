#include <stdio.h>
//unsigned char _iter[2] = {0};
unsigned char cell;
unsigned char memory[0x100];
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
    SetValue(memory);
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, " Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |\n");
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, "      0 |");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        fprintf(doc, "[%3d]", iter_0);
        //memory[iter_1] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        memory[iter_0] = ' ';
        fprintf(doc, "[%c]", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    fprintf(doc, "     16 |");
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        fprintf(doc, "[%3d]", iter_0);
        //memory[iter_1] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        memory[iter_0] = ' ';
        fprintf(doc, "[%c]", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    for (unsigned char iter_0 = 0x2; iter_0 <= 0xF; iter_0++)
    {
        fprintf(doc, "    %3d |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%3d]", memory[cell + iter_1]);
        }
        fprintf(doc, "|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            //if (memory[cell + iter_1] == '\t' || memory[cell + iter_1] == '\n' || memory[cell + iter_1] == '\r') memory[cell + iter_1] = ' ';
            //if (memory[cell + iter_1] < ' ' || memory[cell + iter_1] == 0x7F || memory[cell + iter_1] == 0x98) memory[cell + iter_1] = ' ';
            fprintf(doc, "['%c']", memory[cell + iter_1]);
        }
        fprintf(doc, "|\n");
    }
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+");
    fclose(doc);
}
int main()
{
    OutputToConsole();
    OutputToFile("0.logged");
    return 0;
}