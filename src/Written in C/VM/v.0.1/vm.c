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
char meta_description[] =
    " <%1> = 1\n"
    " <%2> = 2\n"
    " <%3> = 3\n"
    " <%4> = 4\n"
    " <%5> = 5\n"
;
// Пермутация (внедряется в мета-компиляторы и мета-программируемые виртуальные машины, для сборки и привязки динамически модифицируемых/меняющихся опкодов к единой таблице идентификаторов)
/*static inline*/void Permutation(void *opcode_identifier_table[], unsigned char *opcode_table, const unsigned char table_size, const char *meta_description)
{
    //printf("\n <DEBUG>: CALL Permutation\n"); // Эталонный
    //
    //printf("\n\n meta_description: \n%s", meta_description);

    #define HLT 0x01
    #define JMP 0x02
    #define INC 0x03

    opcode_table[0] = INC;
    opcode_table[1] = JMP;
    opcode_table[2] = HLT;

    // Парсинг `meta_description` в DOM-структуру: таблица соотношений [идентификатор опкода :: значение опкода]
    #define MACRO__TABLE 2
    unsigned char opcode_configuration_table[0xFF+1][MACRO__TABLE]; // Первый index идентификатор опкода, второй - значение опкода

    enum {identifier, value};

    opcode_configuration_table[0][identifier] = 1;
    opcode_configuration_table[0][value] = INC; // <%1> = 3

    opcode_configuration_table[1][identifier] = 2;
    opcode_configuration_table[1][value] = JMP; // <%2> = 2

    opcode_configuration_table[2][identifier] = 3;
    opcode_configuration_table[2][value] = HLT; // <%3> = 1

    void *opcode_identifier_table_address[0xFF+1];
    for (unsigned char i = 0; i < table_size; i++){
        // делаем резервное копирование перед внесением изменений (на всякий случай, если потребуется хранить первичное состояние)
        opcode_identifier_table_address[i] = opcode_identifier_table[i];
    }
    // Временно пропустим данный этап и предположим у нас уже есть готовая таблица, воспользуемся ей
    printf("\n\n [До пермутации]:");
    for (int i = 0; i < table_size; i++) printf("\n %ph, <%%%d> = %3d | %02Xh | %03dd", opcode_identifier_table[i], i+1, i, i, i);
    putchar('\n');
    for (int i = 0; i < table_size; i++) opcode_identifier_table[opcode_table[i]] = opcode_identifier_table_address[i];
    printf("\n [После пермутации]:");
    for (int i = 0; i < table_size; i++) printf("\n %ph, <%%%d> = %3d | %02Xh | %03dd", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);
    putchar('\n');
    //printf("\n\n <DEBUG>: RET Permutation\n");
}

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
    printf("\n [До]: Эталонная таблица идентификаторов опкодов (кодов операций), начальное состояние\n");
    for (unsigned char i = 0; i < MACRO__TABLE_SIZE; i++) {
        printf("\n %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);
    }
    // Пермутация (внедряется в мета-компиляторы и мета-программируемые виртуальные машины, для сборки и привязки динамически модифицируемых/меняющихся опкодов к единой таблице идентификаторов)
    Permutation(opcode_identifier_table, opcode_table, MACRO__TABLE_SIZE, meta_description);
    printf("\n [После]: Эталонная таблица идентификаторов опкодов (кодов операций), прошедшая процесс пермутации\n");
    for (unsigned char i = 0; i < MACRO__TABLE_SIZE; i++) {
        printf("\n %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);
    }
    putchar('\n');
    unsigned char opcode = 0;
    goto *opcode_identifier_table[opcode];
    opcode_identifier_1: printf("\n opcode_identifier_1 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[0], 1, opcode_table[0], opcode_table[0], opcode_table[0]); goto *opcode_identifier_table[++opcode];
    opcode_identifier_2: printf("\n opcode_identifier_2 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[1], 2, opcode_table[1], opcode_table[1], opcode_table[1]); goto *opcode_identifier_table[++opcode];
    opcode_identifier_3: printf("\n opcode_identifier_3 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[2], 3, opcode_table[2], opcode_table[2], opcode_table[2]); goto *opcode_identifier_table[++opcode];
    opcode_identifier_4: printf("\n opcode_identifier_4 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[3], 4, opcode_table[3], opcode_table[3], opcode_table[3]); goto *opcode_identifier_table[++opcode];
    opcode_identifier_5: printf("\n opcode_identifier_5 = %ph: <%%%d> = %3d | \\h%02X | \\d%03d", opcode_identifier_table[4], 5, opcode_table[4], opcode_table[4], opcode_table[4]); putchar('\n'); return;

    //Action();
    //#include "..\..\batch_files\action.txt"
}
int main(int argc, char *argv[]) { _(argc, argv); }