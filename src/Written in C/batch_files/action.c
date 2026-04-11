/**/#define DEBUG/**/
#ifdef DEBUG
enum
{
    inc, // <cmd=INC>
    dec, // <cmd=DEC>
    /////////////////////////////////
    // Перессылка данных (8-bit's) //
    mov8_ds_mi = 02,               // <cmd=MOV> mem8 <~ imm8 (Intel: dst src)
    mov8_sd_im,                    // <cmd=MOV> imm8 ~> mem8 ( AT&T: src dst)

    mov8_ds_mm,                    // <cmd=MOV> mem8 <~ mem8 (Intel: dst src)
    mov8_sd_mm,                    // <cmd=MOV> mem8 ~> mem8 ( AT&T: src dst)

    mov8_ds_pi,                    // <cmd=MOV> ptr8 <~ imm8 (Intel: dst src)
    mov8_sd_ip,                    // <cmd=MOV> imm8 ~> ptr8 ( AT&T: src dst)

    mov8_ds_pm,                    // <cmd=MOV> ptr8 <~ mem8 (Intel: dst src)
    mov8_sd_mp,                    // <cmd=MOV> mem8 ~> ptr8 ( AT&T: src dst)

    mov8_ds_mp,                    // <cmd=MOV> mem8 <~ ptr8 (Intel: dst src)
    mov8_sd_pm = 11,               // <cmd=MOV> ptr8 ~> mem8 ( AT&T: src dst)
    // Перессылка данных (8-bit's) //
    /////////////////////////////////
    // Арифметико-логические операции
       add8 = 12, //
       sub8,      //
       mul8,      //
       div8 = 15, //
    /////////////////
    // Сопрограмма //
    call8_i = 16,  // <cmd=CALL> <arg1=src:i8>
    call8_m,       // <cmd=CALL> <arg1=src:m8>
    call8_p,       // <cmd=CALL> <arg1=src:p8>
    // Сопрограмма //
    /////////////////
        ret,      //
       push,      //
        pop = 21, //
    ///////////////
    // Сравнение //
    cmp8_ii = 22, // <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
    cmp8_mi,      // <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
    cmp8_im,      // <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
    cmp8_mm = 25, // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
    // Сравнение //
    ///////////////
    ///////////////////////////////////
    // Безусловный переход (8-bit's) //
    jmp8_i = 26,                     // <cmd=JMP> <arg1=src:i8>
    jmp8_m,                          // <cmd=JMP> <arg1=src:m8>
    jmp8_p = 28,                     // <cmd=JMP> <arg1=src:p8>
    // Безусловный переход (8-bit's) //
    //////////////////////////////////
    // Условные переходы (8-bit's) //
     je8_i = 29,                   //      <cmd=JE> <arg1=src:i8>  (Jump if Equal)
    jne8_i,                        //     <cmd=JNE> <arg1=src:i8>  (Jump if Not Equal)
     jb8_i,                        //      <cmd=JB> <arg1=src:i8>  (Jump if Below)
     ja8_i,                        //      <cmd=JA> <arg1=src:i8>  (Jump if Above)
    jbe8_i, jna8_i = 33,           // <cmd=JBE/JNA> <arg1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
    jae8_i, jnb8_i = 34,           // <cmd=JAE/JNB> <arg1=src:i8>  (Jump if Above or Equal / Jump if Not Below)
    // Условные переходы (8-bit's) //
    /////////////////////////////////
    hlt = 255
};
static unsigned char memory[256] = // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения
{
    mov8_ds_mi, 10, 42,   // mem[10] = 42
    mov8_ds_mp, 20, 10,   // mem[20] = mem[mem[10]] = mem[42] (мусор)
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
static unsigned char memory[0x100]; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения
#endif
static inline void Action()
{
    /*static*/unsigned char ip =       0; // указатель инструкции
    /*static*/unsigned char sp =     255; // указатель стека
    /*static*/unsigned char ef/*zf*/ = 0; // флаг равенства
    /*static*/unsigned char bf       = 0; // (below) флаг меньше / в x86 какой флаг?
    /*static*/unsigned char af       = 0; // (above) флаг больше / в x86 какой флаг?

    // регистры общего назначения для хранения промежуточных результатов
    /*static*/unsigned char a = 0;
    /*static*/unsigned char b = 0;
    /*static*/unsigned char c = 0;
    /*static*/unsigned char d = 0;
    /*static*/unsigned char e = 0;
    /*static*/unsigned char f = 0;

    void *action[] =
    {
        [ 0] = &&___OPERATION_CODE_001, // <cmd=INC>
        [ 1] = &&___OPERATION_CODE_002, // <cmd=DEC>
        //
        [ 2] = &&___OPERATION_CODE_003, // <cmd=MOV> mem8 <~ imm8 (Intel: dst src)
        [ 3] = &&___OPERATION_CODE_004, // <cmd=MOV> imm8 ~> mem8 ( AT&T: src dst)
        //
        [ 4] = &&___OPERATION_CODE_005, // <cmd=MOV> mem8 <~ mem8 (Intel: dst src)
        [ 5] = &&___OPERATION_CODE_006, // <cmd=MOV> mem8 ~> mem8 ( AT&T: src dst)
        //
        [ 6] = &&___OPERATION_CODE_007, // <cmd=MOV> ptr8 <~ imm8 (Intel: dst src)
        [ 7] = &&___OPERATION_CODE_008, // <cmd=MOV> imm8 ~> ptr8 ( AT&T: src dst)
        //
        [ 8] = &&___OPERATION_CODE_009, // <cmd=MOV> ptr8 <~ mem8 (Intel: dst src)
        [ 9] = &&___OPERATION_CODE_010, // <cmd=MOV> mem8 ~> ptr8 ( AT&T: src dst)
        //
        [10] = &&___OPERATION_CODE_011, // <cmd=MOV> mem8 <~ ptr8 (Intel: dst src)
        [11] = &&___OPERATION_CODE_012, // <cmd=MOV> ptr8 ~> mem8 ( AT&T: src dst)
        //
        [12] = &&___OPERATION_CODE_013, // <cmd=ADD>
        [13] = &&___OPERATION_CODE_014, // <cmd=SUB>
        [14] = &&___OPERATION_CODE_015, // <cmd=MUL>
        [15] = &&___OPERATION_CODE_016, // <cmd=DIV>
        ////////////////////////////////////
        // Переход в процедуру (8-bit's)  //
        [16] = &&___OPERATION_CODE_017,   //   <cmd=CALL> <arg1=src:i8>
        [17] = &&___OPERATION_CODE_018,   //   <cmd=CALL> <arg1=src:m8>
        [18] = &&___OPERATION_CODE_019,   //   <cmd=CALL> <arg1=src:p8>
        // Возврат из процедуры (8-bit's) //
        [19] = &&___OPERATION_CODE_020,   //    <cmd=RET>               ; Снимает значение-адрес со стека и совершает переход по нему
        // Управление стеком              //
        [20] = &&___OPERATION_CODE_021,   //   <cmd=PUSH> <arg1=src:i8> ; Заносимое значение в стек пойдёт из непосредственного источника
        [21] = &&___OPERATION_CODE_022,   //    <cmd=POP> <arg1=dst:m8> ; Снимаемое значение со стека пойдёт в приёмник памяти
        ////////////////////////////////////
        // Сравнение (8-bit's)            //
        [22] = &&___OPERATION_CODE_023,  //     <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
        [23] = &&___OPERATION_CODE_024,  //     <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
        [24] = &&___OPERATION_CODE_025,  //     <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
        [25] = &&___OPERATION_CODE_026,  //     <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
        ///////////////////////////////////
        // Безусловный переход (8-bit's) //
        [26] = &&___OPERATION_CODE_027,  //     <cmd=JMP> <arg1=src:i8>
        [27] = &&___OPERATION_CODE_028,  //     <cmd=JMP> <arg1=src:m8>
        [28] = &&___OPERATION_CODE_029,  //     <cmd=JMP> <arg1=src:p8>
        ///////////////////////////////////
        // Условные переходы (8-bit's)   //
        [29] = &&___OPERATION_CODE_030,  //      <cmd=JE> <arg1=src:i8>  (Jump if Equal)
        [30] = &&___OPERATION_CODE_031,  //     <cmd=JNE> <arg1=src:i8>  (Jump if Not Equal)
        [31] = &&___OPERATION_CODE_032,  //      <cmd=JB> <arg1=src:i8>  (Jump if Below)
        [32] = &&___OPERATION_CODE_033,  //      <cmd=JA> <arg1=src:i8>  (Jump if Above)
        [33] = &&___OPERATION_CODE_034,  // <cmd=JBE/JNA> <arg1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
        [34] = &&___OPERATION_CODE_035,  // <cmd=JAE/JNB> <arg1=src:i8>  (Jump if Above or Equal / Jump if Not Below)
        ///////////////////////////////////
        [35 ... 254] = &&___OPERATION_CODE_FROM_036_TO_255,
        [255] = &&___OPERATION_CODE_256 // HLT
    };
   #ifdef DEBUG
    printf("\n Запуск vCPU.");
   #endif
    goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_001: // INC [инкремент произвольной ячейкой памяти]
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+1]]++;
     ip += 2;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_002: // DEC [декремент произвольной ячейкой памяти]
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+1]]--;
     ip += 2;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_003: // MOV mem8 <~ imm8  (Intel: dst, src)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+1]] = memory[ip+2];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_004: // MOV imm8 ~> mem8  (AT&T: src, dst)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+2]] = memory[ip+1];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_005: // MOV mem8 <~ mem8  (Intel)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+1]] = memory[memory[ip+2]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_006: // MOV mem8 ~> mem8  (AT&T)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+2]] = memory[memory[ip+1]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_007: // MOV ptr8 <~ imm8  (Intel: dst_ptr, src_imm)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+1]] = memory[ip+2]; // Записать imm8 в память по адресу, который лежит в ячейке dst_ptr
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_008: // MOV imm8 ~> ptr8  (AT&T: src_imm, dst_ptr)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+2]] = memory[ip+1]; // То же самое: записать imm8 по адресу из dst_ptr
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_009: // MOV ptr8 <~ mem8  (Intel: dst_ptr, src_mem)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+1]] = memory[memory[ip+2]]; // Взять значение из src_mem и записать его по адресу, хранящемуся в dst_ptr
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_010: // MOV mem8 ~> ptr8  (AT&T: src_mem, dst_ptr)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+2]] = memory[memory[ip+1]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_011: // MOV mem8 <~ ptr8  (Intel)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+1]] = memory[memory[memory[ip+2]]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_012: // MOV ptr8 ~> mem8  (AT&T)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+2]] = memory[memory[memory[ip+1]]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_013: // ADD
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     return;
    //////////////////////////////
    ___OPERATION_CODE_014: // SUB
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     return;
    //////////////////////////////
    ___OPERATION_CODE_015: // MUL
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     return;
    //////////////////////////////
    ___OPERATION_CODE_016: // DIV
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     return;
    //////////////////////////////////
    ////////// CALL 8-bit's //////////
    ___OPERATION_CODE_017: // <cmd=CALL> <arg1=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[sp--] = ip + 2;
     ip = memory[ip+1]; // <arg1=src:i8>
     goto *action[memory[ip]];

    ___OPERATION_CODE_018: // <cmd=CALL> <arg1=src:m8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif

    ___OPERATION_CODE_019: // <cmd=CALL> <arg1=src:p8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
    ////////// CALL 8-bit's //////////
    //////////////////////////////////
    ___OPERATION_CODE_020: // <cmd=RET>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     ip = memory[++sp]; // Достаёт адрес возврата и ставит ip на него
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_021: // <cmd=PUSH> <arg1=dst:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[sp--] = memory[memory[ip+1]];
     ip += 2;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_022: // <cmd=POP> <arg1=dst:m8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     memory[memory[ip+1]] = memory[++sp];
     ip += 2;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_023: // <cmd=CMP> <arg1=src:i8> <arg2=src:i8> (подходит и под Intel, и под AT&T)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     a = memory[ip+1]; // <arg1=src:i8>
     b = memory[ip+2]; // <arg2=src:i8>
     ef = (a == b); // ZF (Zero Flag) в x86
     af = (a > b);  // JA (Above) / тут какой флаг в x86???
     bf = (a < b);  // JB (Below) / тут какой флаг в x86???
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_024: // <cmd=CMP> <arg1=src:m8> <arg2=src:i8> (подходит и под Intel, и под AT&T)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     a = memory[memory[ip+1]]; // <arg1=src:m8>
     b = memory[ip+2];         // <arg2=src:i8>
     ef = (a == b); // ZF (Zero Flag) в x86
     af = (a > b);  // JA (Above) / тут какой флаг в x86???
     bf = (a < b);  // JB (Below) / тут какой флаг в x86???
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_025: // <cmd=CMP> <arg1=src:i8> <arg2=src:m8> (подходит и под Intel, и под AT&T)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     a = memory[ip+1];         // <arg1=src:i8>
     b = memory[memory[ip+2]]; // <arg2=src:m8>
     ef = (a == b); // ZF (Zero Flag) в x86
     af = (a > b);  // JA (Above) / тут какой флаг в x86???
     bf = (a < b);  // JB (Below) / тут какой флаг в x86???
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_026: // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     a = memory[memory[ip+1]]; // <arg1=src:m8>
     b = memory[memory[ip+2]]; // <arg2=src:m8>
     ef = (a == b); // ZF (Zero Flag) в x86
     af = (a > b);  // JA (Above) / тут какой флаг в x86???
     bf = (a < b);  // JB (Below) / тут какой флаг в x86???
     ip += 3;
     goto *action[memory[ip]];
    /////////////////////////////////
    ////////// JMP 8-bit's //////////
    ___OPERATION_CODE_027: // <cmd=JMP> <arg1=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     ip = memory[ip+1]; // <arg1=src:i8>
     goto *action[memory[ip]];

    ___OPERATION_CODE_028: // <cmd=JMP> <arg1=src:m8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     ip = memory[memory[ip+1]]; // <arg1=src:m8>
     goto *action[memory[ip]];

    ___OPERATION_CODE_029: // <cmd=JMP> <arg1=src:p8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     ip = memory[memory[memory[ip+1]]]; // <arg1=src:p8>
     goto *action[memory[ip]];
    ////////// JMP 8-bit's //////////
    /////////////////////////////////
    ___OPERATION_CODE_030: //  JE addr8  (Jump if Equal)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (ef) ip = memory[ip+1]; // JE (Jump if Equal): Прыгаем, если ef == 1
     else    ip += 2;           // JE + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_031: // JNE addr8  (Jump if Not Equal)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (!ef) ip = memory[ip+1]; // JNE (Jump if Not Equal): Прыгаем, если ef == 0
     else     ip += 2;           // JNE + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_032: //  JB addr8  (Jump if Below)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (bf) ip = memory[ip+1]; // JB (Jump if Below): Прыгаем, если bf == 1 (первое меньше второго)
     else    ip += 2;           // JB + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_033: //  JA addr8  (Jump if Above)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (af) ip = memory[ip+1]; // JA (Jump if Above): Прыгаем, если af == 1 (первое больше второго)
     else    ip += 2;           // JA + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_034: // JBE/JNA addr8  (Jump if Below or Equal / Jump if Not Above)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (bf|ef) ip = memory[ip+1]; // JBE (Jump if Below or Equal)
     else       ip += 2;           // JBE + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_035: // JAE/JNB addr8  (Jump if Above or Equal / Jump if Not Below)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (af|ef) ip = memory[ip+1]; // JAE/JNB (Jump if Above or Equal / Jump if Not Below)
     else       ip += 2;           // JAE/JNB + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_FROM_036_TO_255:
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     return; // Экстремальный выход
     //goto *action[memory[++ip]]; // Крутим дальше
    //////////////////////////////
    ___OPERATION_CODE_256: // Остановить/завершить выполнение программы
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     return; // Экстремальный выход
    //////////////////////////////
}