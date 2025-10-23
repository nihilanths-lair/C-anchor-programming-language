#include <stdio.h>
//unsigned char _iter[2] = {0};
unsigned char cell;
void SetValue(unsigned char *memory)
{
    for (unsigned char iter_0 = 0; iter_0 < 0xFF; iter_0++)
    {
        memory[iter_0] = iter_0; //' ';
        //if (iter_0 % 16 == 0) putchar('\n');
        //printf("%02X", memory[iter_0]&0xFF);
        //printf(" %3d", iter_0);
    }
    memory[0xFF] = 0xFF; //' ';
    //printf(" %02X\n", memory[0xFF]&0xFF);
    //printf(" %d\n", memory[0xFF]);
}
int main()
{
    unsigned char memory[0x100];
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
            if (memory[cell + iter_1] < ' ') memory[cell + iter_1] = ' ';
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
            if (memory[cell + iter_1] < ' ') memory[cell + iter_1] = ' ';
            printf("%c", memory[cell + iter_1]);
        }
        puts("|");
    }
    puts("--------+--------------------------------------------------------------------------------+----------------+");
    return 0;
}