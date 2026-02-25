#include <stdio.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>

enum Mode { MODE_16, MODE_32, MODE_64 } current_mode = MODE_16;
enum { BIOS, DOS, UEFI, WINDOWS };

// REGISTERS //
// code segment, data segment, stack segment, extended segment
uint16_t CS = 0, DS = 0, SS = 0, ES = 0; // сегментные регистры (всегда 16 бит)
// accumulator, base, counter, data
uint64_t A = 0, B = 0, C = 0, D = 0;     // регистры общего назначения
// base pointer, stack pointer, instruction pointer
uint64_t BP = 0, SP = 0, IP = 0;         // регистры указатели
// source index, destination index
uint64_t SI = 0, DI = 0;                 // индексные регистры

// Для удобства можно определить макросы или inline-функции:
#define  AL (A & 0xFF)
#define  AH ((A >> 8) & 0xFF)
#define  AX (A & 0xFFFF)
#define EAX (A & 0xFFFFFFFF)
#define RAX (A & 0xFFFFFFFFFFFFFFFF)

#define  BL (B & 0xFF)
#define  BH ((B >> 8) & 0xFF)
#define  BX (B & 0xFFFF)
#define EBX (B & 0xFFFFFFFF)
#define RBX (B & 0xFFFFFFFFFFFFFFFF)

#define  CL (C & 0xFF)
#define  CH ((C >> 8) & 0xFF)
#define  CX (C & 0xFFFF)
#define ECX (C & 0xFFFFFFFF)
#define RCX (C & 0xFFFFFFFFFFFFFFFF)

#define  DL (D & 0xFF)
#define  DH ((D >> 8) & 0xFF)
#define  DX (D & 0xFFFF)
#define EDX (D & 0xFFFFFFFF)
#define RDX (D & 0xFFFFFFFFFFFFFFFF)
// аналогично для других регистров

// FLAGS //
// carry, zero, sign, overflow, parity, auxiliary, interupt, direction
bool CF = 0, ZF = 0, SF = 0, OF = 0, PF = 0, AF = 0, IF = 0, DF = 0;

void ShowPanelFor16BitMode()
{
    printf("\n16-bit's mode\n\n");

    printf("CS [%02X%02X]\tDS [%02X%02X]\tSS [%02X%02X]\tES [%02X%02X]\n\n", CS >> 8, CS & 0xFF, DS >> 8, DS & 0xFF, SS >> 8, SS & 0xFF, ES >> 8, ES & 0xFF); // Используются

    printf("BP [%02X%02X]  \n", BP >> 8, BP & 0xFF);
    printf("SP [%02X%02X]  \n", SP >> 8, SP & 0xFF);
    printf("IP [%02X%02X]\n\n", IP >> 8, IP & 0xFF);

    printf("SI [%02X%02X]\tDI [%02X%02X]\n\n", SI >> 8, SI & 0xFF, DI >> 8, DI & 0xFF);

    printf(" R^X-----------E^X----^X\n");
    printf("  A [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n", A>>56, (A>>48) & 0xFF, (A>>40) & 0xFF, (A>>32) & 0xFF, (A>>24) & 0xFF, (A>>16) & 0xFF, (A>>8) & 0xFF, A & 0xFF);
    printf("  B [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n", B>>56, (B>>48) & 0xFF, (B>>40) & 0xFF, (B>>32) & 0xFF, (B>>24) & 0xFF, (B>>16) & 0xFF, (B>>8) & 0xFF, B & 0xFF);
    printf("  C [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n", C>>56, (C>>48) & 0xFF, (C>>40) & 0xFF, (C>>32) & 0xFF, (C>>24) & 0xFF, (C>>16) & 0xFF, (C>>8) & 0xFF, C & 0xFF);
    printf("  D [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n", D>>56, (D>>48) & 0xFF, (D>>40) & 0xFF, (D>>32) & 0xFF, (D>>24) & 0xFF, (D>>16) & 0xFF, (D>>8) & 0xFF, D & 0xFF);
    printf("                       ^H-^L\n");

    printf("FLAGS: C Z S O P A I D\n");
    printf("       %d %d %d %d %d %d %d %d", CF, ZF, SF, OF, PF, AF, IF, DF);
}

