/**/#define DEBUG/**/
#ifdef DEBUG
unsigned char swh = 1;
#include "enum_opcode.txt"
#include "test_code.txt"
#endif
#ifndef DEBUG
static unsigned char memory[0xFFFF+0x01] = {/*Заглушка=*/0xFF}; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения
#endif
#define run_block {
#define end_block }
static inline void Action()
//
run_block
// Указатель на сектор / sector pointer //
/*static*/unsigned char      ip8  = 0; // Instruction pointer  8-bit's
/*static*/unsigned short     ip16 = 0; // Instruction pointer 16-bit's
/*static*/unsigned int       ip24 = 0; // Instruction pointer 24-bit's
/*static*/unsigned int       ip32 = 0; // Instruction pointer 32-bit's
/*static*/unsigned long long ip64 = 0; // Instruction pointer 64-bit's
/*static*/unsigned long long ip   = 0; // Один общий `ip` для каждого режима битности адресов (битовые операции)

/*static*/unsigned char      sp8  = 0-1; // Stack pointer  8-bit's
/*static*/unsigned short     sp16 = 0-1; // Stack pointer 16-bit's
/*static*/unsigned int       sp24 = 0-1; // Stack pointer 24-bit's
/*static*/unsigned int       sp32 = 0-1; // Stack pointer 32-bit's
/*static*/unsigned long long sp64 = 0-1; // Stack pointer 64-bit's
/*static*/unsigned long long sp   = 0-1; // Один общий `sp` для каждого режима битности адресов (битовые операции)

/*static*/unsigned char cs8  = 0; //  Code segment 8-bit's
/*static*/unsigned char ss8  = 0; // Stack segment 8-bit's
/*static*/unsigned char ds8  = 0; //  Data segment 8-bit's

/*static*/unsigned short cs16 = 0; //  Code segment 16-bit's
/*static*/unsigned short ss16 = 0; // Stack segment 16-bit's
/*static*/unsigned short ds16 = 0; //  Data segment 16-bit's

// Скорее всего переделаю на работу с битовыми операциями (т.е. один 8-bit's регистр, вместо нескольких) ; [Заметки]: Как скажется на производительности?
/*static*/unsigned char     ef8  = 0; // флаг равенства (zf в x86)
/*static*/unsigned char     bf8  = 0; // (below) флаг меньше / в x86 какой флаг?
/*static*/unsigned char     af8  = 0; // (above) флаг больше / в x86 какой флаг?
/*static*/unsigned char  flags8  = 0; // Объединяет все флаги (которые выше) в один 8-bit's регистр (для удобности манипулирования можно добавить макросы)
/*static*/unsigned short flags16 = 0; // Расширенный 16-bit's регистр флагов

// Основные видимые (8/16/32-bit's) регистры общего назначения
/*static*/unsigned char      r8  = 0;
/*static*/unsigned short     r16 = 0;
/*static*/unsigned int       r32 = 0;
/*static*/unsigned long long r64 = 0;
// [Под x86] В будущем понадобятся для генерации из байт-кода в машинный код (вынос в отдельные программы, в эмулятор/компилятор)
/*static*/unsigned char      a8  = 0, b8  = 0, c8  = 0, d8  = 0; // аналог  8-bit's GPR, как на старых (ранних) процессорах, один 8-bit's регистр, вместо двух как в 16-bit's
/*static*/unsigned short     a16 = 0, b16 = 0, c16 = 0, d16 = 0; // аналог 16-bit's GPR ( ax,  bx,  cx,  dx), можно обращаться к отдельным младшим 8-bit's половинам
/*static*/unsigned int       a32 = 0, b32 = 0, c32 = 0, d32 = 0; // аналог 32-bit's GPR (eax, ebx, ecx, edx), можно обращаться к отдельным младшим 16/8-bit's половинам
/*static*/unsigned long long a64 = 0, b64 = 0, c64 = 0, d64 = 0; // аналог 64-bit's GPR (rax, rbx, rcx, rdx), можно обращаться к отдельным младшим 32/16/8-bit's половинам
// Дополнительные
/*static*/unsigned char e8 = 0, f8 = 0; // пока не используются

// Скрытые (8-bit's) регистры общего назначения
/*static*/unsigned char temp8_1 = 0, temp8_2 = 0; // используются системой (изменить нельзя)

