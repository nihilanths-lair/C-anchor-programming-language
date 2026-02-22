#include <stdio.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>

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

#define  CL (B & 0xFF)
#define  CH ((B >> 8) & 0xFF)
#define  CX (B & 0xFFFF)
#define ECX (B & 0xFFFFFFFF)
#define RCX (B & 0xFFFFFFFFFFFFFFFF)

#define  DL (B & 0xFF)
#define  DH ((B >> 8) & 0xFF)
#define  DX (B & 0xFFFF)
#define EDX (B & 0xFFFFFFFF)
#define RDX (B & 0xFFFFFFFFFFFFFFFF)
// аналогично для других регистров

// FLAGS //
// carry, zero, sign, overflow, parity, auxiliary, interupt, direction
bool CF = 0, ZF = 0, SF = 0, OF = 0, PF = 0, AF = 0, IF = 0, DF = 0;

void ShowPanelFor16BitMode()
{
    printf("\n16-bit's mode\n\n");

    printf("CS [%02X.%02X]  \n",CS>>8,CS&0xFF);
    printf("DS [%02X.%02X]  \n",DS>>8,DS&0xFF);
    printf("SS [%02X.%02X]  \n",SS>>8,SS&0xFF);
    printf("ES [%02X.%02X]\n\n",ES>>8,ES&0xFF);

    printf("BP [%02X.%02X]  \n",BP>>8,BP&0xFF);
    printf("SP [%02X.%02X]  \n",SP>>8,SP&0xFF);
    printf("IP [%02X.%02X]\n\n",IP>>8,IP&0xFF);

    printf("SI [%02X.%02X]  \n",SI>>8,SI&0xFF);
    printf("DI [%02X.%02X]\n\n",DI>>8,DI&0xFF);
    printf("    AH  AL\n");
    printf("AX [%02X][%02X]  \n", AH, AL);
    printf("BX [%02X][%02X]  \n", BH, BL);
    printf("CX [%02X][%02X]  \n", CH, CL);
    printf("DX [%02X][%02X]\n\n", DH, DL);

    printf("FLAGS: C  Z  S  O  P  A  I  D\n");
    printf("      [%d][%d][%d][%d][%d][%d][%d][%d]", CF, ZF, SF, OF, PF, AF, IF, DF);
}

void ShowPanelFor32BitMode()
{
    printf("\n32-bit's mode\n\n");

    //printf("CS [%02X.%02X]\n\n", CS >> 8, CS & 0xFF); // 16 или 32 битный регистр?
    /*
    printf("DS [00.00.00.00]\n");
    printf("SS [00.00.00.00]\n");
    printf("ES [00.00.00.00]\n");
    */

    printf("EBP [%02X.%02X.%02X.%02X]  \n",BP>>24,(BP>>16)&0xFF,(BP>>8)&0xFF,BP&0xFF);
    printf("ESP [%02X.%02X.%02X.%02X]  \n",SP>>24,(SP>>16)&0xFF,(SP>>8)&0xFF,SP&0xFF);
    printf("EIP [%02X.%02X.%02X.%02X]\n\n",IP>>24,(IP>>16)&0xFF,(IP>>8)&0xFF,IP&0xFF);

    printf("ESI [%02X.%02X.%02X.%02X]  \n",SI>>24,(SI>>16)&0xFF,(SI>>8)&0xFF,SI&0xFF);
    printf("EDI [%02X.%02X.%02X.%02X]\n\n",DI>>24,(DI>>16)&0xFF,(DI>>8)&0xFF,DI&0xFF);
    printf("             AH  AL\n");
    printf("         AX [%02X][%02X]  \n",EAX>>24,(EAX>>16)&0xFF,(EAX>>8)&0xFF,EAX&0xFF);
    printf("EAX [%02X][%02X][%02X][%02X]  \n",EAX>>24,(EAX>>16)&0xFF,(EAX>>8)&0xFF,EAX&0xFF);
    printf("EBX [%02X][%02X][%02X][%02X]  \n",EBX>>24,(EBX>>16)&0xFF,(EBX>>8)&0xFF,EBX&0xFF);
    printf("ECX [%02X][%02X][%02X][%02X]  \n",ECX>>24,(ECX>>16)&0xFF,(ECX>>8)&0xFF,ECX&0xFF);
    printf("EDX [%02X][%02X][%02X][%02X]\n\n",EDX>>24,(EDX>>16)&0xFF,(EDX>>8)&0xFF,EDX&0xFF);

    printf("FLAGS: C  Z  S  O  P  A  I  D\n");
    printf("      [%d][%d][%d][%d][%d][%d][%d][%d]", CF, ZF, SF, OF, PF, AF, IF, DF);
}