void ShowPanelFor32BitMode()
{
    printf("\n32-bit's mode\n\n");

    printf("CS [%02X|%02X]  \n", CS >> 8, CS & 0xFF); // ?
    printf("DS [%02X|%02X]  \n", DS >> 8, DS & 0xFF); // ?
    printf("SS [%02X|%02X]  \n", SS >> 8, SS & 0xFF); // ?
    printf("ES [%02X|%02X]\n\n", ES >> 8, ES & 0xFF); // ?

    printf("EBP [%02X.%02X.%02X.%02X]  \n", BP >> 24, (BP >> 16) & 0xFF, (BP >> 8) & 0xFF, BP & 0xFF);
    printf("ESP [%02X.%02X.%02X.%02X]  \n", SP >> 24, (SP >> 16) & 0xFF, (SP >> 8) & 0xFF, SP & 0xFF);
    printf("EIP [%02X.%02X.%02X.%02X]\n\n", IP >> 24, (IP >> 16) & 0xFF, (IP >> 8) & 0xFF, IP & 0xFF);

    printf("ESI [%02X.%02X.%02X.%02X]\tEDI [%02X.%02X.%02X.%02X]\n", SI >> 24, (SI >> 16) & 0xFF, (SI >> 8) & 0xFF, SI & 0xFF, DI >> 24, (DI >> 16) & 0xFF, (DI >> 8) & 0xFF, DI & 0xFF);

    printf(" R^X-----------E^X----^X\n");
    printf("  A [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n"  , A>>56, (A>>48) & 0xFF, (A>>40) & 0xFF, (A>>32) & 0xFF, (A>>24) & 0xFF, (A>>16) & 0xFF, (A>>8) & 0xFF, A & 0xFF);
    printf("  B [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n"  , B>>56, (B>>48) & 0xFF, (B>>40) & 0xFF, (B>>32) & 0xFF, (B>>24) & 0xFF, (B>>16) & 0xFF, (B>>8) & 0xFF, B & 0xFF);
    printf("  C [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n"  , C>>56, (C>>48) & 0xFF, (C>>40) & 0xFF, (C>>32) & 0xFF, (C>>24) & 0xFF, (C>>16) & 0xFF, (C>>8) & 0xFF, C & 0xFF);
    printf("  D [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n\n", D>>56, (D>>48) & 0xFF, (D>>40) & 0xFF, (D>>32) & 0xFF, (D>>24) & 0xFF, (D>>16) & 0xFF, (D>>8) & 0xFF, D & 0xFF);
    printf("                       ^H-^L\n");

    printf("FLAGS: C Z S O P A I D\n");
    printf("       %d %d %d %d %d %d %d %d", CF, ZF, SF, OF, PF, AF, IF, DF);
}

