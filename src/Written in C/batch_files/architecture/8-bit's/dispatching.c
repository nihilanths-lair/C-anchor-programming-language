/**/#define DEBUG/**/
#ifdef DEBUG
unsigned char swh = 1;
#include "..\..\enum_opcode.txt"
#include "..\..\test_code.txt"
#endif
#ifndef DEBUG
static unsigned char memory[0xFFFF+0x01] = {/*Заглушка=*/0xFF}; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения
#endif

static inline void Dispatching()
{
 /*static*/uint8_t ip8 = 0; // Instruction Pointer
 /*static*/uint8_t dp8 = 0; // Data Pointer
 /*static*/uint8_t sp8 = 0; // Stack Pointer
 /*static*/uint8_t bp8 = 0; // Base Pointer

 /*static*/uint8_t si8 = 0; // Source Index
 /*static*/uint8_t di8 = 0; // Destination Index
 /*static*/uint8_t sd8 = 0; // Source Data (аналог SI)
 /*static*/uint8_t dd8 = 0; // Destination Data (аналог DI)

 /*static*/uint8_t cs8 = 0; // Code Segment
 /*static*/uint8_t ss8 = 0; // Stack Segment
 /*static*/uint8_t ds8 = 0; // Data Segment
 /*static*/uint8_t es8 = 0; // Extended Segment

 /*static*/uint8_t    ef8 = 0; // флаг равенства (zf в x86)
 /*static*/uint8_t    bf8 = 0; // (below) флаг меньше
 /*static*/uint8_t    af8 = 0; // (above) флаг больше
 /*static*/uint8_t flags8 = 0; // Совместный регистр требует манипуляции с битами, что может сказаться на производительности исполнения кода ...

 // Основные видимые (8-bit's) регистры общего назначения
 /*static*/uint8_t r8 = 0;
 /*static*/uint8_t a8 = 0, b8 = 0, c8 = 0, d8 = 0; // аналог младших половин 16-bit's регистров общего назначения: al/bl/cl/dl в x86

 // Дополнительные (пока не используются)
 /*static*/uint8_t e8 = 0, f8 = 0;

 // Скрытые (8-bit's) регистры общего назначения
 /*static*/uint8_t temp8_1 = 0, temp8_2 = 0; // используются системой (изменить нельзя)

// Таблица диспетчеризации (8-bit's)
void *dispatch_mode8[0x100] =
{
    ///////////////////////////////////////////////////
    [ 0] = &&dispatch__mode_8__identifier_opcode_1,  // <cmd=INC>       <dst:r8> ; Инкремент регистра r8
    [ 1] = &&dispatch__mode_8__identifier_opcode_2,  // <cmd=INC>       <dst:m8> ; Инкремент текущей ячейки памяти через прямую адресацию
    [ 2] = &&dispatch__mode_8__identifier_opcode_3,  // <cmd=INC> <arg-1=dst:m8> ; Инкремент произвольной ячейки памяти через прямую адресацию
    [ 3] = &&dispatch__mode_8__identifier_opcode_4,  // <cmd=INC> <arg-1=dst:p8> ; Инкремент произвольной ячейки памяти через косвенную адресацию
    ///////////////////////////////////////////////////
    [ 4] = &&dispatch__mode_8__identifier_opcode_5,  // <cmd=DEC>       <dst:r8> ; Декремент регистра r8
    [ 5] = &&dispatch__mode_8__identifier_opcode_6,  // <cmd=DEC> <arg-1=dst:m8> ; Декремент произвольной ячейки памяти через прямую адресацию
    [ 6] = &&dispatch__mode_8__identifier_opcode_7,  // <cmd=DEC> <arg-1=dst:p8> ; Декремент произвольной ячейки памяти через косвенную адресацию
    ///////////////////////////////////////////////////
    [ 7] = &&dispatch__mode_8__identifier_opcode_8,  // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8> ; l-op << r-op
    [ 8] = &&dispatch__mode_8__identifier_opcode_9,  // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8> ; l-op << r-op
    ///////////////////////////////////////////////////
    [ 9] = &&dispatch__mode_8__identifier_opcode_10, // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8> ; l-op << r-op
    [10] = &&dispatch__mode_8__identifier_opcode_11, // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8> ; l-op << r-op
    ///////////////////////////////////////////////////
    [11] = &&dispatch__mode_8__identifier_opcode_12, // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8> ; l-op << r-op
    ///////////////////////////////////////////////////
    [12] = &&dispatch__mode_8__identifier_opcode_13, // <cmd=MOV dst:r8> <arg-1=src:i8> ; l-op = r-op
    [13] = &&dispatch__mode_8__identifier_opcode_14, // <cmd=MOV dst:r8> <arg-1=src:m8> ; l-op = r-op
    [14] = &&dispatch__mode_8__identifier_opcode_15, // <cmd=MOV dst:r8> <arg-1=src:p8> ; l-op = r-op
    ///////////////////////////////////////////////////
    [15] = &&dispatch__mode_8__identifier_opcode_16, // <cmd=MOV src:r8> <arg-1=dst:m8> ; l-op = r-op
    [16] = &&dispatch__mode_8__identifier_opcode_17, // <cmd=MOV src:r8> <arg-1=dst:p8> ; l-op = r-op
    ///////////////////////////////////////////////////
    [17] = &&dispatch__mode_8__identifier_opcode_18, // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8> ; l-op >> r-op
    [18] = &&dispatch__mode_8__identifier_opcode_19, // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8> ; l-op >> r-op
    ///////////////////////////////////////////////////
    [19] = &&dispatch__mode_8__identifier_opcode_20, // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8> ; l-op >> r-op
    [20] = &&dispatch__mode_8__identifier_opcode_21, // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8> ; l-op >> r-op
    ///////////////////////////////////////////////////
    [21] = &&dispatch__mode_8__identifier_opcode_22, // <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8> ; l-op >> r-op
    ///////////////////////////////////////////////////

    // Арифметико-логические операции
    ///////////////////////////////////////////////////
    [22] = &&dispatch__mode_8__identifier_opcode_23, // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8>
    [23] = &&dispatch__mode_8__identifier_opcode_24, // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8>
    [24] = &&dispatch__mode_8__identifier_opcode_25, // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8>
    [25] = &&dispatch__mode_8__identifier_opcode_26, // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8>
    ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////
    // Переход в процедуру (8-bit's)                 //
    [26] = &&dispatch__mode_8__identifier_opcode_27, // <cmd=CALL> <arg-1=src:i8>
    [27] = &&dispatch__mode_8__identifier_opcode_28, // <cmd=CALL> <arg-1=src:m8>
    [28] = &&dispatch__mode_8__identifier_opcode_29, // <cmd=CALL> <arg-1=src:p8>
    // Возврат из процедуры (8-bit's)                //
    [29] = &&dispatch__mode_8__identifier_opcode_30, // <cmd=RET> ; Снимает значение-адрес со стека и совершает переход по нему
    // Управление стеком (8-bit's)                   //
    [30] = &&dispatch__mode_8__identifier_opcode_31, // <cmd=PUSH> <arg-1=src:i8> ; Заносимое значение-аргумент в стек пойдёт из непосредственного источника
    [31] = &&dispatch__mode_8__identifier_opcode_32, // <cmd=PUSH> <arg-1=src:m8> ; Заносимое значение-аргумент в стек пойдёт из памяти по прямому адресу
    [32] = &&dispatch__mode_8__identifier_opcode_33, // <cmd=PUSH> <arg-1=src:p8> ; Заносимое значение-аргумент в стек пойдёт из памяти по косвенному адресу
    //                                               //
    [33] = &&dispatch__mode_8__identifier_opcode_34, // <cmd=POP> <arg-1=dst:m8>  ; Снимаемое значение-аргумент со стека отправится по прямому адресу
    [34] = &&dispatch__mode_8__identifier_opcode_35, // <cmd=POP> <arg-1=dst:p8>  ; Снимаемое значение-аргумент со стека отправится по косвенному адресу
    ///////////////////////////////////////////////////
    // Сравнение (8-bit's)                           //
    [35] = &&dispatch__mode_8__identifier_opcode_36, // <cmd=CMP> <arg-1=src:i8> <arg-2=src:i8>
    [36] = &&dispatch__mode_8__identifier_opcode_37, // <cmd=CMP> <arg-1=src:m8> <arg-2=src:i8>
    [37] = &&dispatch__mode_8__identifier_opcode_38, // <cmd=CMP> <arg-1=src:i8> <arg-2=src:m8>
    [38] = &&dispatch__mode_8__identifier_opcode_39, // <cmd=CMP> <arg-1=src:m8> <arg-2=src:m8>
    //                                               //
    // Безусловный переход (8-bit's)                 //
    [39] = &&dispatch__mode_8__identifier_opcode_40, // <cmd=JMP> <arg-1=src:i8>
    [40] = &&dispatch__mode_8__identifier_opcode_41, // <cmd=JMP> <arg-1=src:m8>
    [41] = &&dispatch__mode_8__identifier_opcode_42, // <cmd=JMP> <arg-1=src:p8>
    ///////////////////////////////////////////////////
    // Условные переходы (8-bit's)                   //
    [42] = &&dispatch__mode_8__identifier_opcode_43, //      <cmd=JE> <arg-1=src:i8>  (Jump if Equal)
    [43] = &&dispatch__mode_8__identifier_opcode_44, //     <cmd=JNE> <arg-1=src:i8>  (Jump if Not Equal)
    [44] = &&dispatch__mode_8__identifier_opcode_45, //      <cmd=JB> <arg-1=src:i8>  (Jump if Below)
    [45] = &&dispatch__mode_8__identifier_opcode_46, //      <cmd=JA> <arg-1=src:i8>  (Jump if Above)
    [46] = &&dispatch__mode_8__identifier_opcode_47, // <cmd=JBE/JNA> <arg-1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
    [47] = &&dispatch__mode_8__identifier_opcode_48, // <cmd=JAE/JNB> <arg-1=src:i8>  (Jump if Above or Equal / Jump if Not Below)
    ///////////////////////////////////////////////////////////////////////
    [48 ... 254] = &&dispatch__mode_8__identifier_opcode_from_49_to_255, // Неопределённые/неизвестные опкоды
    ///////////////////////////////////////////////////////////////////////
    [255] = &&dispatch__mode_8__identifier_opcode_256 // <cmd=HLT>
};

 unsigned char opcode_table[0x100];
 for (unsigned char i = 0; i < 0xFF; i++) opcode_table[i] = i;
 opcode_table[0xFF] = 0xFF;
 CheckTableStatus(dispatch_mode8, opcode_table, sizeof (opcode_table)-1, 1);
 Permutation(dispatch_mode8, opcode_table, sizeof (opcode_table)-1, "");
 CheckTableStatus(dispatch_mode8, opcode_table, sizeof (opcode_table)-1, 2);

#ifdef DEBUG
 printf("\n Starting vCPU (8-bit's mode)...\n");
#endif
 // Стартуем в едином 8-ми битном режиме адресации!
 goto *dispatch_mode8[memory[ip8]];

// #include "..\..\ShowDashboard.txt" ; Режим отладки вынесен в отдельный файл через #ifdef DEBUG тело #endif
/*/
|*|    (Specification: Intel/AT&T, dst)
|*|
|*|    <cmd=INC>       <dst:r8> ; Инкремент регистра r8
|*|    <cmd=INC>       <dst:m8> ; Инкремент текущей ячейки памяти через прямую адресацию
|*|    <cmd=INC> <arg-1=dst:m8> ; Инкремент произвольной ячейки памяти через прямую адресацию
|*|    <cmd=INC> <arg-1=dst:p8> ; Инкремент произвольной ячейки памяти через косвенную адресацию
|*|
|*|    <cmd=DEC>       <dst:r8> ; Декремент регистра r8
|*|    <cmd=DEC> <arg-1=dst:m8> ; Декремент произвольной ячейки памяти через прямую адресацию
|*|    <cmd=DEC> <arg-1=dst:p8> ; Декремент произвольной ячейки памяти через косвенную адресацию
/*/
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_1: // <cmd=INC> <dst:r8>
#include "..\..\ShowDashboard.txt"     //
 r8++;                                 // <dst:r8> ; Инкремент регистра r8
 ip8++;                                //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_2: // <cmd=INC> <dst:m8>
#include "..\..\ShowDashboard.txt"     //
 memory[ip8]++;                        // <dst:m8> ; Инкремент текущей ячейки памяти через прямую адресацию
 ip8++;                                //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_3: // <cmd=INC> <arg-1=dst:m8>
#include "..\..\ShowDashboard.txt"     //
 memory[memory[ip8+1]]++;              // <arg-1=dst:m8> ; Инкремент произвольной ячейки памяти через прямую адресацию
 ip8 += 2;                             //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_4: // <cmd=INC> <arg-1=dst:p8>
#include "..\..\ShowDashboard.txt"     //
 memory[memory[memory[ip8+1]]]++;      // <arg-1=dst:p8> ; Инкремент произвольной ячейки памяти через косвенную адресацию
 ip8 += 2;                             //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////

/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_5: // <cmd=DEC> <dst:r8> ; Декремент регистра r8
#include "..\..\ShowDashboard.txt"     //
 r8--;                                 // <dst:r8> ; Декремент регистра r8
 ip8++;                                //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_6: // <cmd=DEC> <arg-1=dst:m8>
#include "..\..\ShowDashboard.txt"     //
 memory[memory[ip8+1]]--;              // <arg-1=dst:m8> ; Декремент произвольной ячейки памяти через прямую адресацию
 ip8 += 2;                             //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_7: // <cmd=DEC> <arg-1=dst:p8>
#include "..\..\ShowDashboard.txt"     //
 memory[memory[memory[ip8+1]]]--;      // <arg-1=dst:p8> ; Декремент произвольной ячейки памяти через косвенную адресацию
 ip8 += 2;                             //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////

/// SPECIFICATION: INTEL //
//
// [Inserting abstract ASM-code]: mov a, 1 ; Копирование непосредственного (константного) значения по прямому адресу
// [Inserting abstract   C-code]: a = 1;
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_8:  // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8> ; l-op << r-op (Intel)
 memory[memory[ip8+1]] = memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
#include "..\..\ShowDashboard.txt"      //
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [a], b ; Копирование непосредственного (константного) значения по косвенному адресу
// [Inserting abstract   C-code]: *a = b;    ; Предполагается, что в `b` уже находится какое-либо значение
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_9:          // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8> ; l-op << r-op (Intel)
#include "..\..\ShowDashboard.txt"              //
 memory[memory[memory[ip8+1]]] = memory[ip8+2]; // <arg-1=dst:p8> <arg-2=src:i8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////


// [Inserting abstract ASM-code]: mov a, b ; Копирование значения из прямого адреса в прямой
// [Inserting abstract   C-code]: a = b;
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_10:         // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8> ; l-op << r-op (Intel)
#include "..\..\ShowDashboard.txt"              //
 memory[memory[ip8+1]] = memory[memory[ip8+2]]; // <arg-1=dst:m8> <arg-2=src:m8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [a], b ; Копирование значения из прямого адреса в косвенный
// [Inserting abstract   C-code]: *a = b;
//////////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_11:                 // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8> ; l-op << r-op (Intel)
#include "..\..\ShowDashboard.txt"                      //
 memory[memory[memory[ip8+1]]] = memory[memory[ip8+2]]; // <arg-1=dst:p8> <arg-2=src:m8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov a, [b] ; Копирование значения из косвенного адреса в прямой
// [Inserting abstract   C-code]: a = *b;
//////////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_12:                 // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8> ; l-op << r-op (Intel)
#include "..\..\ShowDashboard.txt"                      //
 memory[memory[ip8+1]] = memory[memory[memory[ip8+2]]]; // <arg-1=dst:m8> <arg-2=src:p8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

/// SPECIFICATION: INTEL/AT&T
//
// [Inserting abstract ASM-code]: mov <r8>, i8 ; Копирование непосредственного (константного) значения в регистр
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_13: // <cmd=MOV dst:r8> <arg-1=src:i8> ; l-op = r-op (Intel/AT&T)
#include "..\..\ShowDashboard.txt"      //
 r8 = memory[ip8+1];                    // <arg-0=dst:r8> <arg-1=src:i8> ; Для VM в данной реализации не важно, будет это Intel или AT&T-syntax, т.к. аргумент-значение (опкод) един под оба варианта
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov <r8>, [m8] ; Копирование значения из прямого адреса в регистр
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_14: // <cmd=MOV dst:r8> <arg-1=src:m8> ; l-op = r-op (Intel/AT&T)
#include "..\..\ShowDashboard.txt"      //
 r8 = memory[memory[ip8+1]];            // <arg-0=dst:r8> <arg-1=src:m8> ; Для VM в данной реализации не важно, будет это Intel или AT&T-syntax, т.к. аргумент-значение (опкод) един под оба варианта
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov <r8>, [[p8]] ; Копирование значения из косвенного адреса в регистр
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_15: // <cmd=MOV dst:r8> <arg-1=src:p8> ; l-op = r-op (Intel/AT&T)
#include "..\..\ShowDashboard.txt"      //
 r8 = memory[memory[memory[ip8+1]]];    // <arg-0=dst:r8> <arg-1=src:p8> ; Для VM в данной реализации не важно, будет это Intel или AT&T-syntax, т.к. аргумент-значение (опкод) един под оба варианта
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [m8], <r8> ; Копирование значения из регистра по прямому адресу
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_16: // <cmd=MOV src:r8> <arg-1=dst:m8> ; l-op = r-op (Intel/AT&T)
#include "..\..\ShowDashboard.txt"      //
 memory[memory[ip8+1]] = r8;            // <arg-1=dst:m8> <arg-0=src:r8> ; Для VM в данной реализации не важно, будет это Intel или AT&T-syntax, т.к. аргумент-значение (опкод) един под оба варианта
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [[p8]], <r8> ; Копирование значения из регистра по косвенному адресу
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_17: // <cmd=MOV src:r8> <arg-1=dst:p8> ; l-op = r-op (Intel/AT&T)
#include "..\..\ShowDashboard.txt"      //
 memory[memory[memory[ip8+1]]] = r8;    // <arg-1=dst:p8> <arg-0=src:r8> ; Для VM в данной реализации не важно, будет это Intel или AT&T-syntax, т.к. аргумент-значение (опкод) един под оба варианта
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

/// SPECIFICATION: AT&T
//
// [Inserting abstract ASM-code]: mov a, 1 ; Копирование непосредственного (константного) значения по прямому адресу
// [Inserting abstract   C-code]: a = 1;
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_18: // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8> ; l-op >> r-op (AT&T)
 memory[memory[ip8+2]] = memory[ip8+1]; // <arg-1=src:i8> <arg-2=dst:m8>
#include "..\..\ShowDashboard.txt"      //
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [a], b ; Копирование непосредственного (константного) значения по косвенному адресу
// [Inserting abstract   C-code]: *a = b;    ; Предполагается, что в `b` уже находится какое-либо значение
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_19:         // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8> ; l-op >> r-op (AT&T)
#include "..\..\ShowDashboard.txt"              //
 memory[memory[memory[ip8+2]]] = memory[ip8+1]; // <arg-1=src:i8> <arg-2=dst:p8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov a, b ; Копирование значения из прямого адреса в прямой
// [Inserting abstract   C-code]: a = b;
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_20:         // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8> ; l-op >> r-op (AT&T)
#include "..\..\ShowDashboard.txt"              //
 memory[memory[ip8+2]] = memory[memory[ip8+1]]; // <arg-1=src:m8> <arg-2=dst:m8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [a], b ; Копирование значения из прямого адреса в косвенный
// [Inserting abstract   C-code]: *a = b;
//////////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_21:                 // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8> ; l-op >> r-op (AT&T)
#include "..\..\ShowDashboard.txt"                      //
 memory[memory[memory[ip8+2]]] = memory[memory[ip8+1]]; // <arg-1=src:m8> <arg-2=dst:p8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov a, [b] ; Копирование значения из косвенного адреса в прямой
// [Inserting abstract   C-code]: a = *b;
//////////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_22:                 // <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8> ; l-op >> r-op (AT&T)
#include "..\..\ShowDashboard.txt"                      //
 memory[memory[ip8+2]] = memory[memory[memory[ip8+1]]]; // <arg-1=src:p8> <arg-2=dst:m8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_23:  // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8>
#include "..\..\ShowDashboard.txt"       //
 memory[memory[ip8+1]] += memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_24:  // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8>
#include "..\..\ShowDashboard.txt"       //
 memory[memory[ip8+1]] -= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_25:  // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8>
#include "..\..\ShowDashboard.txt"       //
 memory[memory[ip8+1]] *= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_26:  // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8>
#include "..\..\ShowDashboard.txt"       //
 memory[memory[ip8+1]] /= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_27: // <cmd=CALL> <arg1=src:i8>
#include "..\..\ShowDashboard.txt"      //
 memory[sp8--] = ip8+2;                 // Запоминаем адрес возврата из процедуры
 ip8 = memory[ip8+1];                   // <arg1=src:i8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_28: // <cmd=CALL> <arg1=src:m8>
#include "..\..\ShowDashboard.txt"      //
 memory[sp8--] = ip8+2;                 // Запоминаем адрес возврата из процедуры
 ip8 = memory[memory[ip8+1]];           // <arg1=src:m8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_29: // <cmd=CALL> <arg1=src:p8>
#include "..\..\ShowDashboard.txt"      //
 memory[sp8--] = ip8+2;                 // Запоминаем адрес возврата из процедуры
 ip8 = memory[memory[memory[ip8+1]]];   // <arg1=src:p8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_30: // <cmd=RET>
#include "..\..\ShowDashboard.txt"      //
 ip8 = memory[++sp8];                   // Достаёт адрес возврата и ставит ip8 на него
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_31: // <cmd=PUSH> <arg1=src:i8>
#include "..\..\ShowDashboard.txt"      //
 memory[sp8--] = memory[ip8+1];         // <arg1=src:i8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_32: // <cmd=PUSH> <arg1=src:m8>
#include "..\..\ShowDashboard.txt"      //
 memory[sp8--] = memory[memory[ip8+1]]; // <arg1=src:m8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_33:         // <cmd=PUSH> <arg1=src:p8>
#include "..\..\ShowDashboard.txt"              //
 memory[sp8--] = memory[memory[memory[ip8+1]]]; // <arg1=src:p8>
 ip8 += 2;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_34: // <cmd=POP> <arg1=dst:m8> ; Снимаемое значение-аргумент со стека отправится по прямому адресу
#include "..\..\ShowDashboard.txt"      //
 memory[memory[ip8+1]] = memory[++sp8]; // <arg1=dst:m8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_35:         // <cmd=POP> <arg1=dst:p8> ; Снимаемое значение-аргумент со стека отправится по косвенному адресу
#include "..\..\ShowDashboard.txt"              //
 memory[memory[memory[ip8+1]]] = memory[++sp8]; // <arg1=dst:p8>
 ip8 += 2;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_36: // <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
#include "..\..\ShowDashboard.txt"      //
 temp8_1 = memory[ip8+1];               // <arg1=src:i8>
 temp8_2 = memory[ip8+2];               // <arg2=src:i8>
 ef8 = (temp8_1 == temp8_2);            // ZF (Zero Flag) в x86
 af8 = (temp8_1 > temp8_2);             // JA (Above) / тут какой флаг в x86???
 bf8 = (temp8_1 < temp8_2);             // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_37: // <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
#include "..\..\ShowDashboard.txt"      //
 temp8_1 = memory[memory[ip8+1]];       // <arg1=src:m8>
 temp8_2 = memory[ip8+2];               // <arg2=src:i8>
 ef8 = (temp8_1 == temp8_2);            // ZF (Zero Flag) в x86
 af8 = (temp8_1 > temp8_2);             // JA (Above) / тут какой флаг в x86???
 bf8 = (temp8_1 < temp8_2);             // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_38: // <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
#include "..\..\ShowDashboard.txt"      //
 temp8_1 = memory[ip8+1];               // <arg1=src:i8>
 temp8_2 = memory[memory[ip8+2]];       // <arg2=src:m8>
 ef8 = (temp8_1 == temp8_2);            // ZF (Zero Flag) в x86
 af8 = (temp8_1 > temp8_2);             // JA (Above) / тут какой флаг в x86???
 bf8 = (temp8_1 < temp8_2);             // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_39: // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
#include "..\..\ShowDashboard.txt"      //
 temp8_1 = memory[memory[ip8+1]];       // <arg1=src:m8>
 temp8_2 = memory[memory[ip8+2]];       // <arg2=src:m8>
 ef8 = (temp8_1 == temp8_2);            // ZF (Zero Flag) в x86
 af8 = (temp8_1 > temp8_2);             // JA (Above) / тут какой флаг в x86???
 bf8 = (temp8_1 < temp8_2);             // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_40: // <cmd=JMP> <arg1=src:i8>
#include "..\..\ShowDashboard.txt"      //
 ip8 = memory[ip8+1];                   // <arg1=src:i8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_41: // <cmd=JMP> <arg1=src:m8>
#include "..\..\ShowDashboard.txt"      //
 ip8 = memory[memory[ip8+1]];           // <arg1=src:m8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_42: // <cmd=JMP> <arg1=src:p8>
#include "..\..\ShowDashboard.txt"      //
 ip8 = memory[memory[memory[ip8+1]]];   // <arg1=src:p8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_43: //  JE addr8  (Jump if Equal)
#include "..\..\ShowDashboard.txt"      //
 if (ef8) ip8 = memory[ip8+1];          // JE (Jump if Equal): Прыгаем, если ef == 1
 else ip8 += 2;                         // JE + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_44: // JNE addr8  (Jump if Not Equal)
#include "..\..\ShowDashboard.txt"      //
 if (!ef8) ip8 = memory[ip8+1];         // JNE (Jump if Not Equal): Прыгаем, если ef == 0
 else ip8 += 2;                         // JNE + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_45: //  JB addr8  (Jump if Below)
#include "..\..\ShowDashboard.txt"      //
 if (bf8) ip8 = memory[ip8+1];          // JB (Jump if Below): Прыгаем, если bf == 1 (первое меньше второго)
 else ip8 += 2;                         // JB + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_46: //  JA addr8  (Jump if Above)
#include "..\..\ShowDashboard.txt"      //
 if (af8) ip8 = memory[ip8+1];          // JA (Jump if Above): Прыгаем, если af == 1 (первое больше второго)
 else ip8 += 2;                         // JA + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_47: // JBE/JNA addr8  (Jump if Below or Equal / Jump if Not Above)
#include "..\..\ShowDashboard.txt"      //
 if (bf8 || ef8) ip8 = memory[ip8+1];   // JBE (Jump if Below or Equal)
 else ip8 += 2;                         // JBE + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_48: // JAE/JNB addr8  (Jump if Above or Equal / Jump if Not Below)
#include "..\..\ShowDashboard.txt"      //
 if (af8 || ef8) ip8 = memory[ip8+1];   // JAE/JNB (Jump if Above or Equal / Jump if Not Below)
 else ip8 += 2;                         // JAE/JNB + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_from_49_to_255: // ; Неопределённые опкоды
#include "..\..\ShowDashboard.txt"                  //
 putchar('\n');                                     //
 return;                                            // ; Экстремальный выход
 //goto *dispatch_mode8[memory[++ip8]];             // ; В режиме отладки, для просмотра след. опкода
//////////////////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_256: // <cmd=hlt> ; Остановить/завершить выполнение программы
#include "..\..\ShowDashboard.txt"       //
 return;                                 // ; Заглушка
///////////////////////////////////////////
//
}