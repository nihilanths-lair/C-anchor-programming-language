#include <stdint.h>
#include <locale.h>
#include <stdio.h>

uint8_t vIP = 0;

char * ProcAsciiChr(char chr)
{
    switch (chr) {
    case '\0': return "\0";
    }
}

int main()
{
    setlocale(0, "");

    printf("    HEX   DEC   ASCII\n");
    for (int i = 0; i < 15; i++) printf("IP  [%02X]\t[%03d]\t[%c]\n", vIP, vIP, vIP), vIP++;

    return 0;
}