// Таблица диспетчеризации I (для 8-ти битного режима адресации)
void *dispatch_mode8[0x100] =
{
    [ 0] = &&dispatch__mode_8__identifier_opcode_1,  // <cmd=INC> <arg-1=dst:m8>
    [ 1] = &&dispatch__mode_8__identifier_opcode_2,  // <cmd=DEC> <arg-1=dst:m8>
    //                                               //
    [ 2] = &&dispatch__mode_8__identifier_opcode_3,  // <cmd=INC> <arg-1=dst:p8>
    [ 3] = &&dispatch__mode_8__identifier_opcode_4,  // <cmd=DEC> <arg-1=dst:p8>
    //                                               //
    //                                               //
    [ 4] = &&dispatch__mode_8__identifier_opcode_5,  // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8> ; l-op << r-op
    [ 5] = &&dispatch__mode_8__identifier_opcode_6,  // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8> ; l-op << r-op
    //                                               //
    [ 6] = &&dispatch__mode_8__identifier_opcode_7,  // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8> ; l-op << r-op
    [ 7] = &&dispatch__mode_8__identifier_opcode_8,  // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8> ; l-op << r-op
    //                                               //
    [ 8] = &&dispatch__mode_8__identifier_opcode_9,  // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8> ; l-op << r-op
    //                                               //
    [ 9] = &&dispatch__mode_8__identifier_opcode_10, // <cmd=MOV> <arg-1=src:i8> <arg-0=dst:r8> ; l-op = r-op
    [10] = &&dispatch__mode_8__identifier_opcode_11, // <cmd=MOV> <arg-1=src:m8> <arg-0=dst:r8> ; l-op = r-op
    [11] = &&dispatch__mode_8__identifier_opcode_12, // <cmd=MOV> <arg-1=src:p8> <arg-0=dst:r8> ; l-op = r-op
    //                                               //
    [12] = &&dispatch__mode_8__identifier_opcode_13, // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8> ; l-op >> r-op
    [13] = &&dispatch__mode_8__identifier_opcode_14, // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8> ; l-op >> r-op
    //                                               //
    [14] = &&dispatch__mode_8__identifier_opcode_15, // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8> ; l-op >> r-op
    [15] = &&dispatch__mode_8__identifier_opcode_16, // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8> ; l-op >> r-op
    //                                               //
    [16] = &&dispatch__mode_8__identifier_opcode_17, // <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8> ; l-op >> r-op

    // Арифметико-логические операции
    ///////////////////////////////////////////////////
    [17] = &&dispatch__mode_8__identifier_opcode_18, // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8>
    [18] = &&dispatch__mode_8__identifier_opcode_19, // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8>
    [19] = &&dispatch__mode_8__identifier_opcode_20, // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8>
    [20] = &&dispatch__mode_8__identifier_opcode_21, // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8>
    ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////
    // Переход в процедуру (8-bit's)                 //
    [21] = &&dispatch__mode_8__identifier_opcode_22, // <cmd=CALL> <arg-1=src:i8>
    [22] = &&dispatch__mode_8__identifier_opcode_23, // <cmd=CALL> <arg-1=src:m8>
    [23] = &&dispatch__mode_8__identifier_opcode_24, // <cmd=CALL> <arg-1=src:p8>
    // Возврат из процедуры (8-bit's)                //
    [24] = &&dispatch__mode_8__identifier_opcode_25, // <cmd=RET> ; Снимает значение-адрес со стека и совершает переход по нему
    // Управление стеком (8-bit's)                   //
    [25] = &&dispatch__mode_8__identifier_opcode_26, // <cmd=PUSH> <arg-1=src:i8> ; Заносимое значение-аргумент в стек пойдёт из непосредственного источника
    [26] = &&dispatch__mode_8__identifier_opcode_27, // <cmd=PUSH> <arg-1=src:m8> ; Заносимое значение-аргумент в стек пойдёт из памяти по прямому адресу
    [27] = &&dispatch__mode_8__identifier_opcode_28, // <cmd=PUSH> <arg-1=src:p8> ; Заносимое значение-аргумент в стек пойдёт из памяти по косвенному адресу
    //                                               //
    [28] = &&dispatch__mode_8__identifier_opcode_29, // <cmd=POP> <arg-1=dst:m8>  ; Снимаемое значение-аргумент со стека отправится по прямому адресу
    [29] = &&dispatch__mode_8__identifier_opcode_30, // <cmd=POP> <arg-1=dst:p8>  ; Снимаемое значение-аргумент со стека отправится по косвенному адресу
    ///////////////////////////////////////////////////
    // Сравнение (8-bit's)                           //
    [30] = &&dispatch__mode_8__identifier_opcode_31, // <cmd=CMP> <arg-1=src:i8> <arg-2=src:i8>
    [31] = &&dispatch__mode_8__identifier_opcode_32, // <cmd=CMP> <arg-1=src:m8> <arg-2=src:i8>
    [32] = &&dispatch__mode_8__identifier_opcode_33, // <cmd=CMP> <arg-1=src:i8> <arg-2=src:m8>
    [33] = &&dispatch__mode_8__identifier_opcode_34, // <cmd=CMP> <arg-1=src:m8> <arg-2=src:m8>
    //                                               //
    // Безусловный переход (8-bit's)                 //
    [34] = &&dispatch__mode_8__identifier_opcode_35, // <cmd=JMP> <arg-1=src:i8>
    [35] = &&dispatch__mode_8__identifier_opcode_36, // <cmd=JMP> <arg-1=src:m8>
    [36] = &&dispatch__mode_8__identifier_opcode_37, // <cmd=JMP> <arg-1=src:p8>
    ///////////////////////////////////////////////////
    // Условные переходы (8-bit's)                   //
    [37] = &&dispatch__mode_8__identifier_opcode_38, //      <cmd=JE> <arg-1=src:i8>  (Jump if Equal)
    [38] = &&dispatch__mode_8__identifier_opcode_39, //     <cmd=JNE> <arg-1=src:i8>  (Jump if Not Equal)
    [39] = &&dispatch__mode_8__identifier_opcode_40, //      <cmd=JB> <arg-1=src:i8>  (Jump if Below)
    [40] = &&dispatch__mode_8__identifier_opcode_41, //      <cmd=JA> <arg-1=src:i8>  (Jump if Above)
    [41] = &&dispatch__mode_8__identifier_opcode_42, // <cmd=JBE/JNA> <arg-1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
    [42] = &&dispatch__mode_8__identifier_opcode_43, // <cmd=JAE/JNB> <arg-1=src:i8>  (Jump if Above or Equal / Jump if Not Below)
    ///////////////////////////////////////////////////
    [43 ... 253] = &&dispatch__mode_8__identifier_opcode_from_44_to_254, // Неопределённые/неизвестные опкоды
    [254] = &&dispatch__mode_8__identifier_opcode_255, // <cmd=?> ; переключение режима адресации (с 8 на 16)
    [255] = &&dispatch__mode_8__identifier_opcode_256  // <cmd=HLT>
};
// Таблица диспетчеризации II (для 16-ти битного режима адресации)
void *dispatch_mode16[0x100] =
{
    [0 ... 252] = &&dispatch__mode_16__identifier_opcode_from_1_to_253, // Неопределённые/неизвестные опкоды
    [253] = &&dispatch__mode_16__identifier_opcode_254, // <cmd=?> ; переключение режима адресации (с 16 на 8)
    [254] = &&dispatch__mode_16__identifier_opcode_255, // <cmd=?> ; переключение режима адресации (с 16 на 24)
    [255] = &&dispatch__mode_8__identifier_opcode_256   // <cmd=HLT>
};
// Таблица диспетчеризации III (для 24-х битного режима адресации)
void *dispatch_mode24[0x100] =
{
    [0 ... 252] = &&dispatch__mode_24__identifier_opcode_from_1_to_253, // Неопределённые/неизвестные опкоды
    [253] = &&dispatch__mode_24__identifier_opcode_254, // <cmd=?> ; переключение режима адресации (с 24 на 16)
    [254] = &&dispatch__mode_24__identifier_opcode_255, // <cmd=?> ; переключение режима адресации (с 24 на 32)
    [255] = &&dispatch__mode_8__identifier_opcode_256   // <cmd=HLT>
};
// Таблица диспетчеризации IV (для 32-х битного режима адресации)
void *dispatch_mode32[0x100] =
{
    [0 ... 252] = &&dispatch__mode_32__identifier_opcode_from_1_to_253, // Неопределённые/неизвестные опкоды
    [253] = &&dispatch__mode_32__identifier_opcode_254, // <cmd=?> ; переключение режима адресации (с 32 на 24)
    [254] = &&dispatch__mode_32__identifier_opcode_255, // <cmd=?> ; переключение режима адресации (с 32 на 64)
    [255] = &&dispatch__mode_8__identifier_opcode_256   // <cmd=HLT>
};
// Таблица диспетчеризации V (для 64-х битного режима адресации)
void *dispatch_mode64[0x100] =
{
    [0 ... 253] = &&dispatch__mode_64__identifier_opcode_from_1_to_254, // Неопределённые/неизвестные опкоды
    [254] = &&dispatch__mode_64__identifier_opcode_255, // <cmd=?> ; переключение режима адресации (с 64 на 32)
    [255] = &&dispatch__mode_8__identifier_opcode_256   // <cmd=HLT>
};
#ifdef DEBUG
 printf("\n Starting vCPU (8-bit's mode)...\n");
