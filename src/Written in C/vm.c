/** Виртуальная машина v.0.1 с x86-подобной архитектурой (CISC) */

//#define MACRO__VIRTUAL_ADDRESS (cs8 << 8) + ip8 // максимально допустимая при двух 8-ми битных регистрах
//#define MACRO__VIRTUAL_ADDRESS (cs16 << 8) + ip16 // максимально допустимая при двух 16-ти битных регистрах

unsigned char memory_tape[256];
volatile unsigned char * _ip = memory_tape;
volatile unsigned char * _sp = &memory_tape[255];
volatile unsigned char * _bp = memory_tape; // Базовый указатель кадра стека
//volatile unsigned char * _dp = memory_tape;
volatile unsigned char * _si = memory_tape;
volatile unsigned char * _di = memory_tape;
char a8 = 0;
char b8 = 0;
char c8 = 0;
char d8 = 0; // (unsigned char) 8-bit's регистр общего назначения (для работы со строками, через указатель)

unsigned char zf = 0; // Флаг нуля: 1 если результаты равны, 0 если нет
unsigned char sf = 0; // 1 если результат CMP отрицательный (a8 < i8)
//
/// Для экспериментов ///
int8_t * _rcv8 = "Hello";
char rcv8 = 0;
char src8 = 0;

// Загрузчик теперь принимает точный размер байт-кода
void Loader_VM(int code_size)
{
    // Безопасное копирование бинарных данных, включая нули
    for (int i = 0; i < code_size; i++) memory_tape[i] = ga__opcodes[i];
}

