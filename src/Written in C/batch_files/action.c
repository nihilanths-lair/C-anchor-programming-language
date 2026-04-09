enum
{
    JMP,  // Безусловный переход
    INC,  // Инкремент произвольной ячейки памяти
    DEC,  // Декремент произвольной ячейки памяти
    MOV,  // Перемещение данных (перезапись)
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
#define _opcode_1 JMP
#define _opcode_2 INC
#define _opcode_3 DEC
#define _opcode_4 MOV
#define _opcode_5 ADD
#define _opcode_6 SUB
#define _opcode_7 MUL
#define _opcode_8 DIV
#define _opcode_9 CMP
#define _opcode_10 CALL
#define _opcode_11 RET
#define _opcode_12 PUSH
#define _opcode_13 POP
#define _opcode_14 JE
#define _opcode_15 JNE
#define _opcode_256 HLT
//#define _opcode_from_4_to_255 NOP
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
static unsigned char ip; // Instruction pointer / Регистр-указатель на инструкцию (след. команду)
ip =   0;
static unsigned char sp; // Stack pointer / Регистр-указатель на стек
sp = 255;
static unsigned char cf; // Comparison flag / Флаг сравнения
cf =   0;
static unsigned char a;
a = 0;
static unsigned char b;
b = 0;

static inline void Action()
{
    void *action[] =
    {
        [0] = &&_opcode_1, // JMP
        [1] = &&_opcode_2, // INC
        [2] = &&_opcode_3, // DEC
        [3] = &&_opcode_4, // ADD
        [4] = &&_opcode_5, // SUB
        [5] = &&_opcode_6, // MUL
        [6] = &&_opcode_7, // DIV
        [7] = &&_opcode_8, // MOV
        [8] = &&_opcode_9, // CMP
        [9] = &&_opcode_10, // CALL
        [10] = &&_opcode_11, // RET
        [11] = &&_opcode_12, // PUSH
        [12] = &&_opcode_13, // POP
        [13] = &&_opcode_14, // JE
        [14] = &&_opcode_15, // JNE
        [15 ... 254] = &&_opcode_from_16_to_255,
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
    _opcode_4: // MOV
     ShowDashboard(memory, ip, sp, "MOV");
     #if !defined INTEL //@Specifics: Intel (справа налево): dest:mem8 <-. src:imm8
     memory[memory[ip+1]] = memory[ip+2]; // Берёт прямое значение из аргумента и кладёт его по адресу в памяти.
     #endif
     #if defined AT_T //@Specifics: AT&T (слева направо): src:imm8 .-> dest:mem8
     memory[memory[ip+2]] = memory[ip+1]; // Берёт прямое значение из аргумента и кладёт его по адресу в памяти.
     #endif
     ip += 3; // Сдвиг на следующую команду (OP + ARG1 + ARG2)
     goto *action[memory[ip]];
    //-/
    _opcode_5: // ADD
    //-/
    _opcode_6: // SUB
    //-/
    _opcode_7: // MUL
    //-/
    _opcode_8: // DIV
    //-/
    _opcode_9: // CMP
     ShowDashboard(memory, ip, sp, "CMP");
     a = memory[ip+1];
     b = memory[ip+2];
     cf = (a == b);
     ip += 3; // Сдвиг на следующую команду (OP + ARG1 + ARG2)
     goto *action[memory[ip]];
    //-/
    _opcode_10: // CALL
     ShowDashboard(memory, ip, sp, "CALL");
     memory[sp--] = ip + 2; // Сохраняет адрес следующей команды (текущий ip + 2 байта: опкод + аргумент)
     ip = memory[ip+1]; // Переход по адресу, указанному в аргументе
     goto *action[memory[ip]];
    //-/
    _opcode_11: // RET
     ShowDashboard(memory, ip, sp, "RET");
     ip = memory[++sp]; // Достаёт адрес возврата и ставит ip на него
     goto *action[memory[ip]];
    //-/
    _opcode_12: // PUSH
     ShowDashboard(memory, ip, sp, "PUSH");
     memory[sp--] = memory[ip+1];
     ip += 2; // Сдвиг на следующую команду (OP + ARG)
     goto *action[memory[ip]];
    //-/
    _opcode_13: // POP
     ShowDashboard(memory, ip, sp, "POP");
     sp++;
     ip += 1; // У POP обычно нет аргумента, просто сдвиг на 1
     goto *action[memory[ip]];
    //-/
    _opcode_14: // JE
     ShowDashboard(memory, ip, sp, "JE");
     if (cf) ip = memory[ip+1]; // Перемещение указателя по новому адресу
     else    ip += 2;           // Пропуск инструкции JE (1 байт) и её аргумент-адрес (1 байт)
     goto *action[memory[ip]];  // Переход на опкод, который лежит по новому адресу
    //-/
    _opcode_15: // JNE
     ShowDashboard(memory, ip, sp, "JNE");
     if (!cf) ip = memory[ip+1]; // Перемещение указателя по новому адресу
     else     ip += 2;           // Пропуск инструкции JNE (1 байт) и её аргумент-адрес (1 байт)
     goto *action[memory[ip]];   // Переход на опкод, который лежит по новому адресу
    //-/
    _opcode_from_16_to_255: // default
     ShowDashboard(memory, ip, sp, "UNKNOWN OPERAND: 16~255");
     goto _extreme_exit; // Просто завершаем
     //goto *action[memory[++ip]]; // Крутим дальше
    //-/
    _opcode_256: // Остановить/завершить выполнение программы
     printf("\n Стоп.");
     //goto _extreme_exit; // Просто завершаем
    //-/
    _extreme_exit:
}