void ShowPanelFor64BitMode()
{
    printf("\n64-bit's mode\n\n");

    printf(" CS [%02X.%02X]\n\n",CS>>8,CS&0xFF); // 16 или 64 битный регистр?
    /*
    printf("DS [00.00.00.00.00.00.00.00]\n"); // Игнорируется (можно не эмулировать)
    printf("SS [00.00.00.00.00.00.00.00]\n"); // Игнорируется (можно не эмулировать)
    printf("ES [00.00.00.00.00.00.00.00]\n"); // Игнорируется (можно не эмулировать)
    */
    printf("RBP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]  \n",BP>>56,(BP>>48)&0xFF,(BP>>40)&0xFF,(BP>>32)&0xFF,(BP>>24)&0xFF,(BP>>16)&0xFF,(BP>>8)&0xFF,BP&0xFF);
    printf("RSP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]  \n",SP>>56,(SP>>48)&0xFF,(SP>>40)&0xFF,(SP>>32)&0xFF,(SP>>24)&0xFF,(SP>>16)&0xFF,(SP>>8)&0xFF,SP&0xFF);
    printf("RIP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]\n\n",IP>>56,(IP>>48)&0xFF,(IP>>40)&0xFF,(IP>>32)&0xFF,(IP>>24)&0xFF,(IP>>16)&0xFF,(IP>>8)&0xFF,IP&0xFF);

    printf("RSI [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]  \n",SI>>56,(SI>>48)&0xFF,(SI>>40)&0xFF,(SI>>32)&0xFF,(SI>>24)&0xFF,(SI>>16)&0xFF,(SI>>8)&0xFF,SI&0xFF);
    printf("RDI [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]\n\n",DI>>56,(DI>>48)&0xFF,(DI>>40)&0xFF,(DI>>32)&0xFF,(DI>>24)&0xFF,(DI>>16)&0xFF,(DI>>8)&0xFF,DI&0xFF);

    printf("RAX /--------------------------------\\\n");
    printf("    |           EAX /----------------|\n");
    printf("    |               |    AX /--------|\n");
    printf("    |               |       | AH  AL |\n");
    printf("    |[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]|\n",RAX>>56,(RAX>>48)&0xFF,(RAX>>40)&0xFF,(RAX>>32)&0xFF,(RAX>>24)&0xFF,(RAX>>16)&0xFF,(RAX>>8)&0xFF,RAX&0xFF);
    printf("    \\--------------------------------/\n");
    printf("RBX /--------------------------------\\\n");
    printf("    |           EBX /----------------|\n");
    printf("    |               |    BX /--------|\n");
    printf("    |               |       | BH  BL |\n");
    printf("    |[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]|\n",RBX>>56,(RBX>>48)&0xFF,(RBX>>40)&0xFF,(RBX>>32)&0xFF,(RBX>>24)&0xFF,(RBX>>16)&0xFF,(RBX>>8)&0xFF,RBX&0xFF);
    printf("    \\--------------------------------/\n");
    printf("RCX /--------------------------------\\\n");
    printf("    |           ECX /----------------|\n");
    printf("    |               |    CX /--------|\n");
    printf("    |               |       | CH  CL |\n");
    printf("    |[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]|\n",RCX>>56,(RCX>>48)&0xFF,(RCX>>40)&0xFF,(RCX>>32)&0xFF,(RCX>>24)&0xFF,(RCX>>16)&0xFF,(RCX>>8)&0xFF,RCX&0xFF);
    printf("    \\--------------------------------/\n");
    printf("RDX /--------------------------------\\\n");
    printf("    |           EDX /----------------|\n");
    printf("    |               |    DX /--------|\n");
    printf("    |               |       | DH  DL |\n");
    printf("    |[%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]|\n",RDX>>56,(RDX>>48)&0xFF,(RDX>>40)&0xFF,(RDX>>32)&0xFF,(RDX>>24)&0xFF,(RDX>>16)&0xFF,(RDX>>8)&0xFF,RDX&0xFF);
    printf("    \\--------------------------------/\n");
    printf("FLAGS: C  Z  S  O  P  A  I  D\n");
    printf("      [%d][%d][%d][%d][%d][%d][%d][%d]", CF, ZF, SF, OF, PF, AF, IF, DF);
}

void StartingIn16BitMode(){}
void SwitchingTo32BitMode(){}
void SwitchingTo64BitMode(){}

int main()
{
    setlocale(0, "");

    ShowPanelFor16BitMode();
    ShowPanelFor32BitMode();
    ShowPanelFor64BitMode();
    return 0;
}