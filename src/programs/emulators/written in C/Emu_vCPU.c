// <!-- Encoding: Windows-1251 -->
#include <stdint.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

char ProcAsciiChr(unsigned char chr)
{
    switch (chr) {
    case '\0'/*000*/: return '·';
    case 0x07/*007*/: return '·';
    case 0x08/*008*/: return '·';
    case 0x09/*009*/: return '·';
    case '\n'/*010*/: return '·';
    case '\r'/*013*/: return '·';
    case 0x1b/*027*/: return '·';
    case 0x95/*149*/: return '·';
    // 30-39 или 048-057: 0-9
    // 41-5A или 065-090: A-Z
    // 61-7A или 097-122: a-z
    //    A8 или     168: Ё
    //    B8 или     184: ё
    // C0-DF или 192-223: А-Я
    // E0-FF или 224-255: а-я
    default: return chr;
    }
}

uint8_t vIP;
uint8_t vSP;
uint8_t vDI;
uint8_t vSI;

uint8_t vMEMORY[0xFF];
uint8_t vSTACK[0xFF];


void ShowDashboard()
{
    printf("    HEX   DEC    ASCII\n");
    //for (int i = 0; i < 256; i++)
    printf("\nIP  [%02X]  [%03d]  [%c]\n", vIP, vIP, ProcAsciiChr(vIP));
    printf("SP  [%02X]  [%03d]  [%c]\n", vSP, vSP, ProcAsciiChr(vSP));
    //printf("DI  [%02X]  [%03d]  [%c]\n", vDI, vDI, ProcAsciiChr(vDI));
    //printf("SI  [%02X]  [%03d]  [%c]\n", vSI, vSI, ProcAsciiChr(vSI));
    char op = 0;
    switch (op){
    case 0:
    {
        printf("\nMEMORY (HEX | DEC | ASCII)\n    ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%02X]", i);
        printf("        ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%03d]", i);
        //printf("        ");
        //for (int i = 0; i < 0x0F + 1; i++) printf("[%c]", i);
        for (int j = 0; j < 0x0F + 1; j++)
        {
            printf("\n[%02X]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F + 1; i++) printf(" %02X ", vMEMORY[i]);
            printf("   ");
            printf("[%03d]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F + 1; i++) printf(" %03d ", vMEMORY[i]);
            printf("   ");
            for (int i = 0; i < 0x0F + 1; i++) printf("%c", ProcAsciiChr(vMEMORY[i]));
        }
    }
    break;
    case 10:
    {
        printf("MEMORY (DEC)\n     ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%03d]", i);
        for (int j = 0; j < 0x0F + 1; j++)
        {
            printf("\n[%03d]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F + 1; i++)
            {
                printf(" %03d ", vMEMORY[i]);
            }
        }
    }
    break;
    case 16:
    {
        printf("MEMORY (HEX)\n    ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%02X]", i);
        for (int j = 0; j < 0x0F + 1; j++)
        {
            printf("\n[%02X]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F + 1; i++)
            {
                printf(" %02X ", vMEMORY[i]);
            }
        }
    }
    break;
    }
}

// Шаг вперёд
void StepForward(){}
// Шаг назад
//void StepBack(){}

enum {
    PUSH = 1,
    INT,
    MOV
};
enum IVT {
    PUTCHAR = 1,
    PRINTF
};

int main()
{
    setlocale(0, "");

    char cmd[128+1];
    while (false)
    {
        printf("CMD> ");

        fgets(cmd, sizeof (cmd), stdin); // fgets считывает строку включая пробелы и '\n'
        cmd[strcspn(cmd, "\n")] = '\0';  // Удаляем символ переноса строки '\n', если он есть
    }

    ShowDashboard();
    vIP = 0x00;
    vSP = 0xFE;
    //vDI = 0x00;
    //vSI = 0x00;
    ShowDashboard();

    char opcode[] = {
        PUSH, 'Г',
        INT, 0x01
    };
    char * ptr_op_code = opcode;
    while (true)
    {
        switch (opcode[vIP]){
        case PUSH:
        {
            ShowDashboard();
            vSTACK[vSP--] = opcode[++vIP];
            ShowDashboard();
        }
        return 0;
        break;
        case INT:
        {
            vIP++; // сдвигаем указатель на след. инструкцию
            switch (*++ptr_op_code){
            case PUTCHAR:
            {
                vIP++; // сдвигаем указатель на след. инструкцию
                putchar(opcode[vIP]);
                break;
            }
            case PRINTF:
            {
                //printf("", );
                break;
            }}
        }
        break;
        }
    }

    return 0;
}
/// Текущая ячейка / Произвольная ячейка
/// -> Выборка -> Декодирование -> Исполнение -> Смещение IP на след. инструкцию ->