#endif
 // Стартуем в 8-ми битном режиме адресации! (Определяется конфигурацией VM)
 goto *dispatch_mode8[memory[ip8]];
#ifdef DEBUG
 printf("\n Starting vCPU (16-bit's mode)...\n");
#endif
 // Стартуем в 16-ти битном режиме адресации! (Определяется конфигурацией VM)
 //goto *dispatch_mode16[memory[ip16]]; // Пока заглушка
#ifdef DEBUG
 printf("\n Starting vCPU (24-bit's mode)...\n");
#endif
 // Стартуем в 24-х битном режиме адресации! (Определяется конфигурацией VM)
 //goto *dispatch_mode24[memory[ip24]]; // Пока заглушка
#ifdef DEBUG
 printf("\n Starting vCPU (32-bit's mode)...\n");
#endif
 // Стартуем в 32-х битном режиме адресации! (Определяется конфигурацией VM)
 //goto *dispatch_mode32[memory[ip32]]; // Пока заглушка
#ifdef DEBUG
 printf("\n Starting vCPU (64-bit's mode)...\n");
#endif
 // Стартуем в 64-х битном режиме адресации! (Определяется конфигурацией VM)
 //goto *dispatch_mode64[memory[ip64]]; // Пока заглушка

/*for (unsigned char i = 39; i <= 254; i++) // Генератор
{
    //printf("\n[%d] = &&__dispatch_mode8__opcode_%03d__,", i-1, i);
    //printf("\n__dispatch_mode8__opcode_%03d__:", i);
}*/

