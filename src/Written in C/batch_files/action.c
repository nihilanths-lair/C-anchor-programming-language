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
    //                             //
    mov8_ds_mm,                    // <cmd=MOV> mem8 <~ mem8 (Intel: dst src)
    mov8_sd_mm,                    // <cmd=MOV> mem8 ~> mem8 ( AT&T: src dst)
    //                             //
    mov8_ds_pi,                    // <cmd=MOV> ptr8 <~ imm8 (Intel: dst src)
    mov8_sd_ip,                    // <cmd=MOV> imm8 ~> ptr8 ( AT&T: src dst)
    //                             //
    mov8_ds_pm,                    // <cmd=MOV> ptr8 <~ mem8 (Intel: dst src)
    mov8_sd_mp,                    // <cmd=MOV> mem8 ~> ptr8 ( AT&T: src dst)
    //                             //
    mov8_ds_mp,                    // <cmd=MOV> mem8 <~ ptr8 (Intel: dst src)
    mov8_sd_pm = 11,               // <cmd=MOV> ptr8 ~> mem8 ( AT&T: src dst)
    // Перессылка данных (8-bit's) //
    /////////////////////////////////

    //////////////////////////////////////////////
    // Арифметико-логические операции (8-bit's) //
    add8 = 12,                                  //
    sub8,                                       //
    mul8,                                       //
    div8 = 15,                                  //
    // Арифметико-логические операции (8-bit's) //
    //////////////////////////////////////////////

    ////////////////////////////////////
    // Переход в процедуру (8-bit's)  //
    call8_i = 16,                     // <cmd=CALL> <arg1=src:i8>
    call8_m,                          // <cmd=CALL> <arg1=src:m8>
    call8_p = 18,                     // <cmd=CALL> <arg1=src:p8>
    // Возврат из процедуры (8-bit's) //
    ret = 19,                         // <cmd=RET>
    // Управление стеком (8-bit's)    //
    push8_i = 20,                     // <cmd=PUSH> <arg1=src:i8>
    push8_m,                          // <cmd=PUSH> <arg1=src:m8>
    push8_p,                          // <cmd=PUSH> <arg1=src:p8>
    //                                //
    pop8 = 23,                        //
    ////////////////////////////////////

    //////////////////////////
    // Сравнение (8-bit's)  //
    cmp8_ii = 24,           // <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
    cmp8_mi,                // <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
    cmp8_im,                // <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
    cmp8_mm = 27,           // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
    // Сравнение (8-bit's)  //
    //////////////////////////

    ///////////////////////////////////
    // Безусловный переход (8-bit's) //
    jmp8_i = 28,                     // <cmd=JMP> <arg1=src:i8>
    jmp8_m,                          // <cmd=JMP> <arg1=src:m8>
    jmp8_p = 30,                     // <cmd=JMP> <arg1=src:p8>
    // Безусловный переход (8-bit's) //
    ///////////////////////////////////

    /////////////////////////////////
    // Условные переходы (8-bit's) //
     je8_i = 31,                   //      <cmd=JE> <arg1=src:i8>  (Jump if Equal)
    jne8_i,                        //     <cmd=JNE> <arg1=src:i8>  (Jump if Not Equal)
     jb8_i,                        //      <cmd=JB> <arg1=src:i8>  (Jump if Below)
     ja8_i,                        //      <cmd=JA> <arg1=src:i8>  (Jump if Above)
    jbe8_i, jna8_i = 35,           // <cmd=JBE/JNA> <arg1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
    jae8_i, jnb8_i = 36,           // <cmd=JAE/JNB> <arg1=src:i8>  (Jump if Above or Equal / Jump if Not Below)
    // Условные переходы (8-bit's) //
    /////////////////////////////////

    hlt = 255
};
static unsigned char memory[256] = // Для быстрого теста/проверки работоспобности движка
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
        ////////////////////////////////////
        // Арифметико-логические операции //
        [12] = &&___OPERATION_CODE_013, // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8> (l <~ r)
        //
        [13] = &&___OPERATION_CODE_014, // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8> (l <~ r)
        //
        [14] = &&___OPERATION_CODE_015, // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8> (l <~ r)
        //
        [15] = &&___OPERATION_CODE_016, // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8> (l <~ r)
        //
        ////////////////////////////////////
        // Переход в процедуру (8-bit's)  //
        [16] = &&___OPERATION_CODE_017,   //   <cmd=CALL> <arg-1=src:i8>
        [17] = &&___OPERATION_CODE_018,   //   <cmd=CALL> <arg-1=src:m8>
        [18] = &&___OPERATION_CODE_019,   //   <cmd=CALL> <arg-1=src:p8>
        // Возврат из процедуры (8-bit's) //
        [19] = &&___OPERATION_CODE_020,   //   <cmd=RET>                 ; Снимает значение-адрес со стека и совершает переход по нему
        // Управление стеком (8-bit's)    //
        [20] = &&___OPERATION_CODE_021,   //   <cmd=PUSH> <arg-1=src:i8> ; Заносимое значение-аргумент в стек пойдёт из непосредственного источника
        [21] = &&___OPERATION_CODE_022,   //   <cmd=PUSH> <arg-1=src:m8> ; Заносимое значение-аргумент в стек пойдёт из памяти по прямому адресу
        [22] = &&___OPERATION_CODE_023,   //   <cmd=PUSH> <arg-1=src:p8> ; Заносимое значение-аргумент в стек пойдёт из памяти по косвенному адресу
        //                                //
        [23] = &&___OPERATION_CODE_024,   //   <cmd=POP> <arg-1=dst:m8>  ; Снимаемое значение-аргумент со стека отправится по прямому адресу
        [24] = &&___OPERATION_CODE_025,   //   <cmd=POP> <arg-1=dst:p8>  ; Снимаемое значение-аргумент со стека отправится по косвенному адресу
        ///////////////////////////////////
        // Сравнение (8-bit's)           //
        [25] = &&___OPERATION_CODE_026,  //     <cmd=CMP> <arg-1=src:i8> <arg-2=src:i8>
        [26] = &&___OPERATION_CODE_027,  //     <cmd=CMP> <arg-1=src:m8> <arg-2=src:i8>
        [27] = &&___OPERATION_CODE_028,  //     <cmd=CMP> <arg-1=src:i8> <arg-2=src:m8>
        [28] = &&___OPERATION_CODE_029,  //     <cmd=CMP> <arg-1=src:m8> <arg-2=src:m8>
        ///////////////////////////////////
        // Безусловный переход (8-bit's) //
        [29] = &&___OPERATION_CODE_030,  //     <cmd=JMP> <arg-1=src:i8>
        [30] = &&___OPERATION_CODE_031,  //     <cmd=JMP> <arg-1=src:m8>
        [31] = &&___OPERATION_CODE_032,  //     <cmd=JMP> <arg-1=src:p8>
        ///////////////////////////////////
        // Условные переходы (8-bit's)   //
        [32] = &&___OPERATION_CODE_033,  //      <cmd=JE> <arg-1=src:i8>  (Jump if Equal)
        [33] = &&___OPERATION_CODE_034,  //     <cmd=JNE> <arg-1=src:i8>  (Jump if Not Equal)
        [34] = &&___OPERATION_CODE_035,  //      <cmd=JB> <arg-1=src:i8>  (Jump if Below)
        [35] = &&___OPERATION_CODE_036,  //      <cmd=JA> <arg-1=src:i8>  (Jump if Above)
        [36] = &&___OPERATION_CODE_037,  // <cmd=JBE/JNA> <arg-1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
        [37] = &&___OPERATION_CODE_038,  // <cmd=JAE/JNB> <arg-1=src:i8>  (Jump if Above or Equal / Jump if Not Below)
        ///////////////////////////////////
        [38 ... 254] = &&___OPERATION_CODE_FROM_039_TO_255,
        [255] = &&___OPERATION_CODE_256  // <cmd=HLT>
        ///////////////////////////////////
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

    /////////////////////////////////////////
    ///////////// ADD (8-bit's) /////////////
    //                                     //
    ___OPERATION_CODE_013:                 // <cmd=ADD> <arg-1=dst:m8> <arg-2=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);        //
    #endif                                 //
     memory[memory[ip+1]] += memory[ip+2]; // <arg-1=dst:m8> <arg-2=src:i8>
     ip += 3;                              //
     goto *action[memory[ip]];             //
    //                                     //
    ///////////// ADD (8-bit's) /////////////
    /////////////////////////////////////////

    /////////////////////////////////////////
    ///////////// SUB (8-bit's) /////////////
    //                                     //
    ___OPERATION_CODE_014:                 // <cmd=SUB> <arg-1=dst:m8> <arg-2=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);        //
    #endif                                 //
     memory[memory[ip+1]] -= memory[ip+2]; // <arg-1=dst:m8> <arg-2=src:i8>
     ip += 3;                              //
     goto *action[memory[ip]];             //
    //                                     //
    ///////////// SUB (8-bit's) /////////////
    /////////////////////////////////////////

    /////////////////////////////////////////
    ///////////// MUL (8-bit's) /////////////
    //                                     //
    ___OPERATION_CODE_015:                 // <cmd=MUL> <arg-1=dst:m8> <arg-2=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);        //
    #endif                                 //
     memory[memory[ip+1]] *= memory[ip+2]; // <arg-1=dst:m8> <arg-2=src:i8>
     ip += 3;                              //
     goto *action[memory[ip]];             //
    //                                     //
    ///////////// MUL (8-bit's) /////////////
    /////////////////////////////////////////

    /////////////////////////////////////////
    ///////////// DIV (8-bit's) /////////////
    //                                     //
    ___OPERATION_CODE_016:                 // <cmd=DIV> <arg-1=dst:m8> <arg-2=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);        //
    #endif                                 //
     memory[memory[ip+1]] /= memory[ip+2]; // <arg-1=dst:m8> <arg-2=src:i8>
     ip += 3;                              //
     goto *action[memory[ip]];             //
    //                                     //
    ///////////// DIV (8-bit's) /////////////
    /////////////////////////////////////////

    //////////////////////////////////////
    /////////// CALL (8-bit's) ///////////
    //                                  //
    ___OPERATION_CODE_017:              // <cmd=CALL> <arg1=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);     //
    #endif                              //
     memory[sp--] = ip+2;               // Запоминаем возврата из процедуры
     ip = memory[ip+1];                 // <arg1=src:i8>
     goto *action[memory[ip]];          //
    //                                  //
    ___OPERATION_CODE_018:              // <cmd=CALL> <arg1=src:m8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);     //
    #endif                              //
     memory[sp--] = ip+2;               // Запоминаем возврата из процедуры
     ip = memory[memory[ip+1]];         // <arg1=src:m8>
     goto *action[memory[ip]];          //
    //                                  //
    ___OPERATION_CODE_019:              // <cmd=CALL> <arg1=src:p8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);     //
    #endif                              //
     memory[sp--] = ip+2;               // Запоминаем возврата из процедуры
     ip = memory[memory[memory[ip+1]]]; // <arg1=src:p8>
     goto *action[memory[ip]];          //
    //                                  //
    /////////// CALL (8-bit's) ///////////
    //////////////////////////////////////

    ___OPERATION_CODE_020: // <cmd=RET>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     ip = memory[++sp]; // Достаёт адрес возврата и ставит ip на него
     goto *action[memory[ip]];

    ////////////////////////////////////////////////
    //////////////// PUSH (8-bit's) ////////////////
    //                                            //
    ___OPERATION_CODE_021:                        // <cmd=PUSH> <arg1=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);               //
    #endif                                        //
     memory[sp--] = memory[ip+1];                 // <arg1=src:i8>
     ip += 2;                                     //
     goto *action[memory[ip]];                    //
    //                                            //
    ___OPERATION_CODE_022:                        // <cmd=PUSH> <arg1=src:m8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);               //
    #endif                                        //
     memory[sp--] = memory[memory[ip+1]];         // <arg1=src:m8>
     ip += 2;                                     //
     goto *action[memory[ip]];                    //
    //                                            //
    ___OPERATION_CODE_023:                        // <cmd=PUSH> <arg1=src:p8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);               //
    #endif                                        //
     memory[sp--] = memory[memory[memory[ip+1]]]; // <arg1=src:p8>
     ip += 2;                                     //
     goto *action[memory[ip]];                    //
    //                                            //
    //////////////// PUSH (8-bit's) ////////////////
    ////////////////////////////////////////////////

    ////////////////////////////////////////////////
    ///////////////// POP (8-bit's) ////////////////
    //                                            //
    ___OPERATION_CODE_024:                        // <cmd=POP> <arg1=dst:m8> ; Снимаемое значение-аргумент со стека отправится по прямому адресу
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);               //
    #endif                                        //
     memory[memory[ip+1]] = memory[++sp];         // <arg1=dst:m8>
     ip += 2;                                     //
     goto *action[memory[ip]];                    //
    //                                            //
    ___OPERATION_CODE_025:                        // <cmd=POP> <arg1=dst:p8> ; Снимаемое значение-аргумент со стека отправится по косвенному адресу
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);               //
    #endif                                        //
     memory[memory[memory[ip+1]]] = memory[++sp]; // <arg1=dst:p8>
     ip += 2;                                     //
     goto *action[memory[ip]];                    //
    //                                            //
    ///////////////// POP (8-bit's) ////////////////
    ////////////////////////////////////////////////

    //////////////////////////////
    ___OPERATION_CODE_026: // <cmd=CMP> <arg1=src:i8> <arg2=src:i8>
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
    ___OPERATION_CODE_027: // <cmd=CMP> <arg1=src:m8> <arg2=src:i8>
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
    ___OPERATION_CODE_028: // <cmd=CMP> <arg1=src:i8> <arg2=src:m8>
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
    ___OPERATION_CODE_029: // <cmd=CMP> <arg1=src:m8> <arg2=src:m8>
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
    ___OPERATION_CODE_030: // <cmd=JMP> <arg1=src:i8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     ip = memory[ip+1]; // <arg1=src:i8>
     goto *action[memory[ip]];

    ___OPERATION_CODE_031: // <cmd=JMP> <arg1=src:m8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     ip = memory[memory[ip+1]]; // <arg1=src:m8>
     goto *action[memory[ip]];

    ___OPERATION_CODE_032: // <cmd=JMP> <arg1=src:p8>
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     ip = memory[memory[memory[ip+1]]]; // <arg1=src:p8>
     goto *action[memory[ip]];
    ////////// JMP 8-bit's //////////
    /////////////////////////////////
    ___OPERATION_CODE_033: //  JE addr8  (Jump if Equal)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (ef) ip = memory[ip+1]; // JE (Jump if Equal): Прыгаем, если ef == 1
     else    ip += 2;           // JE + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_034: // JNE addr8  (Jump if Not Equal)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (!ef) ip = memory[ip+1]; // JNE (Jump if Not Equal): Прыгаем, если ef == 0
     else     ip += 2;           // JNE + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_035: //  JB addr8  (Jump if Below)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (bf) ip = memory[ip+1]; // JB (Jump if Below): Прыгаем, если bf == 1 (первое меньше второго)
     else    ip += 2;           // JB + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_036: //  JA addr8  (Jump if Above)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (af) ip = memory[ip+1]; // JA (Jump if Above): Прыгаем, если af == 1 (первое больше второго)
     else    ip += 2;           // JA + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_037: // JBE/JNA addr8  (Jump if Below or Equal / Jump if Not Above)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (bf|ef) ip = memory[ip+1]; // JBE (Jump if Below or Equal)
     else       ip += 2;           // JBE + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_038: // JAE/JNB addr8  (Jump if Above or Equal / Jump if Not Below)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp);
    #endif
     if (af|ef) ip = memory[ip+1]; // JAE/JNB (Jump if Above or Equal / Jump if Not Below)
     else       ip += 2;           // JAE/JNB + addr8
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_FROM_039_TO_255:
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