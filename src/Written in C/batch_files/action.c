#define DEBUG 1
#ifdef DEBUG
#include "enum_opcode.txt"
static unsigned char memory[0xFFFF+0x01] = // Для быстрого теста/проверки работоспобности движка
{
    // start:
    mov8_dm_si, 10, 5,    // 0: mov [10] 5
    inc8_dm,    10,       // 3: inc [10]
    mov8_dm_sm, 20, 10,   // 5: mov [20] [10]
    add8_dm_si, 20, 3,    // 8: add [20], 3
    cmp8_sm_si, 20, 9,    // 11: cmp [20], 9
    je8_si,     19,       // 14: je  equal (адрес 19)
    hlt,                  // 16: hlt (если не равно)
    // equal:
    call8_si,   21,       // 17: call proc (адрес 21)
    hlt,                  // 19: hlt
    // proc:
    push8_si,   42,       // 21: push 42
    pop8_dm,    30,       // 23: pop [30]
    mov8_dr_si, 123,      // 25: mov <r8> 123
    ret                   // 28: ret
};
#endif
#ifndef DEBUG
static unsigned char memory[0xFFFF+0x01] = {/*Заглушка=*/hlt}; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения
#endif
#define run_block {
#define end_block }
static inline void Action()
run_block
// Указатель на сектор / sector pointer //
    /*static*/unsigned char      ip8  = 0x00;               // Instruction pointer  8-bit's
    /*static*/unsigned short     ip16 = 0x0000;             // Instruction pointer 16-bit's
    /*static*/unsigned int       ip32 = 0x00000000;         // Instruction pointer 32-bit's
    /*static*/unsigned long long ip64 = 0x0000000000000000; // Instruction pointer 64-bit's

    /*static*/unsigned char      sp8  = 0xFF;               // Stack pointer  8-bit's
    /*static*/unsigned short     sp16 = 0xFFFF;             // Stack pointer 16-bit's
    /*static*/unsigned int       sp32 = 0xFFFFFFFF;         // Stack pointer 32-bit's
    /*static*/unsigned long long sp64 = 0xFFFFFFFFFFFFFFFF; // Stack pointer 64-bit's

    /*static*/unsigned char  cs8  = 0x00; // Code segment 8-bit's
    /*static*/unsigned char  ss8  = 0x00; // Stack segment 8-bit's
    /*static*/unsigned char  ds8  = 0x00; // Data segment 8-bit's

    // Скорее всего переделаю на работу с битовыми операциями (т.е. один 8-bit's регистр, вместо нескольких) ; [Заметки]: Как скажется на производительности?
    /*static*/unsigned char ef/*zf*/ = 0; // флаг равенства
    /*static*/unsigned char bf       = 0; // (below) флаг меньше / в x86 какой флаг?
    /*static*/unsigned char af       = 0; // (above) флаг больше / в x86 какой флаг?
    /*static*/unsigned char   flag8  = 0; // Объединяет все флаги (которые выше) в один 8-bit's регистр (для удобности манипулирования можно добавить макросы)
    /*static*/unsigned short flags16 = 0; // Расширенный 16-bit's регистр флагов

    // Основные видимые (8/16/32-bit's) регистры общего назначения
    /*static*/unsigned char      r8  = 0x00;
    /*static*/unsigned short     r16 = 0x0000;
    /*static*/unsigned int       r32 = 0x00000000;
    /*static*/unsigned long long r64 = 0x0000000000000000;
    // [Под x86] В будущем понадобятся для генерации из байт-кода в машинный код (вынос в отдельные программы, в эмулятор/компилятор)
    /*static*/unsigned char       a8,  b8,  c8,  d8 = 0x00;               // аналог  8-bit's GPR, как на старых (ранних) процессорах, один 8-bit's регистр, вместо двух как в 16-bit's
    /*static*/unsigned short     a16, b16, c16, d16 = 0x0000;             // аналог 16-bit's GPR ( ax,  bx,  cx,  dx), можно обращаться к отдельным младшим 8-bit's половинам
    /*static*/unsigned int       a32, b32, c32, d32 = 0x00000000;         // аналог 32-bit's GPR (eax, ebx, ecx, edx), можно обращаться к отдельным младшим 16/8-bit's половинам
    /*static*/unsigned long long a64, b64, c64, d64 = 0x0000000000000000; // аналог 64-bit's GPR (rax, rbx, rcx, rdx), можно обращаться к отдельным младшим 32/16/8-bit's половинам
    // Дополнительные
    /*static*/unsigned char e8 = 0x00; // пока не используется
    /*static*/unsigned char f8 = 0x00; // пока не используется

    // Скрытые (8-bit's) регистры общего назначения
    /*static*/unsigned char temp8_1, temp8_2 = 0; // используются системой (изменить нельзя)

    // Таблица диспетчеризации I (для 8-ти битного режима адресации)
    void *dispatch_mode8[0x100] =
    {
        [ 0] = &&__dispatch_mode8__opcode_001__, // <cmd=INC> <arg-1=dst:m8>
        [ 1] = &&__dispatch_mode8__opcode_002__, // <cmd=DEC> <arg-1=dst:m8>

        [ 2] = &&__dispatch_mode8__opcode_003__, // <cmd=INC> <arg-1=dst:p8>
        [ 3] = &&__dispatch_mode8__opcode_004__, // <cmd=DEC> <arg-1=dst:p8>
        //
        [ 4] = &&__dispatch_mode8__opcode_005__, // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8> ; L <~ R (Intel)
        [ 5] = &&__dispatch_mode8__opcode_006__, // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8> ; L ~> R (AT&T)
        //
        [ 6] = &&__dispatch_mode8__opcode_007__, // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8> ; L <~ R (Intel)
        [ 7] = &&__dispatch_mode8__opcode_008__, // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8> ; L ~> R (AT&T)
        //
        [ 8] = &&__dispatch_mode8__opcode_009__, // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8> ; L <~ R (Intel)
        [ 9] = &&__dispatch_mode8__opcode_010__, // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8> ; L ~> R (AT&T)
        //
        [10] = &&__dispatch_mode8__opcode_011__, // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8> ; L <~ R (Intel)
        [11] = &&__dispatch_mode8__opcode_012__, // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8> ; L ~> R (AT&T)
        //
        [12] = &&__dispatch_mode8__opcode_013__, // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8> ; L <~ R (Intel)
        [13] = &&__dispatch_mode8__opcode_014__, // <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8> ; L ~> R (AT&T)
        //
        ////////////////////////////////////
        // Арифметико-логические операции //
        [14] = &&__dispatch_mode8__opcode_015__, // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8>
        //
        [15] = &&__dispatch_mode8__opcode_016__, // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8>
        //
        [16] = &&__dispatch_mode8__opcode_017__, // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8>
        //
        [17] = &&__dispatch_mode8__opcode_018__, // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8>
        //
        ////////////////////////////////////
        // Переход в процедуру (8-bit's)  //
        [18] = &&__dispatch_mode8__opcode_019__,   //   <cmd=CALL> <arg-1=src:i8>
        [19] = &&__dispatch_mode8__opcode_020__,   //   <cmd=CALL> <arg-1=src:m8>
        [20] = &&__dispatch_mode8__opcode_021__,   //   <cmd=CALL> <arg-1=src:p8>
        // Возврат из процедуры (8-bit's) //
        [21] = &&__dispatch_mode8__opcode_022__,   //   <cmd=RET>                 ; Снимает значение-адрес со стека и совершает переход по нему
        // Управление стеком (8-bit's)    //
        [22] = &&__dispatch_mode8__opcode_023__,   //   <cmd=PUSH> <arg-1=src:i8> ; Заносимое значение-аргумент в стек пойдёт из непосредственного источника
        [23] = &&__dispatch_mode8__opcode_024__,   //   <cmd=PUSH> <arg-1=src:m8> ; Заносимое значение-аргумент в стек пойдёт из памяти по прямому адресу
        [24] = &&__dispatch_mode8__opcode_025__,   //   <cmd=PUSH> <arg-1=src:p8> ; Заносимое значение-аргумент в стек пойдёт из памяти по косвенному адресу
        //                                //
        [25] = &&__dispatch_mode8__opcode_026__,   //   <cmd=POP> <arg-1=dst:m8>  ; Снимаемое значение-аргумент со стека отправится по прямому адресу
        [26] = &&__dispatch_mode8__opcode_027__,   //   <cmd=POP> <arg-1=dst:p8>  ; Снимаемое значение-аргумент со стека отправится по косвенному адресу
        ///////////////////////////////////
        // Сравнение (8-bit's)           //
        [27] = &&__dispatch_mode8__opcode_028__,  //     <cmd=CMP> <arg-1=src:i8> <arg-2=src:i8>
        [28] = &&__dispatch_mode8__opcode_029__,  //     <cmd=CMP> <arg-1=src:m8> <arg-2=src:i8>
        [29] = &&__dispatch_mode8__opcode_030__,  //     <cmd=CMP> <arg-1=src:i8> <arg-2=src:m8>
        [30] = &&__dispatch_mode8__opcode_031__,  //     <cmd=CMP> <arg-1=src:m8> <arg-2=src:m8>
        ///////////////////////////////////
        // Безусловный переход (8-bit's) //
        [31] = &&__dispatch_mode8__opcode_032__,  //     <cmd=JMP> <arg-1=src:i8>
        [32] = &&__dispatch_mode8__opcode_033__,  //     <cmd=JMP> <arg-1=src:m8>
        [33] = &&__dispatch_mode8__opcode_034__,  //     <cmd=JMP> <arg-1=src:p8>
        ///////////////////////////////////
        // Условные переходы (8-bit's)   //
        [34] = &&__dispatch_mode8__opcode_035__,  //      <cmd=JE> <arg-1=src:i8>  (Jump if Equal)
        [35] = &&__dispatch_mode8__opcode_036__,  //     <cmd=JNE> <arg-1=src:i8>  (Jump if Not Equal)
        [36] = &&__dispatch_mode8__opcode_037__,  //      <cmd=JB> <arg-1=src:i8>  (Jump if Below)
        [37] = &&__dispatch_mode8__opcode_038__,  //      <cmd=JA> <arg-1=src:i8>  (Jump if Above)
        [38] = &&__dispatch_mode8__opcode_039__,  // <cmd=JBE/JNA> <arg-1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
        [39] = &&__dispatch_mode8__opcode_040__,  // <cmd=JAE/JNB> <arg-1=src:i8>  (Jump if Above or Equal / Jump if Not Below)
        ///////////////////////////////////
        [40 ... 253] = &&__dispatch_mode8__opcode_from_041_to_253__,
        [254] = &&__dispatch_mode8__opcode_255__, // <cmd=?> ; переключение режима адресации (с 8 на 16)
        [255] = &&__dispatch_mode8__opcode_256__  // <cmd=HLT>
        ///////////////////////////////////
    };
    /* Генератор
    for (unsigned char i = 39; i <= 254; i++)
    {
        //printf("\n[%d] = &&__dispatch_mode8__opcode_%03d__,", i-1, i);
        //printf("\n__dispatch_mode8__opcode_%03d__:", i);
    }
    */
    // Таблица диспетчеризации II (для 16-ти битного режима адресации)
    void *dispatch_mode16[0x100] =
    {
        [0 ... 253] = &&__dispatch_mode16__opcode_from_000_to_253__,
        [254] = &&__dispatch_mode16__opcode_255__, // <cmd=?> ; переключение режима адресации (с 16 на 8)
        [255] = &&__dispatch_mode8__opcode_256__   // <cmd=HLT>
    }; // Пока заглушка
    #ifdef DEBUG
     printf("\n Starting vCPU (8-bit's mode)...\n"); // 1--2, 2--4, 3--8, 4 - 16, 5 - 32, 6 - 64, 7 - 128, 8 - 256, 9 - 512, 10 - 1024
    #endif
    // Стартуем в 8-ми битном режиме адресации! (Определяется конфигурацией VM)
    goto *dispatch_mode8[memory[ip8]];
    // Стартуем в 16-ти битном режиме адресации! (Определяется конфигурацией VM)
    //goto *dispatch_mode16[memory[ip16]]; // Пока заглушка

