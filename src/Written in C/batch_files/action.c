/**/#define DEBUG/**/
#ifdef DEBUG
enum
{
    ///////////
    inc8_dm, // <cmd=INC> <arg-1=dst:m8>
    dec8_dm, // <cmd=DEC> <arg-1=dst:m8>

    inc8_dp, // <cmd=INC> <arg-1=dst:p8>
    dec8_dp, // <cmd=DEC> <arg-1=dst:p8>
    ///////////

    /////////////////////////////////
    // Перессылка данных (8-bit's) //
    mov8_dm_si,                    // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8>
    mov8_si_dm,                    // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8>
    //                             //
    mov8_dm_sm,                    // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8>
    mov8_sm_dm,                    // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8>
    //                             //
    mov8_dp_si,                    // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8>
    mov8_si_dp,                    // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8>
    //                             //
    mov8_dp_sm,                    // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8>
    mov8_sm_dp,                    // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8>
    //                             //
    mov8_dm_sp,                    // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8>
    mov8_sp_dm,                    // <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8>
    // Перессылка данных (8-bit's) //
    /////////////////////////////////

    //////////////////////////////////////////////
    // Арифметико-логические операции (8-bit's) //
    //                                          //
    // Сложение                                 //
    add8_dm_si,                                 // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8>
    add8_dm_sm,                                 // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:m8> ; Заглушка - резервация опкода
    add8_dm_sp,                                 // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:p8> ; Заглушка - резервация опкода
    //                                          //
    add8_dp_si,                                 // <cmd=ADD> <arg-1=dst:p8> <arg-2=src:i8> ; Заглушка - резервация опкода
    add8_dp_sm,                                 // <cmd=ADD> <arg-1=dst:p8> <arg-2=src:m8> ; Заглушка - резервация опкода
    add8_dp_sp,                                 // <cmd=ADD> <arg-1=dst:p8> <arg-2=src:p8> ; Заглушка - резервация опкода
    //                                          //
    // Вычитание                                //
    sub8_dm_si,                                 // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8>
    sub8_dm_sm,                                 // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:m8> ; Заглушка - резервация опкода
    sub8_dm_sp,                                 // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:p8> ; Заглушка - резервация опкода
    //                                          //
    sub8_dp_si,                                 // <cmd=SUB> <arg-1=dst:p8> <arg-2=src:i8> ; Заглушка - резервация опкода
    sub8_dp_sm,                                 // <cmd=SUB> <arg-1=dst:p8> <arg-2=src:m8> ; Заглушка - резервация опкода
    sub8_dp_sp,                                 // <cmd=SUB> <arg-1=dst:p8> <arg-2=src:p8> ; Заглушка - резервация опкода
    //                                          //
    // Умножение                                //
    mul8_dm_si,                                 // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8>
    mul8_dm_sm,                                 // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:m8> ; Заглушка - резервация опкода
    mul8_dm_sp,                                 // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:p8> ; Заглушка - резервация опкода
    //                                          //
    mul8_dp_si,                                 // <cmd=MUL> <arg-1=dst:p8> <arg-2=src:i8> ; Заглушка - резервация опкода
    mul8_dp_sm,                                 // <cmd=MUL> <arg-1=dst:p8> <arg-2=src:m8> ; Заглушка - резервация опкода
    mul8_dp_sp,                                 // <cmd=MUL> <arg-1=dst:p8> <arg-2=src:p8> ; Заглушка - резервация опкода
    //                                          //
    // Деление                                  //
    div8_dm_si,                                 // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8>
    div8_dm_sm,                                 // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:m8> ; Заглушка - резервация опкода
    div8_dm_sp,                                 // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:p8> ; Заглушка - резервация опкода
    //
    div8_dp_si,                                 // <cmd=DIV> <arg-1=dst:p8> <arg-2=src:i8> ; Заглушка - резервация опкода
    div8_dp_sm,                                 // <cmd=DIV> <arg-1=dst:p8> <arg-2=src:m8> ; Заглушка - резервация опкода
    div8_dp_sp,                                 // <cmd=DIV> <arg-1=dst:p8> <arg-2=src:p8> ; Заглушка - резервация опкода
    //                                          //
    // Арифметико-логические операции (8-bit's) //
    //////////////////////////////////////////////

    ////////////////////////////////////
    // Переход в процедуру (8-bit's)  //
    call8_si,                         // <cmd=CALL> <arg1=src:i8>
    call8_sm,                         // <cmd=CALL> <arg1=src:m8>
    call8_sp,                         // <cmd=CALL> <arg1=src:p8>
    //                                //
    // Возврат из процедуры (8-bit's) //
    ret,                              // <cmd=RET>
    //                                //
    // Управление стеком (8-bit's)    //
    push8_si,                         // <cmd=PUSH> <arg1=src:i8>
    push8_sm,                         // <cmd=PUSH> <arg1=src:m8>
    push8_sp,                         // <cmd=PUSH> <arg1=src:p8>
    //                                //
    pop8_dm,                          // <cmd=POP> <arg1=dst:m8>
    pop8_dp,                          // <cmd=POP> <arg1=dst:p8>
    ////////////////////////////////////