// #include "ShowDashboard.txt" ; Режим отладки вынесен в отдельный файл через #ifdef DEBUG тело #endif
/*/
|*|    (Specification: Intel/AT&T, dst)
|*|
|*|     0- 1 <cmd=INC> <arg-1=dst:m8>
|*|     1- 2 <cmd=INC> <arg-1=dst:p8>
/*/
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_1: // <cmd=INC> <arg-1=dst:m8>
#include "ShowDashboard.txt"           //
 memory[memory[ip8+1]]++;              // <arg-1=dst:m8> ; Произвольная ЯП
 ip8 += 2;                             //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_2: // <cmd=INC> <arg-1=dst:p8>
#include "ShowDashboard.txt"           //
 memory[memory[memory[ip8+1]]]++;      // <arg-1=dst:p8> ; Произвольная ЯП
 ip8 += 2;                             //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////
/*/
|*|    (Specification: Intel/AT&T, dst)
|*|
|*|     2- 3 <cmd=DEC> <arg-1=dst:m8>
|*|     3- 4 <cmd=DEC> <arg-1=dst:p8>
/*/
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_3: // <cmd=DEC> <arg-1=dst:m8>
#include "ShowDashboard.txt"           //
 memory[memory[ip8+1]]--;              // <arg-1=dst:m8> ; Произвольная ЯП
 ip8 += 2;                             //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////
/////////////////////////////////////////
dispatch__mode_8__identifier_opcode_4: // <cmd=DEC> <arg-1=dst:p8>
#include "ShowDashboard.txt"           //
 memory[memory[memory[ip8+1]]]--;      // <arg-1=dst:p8> ; Произвольная ЯП
 ip8 += 2;                             //
 goto *dispatch_mode8[memory[ip8]];    //
/////////////////////////////////////////