// #include "ShowDashboard.txt" ; Режим отладки вынесен в отдельный файл через #ifdef DEBUG тело #endif
/*/
|*|    (Specification: Intel/AT&T, dst)
|*|
|*|     0- 1 <cmd=INC> <arg-1=dst:m8>
|*|     2- 3 <cmd=INC> <arg-1=dst:p8>
/*/
//////////////////////////////////////
__dispatch_mode8__opcode_001__:     // <cmd=INC> <arg-1=dst:m8>
#include "ShowDashboard.txt"        //
 memory[memory[ip8+1]]++;           // <arg-1=dst:m8> ; Произвольная ЯП
 ip8 += 2;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
//////////////////////////////////////
__dispatch_mode8__opcode_003__:     // <cmd=INC> <arg-1=dst:p8>
#include "ShowDashboard.txt"        //
 memory[memory[memory[ip8+1]]]++;   // <arg-1=dst:p8> ; Произвольная ЯП
 ip8 += 2;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
/*/
|*|    (Specification: Intel/AT&T, dst)
|*|
|*|     1- 2 <cmd=DEC> <arg-1=dst:m8>
|*|     3- 4 <cmd=DEC> <arg-1=dst:p8>
/*/
//////////////////////////////////////
__dispatch_mode8__opcode_002__:     // <cmd=DEC> <arg-1=dst:m8>
#include "ShowDashboard.txt"        //
 memory[memory[ip8+1]]--;           // <arg-1=dst:m8> ; Произвольная ЯП
 ip8 += 2;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
