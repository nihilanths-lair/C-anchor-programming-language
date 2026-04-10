enum
{
    // Безусловный переход
    JMP,

    INC,
    DEC,

    MOV__DST_MEM8__SRC_IMM8 = 03, // MOV mem8 <~ imm8 (Intel: dst src)
    MOV__SRC_IMM8__DST_MEM8,      // MOV imm8 ~> mem8 ( AT&T: src dst)

    MOV__DST_MEM8__SRC_MEM8,      // MOV mem8 <~ mem8 (Intel: dst src)
    MOV__SRC_MEM8__DST_MEM8,      // MOV mem8 ~> mem8 ( AT&T: src dst)

    MOV__DST_PTR8__SRC_IMM8,      // MOV ptr8 <~ imm8 (Intel: dst src)
    MOV__SRC_IMM8__DST_PTR8,      // MOV imm8 ~> ptr8 ( AT&T: src dst)

    MOV__DST_PTR8__SRC_MEM8,      // MOV ptr8 <~ mem8 (Intel: dst src)
    MOV__SRC_MEM8__DST_PTR8,      // MOV mem8 ~> ptr8 ( AT&T: src dst)

    MOV__DST_MEM8__SRC_PTR8,      // MOV mem8 <~ ptr8 (Intel: dst src)
    MOV__SRC_PTR8__DST_MEM8 = 12, // MOV ptr8 ~> mem8 ( AT&T: src dst)

    ADD,  // 13
    SUB,  // 14
    MUL,  // 15
    DIV,  // 16

    CMP,  // 17
    CALL, // 18
    RET,  // 19
    PUSH, // 20
    POP,  // 21

    // Условные переходы (Specifics: Intel/AT&T: src src)
     JE_I8 = 22, //  JE imm8 imm8  (Jump if Equal)
    JNE_I8,      // JNE imm8 imm8  (Jump if Not Equal)
     JB_I8,      //  JB imm8 imm8  (Jump if Below)
    JBE_I8,      // JBE imm8 imm8  (Jump if Below or Equal)
    JNB_I8,      // JNB imm8 imm8  (Jump if Not Below)
     JA_I8,      //  JA imm8 imm8  (Jump if Above)
    JAE_I8,      // JAE imm8 imm8  (Jump if Above or Equal)
    JNA_I8 = 29, // JNA imm8 imm8  (Jump if Not Above)

    HLT = 255 //
};

static unsigned char memory[256] =
{
    MOV__DST_MEM8__SRC_IMM8, 10, 42,   // mem[10] = 42
    MOV__DST_MEM8__SRC_PTR8, 20, 10,   // mem[20] = mem[mem[10]] = mem[42] (мусор)
    HLT
    /*
    CALL, 3,
    HLT,
    RET,
    HLT,
    'C', '$', '\0', // Строка-1
    'C', '$', '\0', // Строка-2
    HLT
    */
}; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения

static inline void Action()
{
    //static
    unsigned char ip =       0; // указатель инструкции
    //static
    unsigned char sp =     255; // указатель стека
    //static
    unsigned char ef/*zf*/ = 2; // флаг равенства
    //static
    unsigned char bf       = 2; // (below) флаг меньше
    //static
    unsigned char af       = 2; // (above) флаг больше

    void *action[] =
    {
        [ 0] = &&___OPERATION_CODE_1, // JMP
        [ 1] = &&___OPERATION_CODE_2, // INC
        [ 2] = &&___OPERATION_CODE_3, // DEC
        //
        [ 3] = &&___OPERATION_CODE_4, // MOV mem8 <~ imm8 (Intel: dst src)
        [ 4] = &&___OPERATION_CODE_5, // MOV imm8 ~> mem8 ( AT&T: src dst)
        //
        [ 5] = &&___OPERATION_CODE_6, // MOV mem8 <~ mem8 (Intel: dst src)
        [ 6] = &&___OPERATION_CODE_7, // MOV mem8 ~> mem8 ( AT&T: src dst)
        //
        [ 7] = &&___OPERATION_CODE_8, // MOV ptr8 <~ imm8 (Intel: dst src)
        [ 8] = &&___OPERATION_CODE_9, // MOV imm8 ~> ptr8 ( AT&T: src dst)
        //
        [ 9] = &&___OPERATION_CODE_10, // MOV ptr8 <~ mem8 (Intel: dst src)
        [10] = &&___OPERATION_CODE_11, // MOV mem8 ~> ptr8 ( AT&T: src dst)
        //
        [11] = &&___OPERATION_CODE_12, // MOV mem8 <~ ptr8 (Intel: dst src)
        [12] = &&___OPERATION_CODE_13, // MOV ptr8 ~> mem8 ( AT&T: src dst)
        //
        [13] = &&___OPERATION_CODE_14, // ADD
        [14] = &&___OPERATION_CODE_15, // SUB
        [15] = &&___OPERATION_CODE_16, // MUL
        [16] = &&___OPERATION_CODE_17, // DIV
        //
        [17] = &&___OPERATION_CODE_18, // CMP
        [18] = &&___OPERATION_CODE_19, // CALL
        [19] = &&___OPERATION_CODE_20, // RET
        [20] = &&___OPERATION_CODE_21, // PUSH
        [21] = &&___OPERATION_CODE_22, // POP

        // Условные операторы (Specifics: Intel/AT&T: src src)
        [22] = &&___OPERATION_CODE_23, //  JE imm8 imm8  (Jump if Equal)
        [23] = &&___OPERATION_CODE_24, // JNE imm8 imm8  (Jump if Not Equal)
        [24] = &&___OPERATION_CODE_25, //  JB imm8 imm8  (Jump if Below)
        [25] = &&___OPERATION_CODE_26, // JBE imm8 imm8  (Jump if Below or Equal)
        [26] = &&___OPERATION_CODE_27, // JNB imm8 imm8  (Jump if Not Below)
        [27] = &&___OPERATION_CODE_28, //  JA imm8 imm8  (Jump if Above)
        [28] = &&___OPERATION_CODE_29, // JAE imm8 imm8  (Jump if Above or Equal)
        [29] = &&___OPERATION_CODE_30, // JNA imm8 imm8  (Jump if Not Above)

        [30 ... 254] = &&___OPERATION_CODE_FROM_31_TO_255,
        [255] = &&___OPERATION_CODE_256 // HLT
    };
   #define DEBUG
   #ifdef DEBUG
    printf("\n Запуск vCPU.");
   #endif
    goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_1: // JMP
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JMP");
    #endif
     ip = memory[ip+1];
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_2: // INC [инкремент произвольной ячейкой памяти]
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "INC");
    #endif
     memory[memory[ip+1]]++;
     ip += 2;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_3: // DEC [декремент произвольной ячейкой памяти]
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "DEC");
    #endif
     memory[memory[ip+1]]--;
     ip += 2;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_4: // MOV mem8 <~ imm8  (Intel: dst, src)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV mem8 <~ imm8  (Intel: dst, src)");
    #endif
     memory[memory[ip+1]] = memory[ip+2];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_5: // MOV imm8 ~> mem8  (AT&T: src, dst)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV imm8 ~> mem8  (AT&T: src, dst)");
    #endif
     memory[memory[ip+2]] = memory[ip+1];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_6: // MOV mem8 <~ mem8  (Intel)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV mem8 <~ mem8  (Intel)");
    #endif
     memory[memory[ip+1]] = memory[memory[ip+2]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_7: // MOV mem8 ~> mem8  (AT&T)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV mem8 ~> mem8  (AT&T)");
    #endif
     memory[memory[ip+2]] = memory[memory[ip+1]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_8: // MOV ptr8 <~ imm8  (Intel: dst_ptr, src_imm)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV ptr8 <~ imm8  (Intel: dst_ptr, src_imm)");
    #endif
     memory[memory[ip+1]] = memory[ip+2]; // Записать imm8 в память по адресу, который лежит в ячейке dst_ptr
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_9: // MOV imm8 ~> ptr8  (AT&T: src_imm, dst_ptr)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV imm8 ~> ptr8  (AT&T: src_imm, dst_ptr)");
    #endif
     memory[memory[ip+2]] = memory[ip+1]; // То же самое: записать imm8 по адресу из dst_ptr
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_10: // MOV ptr8 <~ mem8  (Intel: dst_ptr, src_mem)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV ptr8 <~ mem8  (Intel: dst_ptr, src_mem)");
    #endif
     memory[memory[ip+1]] = memory[memory[ip+2]]; // Взять значение из src_mem и записать его по адресу, хранящемуся в dst_ptr
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_11: // MOV mem8 ~> ptr8  (AT&T: src_mem, dst_ptr)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV mem8 ~> ptr8  (AT&T: src_mem, dst_ptr)");
    #endif
     memory[memory[ip+2]] = memory[memory[ip+1]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_12: // MOV mem8 <~ ptr8  (Intel)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV mem8 <~ ptr8  (Intel)");
    #endif
     memory[memory[ip+1]] = memory[memory[memory[ip+2]]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_13: // MOV ptr8 ~> mem8  (AT&T)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MOV ptr8 ~> mem8  (AT&T)");
    #endif
     memory[memory[ip+2]] = memory[memory[memory[ip+1]]];
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_14: // ADD
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "ADD");
    #endif
     return;
    //////////////////////////////
    ___OPERATION_CODE_15: // SUB
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "SUB");
    #endif
     return;
    //////////////////////////////
    ___OPERATION_CODE_16: // MUL
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "MUL");
    #endif
     return;
    //////////////////////////////
    ___OPERATION_CODE_17: // DIV
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "DIV");
    #endif
     return;
    //////////////////////////////
    ___OPERATION_CODE_18: // CMP
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "CMP");
    #endif
     ef = (memory[ip+1] == memory[ip+2]);
     ip += 3;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_19: // CALL
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "CALL");
    #endif
     memory[sp--] = ip + 2; // Сохраняет адрес следующей команды (текущий ip + 2 байта: опкод + аргумент)
     ip = memory[ip+1]; // Переход по адресу, указанному в аргументе
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_20: // RET
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "RET");
    #endif
     ip = memory[++sp]; // Достаёт адрес возврата и ставит ip на него
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_21: // PUSH
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "PUSH");
    #endif
     memory[sp--] = memory[memory[ip+1]];
     ip += 2;
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_22: // POP mem8
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "POP");
    #endif
     memory[memory[ip+1]] = memory[++sp];
     ip += 2; // opcode + arg
     goto *action[memory[ip]];
    //////////////////////////////
    ___OPERATION_CODE_23: //  JE imm8 imm8  (Jump if Equal)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JE imm8 imm8  (Jump if Equal)");
    #endif
     if (ef) ip = memory[ip+1]; // Перемещение указателя по новому адресу
     else    ip += 2;           // Пропуск инструкции JE (1 байт) и её аргумент-адрес (1 байт)
     goto *action[memory[ip]];  // Переход на опкод, который лежит по новому адресу
    //////////////////////////////
    ___OPERATION_CODE_24: // JNE imm8 imm8  (Jump if Not Equal)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JNE imm8 imm8  (Jump if Not Equal)");
    #endif
     if (!ef) ip = memory[ip+1]; // Перемещение указателя по новому адресу
     else     ip += 2;           // Пропуск инструкции JNE (1 байт) и её аргумент-адрес (1 байт)
     goto *action[memory[ip]];   // Переход на опкод, который лежит по новому адресу
    //////////////////////////////
    ___OPERATION_CODE_25: //  JB imm8 imm8  (Jump if Below)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JB imm8 imm8  (Jump if Below)");
    #endif
     return; // Заглушка
    //////////////////////////////
    ___OPERATION_CODE_26: // JBE imm8 imm8  (Jump if Below or Equal)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JBE imm8 imm8  (Jump if Below or Equal)");
    #endif
     return; // Заглушка
    //////////////////////////////
    ___OPERATION_CODE_27: // JNB imm8 imm8  (Jump if Not Below)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JNB imm8 imm8  (Jump if Not Below)");
    #endif
     return; // Заглушка
    //////////////////////////////
    ___OPERATION_CODE_28: //  JA imm8 imm8  (Jump if Above)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JA imm8 imm8  (Jump if Above)");
    #endif
     return; // Заглушка
    //////////////////////////////
    ___OPERATION_CODE_29: // JAE imm8 imm8  (Jump if Above or Equal)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JAE imm8 imm8  (Jump if Above or Equal)");
    #endif
     return; // Заглушка
    //////////////////////////////
    ___OPERATION_CODE_30: // JNA imm8 imm8  (Jump if Not Above)
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "JNA imm8 imm8  (Jump if Not Above)");
    #endif
     return; // Заглушка
    //////////////////////////////
    ___OPERATION_CODE_FROM_31_TO_255:
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "UNKNOWN OPERAND: 31~255");
    #endif
     return; // Экстремальный выход
     //goto *action[memory[++ip]]; // Крутим дальше
    //////////////////////////////
    ___OPERATION_CODE_256: // Остановить/завершить выполнение программы
    #ifdef DEBUG
     ShowDashboard(memory, ip, sp, "HLT");
    #endif
     return; // Экстремальный выход
    //////////////////////////////
}