/// SPECIFICATION: INTEL //
//
// [Inserting abstract ASM-code]: mov a, 1 ; Копирование непосредственного (константного) значения по прямому адресу
// [Inserting abstract   C-code]: a = 1;
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_5:  // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8> ; l-op << r-op (Intel)
 memory[memory[ip8+1]] = memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"            //
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [a], b ; Копирование непосредственного (константного) значения по косвенному адресу
// [Inserting abstract   C-code]: *a = b;    ; Предполагается, что в `b` уже находится какое-либо значение
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_6:          // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8> ; l-op << r-op (Intel)
#include "ShowDashboard.txt"                    //
 memory[memory[memory[ip8+1]]] = memory[ip8+2]; // <arg-1=dst:p8> <arg-2=src:i8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////


// [Inserting abstract ASM-code]: mov a, b ; Копирование значения из прямого адреса в прямой
// [Inserting abstract   C-code]: a = b;
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_7:          // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8> ; l-op << r-op (Intel)
#include "ShowDashboard.txt"                    //
 memory[memory[ip8+1]] = memory[memory[ip8+2]]; // <arg-1=dst:m8> <arg-2=src:m8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [a], b ; Копирование значения из прямого адреса в косвенный
// [Inserting abstract   C-code]: *a = b;
//////////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_8:                  // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8> ; l-op << r-op (Intel)
#include "ShowDashboard.txt"                            //
 memory[memory[memory[ip8+1]]] = memory[memory[ip8+2]]; // <arg-1=dst:p8> <arg-2=src:m8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov a, [b] ; Копирование значения из косвенного адреса в прямой
// [Inserting abstract   C-code]: a = *b;
//////////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_9:                  // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8> ; l-op << r-op (Intel)
#include "ShowDashboard.txt"                            //
 memory[memory[ip8+1]] = memory[memory[memory[ip8+2]]]; // <arg-1=dst:m8> <arg-2=src:p8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

/// SPECIFICATION: INTEL/AT&T
//
// [Inserting abstract ASM-code]: mov <r8>, i8 ; Копирование непосредственного (константного) значения в регистр
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_10: // <cmd=MOV> <dst:r8> <arg-1=src:i8> ; l-op = r-op (Intel/AT&T)
#include "ShowDashboard.txt"            //
 r8 = memory[ip8+1];                    // <dst:r8> <arg-1=src:i8> ; Для VM в данной реализации не важно, будет это Intel или AT&T-syntax, т.к. аргумент-значение (опкод) един под оба варианта
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov <r8>, [m8] ; Копирование значения из прямого адреса в регистр
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_11: // <cmd=MOV> <dst:r8> <arg-1=src:m8> ; l-op = r-op (Intel/AT&T)
#include "ShowDashboard.txt"            //
 r8 = memory[memory[ip8+1]];            // <dst:r8> <arg-1=src:m8> ; Для VM в данной реализации не важно, будет это Intel или AT&T-syntax, т.к. аргумент-значение (опкод) един под оба варианта
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov <r8>, [[p8]] ; Копирование значения из косвенного адреса в регистр
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_12: // <cmd=MOV> <dst:r8> <arg-1=src:p8> ; l-op = r-op (Intel/AT&T)
#include "ShowDashboard.txt"            //
 r8 = memory[memory[memory[ip8+1]]];    // <dst:r8> <arg-1=src:p8> ; Для VM в данной реализации не важно, будет это Intel или AT&T-syntax, т.к. аргумент-значение (опкод) един под оба варианта
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

/// SPECIFICATION: AT&T
//
// [Inserting abstract ASM-code]: mov a, 1 ; Копирование непосредственного (константного) значения по прямому адресу
// [Inserting abstract   C-code]: a = 1;
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_13: // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8> ; l-op >> r-op (AT&T)
 memory[memory[ip8+2]] = memory[ip8+1]; // <arg-1=src:i8> <arg-2=dst:m8>
#include "ShowDashboard.txt"            //
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [a], b ; Копирование непосредственного (константного) значения по косвенному адресу
// [Inserting abstract   C-code]: *a = b;    ; Предполагается, что в `b` уже находится какое-либо значение
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_14:         // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8> ; l-op >> r-op (AT&T)
#include "ShowDashboard.txt"                    //
 memory[memory[memory[ip8+2]]] = memory[ip8+1]; // <arg-1=src:i8> <arg-2=dst:p8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov a, b ; Копирование значения из прямого адреса в прямой
