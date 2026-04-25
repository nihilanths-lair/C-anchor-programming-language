const char ProcAsciiChr(unsigned char chr)
{
    switch (chr){
    case '\0': return '·'; // ··0
    case 0x01: return '·'; // ··1
    case 0x02: return '·'; // ··2
    case 0x03: return '·'; // ··3
    case 0x04: return '·'; // ··4
    ///case 0x05: return '·'; // ··5
    case 0x06: return '·'; // ··6
    case 0x07: return '·'; // ··7
    case 0x08: return '·'; // ··8
    case 0x09: return '·'; // ··9
    case '\n': return '·'; // ·10
    case 0x0B: return '·'; // ·11
    case 0x0C: return '·'; // ·12
    case '\r': return '·'; // ·13
    //case 0x0E: return '·'; // ·14
    case 0x0F: return '·'; // ·15
    case 0x10: return '·'; // ·16
    case 0x11: return '·'; // ·17
    case 0x12: return '·'; // ·18
    case 0x13: return '·'; // ·19
    case 0x14: return '·'; // ·20
    case 0x1B: return '·'; // ·27
    // 30-39 или 048-057: 0-9
    // 41-5A или 065-090: A-Z
    // 61-7A или 097-122: a-z
    case 0x90: return '·'; // 144
    case 0x95: return '·'; // 149
    //    A8 или     168: Ё
    //    B8 или     184: ё
    // C0-DF или 192-223: А-Я
    // E0-FF или 224-255: а-я
    default: return chr;
    }
}
const char * ProcAsciiChrEx(const char chr)
{
    char * res = "····";
    switch (chr){
    case 0: res = "'\\0'"; break;
    default: res = "'$' ";
    }
    return res;
}
#include <stdarg.h>
const char * format(const char *str_fmt, ...)
{
    va_list args;
    va_start(args, str_fmt);
    while (*str_fmt != '\0')
    {
        switch (*++str_fmt){
        //
        case '%':
          //printf("\n ^^= %%");
          switch (*++str_fmt){
          //
          case 'd':
            printf("\n ^= %%d");
            int arg = va_arg(args, int);
            printf("\n va_arg = %d\n", arg);
            break;
          }
          break;
        }
    }
    va_end(args);
    //printf("\n str_fmt = \"%s\"", str_fmt);
    putchar('\n');
    printf("\n ~~~ %d\n", sizeof (str_fmt));
    int i;
    for (i = 0; str_fmt[i+1] != '\0'; i++) printf(" '%c',", str_fmt[i]);
    printf(" '%c'", str_fmt[i]);
    //sprintf(buf, "", );
    return str_fmt;
}
const char * valstr(unsigned char val)
{
    unsigned char val_str[3+1];
    unsigned char *ptr__val_str = val_str;
    switch (val){
    case 0: strcpy(val_str, "0"); break;
    case 1: strcpy(val_str, "1"); break;
    case 2: strcpy(val_str, "2"); break;
    case 3: strcpy(val_str, "3"); break;
    case 4: strcpy(val_str, "4"); break;
    case 5: strcpy(val_str, "5"); break;
    case 6: strcpy(val_str, "6"); break;
    case 7: strcpy(val_str, "7"); break;
    case 8: strcpy(val_str, "8"); break;
    case 9: strcpy(val_str, "9"); break;
    case 10: strcpy(val_str, "10"); break;
    }
    return ptr__val_str;
}