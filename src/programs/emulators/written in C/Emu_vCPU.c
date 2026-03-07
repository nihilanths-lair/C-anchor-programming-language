#include <stdint.h>
#include <locale.h>
#include <stdio.h>

uint8_t vIP = 0;

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

int main()
{
    setlocale(0, "");

    printf("    HEX   DEC    ASCII\n");
    for (int i = 0; i < 256; i++) printf("IP  [%02X]  [%03d]  [%c]\n", vIP, vIP, ProcAsciiChr(vIP)), vIP++;

    return 0;
}