//#define MACRO__VIRTUAL_ADDRESS (cs8 << 8) + ip8 // максимально допустимая при двух 8-ми битных регистрах
//#define MACRO__VIRTUAL_ADDRESS (cs16 << 8) + ip16 // максимально допустимая при двух 16-ти битных регистрах

unsigned char ga__memory_tape[MACRO__MAXIMUM_CODE_LIMIT]; // плоская модель памяти, стек находится здесь же (заполнение с конца)
unsigned char * gp__memory_tape = ga__memory_tape;

uint8_t cs8 = 0; // (unsigned char) 8-bit's сегментный-регистр
uint8_t ip8 = -1; // (unsigned char) 8-bit's регистр-указатель на инструкцию
uint8_t sp8 = -1; // (unsigned char) 8-bit's регистр-указатель на стек
uint8_t  a8 = -1; //          (char) 8-bit's регистр общего назначения
int8_t   b8 = 0; //          (char) 8-bit's регистр общего назначения

uint16_t cs16 = 0; // (unsigned short) 16-bit's сегментный-регистр
uint16_t ip16 = -1; // (unsigned short) 16-bit's регистр-указатель на инструкцию
uint16_t sp16 = -1; // (unsigned short) 16-bit's регистр-указатель на стек
uint16_t  a16 = -1; //          (short) 16-bit's регистр общего назначения
int16_t   b16 = 0; //          (short) 16-bit's регистр общего назначения

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
    for (int i = 0; i < len; i++) ga__memory_tape[i] = ga__opcodes[i];
    //printf("\n После | %03d %03d %03d = %02X %02X %02X", memory_tape[0], memory_tape[1], memory_tape[2], memory_tape[0], memory_tape[1], memory_tape[2]);
}
//
void Debug_Loader_VM()
{
    //putchar('\n');
    int len = strlen(gp__memory_tape);//+1
    printf("\n Оп-код (dec):");
    for (int i = 0; i < len; i++) printf(" %03d", ga__memory_tape[i]);
    printf("\n Оп-код (hex):");
    for (int i = 0; i < len; i++) printf("  %02X", ga__memory_tape[i]);
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
    static unsigned short step = -1;
    printf("\n -----------\n Шаг: %d", step++);
    switch (1){
    case 0:
    {
        printf("\n -----------------------------------");
        printf("\n  REGISTER |   DEC   |  HEX  | TEXT");
        printf("\n           |         |       |");
        printf("\n      ip8  |     %03d | %02X    | %d", ip8, ip8, ip8);
        printf("\n      sp8  |     %03d | %02X    | %d", sp8, sp8, sp8);
        printf("\n       a8  |     %03d | %02X    | %d",  a8,  a8,  a8);
        printf("\n           |         |       |");
        printf("\n      ip16 | %03d %03d | %02X %02X | %s", ip16>>8, ip16&0xFF, ip16>>8, ip16&0xFF, numf(ip16));
        printf("\n      sp16 | %03d %03d | %02X %02X | %s", sp16>>8, sp16&0xFF, sp16>>8, sp16&0xFF, numf(sp16));
        printf("\n       a16 | %03d %03d | %02X %02X | %s",  a16>>8,  a16&0xFF,  a16>>8,  a16&0xFF,  numf(a16));
        printf("\n -------------------------------------");
        break;
    }
    case 1:
    {
        printf("\n ---------------------------------------------------------");
        printf("\n  REGISTER |   DEC   |  HEX  |         BIN         | TEXT");
        printf("\n           |         |       |                     |");
        // Вывод 8-битных регистров
        printf("\n      ip8  |     %03d | %02X    | %s           | %d", ip8, ip8, bin8(ip8), ip8);
        printf("\n      sp8  |     %03d | %02X    | %s           | %d", sp8, sp8, bin8(sp8), sp8);
        printf("\n       a8  |     %03d | %02X    | %s           | %d",  a8,  a8, bin8( a8),  a8);
        printf("\n           |         |       |                     |");
        // Вывод 16-битных регистров
        // Разделяем hex на старший и младший байт через битовые сдвиги для формата %02X,%02X
        printf("\n      ip16 | %03d %03d | %02X %02X | %s | %s", ip16>>8, ip16&0xFF, ip16>>8, ip16&0xFF, bin16(ip16), numf(ip16));
        printf("\n      sp16 | %03d %03d | %02X %02X | %s | %s", sp16>>8, sp16&0xFF, sp16>>8, sp16&0xFF, bin16(sp16), numf(sp16));
        printf("\n       a16 | %03d %03d | %02X %02X | %s | %s",  a16>>8,  a16&0xFF,  a16>>8,  a16&0xFF, bin16( a16),  numf(a16));
        printf("\n -----------------------------------------------------------");
    }}
}
//
void dbg_MemoryState()
{

}
//
void Executor_VM() // Spin / Executor (исполнитель) / Evaluator (древоходец)
{
    //dbg_RegisterState();
    repeat:
    dbg_RegisterState();
    switch (*gp__memory_tape){
    //
    case 0x75: // 8-bit's addr-on | jmp i8
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        gp__memory_tape = gp__memory_tape + (*(++gp__memory_tape)); // jump offset
        goto repeat;
    }
    case 0x01: // 8-bit's addr-on | mov i8, a8
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        a8 = *(++gp__memory_tape);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x02: // 8-bit's addr-on | mov i8, b8
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        b8 = *(++gp__memory_tape);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x03: // 8-bit's addr-on | add i8 a8 ; сложение
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        a8 += *(++gp__memory_tape);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x04: // 8-bit's addr-on | sub i8 a8 ; вычитание
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        a8 -= *(++gp__memory_tape);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x05: // 8-bit's addr-on | mul i8 a8 ; умножение
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        a8 *= *(++gp__memory_tape);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x06: // 8-bit's addr-on | div i8 a8 ; деление
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        a8 /= *(++gp__memory_tape);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x76: // OUT (распечатать строку на консоль)
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        printf("%s", _rcv8);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x77: // OUT (распечатать число на консоль)
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        printf("%d", a8);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x78: // OUT (распечатать символ на консоль)
    {
        printf("\n \\d%03d = \\h%02X", *gp__memory_tape, *gp__memory_tape);
        putchar(a8);
        ++gp__memory_tape;
        goto repeat;
    }
    case 0x79: printf("\n Stopped.."); return;
    default: printf("\n Unknown opcode, stopped.."); return;
    //
    }
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