// [Inserting abstract   C-code]: a = b;
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_15:         // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8> ; l-op >> r-op (AT&T)
#include "ShowDashboard.txt"                    //
 memory[memory[ip8+2]] = memory[memory[ip8+1]]; // <arg-1=src:m8> <arg-2=dst:m8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov [a], b ; Копирование значения из прямого адреса в косвенный
// [Inserting abstract   C-code]: *a = b;
//////////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_16:                 // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8> ; l-op >> r-op (AT&T)
#include "ShowDashboard.txt"                            //
 memory[memory[memory[ip8+2]]] = memory[memory[ip8+1]]; // <arg-1=src:m8> <arg-2=dst:p8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

// [Inserting abstract ASM-code]: mov a, [b] ; Копирование значения из косвенного адреса в прямой
// [Inserting abstract   C-code]: a = *b;
//////////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_17:                 // <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8> ; l-op >> r-op (AT&T)
#include "ShowDashboard.txt"                            //
 memory[memory[ip8+2]] = memory[memory[memory[ip8+1]]]; // <arg-1=src:p8> <arg-2=dst:m8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_18:  // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"             //
 memory[memory[ip8+1]] += memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_19:  // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"             //
 memory[memory[ip8+1]] -= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_20:  // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"             //
 memory[memory[ip8+1]] *= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_21:  // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"             //
 memory[memory[ip8+1]] /= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_22: // <cmd=CALL> <arg1=src:i8>
#include "ShowDashboard.txt"            //
 memory[sp8--] = ip8+2;                 // Запоминаем адрес возврата из процедуры
 ip8 = memory[ip8+1];                   // <arg1=src:i8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_23: // <cmd=CALL> <arg1=src:m8>
#include "ShowDashboard.txt"            //
 memory[sp8--] = ip8+2;                 // Запоминаем адрес возврата из процедуры
 ip8 = memory[memory[ip8+1]];           // <arg1=src:m8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_24: // <cmd=CALL> <arg1=src:p8>
#include "ShowDashboard.txt"            //
 memory[sp8--] = ip8+2;                 // Запоминаем адрес возврата из процедуры
 ip8 = memory[memory[memory[ip8+1]]];   // <arg1=src:p8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_25: // <cmd=RET>
#include "ShowDashboard.txt"            //
 ip8 = memory[++sp8];                   // Достаёт адрес возврата и ставит ip8 на него
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_26: // <cmd=PUSH> <arg1=src:i8>
#include "ShowDashboard.txt"            //
 memory[sp8--] = memory[ip8+1];         // <arg1=src:i8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_27: // <cmd=PUSH> <arg1=src:m8>
#include "ShowDashboard.txt"            //
 memory[sp8--] = memory[memory[ip8+1]]; // <arg1=src:m8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_28:         // <cmd=PUSH> <arg1=src:p8>
#include "ShowDashboard.txt"                    //
 memory[sp8--] = memory[memory[memory[ip8+1]]]; // <arg1=src:p8>
 ip8 += 2;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_29: // <cmd=POP> <arg1=dst:m8> ; Снимаемое значение-аргумент со стека отправится по прямому адресу
#include "ShowDashboard.txt"            //
 memory[memory[ip8+1]] = memory[++sp8]; // <arg1=dst:m8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_30:         // <cmd=POP> <arg1=dst:p8> ; Снимаемое значение-аргумент со стека отправится по косвенному адресу
#include "ShowDashboard.txt"                    //
 memory[memory[memory[ip8+1]]] = memory[++sp8]; // <arg1=dst:p8>
 ip8 += 2;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_31: // <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
#include "ShowDashboard.txt"            //
 temp8_1 = memory[ip8+1];               // <arg1=src:i8>
 temp8_2 = memory[ip8+2];               // <arg2=src:i8>
 ef8 = (temp8_1 == temp8_2);            // ZF (Zero Flag) в x86
 af8 = (temp8_1 > temp8_2);             // JA (Above) / тут какой флаг в x86???
 bf8 = (temp8_1 < temp8_2);             // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_32: // <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
