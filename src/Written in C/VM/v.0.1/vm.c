// <!-- Virtual Machine C$ v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>
#include <string.h>

#include "..\..\batch_files\ProcAsciiChr.c"
#include "..\..\batch_files\ShowDashboard.c"
#include "..\..\batch_files\Permutation.c"

#include "..\..\batch_files\action.c"

// Мета-описание для привязки опкодов к своим уникальным адресам/ключам/индексам)
// <%?> - является идентификатором опкода (кода операции)
// ? - является самим опкодом (может задаваться в ascii/hex/dec/bin формах)
char meta_description[] =
    " <%1> = 1\n"
    " <%2> = 2\n"
    " <%3> = 3\n"
    " <%4> = 4\n"
    " <%5> = 5\n"
;

void _()
{
    setlocale(0, "");
    //
    #define MACRO__TABLE_SIZE 5
    unsigned char opcode_table[MACRO__TABLE_SIZE] = {
        0, 1, 2, 3, 4
    }; // Таблица опкодов (или кодов операций)
    void *opcode_identifier_table[MACRO__TABLE_SIZE] = {
        &&opcode_identifier_1,
        &&opcode_identifier_2,
        &&opcode_identifier_3,
        &&opcode_identifier_4,
        &&opcode_identifier_5
    }; // Таблица идентификаторов опкодов (или кодов операций)
    CheckTableStatus(opcode_identifier_table, opcode_table, sizeof (opcode_table));
    Permutation(opcode_identifier_table, opcode_table, sizeof (opcode_table), meta_description);
    CheckTableStatus(opcode_identifier_table, opcode_table, sizeof (opcode_table));
    goto _0;

    opcode_identifier_1:
     printf("\n opcode_identifier_1 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[0], 1, opcode_table[0], opcode_table[0], opcode_table[0]);
    goto _0;

    opcode_identifier_2:
     printf("\n opcode_identifier_2 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[1], 2, opcode_table[1], opcode_table[1], opcode_table[1]);
    goto _0;

    opcode_identifier_3:
     printf("\n opcode_identifier_3 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[2], 3, opcode_table[2], opcode_table[2], opcode_table[2]);
    goto _0;

    opcode_identifier_4:
     printf("\n opcode_identifier_4 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[3], 4, opcode_table[3], opcode_table[3], opcode_table[3]);
    goto _0;

    opcode_identifier_5:
     printf("\n opcode_identifier_5 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[4], 5, opcode_table[4], opcode_table[4], opcode_table[4]);
    goto _0;
    
    _0:
    //Action();
    //#include "..\..\batch_files\action.txt"
}
int main(int argc, char *argv[]) { _(argc, argv); }