//////////////////////////////////////////
__dispatch_mode8__opcode_004__:         // <cmd=DEC> <arg-1=dst:p8>
#include "ShowDashboard.txt"            //
 memory[memory[memory[ip8+1]]]--;       // <arg-1=dst:p8> ; Произвольная ЯП
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
/*/
|*|    (Specification: Intel, dst <~ src)
|*|
|*|          <cmd=MOV> <arg-1=src:i8> <arg-2=src:i8> ; /!\ Недопустимо: src =! src (Semantic error)
|*|          <cmd=MOV> <arg-1=dst:r8> <arg-2=src:i8>
|*|     4- 5 <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8>
|*|     8- 9 <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8>
|*|
|*|          <cmd=MOV> <arg-1=src:i8> <arg-2=src:m8> ; /!\ Недопустимо: src =! src (Semantic error)
|*|     6- 7 <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8>
|*|    10-11 <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8>
|*|
|*|          <cmd=MOV> <arg-1=src:i8> <arg-2=src:p8> ; /!\ Недопустимо: src =! src (Semantic error)
|*|    12-13 <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8>
|*|     ?- ? <cmd=MOV> <arg-1=dst:p8> <arg-2=src:p8> ; /!\ Отсутствует реализация (избыточно?)
|*|
|*|    (Specification: AT&T, src ~> dst)
|*|
|*|          <cmd=MOV> <arg-1=src:i8> <arg-2=src:i8> ; /!\ Недопустимо: src =! src (Semantic error)
|*|     5- 6 <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8>
|*|     9-10 <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8>
|*|
|*|          <cmd=MOV> <arg-1=src:m8> <arg-2=src:i8> ; /!\ Недопустимо: src =! src (Semantic error)
|*|     7- 8 <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8>
|*|    11-12 <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8>
|*|
|*|          <cmd=MOV> <arg-1=src:p8> <arg-2=src:i8> ; /!\ Недопустимо: src =! src (Semantic error)
|*|    13-14 <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8>
|*|     ?- ? <cmd=MOV> <arg-1=src:p8> <arg-2=dst:p8> ; /!\ Отсутствует реализация (избыточно?)
/*/
// SPECIFICATION: INTEL //
//
//  ?- ? <cmd=MOV> <arg-1=dst:r8> <arg-2=src:i8>
// [Inserting abstract ASM-code]: mov <r8>, 1
// [Inserting abstract   C-code]: -
//////////////////////////////////////////
__dispatch_mode8__opcode_000__:         // <cmd=MOV> <arg-1=dst:r8> <arg-2=src:i8> ; L <~ R (Intel)
#include "ShowDashboard.txt"            //
 r8 = memory[ip8+1];                    // <arg-1=dst:r8> <arg-2=src:i8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// 4- 5 <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8> ; Копирование непосредственного (константного) значения по прямому адресу
