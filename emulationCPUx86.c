#include <stdio.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>

// Для удобства можно определить макросы или inline-функции:
//#define AX (RAX & 0xFFFF)
//#define AH ((RAX >> 8) & 0xFF)
//#define AL (RAX & 0xFF)
//#define EAX (RAX & 0xFFFFFFFF)
// аналогично для других регистров

// REGISTERS //

// accumulator, base, counter, data
uint16_t  AX = 0,  BX = 0,  CX = 0,  DX = 0;
uint32_t EAX = 0, EBX = 0, ECX = 0, EDX = 0;
uint64_t RAX = 0, RBX = 0, RCX = 0, RDX = 0;

// code segment, data segment, stack segment, extended segment
uint16_t CS = 0, DS = 0, SS = 0, ES = 0;

// base pointer, stack pointer, instruction pointer
uint16_t  BP = 0,  SP = 0,  IP = 0;
uint32_t EBP = 0, ESP = 0, EIP = 0;
uint64_t RBP = 0, RSP = 0, RIP = 0;

// source index, destination index
uint64_t SI = 0, DI = 0;

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

    printf("AX [%02X][%02X]  \n",AX>>8,AX&0xFF);
    printf("BX [%02X][%02X]  \n",BX>>8,BX&0xFF);
    printf("CX [%02X][%02X]  \n",CX>>8,CX&0xFF);
    printf("DX [%02X][%02X]\n\n",DX>>8,DX&0xFF);

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

    printf("EBP [%02X.%02X.%02X.%02X]  \n",EBP>>24,(EBP>>16)&0xFF,(EBP>>8)&0xFF,EBP&0xFF);
    printf("ESP [%02X.%02X.%02X.%02X]  \n",ESP>>24,(ESP>>16)&0xFF,(ESP>>8)&0xFF,ESP&0xFF);
    printf("EIP [%02X.%02X.%02X.%02X]\n\n",EIP>>24,(EIP>>16)&0xFF,(EIP>>8)&0xFF,EIP&0xFF);

    printf("ESI [%02X.%02X.%02X.%02X]  \n",SI>>24,(SI>>16)&0xFF,(SI>>8)&0xFF,SI&0xFF);
    printf("EDI [%02X.%02X.%02X.%02X]\n\n",DI>>24,(DI>>16)&0xFF,(DI>>8)&0xFF,DI&0xFF);

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
    printf("RBP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]  \n",RBP>>56,(RBP>>48)&0xFF,(RBP>>40)&0xFF,(RBP>>32)&0xFF,(RBP>>24)&0xFF,(RBP>>16)&0xFF,(RBP>>8)&0xFF,RBP&0xFF);
    printf("RSP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]  \n",RSP>>56,(RSP>>48)&0xFF,(RSP>>40)&0xFF,(RSP>>32)&0xFF,(RSP>>24)&0xFF,(RSP>>16)&0xFF,(RSP>>8)&0xFF,RSP&0xFF);
    printf("RIP [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]\n\n",RIP>>56,(RIP>>48)&0xFF,(RIP>>40)&0xFF,(RIP>>32)&0xFF,(RIP>>24)&0xFF,(RIP>>16)&0xFF,(RIP>>8)&0xFF,RIP&0xFF);

    printf("RSI [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]  \n",SI>>56,(SI>>48)&0xFF,(SI>>40)&0xFF,(SI>>32)&0xFF,(SI>>24)&0xFF,(SI>>16)&0xFF,(SI>>8)&0xFF,SI&0xFF);
    printf("RDI [%02X.%02X.%02X.%02X.%02X.%02X.%02X.%02X]\n\n",DI>>56,(DI>>48)&0xFF,(DI>>40)&0xFF,(DI>>32)&0xFF,(DI>>24)&0xFF,(DI>>16)&0xFF,(DI>>8)&0xFF,DI&0xFF);

    printf("RAX [%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]  \n",RAX>>56,(RAX>>48)&0xFF,(RAX>>40)&0xFF,(RAX>>32)&0xFF,(RAX>>24)&0xFF,(RAX>>16)&0xFF,(RAX>>8)&0xFF,RAX&0xFF);
    printf("RBX [%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]  \n",RBX>>56,(RBX>>48)&0xFF,(RBX>>40)&0xFF,(RBX>>32)&0xFF,(RBX>>24)&0xFF,(RBX>>16)&0xFF,(RBX>>8)&0xFF,RBX&0xFF);
    printf("RCX [%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]  \n",RCX>>56,(RCX>>48)&0xFF,(RCX>>40)&0xFF,(RCX>>32)&0xFF,(RCX>>24)&0xFF,(RCX>>16)&0xFF,(RCX>>8)&0xFF,RCX&0xFF);
    printf("RDX [%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]\n\n",RDX>>56,(RDX>>48)&0xFF,(RDX>>40)&0xFF,(RDX>>32)&0xFF,(RDX>>24)&0xFF,(RDX>>16)&0xFF,(RDX>>8)&0xFF,RDX&0xFF);

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