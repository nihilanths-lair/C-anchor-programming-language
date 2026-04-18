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
/*static inline*/void Permutation(void *opcode_identifier_table[], unsigned char table_size,/*const char *opcode_table,*/const char *meta_description)
{
    //printf("\n <DEBUG>: CALL Permutation\n"); // Эталонный
    //unsigned char opcode_table[0xFF+1];
    // Парсинг `meta_description` в DOM-структуру: таблица соотношений [идентификатор опкода :: значение опкода]
    unsigned char opcode_configuration_table[0xFF+1][2]; // Первый index идентификатор опкода, второй - значение опкода
    //unsigned char opcode_configuration_table[2][0xFF+1]; // Первый index идентификатор опкода, второй - значение опкода
    enum { identifier, value };
    #define HLT 0x01 // <%1>
    #define JMP 0x02 // <%2>
    #define INC 0x03 // <%3>
    unsigned char opcode_table[] = {INC, JMP, HLT};
    opcode_configuration_table[0][value] = INC; // <%3>
    opcode_configuration_table[1][value] = JMP; // <%2>
    opcode_configuration_table[2][value] = HLT; // <%1>

    void *opcode_identifier_table_address[0xFF+1];
    // Временно пропустим данный этап и предположим у нас уже есть готовая таблица, воспользуемся ей
    //printf("\n\n [До пермутации]:\n"); / гасим
    printf("\n\n             Address, Identifier = Opcode: ASCII | HEX | DEC");
    for (int i = 0; i < sizeof (opcode_table); i++)
    {
        printf("\n\n    [До пермутации]: %ph, <%%%d> = %d | %02Xh | %03dd", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);

        printf("\n opcode_table[%d] = %d", i, opcode_table[i]); // Все опкоды (коды операций)
        opcode_identifier_table[opcode_table[i]] = opcode_identifier_table[i]; // Каждый опкод (код операции) маппим на внутреннюю логику

        printf("\n [После пермутации]: %ph, <%%%d> = %d | %02Xh | %03dd", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);
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
    #define MACRO__TABLE_SIZE 5
    void *opcode_identifier_table[MACRO__TABLE_SIZE] = {
        &&opcode_identifier_1,
        &&opcode_identifier_2,
        &&opcode_identifier_3,
        &&opcode_identifier_4,
        &&opcode_identifier_5
    }; // Таблица идентификаторов опкодов (кодов операций)
    printf("\n [До]: Эталонная таблица идентификаторов опкодов (кодов операций)\n");
    for (unsigned char i = 0; i < MACRO__TABLE_SIZE; i++) printf("\n %ph: <%%%d> = %d | \\h%02X | \\d%03d", opcode_identifier_table[i], i+1, i, i, i);
    goto _0;
    opcode_identifier_1: goto _0;
    opcode_identifier_2: goto _0;
    opcode_identifier_3: goto _0;
    opcode_identifier_4: goto _0;
    opcode_identifier_5: goto _0;
    _0:
    // Пермутация (внедряется в мета-компиляторы и мета-программируемые виртуальные машины, для сборки и привязки динамически модифицируемых/меняющихся опкодов к единой таблице идентификаторов)
    Permutation(opcode_identifier_table, MACRO__TABLE_SIZE, /*opcode_table,*/"");
    printf("\n [После]: Эталонная таблица идентификаторов опкодов (кодов операций)\n");
    for (unsigned char i = 0; i < MACRO__TABLE_SIZE; i++) printf("\n %ph: <%%%d> = %d | \\h%02X | \\d%03d", opcode_identifier_table[i], i+1, i, i, i);
    putchar('\n');

    //Action();
    //#include "..\..\batch_files\action.txt"
}
int main(int argc, char *argv[]) { _(argc, argv); }