// [Inserting abstract ASM-code]: mov a, 1
// [Inserting abstract   C-code]: a = 1;
//////////////////////////////////////////
__dispatch_mode8__opcode_005__:         // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8> ; L <~ R (Intel)
#include "ShowDashboard.txt"            //
 memory[memory[ip8+1]] = memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// 8- 9 <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8> ; Копирование непосредственного (константного) значения по косвенному адресу
// [Inserting abstract ASM-code]: mov [a], b    ; Предполагается, что в `b` уже находится `1`
// [Inserting abstract   C-code]: *a = b;       ; Предполагается, что в `b` уже находится `1`
//////////////////////////////////////////////////
__dispatch_mode8__opcode_009__:                 // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8> ; L <~ R (Intel)
#include "ShowDashboard.txt"                    //
 memory[memory[memory[ip8+1]]] = memory[ip8+2]; // <arg-1=dst:p8> <arg-2=src:i8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// 6- 7 <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8> ; Копирование значения из прямого адреса в прямой
// [Inserting abstract ASM-code]: mov a, b
// [Inserting abstract   C-code]: a = b;
//////////////////////////////////////////////////
__dispatch_mode8__opcode_007__:                 // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8> ; L <~ R (Intel)
#include "ShowDashboard.txt"                    //
 memory[memory[ip8+1]] = memory[memory[ip8+2]]; // <arg-1=dst:m8> <arg-2=src:m8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// 10-11 <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8> ; Копирование значения из прямого адреса в косвенный