    //////////////////////////
    // Сравнение (8-bit's)  //
    cmp8_si_si,             // <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
    cmp8_sm_si,             // <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
    cmp8_si_sm,             // <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
    cmp8_sm_sm,             // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
    // Сравнение (8-bit's)  //
    //////////////////////////

    ///////////////////////////////////
    // Безусловный переход (8-bit's) //
    jmp8_si,                         // <cmd=JMP> <arg1=src:i8> ; JMP addr8 / JMP XX
    jmp8_sm,                         // <cmd=JMP> <arg1=src:m8> ; JMP addr8 / JMP XX
    jmp8_sp,                         // <cmd=JMP> <arg1=src:p8> ; JMP addr8 / JMP XX
    // Безусловный переход (16-bit's) //
    jmp16_si,                         // <cmd=JMP> <arg1=src:i16> ; JMP addr16 / JMP XX XX
    jmp16_sm,                         // <cmd=JMP> <arg1=src:m16> ; JMP addr16 / JMP XX XX
    jmp16_sp,                         // <cmd=JMP> <arg1=src:p16> ; JMP addr16 / JMP XX XX
    ///////////////////////////////////

    /////////////////////////////////
    // Условные переходы (8-bit's) //
     je8_si,                       //      <cmd=JE> <arg1=src:i8>  (Jump if Equal)
    jne8_si,                       //     <cmd=JNE> <arg1=src:i8>  (Jump if Not Equal)
     jb8_si,                       //      <cmd=JB> <arg1=src:i8>  (Jump if Below)
     ja8_si,                       //      <cmd=JA> <arg1=src:i8>  (Jump if Above)
    jbe8_si, jna8_si = 56,         // <cmd=JBE/JNA> <arg1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
    jae8_si, jnb8_si = 57,         // <cmd=JAE/JNB> <arg1=src:i8>  (Jump if Above or Equal / Jump if Not Below)
    // Условные переходы (8-bit's) //
    /////////////////////////////////

