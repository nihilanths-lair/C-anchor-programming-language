#include <stdio.h>
unsigned char collection[2048] =
{"\
1: jmp 2\n\
2: jmp 1\
"};
int main()
{
    //*collection = '\0';
    printf(collection);
    return 0;
}
// syntax assembly: [метка:][мнемоника][операнд-1][операнд-2]
/*/
    1 | 1: jmp 2 | jmp 2
    2 | 2: jmp 1 | jmp 0
/*/