// [Inserting abstract ASM-code]: mov [a], b
// [Inserting abstract   C-code]: *a = b;
//////////////////////////////////////////////////////////
__dispatch_mode8__opcode_011__:                         // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8> ; L <~ R (Intel)
#include "ShowDashboard.txt"                            //
 memory[memory[memory[ip8+1]]] = memory[memory[ip8+2]]; // <arg-1=dst:p8> <arg-2=src:m8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

// 12-13 <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8> ; Копирование значения из косвенного адреса в прямой
// [Inserting abstract ASM-code]: mov a, [b]
// [Inserting abstract   C-code]: a = *b;
//////////////////////////////////////////////////////////
__dispatch_mode8__opcode_013__:                         // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8> ; L <~ R (Intel)
#include "ShowDashboard.txt"                            //
 memory[memory[ip8+1]] = memory[memory[memory[ip8+2]]]; // <arg-1=dst:m8> <arg-2=src:p8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

// SPECIFICATION: AT&T //
//
// 5- 6 <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8> ; Копирование непосредственного (константного) значения по прямому адресу
// [Inserting abstract ASM-code]: mov a, 1
// [Inserting abstract   C-code]: a = 1;
//////////////////////////////////////////
__dispatch_mode8__opcode_006__:         // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8> ; L ~> R (AT&T)
#include "ShowDashboard.txt"            //
 memory[memory[ip8+2]] = memory[ip8+1]; // <arg-1=src:i8> <arg-2=dst:m8>
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////