#include "ShowDashboard.txt"            //
 temp8_1 = memory[memory[ip8+1]];       // <arg1=src:m8>
 temp8_2 = memory[ip8+2];               // <arg2=src:i8>
 ef8 = (temp8_1 == temp8_2);            // ZF (Zero Flag) в x86
 af8 = (temp8_1 > temp8_2);             // JA (Above) / тут какой флаг в x86???
 bf8 = (temp8_1 < temp8_2);             // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_33: // <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
#include "ShowDashboard.txt"            //
 temp8_1 = memory[ip8+1];               // <arg1=src:i8>
 temp8_2 = memory[memory[ip8+2]];       // <arg2=src:m8>
 ef8 = (temp8_1 == temp8_2);            // ZF (Zero Flag) в x86
 af8 = (temp8_1 > temp8_2);             // JA (Above) / тут какой флаг в x86???
 bf8 = (temp8_1 < temp8_2);             // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_34: // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
#include "ShowDashboard.txt"            //
 temp8_1 = memory[memory[ip8+1]];       // <arg1=src:m8>
 temp8_2 = memory[memory[ip8+2]];       // <arg2=src:m8>
 ef8 = (temp8_1 == temp8_2);            // ZF (Zero Flag) в x86
 af8 = (temp8_1 > temp8_2);             // JA (Above) / тут какой флаг в x86???
 bf8 = (temp8_1 < temp8_2);             // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_35: // <cmd=JMP> <arg1=src:i8>
#include "ShowDashboard.txt"            //
 ip8 = memory[ip8+1];                   // <arg1=src:i8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_36: // <cmd=JMP> <arg1=src:m8>
#include "ShowDashboard.txt"            //
 ip8 = memory[memory[ip8+1]];           // <arg1=src:m8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_37: // <cmd=JMP> <arg1=src:p8>
#include "ShowDashboard.txt"            //
 ip8 = memory[memory[memory[ip8+1]]];   // <arg1=src:p8>
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_38: //  JE addr8  (Jump if Equal)
#include "ShowDashboard.txt"            //
 if (ef8) ip8 = memory[ip8+1];          // JE (Jump if Equal): Прыгаем, если ef == 1
 else ip8 += 2;                         // JE + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_39: // JNE addr8  (Jump if Not Equal)
#include "ShowDashboard.txt"            //
 if (!ef8) ip8 = memory[ip8+1];         // JNE (Jump if Not Equal): Прыгаем, если ef == 0
 else ip8 += 2;                         // JNE + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_40: //  JB addr8  (Jump if Below)
#include "ShowDashboard.txt"            //
 if (bf8) ip8 = memory[ip8+1];          // JB (Jump if Below): Прыгаем, если bf == 1 (первое меньше второго)
 else ip8 += 2;                         // JB + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_41: //  JA addr8  (Jump if Above)
#include "ShowDashboard.txt"            //
 if (af8) ip8 = memory[ip8+1];          // JA (Jump if Above): Прыгаем, если af == 1 (первое больше второго)
 else ip8 += 2;                         // JA + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_42: // JBE/JNA addr8  (Jump if Below or Equal / Jump if Not Above)
#include "ShowDashboard.txt"            //
 if (bf8 || ef8) ip8 = memory[ip8+1];   // JBE (Jump if Below or Equal)
 else ip8 += 2;                         // JBE + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
dispatch__mode_8__identifier_opcode_43: // JAE/JNB addr8  (Jump if Above or Equal / Jump if Not Below)
#include "ShowDashboard.txt"            //
 if (af8 || ef8) ip8 = memory[ip8+1];   // JAE/JNB (Jump if Above or Equal / Jump if Not Below)
 else ip8 += 2;                         // JAE/JNB + addr8
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

//////////////////////////////////////////////////////
dispatch__mode_8__identifier_opcode_from_44_to_254: // ; Неопределённые опкоды
#include "ShowDashboard.txt"                        //
 putchar('\n');                                     //
 return;                                            // ; Экстремальный выход
 //goto *dispatch_mode8[memory[++ip8]];             // ; В режиме отладки, для просмотра след. опкода
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
dispatch__mode_16__identifier_opcode_from_1_to_253: // ; Неопределённые опкоды
#include "ShowDashboard.txt"                        //
 putchar('\n');                                     //
 return;                                            // ; Экстремальный выход
 //goto *dispatch_mode16[memory[++ip16]];           // ; В режиме отладки, для просмотра след. опкода
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
dispatch__mode_24__identifier_opcode_from_1_to_253: // ; Неопределённые опкоды
#include "ShowDashboard.txt"                        //
 putchar('\n');                                     //
 return;                                            // ; Экстремальный выход
 //goto *dispatch_mode24[memory[++ip24]];           // ; В режиме отладки, для просмотра след. опкода
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
dispatch__mode_32__identifier_opcode_from_1_to_253: // ; Неопределённые опкоды
#include "ShowDashboard.txt"                        //
 putchar('\n');                                     //
 return;                                            // ; Экстремальный выход
