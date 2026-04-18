// <!-- Virtual Machine C$ v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>
#include <string.h>

#include "..\..\batch_files\ProcAsciiChr.c"
#include "..\..\batch_files\ShowDashboard.c"
//#include "..\..\batch_files\Permutation.c"

#include "..\..\batch_files\action.c"

// Мета-описание для привязки опкодов к своим уникальным адресам/ключам/индексам)
// <%?> - является идентификатором опкода (кода операции)
// ? - является самим опкодом (может задаваться в ascii/hex/dec/bin формах)
char meta_description[] = "\
<%1> = 1\
<%2> = 2\
<%3> = 3\
<%4> = 4\
<%5> = 5\
";
// Пермутация (внедряется в мета-компиляторы и мета-программируемые виртуальные машины, для сборки и привязки динамически модифицируемых/меняющихся опкодов к единой таблице идентификаторов)
/*static inline*/void Permutation(void *opcode_identifier_table[],/*const char *opcode_table,*/const char *meta_description)
{
    //printf("\n <DEBUG>: CALL Permutation\n"); // Эталонный
    char opcode_table[0xFF];
    // Парсинг `meta_description` в DOM-структуру: таблица соотношений [идентификатор опкода :: значение опкода]
    #define HLT 0x01 // <%1>
    #define JMP 0x02 // <%2>
    #define INC 0x03 // <%3>
    opcode_table[0] = INC; // <%3>
    opcode_table[1] = JMP; // <%2>
    opcode_table[2] = HLT; // <%1>
    // Временно пропустим данный этап и предположим у нас уже есть готовая таблица, воспользуемся ей
    //printf("\n\n [До пермутации]:\n"); / гасим
    printf("\n                          Address, Identifier = Opcode: ASCII | HEX | DEC");
    for (int i = 0; i < 3; i++)
    {
        printf("\n\n    [До пермутации]: 0x%p, <%%%d> = %d | %02Xh | %03dd", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);

        //printf("\n opcode_table[%d] = %d", i, opcode_table[i]); // Все опкоды (коды операций)
        //opcode_identifier_table[opcode_table[i]] = opcode_identifier_table[i]; // Каждый опкод (код операции) маппим на внутреннюю логику

        printf("\n [После пермутации]: 0x%p, <%%%d> = %d | %02Xh | %03dd", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);
    }
    putchar('\n');
    //printf("\n\n [После пермутации]:"); / гасим
    //printf("\n\n <DEBUG>: RET Permutation\n");
}
//for (int i = 0; i < sizeof (opcode_configuration_file); i++) current_isa[opcode_configuration_file[i]] = opcode_table[i];
//for (int i = 0; i < sizeof (opcode_configuration_file); i++) printf("\n 0x%p, 0x%p", opcode_table[i], current_isa[i]);

void _()
{
    setlocale(0, "");
    //
    //char opcode_table[] = {}; // Таблица опкодов (или кодов операций)
    void *opcode_identifier_table[] = {
        &&opcode_identifier_1,
        &&opcode_identifier_2,
        &&opcode_identifier_3,
        &&opcode_identifier_4,
        &&opcode_identifier_5
    }; // Таблица идентификаторов опкодов (кодов операций)
    goto _0;
    opcode_identifier_1: printf("\n <%%1> = %d", 1); goto _0;
    opcode_identifier_2: printf("\n <%%2> = %d", 2); goto _0;
    opcode_identifier_3: printf("\n <%%3> = %d", 3); goto _0;
    opcode_identifier_4: printf("\n <%%4> = %d", 4); goto _0;
    opcode_identifier_5: printf("\n <%%5> = %d", 5); goto _0;
    _0:
    // Пермутация (внедряется в мета-компиляторы и мета-программируемые виртуальные машины, для сборки и привязки динамически модифицируемых/меняющихся опкодов к единой таблице идентификаторов)
    Permutation(opcode_identifier_table,/*opcode_table,*/"");

    Action();
    //#include "..\..\batch_files\action.txt"
}
int main(int argc, char *argv[]) { _(argc, argv); }