// 9-10 <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8> ; Копирование непосредственного (константного) значения по косвенному адресу
// [Inserting abstract ASM-code]: mov [a], b    ; Предполагается, что в `b` уже находится `1`
// [Inserting abstract   C-code]: *a = b;       ; Предполагается, что в `b` уже находится `1`
//////////////////////////////////////////////////
__dispatch_mode8__opcode_010__:                 // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8> ; L ~> R (AT&T)
#include "ShowDashboard.txt"                    //
 memory[memory[memory[ip8+2]]] = memory[ip8+1]; // <arg-1=src:i8> <arg-2=dst:p8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// 7- 8 <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8> ; Копирование значения из прямого адреса в прямой
// [Inserting abstract ASM-code]: mov a, b
// [Inserting abstract   C-code]: a = b;
//////////////////////////////////////////////////
__dispatch_mode8__opcode_008__:                 // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8> ; L ~> R (AT&T)
#include "ShowDashboard.txt"                    //
 memory[memory[ip8+2]] = memory[memory[ip8+1]]; // <arg-1=src:m8> <arg-2=dst:m8>
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

// 11-12 <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8> ; Копирование значения из прямого адреса в косвенный
// [Inserting abstract ASM-code]: mov [a], b
// [Inserting abstract   C-code]: *a = b;
//////////////////////////////////////////////////////////
__dispatch_mode8__opcode_012__:                         // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8> ; L ~> R (AT&T)
#include "ShowDashboard.txt"                            //
 memory[memory[memory[ip8+2]]] = memory[memory[ip8+1]]; // <arg-1=src:m8> <arg-2=dst:p8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

// 13-14 <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8> ; Копирование значения из косвенного адреса в прямой
// [Inserting abstract ASM-code]: mov a, [b]
// [Inserting abstract   C-code]: a = *b;
//////////////////////////////////////////////////////////
__dispatch_mode8__opcode_014__:                         // <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8> ; L ~> R (AT&T)
#include "ShowDashboard.txt"                            //
 memory[memory[ip8+2]] = memory[memory[memory[ip8+1]]]; // <arg-1=src:p8> <arg-2=dst:m8>
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////

///////////////////////////////////////////
__dispatch_mode8__opcode_015__:          // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"             //
 memory[memory[ip8+1]] += memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
__dispatch_mode8__opcode_016__:          // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"             //
 memory[memory[ip8+1]] -= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
__dispatch_mode8__opcode_017__:          // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"             //
 memory[memory[ip8+1]] *= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

///////////////////////////////////////////
__dispatch_mode8__opcode_018__:          // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8>
#include "ShowDashboard.txt"             //
 memory[memory[ip8+1]] /= memory[ip8+2]; // <arg-1=dst:m8> <arg-2=src:i8>
 ip8 += 3;                               //
 goto *dispatch_mode8[memory[ip8]];      //
///////////////////////////////////////////

//////////////////////////////////////
__dispatch_mode8__opcode_019__:     // <cmd=CALL> <arg1=src:i8>
#include "ShowDashboard.txt"        //
 memory[sp8--] = ip8+2;             // Запоминаем адрес возврата из процедуры
 ip8 = memory[ip8+1];               // <arg1=src:i8>
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
//////////////////////////////////////
__dispatch_mode8__opcode_020__:     // <cmd=CALL> <arg1=src:m8>
#include "ShowDashboard.txt"        //
 memory[sp8--] = ip8+2;             // Запоминаем адрес возврата из процедуры
 ip8 = memory[memory[ip8+1]];       // <arg1=src:m8>
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
////////////////////////////////////////
__dispatch_mode8__opcode_021__:       // <cmd=CALL> <arg1=src:p8>
#include "ShowDashboard.txt"          //
 memory[sp8--] = ip8+2;               // Запоминаем адрес возврата из процедуры
 ip8 = memory[memory[memory[ip8+1]]]; // <arg1=src:p8>
 goto *dispatch_mode8[memory[ip8]];   //
////////////////////////////////////////