void ShowPanelFor64BitMode()
{
    printf("\n64-bit's mode\n\n");

    printf("CS [%02X|%02X]\n", CS >> 8, CS & 0xFF); // Используется
    printf("DS [%02X|%02X]\tSS [%02X|%02X]\tES [%02X|%02X]\n", DS >> 8, DS & 0xFF, SS >> 8, SS & 0xFF, ES >> 8, ES & 0xFF); // Игнорируется (можно не эмулировать)

    printf("RBP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]  \n", BP>>56, (BP>>48) & 0xFF, (BP>>40) & 0xFF, (BP>>32) & 0xFF, (BP>>24) & 0xFF, (BP>>16) & 0xFF, (BP>>8) & 0xFF, BP & 0xFF);
    printf("RSP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]  \n", SP>>56, (SP>>48) & 0xFF, (SP>>40) & 0xFF, (SP>>32) & 0xFF, (SP>>24) & 0xFF, (SP>>16) & 0xFF, (SP>>8) & 0xFF, SP & 0xFF);
    printf("RIP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]\n\n", IP>>56, (IP>>48) & 0xFF, (IP>>40) & 0xFF, (IP>>32) & 0xFF, (IP>>24) & 0xFF, (IP>>16) & 0xFF, (IP>>8) & 0xFF, IP & 0xFF);

    printf("RSI [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]\tRDI [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]\n",
        SI >> 56, (SI >> 48) & 0xFF, (SI >> 40) & 0xFF, (SI >> 32) & 0xFF, (SI >> 24) & 0xFF, (SI >> 16) & 0xFF, (SI >> 8) & 0xFF, SI & 0xFF,
        DI >> 56, (DI >> 48) & 0xFF, (DI >> 40) & 0xFF, (DI >> 32) & 0xFF, (DI >> 24) & 0xFF, (DI >> 16) & 0xFF, (DI >> 8) & 0xFF, DI & 0xFF
    );

    printf(" R^X-----------E^X----^X\n");
    printf("  A [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n"  , A>>56, (A>>48) & 0xFF, (A>>40) & 0xFF, (A>>32) & 0xFF, (A>>24) & 0xFF, (A>>16) & 0xFF, (A>>8) & 0xFF, A & 0xFF);
    printf("  B [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n"  , B>>56, (B>>48) & 0xFF, (B>>40) & 0xFF, (B>>32) & 0xFF, (B>>24) & 0xFF, (B>>16) & 0xFF, (B>>8) & 0xFF, B & 0xFF);
    printf("  C [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n"  , C>>56, (C>>48) & 0xFF, (C>>40) & 0xFF, (C>>32) & 0xFF, (C>>24) & 0xFF, (C>>16) & 0xFF, (C>>8) & 0xFF, C & 0xFF);
    printf("  D [%02X.%02X.%02X.%02X|%02X.%02X|%02X|%02X]\n\n", D>>56, (D>>48) & 0xFF, (D>>40) & 0xFF, (D>>32) & 0xFF, (D>>24) & 0xFF, (D>>16) & 0xFF, (D>>8) & 0xFF, D & 0xFF);
    printf("                       ^H-^L\n");

    printf("FLAGS: C Z S O P A I D\n");
    printf("       %d %d %d %d %d %d %d %d", CF, ZF, SF, OF, PF, AF, IF, DF);
}

void SwitchingTo16BitMode() { current_mode = MODE_16; }
void SwitchingTo32BitMode() { current_mode = MODE_32; }
void SwitchingTo64BitMode() { current_mode = MODE_64; }

void Emulate_BIOS()
{
    SwitchingTo16BitMode();

    // Инициализируем
    /// ...
}
void Emulate_MS_DOS()
{
    SwitchingTo16BitMode();
    CS = 0x0100, DS = 0x0100, SS = 0x0100, ES = 0x0100;
    SP = 0xFFFE;
}
void Emulate_UEFI()
{
    SwitchingTo64BitMode();

    // Инициализируем
    /// ...
}

void Emulate_OS_Windows()
{
    SwitchingTo64BitMode();

    // Инициализируем
    /// ...
}

int main()
{
    setlocale(0, "");

    // Задайте режим какой интерфейс эмулировать
    unsigned char emulate = DOS; // 1 - BIOS, 2 - DOS, 3 - UEFI, 4 - Windows

    if (emulate == BIOS)
    {
        Emulate_BIOS();
        ShowPanelFor16BitMode();
    }
    else if (emulate == DOS)
    {
        Emulate_MS_DOS();
        ShowPanelFor16BitMode(); // MS-DOS тоже 16-битный
    }
    else if (emulate == UEFI)
    {
        Emulate_UEFI();
        ShowPanelFor64BitMode();
    }
    else if (emulate == WINDOWS)
    {
        Emulate_OS_Windows();
        ShowPanelFor64BitMode();
    }
    return 0;
}