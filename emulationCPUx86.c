#include <stdio.h>
#include <locale.h>
//#include <stdbool.h>
//#define bool _Bool

void ShowPanelFor16bitMode()
{
    printf("\
    CS [%02X%02X]\n\
    DS [%02X%02X]\n\
    SS [%02X%02X]\n\
    ES [%02X%02X]\n\
\n\
    BP [%02X%02X]\n\
    SP [%02X%02X]\n\
    IP [%02X%02X]\n\
\n\
    SI [%02X%02X]\n\
    DI [%02X%02X]\n\
\n\
    AX [%02X][%02X]\n\
    BX [%02X][%02X]\n\
    CX [%02X][%02X]\n\
    DX [%02X][%02X]\
\n\n\
    FLAGS: C  Z  S  O  P  A  I  D\n\
          [%d][%d][%d][%d][%d][%d][%d][%d]\
    ",
    5, 2,
    15, 5,
    7, 28,
    3, 1,

    0, 0,
    0, 0,
    0, 0,

    6, 0,
    0, 4,

    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
    );
}

void StartingProcessorIn16bitMode()
{
    short cs; // code segment
    short ds; // data segment
    short ss; // stack segment
    short es; // extended segment

    short bp; // base pointer
    short sp; // stack pointer
    short ip; // instruction pointer

    short si; // source index
    short di; // destination index

    short ax; // accumulator
    short bx; // base
    short cx; // counter
    short dx; // data

    // FLAGS
    _Bool c; // carry
    _Bool z; // zero
    _Bool s; // sign
    _Bool o; // overflow
    _Bool p; // parity
    _Bool a; // auxiliary
    _Bool i; // interupt
    _Bool d; // direction
}

int main()
{
    setlocale(0, "");

    StartingProcessorIn16bitMode();
    ShowPanelFor16bitMode();
    return 0;
}