//////////////////////////////////////
__dispatch_mode8__opcode_022__:     // <cmd=RET>
#include "ShowDashboard.txt"        //
 ip8 = memory[++sp8];               // Достаёт адрес возврата и ставит ip8 на него
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////

//////////////////////////////////////
__dispatch_mode8__opcode_023__:     // <cmd=PUSH> <arg1=src:i8>
#include "ShowDashboard.txt"        //
 memory[sp8--] = memory[ip8+1];     // <arg1=src:i8>
 ip8 += 2;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
//////////////////////////////////////////
__dispatch_mode8__opcode_024__:         // <cmd=PUSH> <arg1=src:m8>
#include "ShowDashboard.txt"            //
 memory[sp8--] = memory[memory[ip8+1]]; // <arg1=src:m8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////////////
__dispatch_mode8__opcode_025__:                 // <cmd=PUSH> <arg1=src:p8>
#include "ShowDashboard.txt"                    //
 memory[sp8--] = memory[memory[memory[ip8+1]]]; // <arg1=src:p8>
 ip8 += 2;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

//////////////////////////////////////////
__dispatch_mode8__opcode_026__:         // <cmd=POP> <arg1=dst:m8> ; Снимаемое значение-аргумент со стека отправится по прямому адресу
#include "ShowDashboard.txt"            //
 memory[memory[ip8+1]] = memory[++sp8]; // <arg1=dst:m8>
 ip8 += 2;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////////////
__dispatch_mode8__opcode_027__:                 // <cmd=POP> <arg1=dst:p8> ; Снимаемое значение-аргумент со стека отправится по косвенному адресу
#include "ShowDashboard.txt"                    //
 memory[memory[memory[ip8+1]]] = memory[++sp8]; // <arg1=dst:p8>
 ip8 += 2;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////

//////////////////////////////////////
__dispatch_mode8__opcode_028__:     // <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
#include "ShowDashboard.txt"        //
 temp8_1 = memory[ip8+1];           // <arg1=src:i8>
 temp8_2 = memory[ip8+2];           // <arg2=src:i8>
 ef = (temp8_1 == temp8_2);         // ZF (Zero Flag) в x86
 af = (temp8_1 > temp8_2);          // JA (Above) / тут какой флаг в x86???
 bf = (temp8_1 < temp8_2);          // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_029__:     // <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
#include "ShowDashboard.txt"        //
 temp8_1 = memory[memory[ip8+1]];   // <arg1=src:m8>
 temp8_2 = memory[ip8+2];           // <arg2=src:i8>
 ef = (temp8_1 == temp8_2);         // ZF (Zero Flag) в x86
 af = (temp8_1 > temp8_2);          // JA (Above) / тут какой флаг в x86???
 bf = (temp8_1 < temp8_2);          // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_030__:     // <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
#include "ShowDashboard.txt"        //
 temp8_1 = memory[ip8+1];           // <arg1=src:i8>
 temp8_2 = memory[memory[ip8+2]];   // <arg2=src:m8>
 ef = (temp8_1 == temp8_2);         // ZF (Zero Flag) в x86
 af = (temp8_1 > temp8_2);          // JA (Above) / тут какой флаг в x86???
 bf = (temp8_1 < temp8_2);          // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_031__:     // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
#include "ShowDashboard.txt"        //
 temp8_1 = memory[memory[ip8+1]];   // <arg1=src:m8>
 temp8_2 = memory[memory[ip8+2]];   // <arg2=src:m8>
 ef = (temp8_1 == temp8_2);         // ZF (Zero Flag) в x86
 af = (temp8_1 > temp8_2);          // JA (Above) / тут какой флаг в x86???
 bf = (temp8_1 < temp8_2);          // JB (Below) / тут какой флаг в x86???
 ip8 += 3;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////

//////////////////////////////////////
__dispatch_mode8__opcode_032__:     // <cmd=JMP> <arg1=src:i8>
#include "ShowDashboard.txt"        //
 ip8 = memory[ip8+1];               // <arg1=src:i8>
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_033__:     // <cmd=JMP> <arg1=src:m8>
#include "ShowDashboard.txt"        //
 ip8 = memory[memory[ip8+1]];       // <arg1=src:m8>
 goto *dispatch_mode8[memory[ip8]]; //
