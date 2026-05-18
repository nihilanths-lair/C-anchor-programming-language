//#define MACRO__VIRTUAL_ADDRESS (cs8 << 8) + ip8 // максимально допустимая при двух 8-ми битных регистрах
//#define MACRO__VIRTUAL_ADDRESS (cs16 << 8) + ip16 // максимально допустимая при двух 16-ти битных регистрах

unsigned char memory_tape[MACRO__MAXIMUM_CODE_LIMIT]; // Лента памяти
unsigned char * _ip = memory_tape; // Регистр-указатель на инструкцию
unsigned char * _sp = &memory_tape[MACRO__MAXIMUM_CODE_LIMIT-1]; // Регистр-указатель на стек

uint8_t cs8 = 0; // (unsigned char) 8-bit's сегментный-регистр
uint8_t dp8 = 0; // (unsigned char) 8-bit's регистр-указатель на данные
uint8_t si8 = 0; // (unsigned char) 8-bit's
uint8_t di8 = 0; // (unsigned char) 8-bit's
uint8_t  a8 = 0; // (unsigned char) 8-bit's регистр общего назначения
uint8_t  b8 = 0; // (unsigned char) 8-bit's регистр общего назначения
uint8_t  c8 = 0; // (unsigned char) 8-bit's регистр общего назначения
uint8_t  d8 = 0; // (unsigned char) 8-bit's регистр общего назначения (для работы со строками, через указатель)

uint16_t cs16 = 0; // (unsigned short) 16-bit's сегментный-регистр
uint16_t ip16 = 0; // (unsigned short) 16-bit's регистр-указатель на инструкцию
uint16_t sp16 = 0; // (unsigned short) 16-bit's регистр-указатель на стек
uint16_t dp16 = 0; // (unsigned short) 16-bit's регистр-указатель на данные
uint16_t si16 = 0; // (unsigned short) 16-bit's
uint16_t di16 = 0; // (unsigned short) 16-bit's
uint16_t  a16 = 0; // (unsigned short) 16-bit's регистр общего назначения
uint16_t  b16 = 0; // (unsigned short) 16-bit's регистр общего назначения
uint16_t  c16 = 0; // (unsigned short) 16-bit's регистр общего назначения
uint16_t  d16 = 0; // (unsigned short) 16-bit's регистр общего назначения (для работы со строками, через указатель)

uint32_t cs24 = 0; // (unsigned int) 32-bit's сегментный-регистр
uint32_t ip24 = 0; // (unsigned int) 32-bit's регистр-указатель на инструкцию
uint32_t sp24 = 0; // (unsigned int) 32-bit's регистр-указатель на стек
int32_t   a24 = 0; //          (int) 32-bit's регистр общего назначения
int32_t   b24 = 0; //          (int) 32-bit's регистр общего назначения

uint32_t cs32 = 0; // (unsigned int) 32-bit's сегментный-регистр
uint32_t ip32 = 0; // (unsigned int) 32-bit's регистр-указатель на инструкцию
uint32_t sp32 = 0; // (unsigned int) 32-bit's регистр-указатель на стек
int32_t   a32 = 0; //          (int) 32-bit's регистр общего назначения
int32_t   b32 = 0; //          (int) 32-bit's регистр общего назначения

uint64_t cs64 = 0; // (unsigned long long) 64-bit's сегментный-регистр
uint64_t ip64 = 0; // (unsigned long long) 64-bit's регистр-указатель на инструкцию
uint64_t sp64 = 0; // (unsigned long long) 64-bit's регистр-указатель на стек
int64_t   a64 = 0; //          (long long) 64-bit's регистр общего назначения
int64_t   b64 = 0; //          (long long) 64-bit's регистр общего назначения

unsigned char zf = 0; // Флаг нуля: 1 если результаты равны, 0 если нет
//
/// Для экспериментов ///
int8_t * _rcv8 = "Hello";
int8_t rcv8 = 0; // char
int8_t src8 = 0; // char

int16_t rcv16 = 0; // short
int16_t src16 = 0; // short

int32_t rcv32 = 0; // int
int32_t src32 = 0; // int

