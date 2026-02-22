#include <stdio.h>
#include <locale.h>
//#include <stdbool.h>
//#define bool _Bool

short cs = 0x0000; // code segment
short ds = 0x0000; // data segment
short ss = 0x0000; // stack segment
short es = 0x0000; // extended segment

short bp = 0x0000; // base pointer
short sp = 0x0000; // stack pointer
short ip = 0x0000; // instruction pointer

short si = 0x0000; // source index
short di = 0x0000; // destination index

short ax = 0x0000; // accumulator
short bx = 0x0000; // base
short cx = 0x0000; // counter
short dx = 0x0000; // data

// FLAGS
_Bool c, z, s, o, p, a, i, d = 0; // carry, zero, sign, overflow, parity, auxiliary, interupt, direction

void ShowPanelFor16bitMode()
{
    printf("CS [%02X.%02X]\n", cs >> 8, cs & 0xFF);
    printf("DS [%02X.%02X]\n", ds >> 8, ds & 0xFF);
    printf("SS [%02X.%02X]\n", ss >> 8, ss & 0xFF);
    printf("ES [%02X.%02X]\n", es >> 8, es & 0xFF);
    putchar('\n');
    printf("BP [%02X.%02X]\n", bp >> 8, bp & 0xFF);
    printf("SP [%02X.%02X]\n", sp >> 8, sp & 0xFF);
    printf("IP [%02X.%02X]\n", ip >> 8, ip & 0xFF);
    putchar('\n');
    printf("SI [%02X.%02X]\n", si >> 8, si & 0xFF);
    printf("DI [%02X.%02X]\n", di >> 8, di & 0xFF);
    putchar('\n');
    printf("AX [%02X][%02X]\n", ax >> 8, ax & 0xFF);
    printf("BX [%02X][%02X]\n", bx >> 8, bx & 0xFF);
    printf("CX [%02X][%02X]\n", cx >> 8, cx & 0xFF);
    printf("DX [%02X][%02X]\n", dx >> 8, dx & 0xFF);
    
    printf("FLAGS: C  Z  S  O  P  A  I  D\n");
    printf("      [%d][%d][%d][%d][%d][%d][%d][%d]", c, z, s, o, p, a, i, d);
}

void StartingProcessorIn16bitMode()
{
    
}

int main()
{
    setlocale(0, "");
    putchar('\n');
    StartingProcessorIn16bitMode();
    ShowPanelFor16bitMode();
    putchar('\n');
    return 0;
}