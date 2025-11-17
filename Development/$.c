#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define then {
#define end }

unsigned char assembler_source_code[] =
{"\
; Исходный код Assembly\n\
1: jmp 2 ; 001 002\n\
2: jmp 1 ; 001 001\
"};
unsigned char *c_anchor_source_code;
unsigned char *c_anchor_source_code_2;
//--------------------------------------------------//
#include "tape.h"
#include "container.h"
//--------------------------------------------------//
FILE *desc = NULL;
long file_size = 0;
long long iter_0;
long long iter_1;
long long iter_2;
long long iter_3;
long long iter_4;
long long iter_5;
#define bool _Bool
#define false 0
#define true 1
void strpack(char *dest, const char *source, bool remove_whitespace_characters)
{
    iter_1 = 0;
    iter_2 = 0;
    while (source[iter_1] != '\0')
    {
        switch (source[iter_1]) then
        case 10: // Символ переноса строки
        {
            dest[iter_2] = '\\';
            iter_2++;
            dest[iter_2] = 'n';
            iter_2++;
            break;
        }
        case 13: // Символ каретки
        {
            dest[iter_2] = '\\';
            iter_2++;
            dest[iter_2] = 'r';
            iter_2++;
            break;
        }
        case 32: // Символ пробела
        {
            if (remove_whitespace_characters) break;
        }
        default:
        {
            dest[iter_2] = source[iter_1];
            iter_2++;
        }
        end
        iter_1++;
    }
}
void strunpack()
{

}
void zeroing(char *arr, int size)
{
    iter_0 = -1;
    do arr[iter_0] = '\0';
    while (++iter_0 != size);
}
int main()
{
    setlocale(0, "");
    // Открываем файл
    desc = fopen("source_code.ca", "rb");
    // При неудачной попытки открытия файла
    if (desc == NULL) return -1;
    // Узнаем размер файла для создания массива подходящего под хранение данных
    fseek(desc, 0, SEEK_END);
    file_size = ftell(desc);
    //
    puts("--");
    printf("file_size = %ld\n", file_size);
    // Выделяем память для работы с ней
    c_anchor_source_code = malloc(file_size);
    if (c_anchor_source_code == NULL) return -2;
    fseek(desc, 0, SEEK_SET);
    fread(c_anchor_source_code, file_size+1, 1, desc);
    // Закрываем файл
    fclose(desc);
    printf("len = %d\n", strlen(c_anchor_source_code));
    printf("size = %d\n", sizeof (c_anchor_source_code));
    printf("c_anchor_source_code =\n--\n%s\n", c_anchor_source_code);
    puts("--");
    // Выделяем память для работы с ней
    c_anchor_source_code_2 = malloc(file_size*2);
    zeroing(c_anchor_source_code_2, file_size*2);
    strpack(c_anchor_source_code_2, c_anchor_source_code, false);
    printf("c_anchor_source_code_2 =\n--\n%s\n", c_anchor_source_code_2);
    zeroing(c_anchor_source_code_2, file_size*2);
    strpack(c_anchor_source_code_2, c_anchor_source_code, true);
    printf("-\n%s\n", c_anchor_source_code_2);
    printf("--");
    // Освобождаем память когда больше не нужна
    free(c_anchor_source_code_2);
    free(c_anchor_source_code);
    //strcpy(collection, assembler_source_code);
    //strcpy(collection, c_anchor_source_code);
    //*collection = '\0';
    return 0;
}
/*/
// Syntax assembly: [Номер метки:][Символическая мнемоника][Операнд-1][Операнд-2]
-------------------------------------------------------------------------------------------------------
Номер метки является необязательным условием, операнд может быть один, два (а в новой модели и три/четыре), а может вовсе отсутствовать
Ниже ассемблерный код который выполняется бесконечно (зациклен)
0 | 0: #model-1\n\
1 | 1: jmp 2 ; однострочный комментарий | 001 002
2 | 2: jmp 1 ; однострочный комментарий | 001 001
На выходе получим байт-код: 001 002 001 001 в 10-й СС или 01 02 01 01 в 16-й СС
-------------------------------------------------------------------------------------------------------
Зашитые операции байт-кодов (символические мнемоники можно кодировать свои):
001|01 - JMP/Jmp/jmp - Переход (непосредственный прыжок) к метке, сравнимо с телепортацией с одного места в другое, в качестве операнда указывается номер метки
002|02 - MOV/Mov/mov - Поместить в ячейку памяти значение, операнд-1 - номер ячейки куда поместить значение, операнд-2 - само помещаемое значение
--
Операции АЛУ (арифметико-логического устройства)

003|03 - ADD/Add/add - Оператор сложения, операнд-1 является приёмником, операнд-2 источником
004|04 - SUB/Sub/sub - Оператор вычитания, операнд-1 является приёмником, операнд-2 источником
005|05 - MUL/Mul/mul - Оператор умножения, операнд-1 является приёмником, операнд-2 источником
006|06 - DIV/Div/div - Оператор деления, операнд-1 является приёмником, операнд-2 источником
--
007|07 - CALL/Call/call - Вызов процедуры, в качестве операнда выступает адрес процедуры
008|08 - PUSH/Push/push - Положить на верхушку стека (втолкнуть), в качестве операнда выступает аргумент
009|09 - POP/Pop/pop    - Снять с верхушки стека (вытолкнуть), без операндов
010|0A - RET/Ret/ret    - Достать адрес из стека и вернуться к месту находящее сразу же после вызова процедуры
--
011|0B - CMP/Cmp/cmp    - Оператор сравнения, в первый и второй операнд помещаются значения, которые будут сравниваться между собой
012|0C - JNZ/Jnz/jnz / JNE/Jne/jne - Если результат сравнения был не равен 0, то перейти по адресу указываемому в первом операнде
013|0D - JZ/Jz/jz / je/Je/je       - Если результат сравнения был равен 0, то перейти по адресу указываемому в первом операнде
014|0E - LOOP/Loop/loop - Уменьшить ячейку памяти на единицу указываемую в первом операнде и перейти по адресу указываемого во втором операнде
--
015|0F - STUB/Stub/stub - Заглушка, говорящая о том, что ничего делать не надо
--
016|10 - MovePointerForward/GPTR+  - Сместить позицию указателя на шаг вперёд
017|11 - MovePointerBackward/GPTR- - Сместить позицию указателя на шаг назад
--
018|12 - INC/Inc/inc - Оператор инкремента
019|13 - DEC/Dec/dec - Оператор декремента
---------------------------------------------------------------------------------------------------------------------
CPU - ПРОЦЕССОР
 000
[???]
--
MEMORY - ПАМЯТЬ
 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 ... 255 .... 65'535 ... 4'294'967'295
[001][002][001][001][???][???][???][???][???][???][???][???][???][???][???][???]...[???].....[???].........[???]
--
STORAGE - НАКОПИТЕЛЬ
 000 ... ???
[???]...[???]
---------------------------------------------------------------------------------------------------------------------
/*/