// Отладчик выводит ровно столько байт, сколько было загружено
void Debug_Loader_VM(int code_size)
{
    printf("\n Оп-код (dec):");
    for (int i = 0; i < code_size; i++) printf(" %03d", memory_tape[i]);
    printf("\n Оп-код (hex):");
    for (int i = 0; i < code_size; i++) printf("  %02X", memory_tape[i]);
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
    int ip8 = _ip - memory_tape;
    int sp8 = _sp - memory_tape;
    int bp8 = _bp - memory_tape;
    int si8 = _si - memory_tape;
    int di8 = _di - memory_tape;
    static unsigned char cs8 = 0; // (unsigned char) 8-bit's сегментный-регистр
    static unsigned short cs16 = 0; // (unsigned short) 16-bit's сегментный-регистр
    static unsigned short ip16 = 0; // (unsigned short) 16-bit's регистр-указатель на инструкцию
    static unsigned short sp16 = 0; // (unsigned short) 16-bit's регистр-указатель на стек
    static unsigned short dp16 = 0; // (unsigned short) 16-bit's регистр-указатель на данные
    static unsigned short si16 = 0;
    static unsigned short di16 = 0;
    static unsigned short  a16 = 0;
    static unsigned short  b16 = 0;
    static unsigned short  c16 = 0;
    static unsigned short  d16 = 0; // (unsigned short) 16-bit's регистр общего назначения (для работы со строками, через указатель)
    static unsigned short step = 0;
    printf("\n -----------\n Шаг: %d", step++);
    switch (1){
    case 0:
    {
        printf("\n -----------------------------------");
        printf("\n  REGISTER |   DEC   |  HEX  | TEXT");
        printf("\n           |         |       |");
        printf("\n       zf  |     %03d | %02X    | %d",  zf,  zf,  zf);
        printf("\n       sf  |     %03d | %02X    | %d",  sf,  sf,  sf);
        printf("\n           |         |       |");
        printf("\n      ip8  |     %03d | %02X    | %d", ip8, ip8, ip8);
        printf("\n      sp8  |     %03d | %02X    | %d", sp8, sp8, sp8);
        printf("\n      bp8  |     %03d | %02X    | %d", bp8, bp8, bp8);
        printf("\n      si8  |     %03d | %02X    | %d", si8, si8, si8);
        printf("\n      di8  |     %03d | %02X    | %d", di8, di8, di8);
        printf("\n       a8  |     %03d | %02X    | %d", a8&0xFF, a8&0xFF, a8);
        printf("\n       b8  |     %03d | %02X    | %d", b8&0xFF, b8&0xFF, b8);
        printf("\n       c8  |     %03d | %02X    | %d", c8&0xFF, c8&0xFF, c8);
        printf("\n       d8  |     %03d | %02X    | %d", d8&0xFF, d8&0xFF, d8);
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
        printf("\n       sf  |     %03d | %02X    | %s           | %d",  sf,  sf, bin8( sf),  sf);
        printf("\n           |         |       |                     |");
        printf("\n      ip8  |     %03d | %02X    | %s           | %d", ip8, ip8, bin8(ip8), ip8);
        printf("\n      sp8  |     %03d | %02X    | %s           | %d", sp8, sp8, bin8(sp8), sp8);
        printf("\n      bp8  |     %03d | %02X    | %s           | %d", bp8, bp8, bin8(bp8), bp8);
        printf("\n      si8  |     %03d | %02X    | %s           | %d", si8, si8, bin8(si8), si8);
        printf("\n      di8  |     %03d | %02X    | %s           | %d", di8, di8, bin8(di8), di8);
        printf("\n       a8  |     %03d | %02X    | %s           | %d", a8&0xFF, a8&0xFF, bin8(a8), a8);
        printf("\n       b8  |     %03d | %02X    | %s           | %d", b8&0xFF, b8&0xFF, bin8(b8), b8);
        printf("\n       c8  |     %03d | %02X    | %s           | %d", c8&0xFF, c8&0xFF, bin8(c8), c8);
        printf("\n       d8  |     %03d | %02X    | %s           | %d", d8&0xFF, d8&0xFF, bin8(d8), d8);
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
        [0] = &&_0,
        [1] = &&_1,
        [2] = &&_2,
        [3] = &&_3,
        [4] = &&_4,
        [5] = &&_5,
        [6] = &&_6,
        [7] = &&_7,
        [8] = &&_8,
        [9] = &&_9,
        [10] = &&_10,
        [11] = &&_11,
        [12] = &&_12,
        [13] = &&_13,
        [14] = &&_14, // inc/dec a8
        [15] = &&_15,
        [16] = &&_16,
        [17] = &&_17,
        [18] = &&_18,
        [19] = &&_19,
        [20] = &&_20,
        [21] = &&_21,
        [22] = &&_22,
        [23] = &&_23,
        [24] = &&_24,
        [25] = &&_25,
        [26] = &&_26,
        [27] = &&_27,
        [28] = &&_28,
        [29] = &&_29,
        [30] = &&_30,
        [31] = &&_31,
        [32] = &&_32,
        [33] = &&_33,
        [34] = &&_34,
        [35] = &&_35,
        [36] = &&_36,
        [37] = &&_37,
        [38] = &&_38,
        [39] = &&_39,
        [40] = &&_40,
        [41] = &&_41,
        [42] = &&_42,
        [43] = &&_43,
        [44] = &&_44,
        [45 ... 255] = &&_255
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

    DISPATCH();
    _0: // hlt
    {
        printf("\n Stopped..");
        return;
    }
    //-/ Перессылка данных //
    _1: // mov a8, i8 ; переместить из памяти i8 в регистр a8
    {
        PRINT_OPCODE();
        a8 = _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _2: // mov b8, i8 ; переместить из памяти i8 в регистр b8
    {
        PRINT_OPCODE();
        b8 = _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _3: // mov a8, b8 ; переместить из регистра b8 в регистр a8
    {
        PRINT_OPCODE();
        a8 = b8;
        ++_ip;
        DISPATCH();
    }
    _4: // mov b8, a8 ; переместить из регистра a8 в регистр b8
    {
        PRINT_OPCODE();
        b8 = a8;
        ++_ip;
        DISPATCH();
    }
    // Перессылка данных /-//
    //-/ Арифметико-логические операции //
    _5: // add a8, i8
    {
        PRINT_OPCODE();
        a8 += _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _6: // sub a8, i8
    {
        PRINT_OPCODE();
        a8 -= _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _7: // mul a8, i8
    {
        PRINT_OPCODE();
        a8 *= _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _8: // div a8, i8
    {
        PRINT_OPCODE();
        a8 /= _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _9: // rsub a8, i8
    {
        PRINT_OPCODE();
        a8 = _ip[1] - a8;
        _ip += 2;
        DISPATCH();
    }
    _10: // rdiv a8, i8
    {
        PRINT_OPCODE();
        a8 = _ip[1] / a8;
        _ip += 2;
        DISPATCH();
    }
    // Арифметико-логические операции /-//
    _11: // neg a8
    {
        PRINT_OPCODE();
        a8 = -a8;
        ++_ip;
        DISPATCH();
    }
    _12: // jmp i8
    {
        PRINT_OPCODE();
        _ip += (( (char) _ip[1]) + 2);
        DISPATCH();
    }
    _13: // cmp a8, i8
    {
        PRINT_OPCODE();
        zf = (a8 == (signed char) _ip[1]);
        sf = (a8 <  (signed char) _ip[1]);
        _ip += 2;
        DISPATCH();
    }
    _14: // je i8
    {
        PRINT_OPCODE();
        if (zf == 1) _ip += (( (char) _ip[1]) + 2); // Прыгаем вперед или назад по знаковому смещению
        else _ip += 2;
        DISPATCH();
    }
    _15: // jne i8
    {
        PRINT_OPCODE();
        if (zf == 0) _ip += (( (char) _ip[1]) + 2); // Прыгаем вперед или назад по знаковому смещению.
        else _ip += 2;
        DISPATCH();
    }
    _16: // inc a8 ; Инкремент регистра a8.
    {
        PRINT_OPCODE();
        ++a8;
        ++_ip;
        DISPATCH();
    }
    _17: // dec a8 ; Декремент регистра a8.
    {
        PRINT_OPCODE();
        --a8;
        ++_ip;
        DISPATCH();
    }
    _18: // push a8 ; Положить в стек значение взятое из регистра a8.
    {
        PRINT_OPCODE();
        *(--_sp) = a8;
        ++_ip;
        DISPATCH();
    }
    _19: // pop a8 ; Снять со стека значение и поместить в регистр a8.
    {
        PRINT_OPCODE();
        a8 = *(_sp++);
        ++_ip;
        DISPATCH();
    }
    _20: // call i8 ; Вызов процедуры.
    {
        PRINT_OPCODE();
        // 1. Толкаем адрес возврата в стек.
        *(--_sp) = (_ip+2) - memory_tape; // Следующий байт — аргумент. Инструкция возврата находится ровно через 2 байта от текущего положения.
        // 2. Осуществляем переход.
        _ip = memory_tape + _ip[1]; // Читаем адрес назначения из следующего байта (_ip[1]) и прибавляем к базе памяти.
        DISPATCH();
    }
    _21: // ret ; Возврат из процедуры.
    {
        PRINT_OPCODE();
        _ip = memory_tape + *(_sp++);
        DISPATCH();
    }
    _22: // mov si, i8
    {
        PRINT_OPCODE();
        _si = memory_tape + _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _23: // mov di, i8
    {
        PRINT_OPCODE();
        _di = memory_tape + _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _24: // ld a8, [si] ; Косвенное чтение (a8 = *_si).
    {
        PRINT_OPCODE();
        a8 = *_si; // Мгновенное разыменование без сложения баз памяти!
        ++_ip;
        DISPATCH();
    }
    _25: // st [di], a8 ; Косвенная запись (*_di = a8).
    {
        PRINT_OPCODE();
        *_di = a8; // Мгновенная запись
        ++_ip;
        DISPATCH();
    }
    _26: // inc si ; Продвижение источника.
    {
        PRINT_OPCODE();
        ++_si;
        ++_ip;
        DISPATCH();
    }
    _27: // inc di ; Продвижение приемника.
    {
        PRINT_OPCODE();
        ++_di;
        ++_ip;
        DISPATCH();
    }
    _28: // jl i8 ; Меньше (sf == 1)
    {
        PRINT_OPCODE();
        if (sf == 1) _ip += (( (char) _ip[1]) + 2);
        else _ip += 2;
        DISPATCH();
    }
    _29: // jle i8 ; Меньше или равно (sf == 1 или zf == 1)
    {
        PRINT_OPCODE();
        if (sf == 1 || zf == 1) _ip += (( (char) _ip[1]) + 2);
        else _ip += 2;
        DISPATCH();
    }
    _30: // jg i8 ; Больше (sf == 0 и zf == 0)
    {
        PRINT_OPCODE();
        if (sf == 0 && zf == 0) _ip += (( (char) _ip[1]) + 2);
        else _ip += 2;
        DISPATCH();
    }
    _31: // jge i8 ; Больше или равно (sf == 0)
    {
        PRINT_OPCODE();
        if (sf == 0) _ip += (( (char) _ip[1]) + 2);
        else _ip += 2;
        DISPATCH();
    }
    _32: // mov bp, sp ; Зафиксировать базу кадра функции
    {
        PRINT_OPCODE();
        _bp = _sp;
        ++_ip;
        DISPATCH();
    }
    _33: // mov sp, bp ; Освободить локальные переменные перед RET
    {
        PRINT_OPCODE();
        _sp = _bp;
        ++_ip;
        DISPATCH();
    }
    _34: // push bp ; Сохранить старый якорь на стеке
    {
        PRINT_OPCODE();
        *(--_sp) = _bp - memory_tape; // Стек хранит байты, поэтому переводим указатель хоста в индекс
        ++_ip;
        DISPATCH();
    }
    _35: // pop bp ; Восстановить старый якорь
    {
        PRINT_OPCODE();
        _bp = memory_tape + *(_sp++);
        ++_ip;
        DISPATCH();
    }
    //-/ Логические побитовые операции //
    _36: // and a8, i8 ; побитовое И
    {
        PRINT_OPCODE();
        a8 &= _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _37: // or a8, i8 ; побитовое ИЛИ
    {
        PRINT_OPCODE();
        a8 |= _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _38: // xor a8, i8 ; побитовое исключающее ИЛИ
    {
        PRINT_OPCODE();
        a8 ^= _ip[1];
        _ip += 2;
        DISPATCH();
    }
    _39: // not a8 ; побитовая инверсия (НЕ)
    {
        PRINT_OPCODE();
        a8 = ~a8;
        ++_ip;
        DISPATCH();
    }
    // Логические побитовые операции /-//
    //-/ Прямой доступ к памяти //
    _40: // ld a8, [i8] ; Загрузить в a8 значение из ячейки памяти с адресом i8
    {
        PRINT_OPCODE();
        a8 = memory_tape[_ip[1]]; // Читаем адрес из следующего байта кода (_ip[1]) и извлекаем данные из этой ячейки нашего массива памяти
        _ip += 2;
        DISPATCH();
    }
    _41: // st [i8], a8 ; Записать значение из a8 в ячейку памяти с адресом i8
    {
        PRINT_OPCODE();
        memory_tape[_ip[1]] = a8; // Читаем адрес назначения из _ip[1] и записываем туда текущий аккумулятор a8
        _ip += 2;
        DISPATCH();
    }
    // Прямой доступ к памяти /-//
    _42: // OUT string
    {
        PRINT_OPCODE();
        printf("%s", _rcv8);
        ++_ip;
        DISPATCH();
    }
    _43: // OUT number
    {
        PRINT_OPCODE();
        printf("%d", a8);
        ++_ip;
        DISPATCH();
    }
    _44: // OUT char
    {
        PRINT_OPCODE();
        putchar(a8);
        ++_ip;
        DISPATCH();
    }
    _255:
    {
        printf("\n Unknown opcode 0x%02X, stopped..", *_ip);
        return;
    }
    #undef DISPATCH
    #undef PRINT_OPCODE
}