////////////////////////////////////////
__dispatch_mode8__opcode_034__:       // <cmd=JMP> <arg1=src:p8>
#include "ShowDashboard.txt"          //
 ip8 = memory[memory[memory[ip8+1]]]; // <arg1=src:p8>
 goto *dispatch_mode8[memory[ip8]];   //
////////////////////////////////////////

//////////////////////////////////////
__dispatch_mode8__opcode_035__:     //  JE addr8  (Jump if Equal)
#include "ShowDashboard.txt"        //
 if (ef) ip8 = memory[ip8+1];       // JE (Jump if Equal): Прыгаем, если ef == 1
 else    ip8 += 2;                  // JE + addr8
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_036__:     // JNE addr8  (Jump if Not Equal)
#include "ShowDashboard.txt"        //
 if (!ef) ip8 = memory[ip8+1];      // JNE (Jump if Not Equal): Прыгаем, если ef == 0
 else     ip8 += 2;                 // JNE + addr8
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_037__:     //  JB addr8  (Jump if Below)
#include "ShowDashboard.txt"        //
 if (bf) ip8 = memory[ip8+1];       // JB (Jump if Below): Прыгаем, если bf == 1 (первое меньше второго)
 else    ip8 += 2;                  // JB + addr8
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_038__:     //  JA addr8  (Jump if Above)
#include "ShowDashboard.txt"        //
 if (af) ip8 = memory[ip8+1];       // JA (Jump if Above): Прыгаем, если af == 1 (первое больше второго)
 else    ip8 += 2;                  // JA + addr8
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_039__:     // JBE/JNA addr8  (Jump if Below or Equal / Jump if Not Above)
#include "ShowDashboard.txt"        //
 if (bf||ef) ip8 = memory[ip8+1];   // JBE (Jump if Below or Equal)
 else       ip8 += 2;               // JBE + addr8
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////
__dispatch_mode8__opcode_040__:     // JAE/JNB addr8  (Jump if Above or Equal / Jump if Not Below)
#include "ShowDashboard.txt"        //
 if (af||ef) ip8 = memory[ip8+1];   // JAE/JNB (Jump if Above or Equal / Jump if Not Below)
 else       ip8 += 2;               // JAE/JNB + addr8
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////

//////////////////////////////////////////////
__dispatch_mode8__opcode_from_041_to_253__: // ; Неопределённые опкоды
#include "ShowDashboard.txt"                //
 putchar('\n');
 return;                                    // ; Экстремальный выход
 //goto *dispatch_mode8[memory[++ip8]];     // ; Крутим дальше
//////////////////////////////////////////////
///////////////////////////////////////////////
__dispatch_mode16__opcode_from_000_to_253__: // ; Неопределённые опкоды
#include "ShowDashboard.txt"                 //
 return;                                     // ; Экстремальный выход
 //goto *dispatch_mode8[memory[++ip8]];      // ; Крутим дальше
///////////////////////////////////////////////

///////////////////////////////////////////
__dispatch_mode16__opcode_255__:         // <cmd=?> ; Переключение на 8-ми битный режим адресации
#include "ShowDashboard.txt"             //
 ip8 = ip16;                             //
 goto *dispatch_mode8[ip8];              //
///////////////////////////////////////////

////////////////////////////////////////////
__dispatch_mode8__opcode_255__:           // <cmd=?> ; Переключение на 16-ти битный режим адресации
#include "ShowDashboard.txt"              //
 ip16 = ip8;                              //
 goto *dispatch_mode16[ip16];             //
////////////////////////////////////////////

////////////////////////////////////
__dispatch_mode8__opcode_256__:   // <cmd=hlt> ; Остановить/завершить выполнение программы
#include "ShowDashboard.txt"      //
 return;                          //
////////////////////////////////////
end_block