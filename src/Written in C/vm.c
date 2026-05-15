//#define MACRO__VIRTUAL_ADDRESS (cs8 << 8) + ip8 // максимально допустимая при двух 8-ми битных регистрах
//#define MACRO__VIRTUAL_ADDRESS (cs16 << 8) + ip16 // максимально допустимая при двух 16-ти битных регистрах

unsigned char gl__memory_tape[MACRO__MAXIMUM_CODE_LIMIT]; // плоская модель памяти, стек находится здесь же (заполнение с конца)
unsigned char * gl__ptr__memory_tape = gl__memory_tape;

uint8_t cs8 = 0; // (unsigned char) 8-bit's сегментный-регистр
uint8_t ip8 = 0; // (unsigned char) 8-bit's регистр-указатель на инструкцию
uint8_t sp8 = 0; // (unsigned char) 8-bit's регистр-указатель на стек
int8_t   a8 = 0; //          (char) 8-bit's регистр общего назначения
int8_t   b8 = 0; //          (char) 8-bit's регистр общего назначения

uint16_t cs16 = 0; // (unsigned short) 16-bit's сегментный-регистр
uint16_t ip16 = 0; // (unsigned short) 16-bit's регистр-указатель на инструкцию
uint16_t sp16 = 0; // (unsigned short) 16-bit's регистр-указатель на стек
int16_t   a16 = 0; //          (short) 16-bit's регистр общего назначения
int16_t   b16 = 0; //          (short) 16-bit's регистр общего назначения

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
    int len = strlen(gl__opcodes);//+1
    for (int i = 0; i < len; i++) gl__memory_tape[i] = gl__opcodes[i];
    //printf("\n После | %03d %03d %03d = %02X %02X %02X", memory_tape[0], memory_tape[1], memory_tape[2], memory_tape[0], memory_tape[1], memory_tape[2]);
}
//
void Debug_Loader_VM()
{
    //putchar('\n');
    int len = strlen(gl__memory_tape);//+1
    printf("\n Оп-код (dec):");
    for (int i = 0; i < len; i++) printf(" %03d", gl__memory_tape[i]);
    printf("\n Оп-код (hex):");
    for (int i = 0; i < len; i++) printf("  %02X", gl__memory_tape[i]);
}
//
void Executor_VM() // Spin / Executor (исполнитель) / Evaluator (древоходец)
{
    switch_run:
    switch (*gl__ptr__memory_tape){
    //
    case 0x01: // 8-bit's addr-on | mov i8, a8 ; / AT&T-specification
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        a8 = *(++gl__ptr__memory_tape);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x02: // 8-bit's addr-on | mov i8, b8 ; / AT&T-specification
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        b8 = *(++gl__ptr__memory_tape);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x03: // 8-bit's addr-on | add i8 a8 ; сложение / AT&T-specification
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        a8 += *(++gl__ptr__memory_tape);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x04: // 8-bit's addr-on | sub i8 a8 ; вычитание / AT&T-specification
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        a8 -= *(++gl__ptr__memory_tape);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x05: // 8-bit's addr-on | mul i8 a8 ; умножение / AT&T-specification
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        a8 *= *(++gl__ptr__memory_tape);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x06: // 8-bit's addr-on | div i8 a8 ; деление / AT&T-specification
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        a8 /= *(++gl__ptr__memory_tape);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x76: // OUT (распечатать строку на консоль)
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        printf("%s", _rcv8);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x77: // OUT (распечатать число на консоль)
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        printf("%d", rcv16);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x78: // OUT (распечатать символ на консоль)
    {
        printf("\n \\d%03d = \\h%02X", *gl__ptr__memory_tape, *gl__ptr__memory_tape);
        putchar(rcv16);
        ++gl__ptr__memory_tape;
        goto switch_run;
    }
    case 0x79: printf("\n Stopped.."); return;
    default: printf("\n Unknown opcode, stopped.."); return;
    //
    }
}