    hlt = 255
};
static unsigned char memory[0xFFFF+0x01] = // Для быстрого теста/проверки работоспобности движка
{
    mov8_dm_si, 10, 42, // mem[10] = 42
    mov8_dm_sp, 20, 10, // mem[20] = mem[mem[10]] = mem[42] (мусор)
    hlt
    /*
    CALL, 3,
    HLT,
    RET,
    HLT,
    'C', '$', '\0', // Строка-1
    'C', '$', '\0', // Строка-2
    HLT
    */
};
#endif
#ifndef DEBUG
static unsigned char memory[0xFFFF+0x01] = {/*Заглушка=*/hlt}; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения
#endif
#define run_block {
#define end_block }
static inline void Action()
run_block
    /*static*/unsigned char  ip8  = 0x00;   // Instruction pointer 8-bit's
    /*static*/unsigned short ip16 = 0x0000; // Instruction pointer 16-bit's

    /*static*/unsigned char  sp8  = 0xFF;   // Stack pointer 8-bit's
    /*static*/unsigned short sp16 = 0xFFFF; // Stack pointer 16-bit's

    /*static*/unsigned char  cs8  = 0x00; // Code segment 8-bit's
    /*static*/unsigned char  ss8  = 0x00; // Stack segment 8-bit's
    /*static*/unsigned char  ds8  = 0x00; // Data segment 8-bit's

    /*static*/unsigned char ef/*zf*/ = 0; // флаг равенства
    /*static*/unsigned char bf       = 0; // (below) флаг меньше / в x86 какой флаг?
    /*static*/unsigned char af       = 0; // (above) флаг больше / в x86 какой флаг?

    // Скрытые регистры общего назначения для хранения промежуточных результатов
    /*static*/unsigned char a = 0;
    /*static*/unsigned char b = 0;
    /*static*/unsigned char c = 0;
    /*static*/unsigned char d = 0;
    /*static*/unsigned char e = 0;
    /*static*/unsigned char f = 0;

    // Таблица диспетчеризации I (для 8-ти битного режима адресации)
    void *dispatch_mode8[0x100] =
    {
        [ 0] = &&__dispatch_mode8__opcode_001__, // <cmd=INC> <arg-1=dst:m8>
        [ 1] = &&__dispatch_mode8__opcode_002__, // <cmd=DEC> <arg-1=dst:m8>

        [ 2] = &&__dispatch_mode8__opcode_003__, // <cmd=INC> <arg-1=dst:p8>
        [ 3] = &&__dispatch_mode8__opcode_004__, // <cmd=DEC> <arg-1=dst:p8>
        //
        [ 4] = &&__dispatch_mode8__opcode_005__, // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:i8>
        [ 5] = &&__dispatch_mode8__opcode_006__, // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:m8>
        //
        [ 6] = &&__dispatch_mode8__opcode_007__, // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:m8>
        [ 7] = &&__dispatch_mode8__opcode_008__, // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:m8>
        //
        [ 8] = &&__dispatch_mode8__opcode_009__, // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8>
        [ 9] = &&__dispatch_mode8__opcode_010__, // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8>
        //
        [10] = &&__dispatch_mode8__opcode_011__, // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8>
        [11] = &&__dispatch_mode8__opcode_012__, // <cmd=MOV> <arg-1=src:m8> <arg-2=dst:p8>
        //
        [12] = &&__dispatch_mode8__opcode_013__, // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8>
        [13] = &&__dispatch_mode8__opcode_014__, // <cmd=MOV> <arg-1=src:p8> <arg-2=dst:m8>
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
     printf("\n Starting vCPU (8-bit's mode)..."); // 1--2, 2--4, 3--8, 4 - 16, 5 - 32, 6 - 64, 7 - 128, 8 - 256, 9 - 512, 10 - 1024
    #endif
    // Стартуем в 8-ми битном режиме адресации! (Определяется конфигурацией VM)
    goto *dispatch_mode8[memory[ip8]];
    // Стартуем в 16-ти битном режиме адресации! (Определяется конфигурацией VM)
    goto *dispatch_mode16[memory[ip16]]; // Пока заглушка

//////////////////////////////////////
__dispatch_mode8__opcode_001__:     // <cmd=INC> <arg-1=dst:m8>
#include "ShowDashboard.txt"        // Режим отладки вынесен в отдельный файл через #ifdef DEBUG тело #endif
 memory[memory[ip8+1]]++;           // <arg-1=dst:m8> ; Произвольная ЯП
 ip8 += 2;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////

//////////////////////////////////////
__dispatch_mode8__opcode_002__:     // <cmd=DEC> <arg-1=dst:m8>
#include "ShowDashboard.txt"        // Режим отладки вынесен в отдельный файл через #ifdef DEBUG тело #endif
 memory[memory[ip8+1]]--;           // <arg-1=dst:m8> ; Произвольная ЯП
 ip8 += 2;                          //
 goto *dispatch_mode8[memory[ip8]]; //
//////////////////////////////////////

//////////////////////////////////////////
__dispatch_mode8__opcode_003__:         // <cmd=INC> <arg-1=dst:p8>
#include "ShowDashboard.txt"            // Режим отладки вынесен в отдельный файл через #ifdef DEBUG тело #endif
//////////////////////////////////////////

//////////////////////////////////////////
__dispatch_mode8__opcode_004__:         // <cmd=DEC> <arg-1=dst:p8>
#include "ShowDashboard.txt"            // Режим отладки вынесен в отдельный файл через #ifdef DEBUG тело #endif
//////////////////////////////////////////

//////////////////////////////////////////
__dispatch_mode8__opcode_005__:         // <cmd=MOV> mem8 <~ imm8
#include "ShowDashboard.txt"            //
 memory[memory[ip8+1]] = memory[ip8+2]; //
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
__dispatch_mode8__opcode_006__:         // <cmd=MOV> imm8 ~> mem8
#include "ShowDashboard.txt"            //
 memory[memory[ip8+2]] = memory[ip8+1]; //
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////////////
__dispatch_mode8__opcode_007__:                 // <cmd=MOV> mem8 <~ mem8
#include "ShowDashboard.txt"                    //
 memory[memory[ip8+1]] = memory[memory[ip8+2]]; //
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////
//////////////////////////////////////////////////
__dispatch_mode8__opcode_008__:                 // <cmd=MOV> <arg-1=mem8 ~> mem8
#include "ShowDashboard.txt"                    //
 memory[memory[ip8+2]] = memory[memory[ip8+1]]; //
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////
//////////////////////////////////////////
__dispatch_mode8__opcode_009__:         // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:i8>
#include "ShowDashboard.txt"            //
 memory[memory[ip8+1]] = memory[ip8+2]; // <arg-1=dst:p8> <arg-2=src:i8>
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////
__dispatch_mode8__opcode_010__:         // <cmd=MOV> <arg-1=src:i8> <arg-2=dst:p8>
#include "ShowDashboard.txt"            //
 memory[memory[ip8+2]] = memory[ip8+1]; // <arg-1=src:i8> <arg-2=dst:p8>
 ip8 += 3;                              //
 goto *dispatch_mode8[memory[ip8]];     //
//////////////////////////////////////////
//////////////////////////////////////////////////
__dispatch_mode8__opcode_011__:                 // <cmd=MOV> ptr8 <~ mem8  (Intel: dst_ptr, src_mem)
#include "ShowDashboard.txt"                    //
 memory[memory[ip8+1]] = memory[memory[ip8+2]]; // ; Взять значение из src_mem и записать его по адресу, хранящемуся в dst_ptr
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////
//////////////////////////////////////////////////
__dispatch_mode8__opcode_012__:                 // <cmd=MOV> mem8 ~> ptr8  (AT&T: src_mem, dst_ptr)
#include "ShowDashboard.txt"                    //
 memory[memory[ip8+2]] = memory[memory[ip8+1]]; //
 ip8 += 3;                                      //
 goto *dispatch_mode8[memory[ip8]];             //
//////////////////////////////////////////////////
//////////////////////////////////////////////////////////
__dispatch_mode8__opcode_013__:                         // <cmd=MOV> <arg-1=dst:m8> <arg-2=src:p8>
#include "ShowDashboard.txt"                            //
 memory[memory[ip8+1]] = memory[memory[memory[ip8+2]]]; //
 ip8 += 3;                                              //
 goto *dispatch_mode8[memory[ip8]];                     //
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
__dispatch_mode8__opcode_014__:                         // <cmd=MOV> <arg-1=dst:p8> <arg-2=src:m8>
#include "ShowDashboard.txt"                            //
 memory[memory[ip8+2]] = memory[memory[memory[ip8+1]]]; // <arg-1=dst:p8> <arg-2=src:m8>
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

//////////////////////////////
__dispatch_mode8__opcode_028__: // <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
#include "ShowDashboard.txt"                    //
 a = memory[ip8+1]; // <arg1=src:i8>
 b = memory[ip8+2]; // <arg2=src:i8>
 ef = (a == b); // ZF (Zero Flag) в x86
 af = (a > b);  // JA (Above) / тут какой флаг в x86???
 bf = (a < b);  // JB (Below) / тут какой флаг в x86???
 ip8 += 3;
 goto *dispatch_mode8[memory[ip8]];
//////////////////////////////

__dispatch_mode8__opcode_029__: // <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
#include "ShowDashboard.txt"                    //
 a = memory[memory[ip8+1]]; // <arg1=src:m8>
 b = memory[ip8+2];         // <arg2=src:i8>
 ef = (a == b); // ZF (Zero Flag) в x86
 af = (a > b);  // JA (Above) / тут какой флаг в x86???
 bf = (a < b);  // JB (Below) / тут какой флаг в x86???
 ip8 += 3;
 goto *dispatch_mode8[memory[ip8]];
//////////////////////////////

__dispatch_mode8__opcode_030__: // <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
#include "ShowDashboard.txt"    //
 a = memory[ip8+1];         // <arg1=src:i8>
 b = memory[memory[ip8+2]]; // <arg2=src:m8>
 ef = (a == b); // ZF (Zero Flag) в x86
 af = (a > b);  // JA (Above) / тут какой флаг в x86???
 bf = (a < b);  // JB (Below) / тут какой флаг в x86???
 ip8 += 3;
 goto *dispatch_mode8[memory[ip8]];

//////////////////////////////
__dispatch_mode8__opcode_031__: // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
#include "ShowDashboard.txt"    //
 a = memory[memory[ip8+1]]; // <arg1=src:m8>
 b = memory[memory[ip8+2]]; // <arg2=src:m8>
 ef = (a == b); // ZF (Zero Flag) в x86
 af = (a > b);  // JA (Above) / тут какой флаг в x86???
 bf = (a < b);  // JB (Below) / тут какой флаг в x86???
 ip8 += 3;
 goto *dispatch_mode8[memory[ip8]];
/////////////////////////////////
////////// JMP 8-bit's //////////
__dispatch_mode8__opcode_032__: // <cmd=JMP> <arg1=src:i8>
#include "ShowDashboard.txt"    //
ip8 = memory[ip8+1]; // <arg1=src:i8>
 goto *dispatch_mode8[memory[ip8]];
//////////////////////////////
__dispatch_mode8__opcode_033__: // <cmd=JMP> <arg1=src:m8>
#include "ShowDashboard.txt"    //
 ip8 = memory[memory[ip8+1]]; // <arg1=src:m8>
 goto *dispatch_mode8[memory[ip8]];
//////////////////////////////
__dispatch_mode8__opcode_034__: // <cmd=JMP> <arg1=src:p8>
#include "ShowDashboard.txt"    //
 ip8 = memory[memory[memory[ip8+1]]]; // <arg1=src:p8>
 goto *dispatch_mode8[memory[ip8]];
////////// JMP 8-bit's //////////
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