int64_t rcv64 = 0; // long long
int64_t src64 = 0; // long long
//-/
void Loader_VM()
{
    //printf("\n    До | %03d %03d %03d = %02X %02X %02X", opcodes[0], opcodes[1], opcodes[2], opcodes[0], opcodes[1], opcodes[2]);
    //printf("\n    До | %03d %03d %03d = %02X %02X %02X", memory_tape[0], memory_tape[1], memory_tape[2], memory_tape[0], memory_tape[1], memory_tape[2]);
    int len = strlen(ga__opcodes);//+1
    for (int i = 0; i < len; i++) memory_tape[i] = ga__opcodes[i];
    //printf("\n После | %03d %03d %03d = %02X %02X %02X", memory_tape[0], memory_tape[1], memory_tape[2], memory_tape[0], memory_tape[1], memory_tape[2]);
}
//
void Debug_Loader_VM()
{
    //putchar('\n');
    int len = strlen(_ip);//+1
    printf("\n Оп-код (dec):");
    for (int i = 0; i < len; i++) printf(" %03d", memory_tape[i]);
    printf("\n Оп-код (hex):");
    for (int i = 0; i < len; i++) printf("  %02X", memory_tape[i]);
}
//
// Функция для 8-битных регистров (вывод в формате 0000:0000)
char * bin8(unsigned char num)
{
    static char sbin[9+1]; // 8 бит + 1 двоеточие + 1 нуль-терминал
    sbin[9] = '\0';
    sbin[4] = '_'; // Разделитель тетрад

    int pos = 8;
    for (int i = 0; i < 8; i++)
    {
        if (pos == 4) pos--; // Пропускаем позицию двоеточия

        sbin[pos--] = (num % 2 == 0) ? '0':'1';
        num /= 2;
    }
    return sbin;
}

