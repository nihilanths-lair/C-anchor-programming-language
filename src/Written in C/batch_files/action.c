enum
{
     JMP  =  1,
     CMP,       // Сравнение прямых значений
     LEA,       // Load Effective Address / Загрузить эффективно адрес
      JE,       // Если равно (условие истинно)
     JNE,       // Если не равно (условие не истинно)
    CALL,       // Позвонить (вызвать процедуру)
    PUSH,       // Положить в стек
     POP,       // Снять со стека
     HLT  = 255 // Завершить выполнение программы
};
#define _opcode_1 JMP
#define _opcode_2 CMP
#define _opcode_3 CALL
//#define _opcode_from_4_to_255 NOP
static unsigned char memory[255] =
{
    CALL, 1,
    CMP, 0, 0, // CMP r/r, CMP m/m, CMP r/i, CMP m/i
    JE, 0,
    'C', '$', '\0', // Строка-1
    'C', '$', '\0', // Строка-2
    HLT
}; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения
static unsigned char ip =   0; // Instruction pointer / Регистр-указатель на инструкцию (след. команду)
static unsigned char cf =   0; //     Comparison flag / Флаг сравнения
static unsigned char sp = 255; //       Stack pointer / Регистр-указатель на стек

static inline void Action()
{
    void *action[] =
    {
        [0] = &&_opcode_1,
        [1] = &&_opcode_2,
        [2] = &&_opcode_3,
        [3 ... 255] = &&_opcode_from_4_to_255
    };

    printf("\n Старт.");
    ShowDashboard(memory); // Логируем
    goto *action[memory[ip]];

    _opcode_1: // Безусловный переход
     ShowDashboard(memory); // Логируем
     ip = memory[++ip];
     goto *action[memory[ip]];

    _opcode_2: // Операция сравнения
     ShowDashboard(memory); // Логируем
     cf = (memory[++ip] == memory[++ip]);
     goto *action[memory[++ip]];

    _opcode_3: // Позвонить (вызвать процедуру)
     ShowDashboard(memory); // Логируем
     memory[sp] = ip; // Положить в стек адрес точки возврата из процедуры
     goto *action[memory[++ip]];

    _opcode_from_4_to_255: // default
     printf("\n \\4~\\255");
}