//goto *dispatch_mode32[memory[++ip32]];            // ; В режиме отладки, для просмотра след. опкода
//////////////////////////////////////////////////////
dispatch__mode_64__identifier_opcode_from_1_to_254: // ; Неопределённые опкоды
#include "ShowDashboard.txt"                        //
 putchar('\n');                                     //
 return;                                            // ; Экстремальный выход
//goto *dispatch_mode64[memory[++ip64]];            // ; В режиме отладки, для просмотра след. опкода
//////////////////////////////////////////////////////

///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_255: // <id_op=255, smb_mnc=?> ; Переход с 8-ми в 16-ти битный режим адресации
#include "ShowDashboard.txt"             //
 ip16 = ip8;                             //
 sp16 = sp8;                             //
 goto *dispatch_mode16[ip16];            //
///////////////////////////////////////////
////////////////////////////////////////////
dispatch__mode_16__identifier_opcode_254: // <id_op=254, smb_mnc=?> ; Переход с 16-ти в 8-ми битный режим адресации
#include "ShowDashboard.txt"              //
 ip8 = ip16;                              //
 sp8 = sp16;                              //
 goto *dispatch_mode8[ip8];               //
////////////////////////////////////////////
////////////////////////////////////////////
dispatch__mode_16__identifier_opcode_255: // <id_op=255, smb_mnc=?> ; Переход с 16-ти в 24-х битный режим адресации
#include "ShowDashboard.txt"              //
 ip24 = ip16;                             //
 sp24 = sp16;                             //
 goto *dispatch_mode24[ip24];             //
////////////////////////////////////////////
////////////////////////////////////////////
dispatch__mode_24__identifier_opcode_254: // <id_op=255, smb_mnc=?> ; Переход с 24-х в 16-х битный режим адресации
#include "ShowDashboard.txt"              //
 ip16 = ip24;                             //
 sp16 = sp24;                             //
 goto *dispatch_mode16[ip16];             //
////////////////////////////////////////////
////////////////////////////////////////////
dispatch__mode_24__identifier_opcode_255: // <id_op=255, smb_mnc=?> ; Переход с 24-х в 32-х битный режим адресации
#include "ShowDashboard.txt"              //
 ip32 = ip24;                             //
 sp32 = sp24;                             //
 goto *dispatch_mode32[ip32];             //
////////////////////////////////////////////
////////////////////////////////////////////
dispatch__mode_32__identifier_opcode_254: // <id_op=255, smb_mnc=?> ; Переход с 32-х в 24-х битный режим адресации
#include "ShowDashboard.txt"              //
 ip24 = ip32;                             //
 sp24 = sp32;                             //
 goto *dispatch_mode24[ip24];             //
////////////////////////////////////////////
////////////////////////////////////////////
dispatch__mode_32__identifier_opcode_255: // <id_op=255, smb_mnc=?> ; Переход с 32-х в 64-х битный режим адресации
#include "ShowDashboard.txt"              //
 ip64 = ip32;                             //
 sp64 = sp32;                             //
 goto *dispatch_mode64[ip64];             //
////////////////////////////////////////////
////////////////////////////////////////////
dispatch__mode_64__identifier_opcode_255: // <id_op=255, smb_mnc=?> ; Переход с 64-х в 32-х битный режим адресации
#include "ShowDashboard.txt"              //
 ip32 = ip64;                             //
 sp32 = sp64;                             //
 goto *dispatch_mode32[ip32];             //
////////////////////////////////////////////
///////////////////////////////////////////
dispatch__mode_8__identifier_opcode_256: // <cmd=hlt> ; Остановить/завершить выполнение программы
#include "ShowDashboard.txt"             //
 return;                                 // ; Заглушка
///////////////////////////////////////////
//
end_block