// Функция для 16-битных регистров (вывод в формате 0000:0000,0000:0000)
char * bin16(unsigned short num)
{
    static char sbin[19+1]; // 16 бит + 3 разделителя + 1 нуль-терминал
    sbin[19] = '\0';
    sbin[4]  = '_'; // Разделитель тетрад
    sbin[9]  = ' ';
    sbin[14] = '_'; // Разделитель тетрад

    int pos = 18;
    for (int i = 0; i < 16; i++)
    {
        if (pos == 14 || pos == 9 || pos == 4) pos--; // Пропускаем разделители

        sbin[pos--] = (num % 2 == 0) ? '0':'1';
        num /= 2;
    }
    return sbin;
}
//
/// Функция форматирования чисел с разделением тысяч апострофом ///
char * numf(long long num)
{
    // 4 буфера, чтобы можно было вызвать функцию до 4 раз внутри одного printf
    static char buffers[4][32];
    static int buf_idx = 0;
    
    // Выбираем текущий буфер и очищаем его
    char * str = buffers[buf_idx];
    buf_idx = (buf_idx + 1) % 4; 
    
    char temp[32];
    // Переводим число в обычную строку
    sprintf(temp, "%lld", num);
    
    int len = strlen(temp);
    int insert_quotes = (len - 1) / 3; // Сколько апострофов нужно вставить
    int new_len = len + insert_quotes;
    
    str[new_len] = '\0';
    
    int src = len - 1;
    int dst = new_len - 1;
    int digit_count = 0;
    
    // Идем с конца строки и копируем цифры, вставляя апостроф каждые 3 знака
    while (src >= 0)
    {
        if (digit_count == 3)
        {
            str[dst--] = '\''; // Если нужен пробел, замените '\'' на ' '
            digit_count = 0;
        }
        str[dst--] = temp[src--];
        digit_count++;
    }
    return str;
}
//
void dbg_RegisterState()
{
    unsigned char ip = _ip - memory_tape;
    unsigned char sp = _sp - memory_tape;
    static unsigned short step = -1;
    printf("\n -----------\n Шаг: %d", step++);
    switch (1){
    case 0:
    {
        printf("\n -----------------------------------");
        printf("\n  REGISTER |   DEC   |  HEX  | TEXT");
        printf("\n           |         |       |");
        printf("\n       zf  |     %03d | %02X    | %d",  zf,  zf,  zf);
        printf("\n           |         |       |");
        printf("\n      ip8  |     %03d | %02X    | %d", ip,  ip,  ip);
        printf("\n      sp8  |     %03d | %02X    | %d", sp,  sp,  sp);
        printf("\n      dp8  |     %03d | %02X    | %d", dp8, dp8, dp8);
        printf("\n      si8  |     %03d | %02X    | %d", si8, si8, si8);
        printf("\n      di8  |     %03d | %02X    | %d", di8, di8, di8);
        printf("\n       a8  |     %03d | %02X    | %d",  a8,  a8,  a8);
        printf("\n       b8  |     %03d | %02X    | %d",  b8,  b8,  b8);
        printf("\n       c8  |     %03d | %02X    | %d",  c8,  c8,  c8);
        printf("\n       d8  |     %03d | %02X    | %d",  d8,  d8,  d8);
        printf("\n           |         |       |");
        printf("\n      ip16 | %03d %03d | %02X %02X | %s", ip16>>8, ip16&0xFF, ip16>>8, ip16&0xFF, numf(ip16));
        printf("\n      sp16 | %03d %03d | %02X %02X | %s", sp16>>8, sp16&0xFF, sp16>>8, sp16&0xFF, numf(sp16));
        printf("\n      dp16 | %03d %03d | %02X %02X | %s", dp16>>8, dp16&0xFF, dp16>>8, dp16&0xFF, numf(dp16));
        printf("\n      si16 | %03d %03d | %02X %02X | %s", si16>>8, si16&0xFF, si16>>8, si16&0xFF, numf(si16));
        printf("\n      di16 | %03d %03d | %02X %02X | %s", di16>>8, di16&0xFF, di16>>8, di16&0xFF, numf(di16));
        printf("\n       a16 | %03d %03d | %02X %02X | %s",  a16>>8,  a16&0xFF,  a16>>8,  a16&0xFF,  numf(a16));
        printf("\n       b16 | %03d %03d | %02X %02X | %s",  b16>>8,  b16&0xFF,  b16>>8,  b16&0xFF,  numf(b16));
        printf("\n       c16 | %03d %03d | %02X %02X | %s",  c16>>8,  c16&0xFF,  c16>>8,  c16&0xFF,  numf(c16));
        printf("\n       d16 | %03d %03d | %02X %02X | %s",  d16>>8,  d16&0xFF,  d16>>8,  d16&0xFF,  numf(d16));
        printf("\n -------------------------------------");
        break;
    }
    case 1:
    {
        printf("\n ---------------------------------------------------------");
        printf("\n  REGISTER |   DEC   |  HEX  |         BIN         | TEXT");
        printf("\n           |         |       |                     |");
        // Вывод 8-битных регистров
        printf("\n       zf  |     %03d | %02X    | %s           | %d",  zf,  zf, bin8( zf),  zf);
        printf("\n           |         |       |                     |");
        printf("\n      ip8  |     %03d | %02X    | %s           | %d", ip,  ip,  bin8(ip),  ip);
        printf("\n      sp8  |     %03d | %02X    | %s           | %d", sp,  sp,  bin8(sp),  sp);
        printf("\n      dp8  |     %03d | %02X    | %s           | %d", dp8, dp8, bin8(dp8), dp8);
        printf("\n      si8  |     %03d | %02X    | %s           | %d", si8, si8, bin8(si8), si8);
        printf("\n      di8  |     %03d | %02X    | %s           | %d", di8, di8, bin8(di8), di8);
        printf("\n       a8  |     %03d | %02X    | %s           | %d",  a8,  a8, bin8( a8),  (signed char) a8);
        printf("\n       b8  |     %03d | %02X    | %s           | %d",  b8,  b8, bin8( b8),  b8);
        printf("\n       c8  |     %03d | %02X    | %s           | %d",  c8,  c8, bin8( c8),  c8);
        printf("\n       d8  |     %03d | %02X    | %s           | %d",  d8,  d8, bin8( d8),  d8);
        printf("\n           |         |       |                     |");
        // Вывод 16-битных регистров
        // Разделяем hex на старший и младший байт через битовые сдвиги для формата %02X,%02X
        printf("\n      ip16 | %03d %03d | %02X %02X | %s | %s", ip16>>8, ip16&0xFF, ip16>>8, ip16&0xFF, bin16(ip16), numf(ip16));
        printf("\n      sp16 | %03d %03d | %02X %02X | %s | %s", sp16>>8, sp16&0xFF, sp16>>8, sp16&0xFF, bin16(sp16), numf(sp16));
        printf("\n      dp16 | %03d %03d | %02X %02X | %s | %s", dp16>>8, dp16&0xFF, dp16>>8, dp16&0xFF, bin16(dp16), numf(dp16));
        printf("\n      si16 | %03d %03d | %02X %02X | %s | %s", si16>>8, si16&0xFF, si16>>8, si16&0xFF, bin16(si16), numf(si16));
        printf("\n      di16 | %03d %03d | %02X %02X | %s | %s", di16>>8, di16&0xFF, di16>>8, di16&0xFF, bin16(di16), numf(di16));
        printf("\n       a16 | %03d %03d | %02X %02X | %s | %s",  a16>>8,  a16&0xFF,  a16>>8,  a16&0xFF, bin16( a16),  numf(a16));
        printf("\n       b16 | %03d %03d | %02X %02X | %s | %s",  b16>>8,  b16&0xFF,  b16>>8,  b16&0xFF, bin16( b16),  numf(b16));
        printf("\n       c16 | %03d %03d | %02X %02X | %s | %s",  c16>>8,  c16&0xFF,  c16>>8,  c16&0xFF, bin16( c16),  numf(c16));
        printf("\n       d16 | %03d %03d | %02X %02X | %s | %s",  d16>>8,  d16&0xFF,  d16>>8,  d16&0xFF, bin16( d16),  numf(d16));
        printf("\n -----------------------------------------------------------");
    }}
}
//
void dbg_MemoryState()
{
    switch (0){
    case 0:
    {
        printf("\n MEMORY: DEC, HEX\n");
        unsigned char i2;
        for (unsigned char i = 0; i < 16; i++)
        {
            i2 = i*16;
            printf("\n  %3d | ", i2);
            for (unsigned char j = 0; j < 8; j++) printf("%03d ", memory_tape[i2+j]);
            printf(" \t %2X | ", i2);
            for (unsigned char j = 0; j < 8; j++) printf("%02X ", memory_tape[i2+j]);

            i2 += 8;
            printf("\n  %3d | ", i2);
            for (unsigned char j = 8; j < 16; j++) printf("%03d ", memory_tape[i2+j]);
            printf(" \t %2X | ", i2);
            for (unsigned char j = 8; j < 16; j++) printf("%02X ", memory_tape[i2+j]);
            //printf("\n ---------------------------------------\t------------------------------");
        }
    }
    case 1:
    {
        // ... //
    }}
}
//
void Disassembly()
{
    // ... //
}
//
// Для отключения отладки и включения турбо-режима необходимо закомментировать строку ниже:
#define VM_DEBUG_MODE
//
void Executor_VM() // Spin / Executor (исполнитель) / Evaluator (древоходец)
{
    static void * transaction_codes[256] =
    {
        [0 ... 255] = &&_80h,
        [1] = &&_1, [2] = &&_2, [3] = &&_3,
        [4] = &&_4, [5] = &&_5, [6] = &&_6,
        [7] = &&_7, [8] = &&_8, [9] = &&_9,
        [10] = &&_10, [11] = &&_11, [12] = &&_12,
        [13] = &&_13, [14] = &&_14, // inc/dec a8
        [0x75] = &&_75h, [0x76] = &&_76h, [0x77] = &&_77h,
        [0x78] = &&_78h, [0x79] = &&_79h
    };
    // Интеллектуальный макрос диспетчеризации
    #ifdef VM_DEBUG_MODE
        #define DISPATCH() \
            dbg_RegisterState(); \
            dbg_MemoryState(); \
            goto *transaction_codes[*_ip]
        
        // В режиме отладки макрос PRINT_OPCODE будет выводить логи
        #define PRINT_OPCODE() printf("\n \\d%03d = \\h%02X", *_ip, *_ip)
    #else
        // В турбо-режиме макрос превращается в ОДНУ строчку перехода, без вызова функций отладки
        #define DISPATCH() goto *transaction_codes[*_ip]
        
        // В турбо-режиме принт превращается в пустоту и полностью удаляется компилятором
        #define PRINT_OPCODE() 
    #endif

    // Старт виртуалки
    DISPATCH();

    _75h: // jmp i8
    {
        PRINT_OPCODE();
        _ip += (char) _ip[1] + 2;
        DISPATCH();
    }
    _1: // mov a8, i8
    {
        PRINT_OPCODE();
        a8 = *(++_ip);
        ++_ip;
        DISPATCH();
    }
    _2: // mov i8, b8
    {
        PRINT_OPCODE();
        b8 = *(++_ip);
        ++_ip;
        DISPATCH();
    }
    _3: // add a8, i8
    {
        PRINT_OPCODE();
        a8 += *(++_ip);
        ++_ip;
        DISPATCH();
    }
    _4: // sub a8, i8
    {
        PRINT_OPCODE();
        a8 -= *(++_ip);
        ++_ip;
        DISPATCH();
    }
    _5: // mul a8, i8
    {
        PRINT_OPCODE();
        a8 *= *(++_ip);
        ++_ip;
        DISPATCH();
    }
    _6: // div a8, i8
    {
        PRINT_OPCODE();
        a8 /= *(++_ip);
        ++_ip;
        DISPATCH();
    }
    _7: // rsub a8, i8
    {
        PRINT_OPCODE();
        a8 = *(++_ip) - a8;
        ++_ip;
        DISPATCH();
    }
    _8: // rdiv a8, i8
    {
        PRINT_OPCODE();
        a8 = *(++_ip) / a8;
        ++_ip;
        DISPATCH();
    }
    _9: // neg a8
    {
        PRINT_OPCODE();
        a8 = -a8;
        ++_ip;
        DISPATCH();
    }
    _10: // 8-bit's addr-on | cmp a8, i8 ; сравнение
    {
        PRINT_OPCODE();
        unsigned char i8 = *(++_ip);
        if (a8 == i8) zf = 1;
        else zf = 0;
        ++_ip;
        DISPATCH();
    }
    _11: // 8-bit's addr-on | je i8 ; перейти, если равно (zf == 1)
    {
        PRINT_OPCODE();
        if (zf == 1) _ip += (char) _ip[1] + 2; // Прыгаем вперед или назад по знаковому смещению
        else _ip += 2; // Если условия нет, просто перешагиваем аргумент смещения
        DISPATCH();
    }
    _12: // 8-bit's addr-on | jne i8 ; перейти, если не равно (zf == 0)
    {
        PRINT_OPCODE();
        if (zf == 0) _ip += (char) _ip[1] + 2; // Прыгаем вперед или назад по знаковому смещению
        else _ip += 2; // Если условия нет, просто перешагиваем аргумент смещения
        DISPATCH();
    }
    _13: // 8-bit's addr-on | inc a8 ; инкремент регистра a8
    {
        PRINT_OPCODE();
        ++a8;
        ++_ip;
        DISPATCH();
    }
    _14: // 8-bit's addr-on | dec a8 ; декремент регистра a8
    {
        PRINT_OPCODE();
        --a8;
        ++_ip;
        DISPATCH();
    }
    _15: // 8-bit's addr-on | push a8 ; стек растет вниз (к началу памяти)
    {
        PRINT_OPCODE();
        --_sp;     // Сдвигаем указатель стека вниз
        *_sp = a8; // Записываем значение регистра
        ++_ip;
        DISPATCH();
    }
    _16: // 8-bit's addr-on | pop a8
    {
        PRINT_OPCODE();
        a8 = *_sp; // Читаем значение из стека
        ++_sp;     // Сдвигаем указатель стека вверх
        ++_ip;
        DISPATCH();
    }
    _17: // 2 байта | call i8 ; Вызов процедуры (Максимальная оптимизация)
    {
        PRINT_OPCODE();
        // 1. Толкаем адрес возврата в стек.
        *(--_sp) = (_ip+2) - memory_tape; // _ip сейчас стоит на опкоде. Следующий байт — аргумент. Инструкция возврата находится ровно через 2 байта от текущего положения.
        // 2. Осуществляем переход.
        _ip = memory_tape + _ip[1]; // Читаем адрес назначения из следующего байта (_ip[1]) и прибавляем к базе памяти.
        DISPATCH();
    }
    _18: // 1 байт | ret ; Вернуться из процедуры
    {
        PRINT_OPCODE();
        _ip = memory_tape + *(_sp++); // Достаем индекс возврата из стека, восстанавливаем указатель хоста и сдвигаем стек вверх.
        DISPATCH();
    }
    _76h: // OUT string
    {
        PRINT_OPCODE();
        printf("%s", _rcv8);
        ++_ip;
        DISPATCH();
    }
    _77h: // OUT number
    {
        PRINT_OPCODE();
        printf("%d", a8);
        ++_ip;
        DISPATCH();
    }
    _78h: // OUT char
    {
        PRINT_OPCODE();
        putchar(a8);
        ++_ip;
        DISPATCH();
    }
    _79h:
        printf("\n Stopped.."); 
        return;
    _80h:
        printf("\n Unknown opcode 0x%02X, stopped..", *_ip); 
        return;

    #undef DISPATCH
    #undef PRINT_OPCODE
}
/*
char * Bin()
{
    long long num = 4; //snum[0]-'0';
    static char sbin[64+1]; // tetrad
    sbin[64] = '\0';
    // Обработка случая, если изначально был введен 0
    if (!num) sbin[63] = '0';
    // Цикл деления на 2
    unsigned char pos = 63;
    while (num > 0)
    {
        if (num % 2 == 0) sbin[--pos] = '0'; // Если делится без остатка
        else sbin[--pos] = '1'; // Если есть остаток 1
        num /= 2; // Уменьшаем число в 2 раза
    }
    // Указатель на начало получившейся бинарной строки
    char * result = &sbin[pos];
    return result; // Возвращаем бинарные данные в виде текста
    //return atoi(result); // Возвращаем бинарные данные в виде числа
}
printf("\n Binary result: \"%s\".\n", result);
*/