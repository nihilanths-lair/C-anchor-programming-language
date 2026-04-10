enum
{
    JMP,  // Безусловный переход
    INC,  // Инкремент произвольной ячейки памяти
    DEC,  // Декремент произвольной ячейки памяти
    //- Перемещение данных (перезапись) -/
    MOV_MEM8_IMM8, // 3
    MOV_IMM8_MEM8, // 4

    
    //-/
    ADD,  // Сложение
    SUB,  // Вычитание
    MUL,  // Умножение
    DIV,  // Деление
    CMP,  // Сравнение прямых значений
    CALL, // Позвонить (вызвать процедуру)
    RET,  // Вернуться из процедуры
    PUSH, // Положить в стек
    POP,  // Снять со стека
    JE,   // Условный оператор
    JNE,  // Условный оператор (инвертированный)
    HLT  = 255 // Завершить выполнение программы
};

static unsigned char memory[256] =
{
    CALL, 3,
    //CMP, 0, 0,
    HLT,
    //CMP, 0, 0,
    RET,
    HLT,
    'C', '$', '\0', // Строка-1
    'C', '$', '\0', // Строка-2
    HLT
}; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения

static inline void Action()
{
    unsigned char ip =   0; // Instruction pointer / Регистр-указатель на инструкцию (след. команду)
    unsigned char sp = 255; // Stack pointer / Регистр-указатель на стек
    unsigned char cf =   2; // Comparison flag / Флаг сравнения

    void *action[] =
    {
        [0] = &&_opcode_1, // JMP
        [1] = &&_opcode_2, // INC
        [2] = &&_opcode_3, // DEC

        [3] = &&_opcode_4, // MOV mem8 <~ imm8 (Intel: dst src)
        [4] = &&_opcode_5, // MOV imm8 ~> mem8 ( AT&T: src dst)

        [5] = &&_opcode_6, // MOV mem8 <~ mem8 (Intel: dst src)
        [6] = &&_opcode_7, // MOV mem8 ~> mem8 ( AT&T: src dst)

        [7] = &&_opcode_8, // ADD
        [8] = &&_opcode_9, // SUB
        [9] = &&_opcode_10, // MUL
        [10] = &&_opcode_11, // DIV
        [11] = &&_opcode_12, // CMP
        [12] = &&_opcode_13, // CALL
        [13] = &&_opcode_14, // RET
        [14] = &&_opcode_15, // PUSH
        [15] = &&_opcode_16, // POP
        [16] = &&_opcode_17, // JE
        [17] = &&_opcode_18, // JNE
        [18 ... 254] = &&_opcode_from_19_to_255,
        [255] = &&_opcode_256 // HLT
    };

    printf("\n Старт.");
    goto *action[memory[ip]];
    //-/
    _opcode_1: // JMP
     ShowDashboard(memory, ip, sp, "JMP");
     ip = memory[ip+1]; // Берёт адрес из аргумента
     goto *action[memory[ip]];
    //-/
    _opcode_2: // INC [инкремент произвольной ячейкой памяти]
     ShowDashboard(memory, ip, sp, "INC"); //@Specifics: Intel/AT&T
     memory[memory[ip+1]]++; // Берёт из аргумента адрес ячейки памяти, и производит над ним инкрементацию.
     ip += 2;
     goto *action[memory[ip]];
    //-/
    _opcode_3: // DEC [декремент произвольной ячейкой памяти]
     ShowDashboard(memory, ip, sp, "DEC"); //@Specifics: Intel/AT&T
     memory[memory[ip+1]]--; // Берёт из аргумента адрес ячейки памяти, и производит над ним декрементацию.
     ip += 2;
     goto *action[memory[ip]];
    //-/
    _opcode_4: // MOV mem8:dst imm8:src (Intel)
     ShowDashboard(memory, ip, sp, "MOV mem8:dst imm8:src");
     memory[memory[ip+1]] = memory[ip+2]; // mem8 <- imm8
     ip += 3;
     goto *action[memory[ip]];

    _opcode_5: // MOV imm8:src mem8:dst (AT&T)
     ShowDashboard(memory, ip, sp, "MOV imm8:src mem8:dst");
     memory[memory[ip+2]] = memory[ip+1]; // imm8 -> mem8
     ip += 3;
     goto *action[memory[ip]];
    //-/
    _opcode_6: //
    //-/
    _opcode_7: //
    //-/
    _opcode_8: // ADD
    //-/
    _opcode_9: // SUB
    //-/
    _opcode_10: // MUL
    //-/
    _opcode_11: // DIV
    //-/
    _opcode_12: // CMP
     ShowDashboard(memory, ip, sp, "CMP");
     cf = (memory[ip+1] == memory[ip+2]);
     ip += 3; // Сдвиг на следующую команду (OP + ARG1 + ARG2)
     goto *action[memory[ip]];
    //-/
    _opcode_13: // CALL
     ShowDashboard(memory, ip, sp, "CALL");
     memory[sp--] = ip + 2; // Сохраняет адрес следующей команды (текущий ip + 2 байта: опкод + аргумент)
     ip = memory[ip+1]; // Переход по адресу, указанному в аргументе
     goto *action[memory[ip]];
    //-/
    _opcode_14: // RET
     ShowDashboard(memory, ip, sp, "RET");
     ip = memory[++sp]; // Достаёт адрес возврата и ставит ip на него
     goto *action[memory[ip]];
    //-/
    _opcode_15: // PUSH
     ShowDashboard(memory, ip, sp, "PUSH");
     memory[sp--] = memory[ip+1];
     ip += 2; // Сдвиг на следующую команду (OP + ARG)
     goto *action[memory[ip]];
    //-/
    _opcode_16: // POP
     ShowDashboard(memory, ip, sp, "POP");
     sp++;
     ip += 1; // У POP обычно нет аргумента, просто сдвиг на 1
     goto *action[memory[ip]];
    //-/
    _opcode_17: // JE
     ShowDashboard(memory, ip, sp, "JE");
     if (cf) ip = memory[ip+1]; // Перемещение указателя по новому адресу
     else    ip += 2;           // Пропуск инструкции JE (1 байт) и её аргумент-адрес (1 байт)
     goto *action[memory[ip]];  // Переход на опкод, который лежит по новому адресу
    //-/
    _opcode_18: // JNE
     ShowDashboard(memory, ip, sp, "JNE");
     if (!cf) ip = memory[ip+1]; // Перемещение указателя по новому адресу
     else     ip += 2;           // Пропуск инструкции JNE (1 байт) и её аргумент-адрес (1 байт)
     goto *action[memory[ip]];   // Переход на опкод, который лежит по новому адресу
    //-/
    _opcode_from_19_to_255: // default
     ShowDashboard(memory, ip, sp, "UNKNOWN OPERAND: 19~255");
     goto _extreme_exit; // Просто завершаем
     //goto *action[memory[++ip]]; // Крутим дальше
    //-/
    _opcode_256: // Остановить/завершить выполнение программы
     printf("\n Стоп.");
     //goto _extreme_exit; // Просто завершаем
    //-/
    _extreme_exit:
}