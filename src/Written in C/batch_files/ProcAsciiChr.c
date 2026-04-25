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
    /*static*/char * res = "\0\0\0\0";
    switch (chr){
    //
    case 0: res = "'\\0' "; break; // Спец. символы
    case 1: res = "'\\1' "; break; // Спец. символы
    case 2: res = "'\\2' "; break; // Спец. символы
    case 3: res = "'\\3' "; break; // Спец. символы
    case 4: res = "'\\4' "; break; // Спец. символы
    case 5: res = "'\\5' "; break; // Спец. символы
    case 6: res = "'\\6' "; break; // Спец. символы
    case 7: res = "'\\7' "; break; // Спец. символы
    case 8: res = "'\\8' "; break; // Спец. символы
    case 9: res = "'\\9' "; break; // Спец. символы
    case 10: res = "'\\10'"; break; // Спец. символы
    case 11: res = "'\\11'"; break; // Спец. символы
    case 12: res = "'\\12'"; break; // Спец. символы
    case 13: res = "'\\13'"; break; // Спец. символы
    case 14: res = "'\\14'"; break; // Спец. символы
    case 15: res = "'\\15'"; break; // Спец. символы
    case 16: res = "'\\16'"; break; // Спец. символы
    case 17: res = "'\\17'"; break; // Спец. символы
    case 18: res = "'\\18'"; break; // Спец. символы
    case 19: res = "'\\19'"; break; // Спец. символы
    case 20: res = "'\\20'"; break; // Спец. символы
    case 21: res = "'\\21'"; break; // Спец. символы
    case 22: res = "'\\22'"; break; // Спец. символы
    case 23: res = "'\\23'"; break; // Спец. символы
    case 24: res = "'\\24'"; break; // Спец. символы
    case 25: res = "'\\25'"; break; // Спец. символы
    case 26: res = "'\\26'"; break; // Спец. символы
    case 27: res = "'\\27'"; break; // Спец. символы
    case 28: res = "'\\28'"; break; // Спец. символы
    case 29: res = "'\\29'"; break; // Спец. символы
    case 30: res = "'\\30'"; break; // Спец. символы
    case 31: res = "'\\31'"; break; // Спец. символы
    //
    case 'A': res = "'A'  "; break;
    case 'B': res = "'B'  "; break;
    case 'C': res = "'C'  "; break;
    case 'D': res = "'D'  "; break;
    case 'E': res = "'E'  "; break;
    case 'F': res = "'F'  "; break;
    //
    case '$': res = "'$'  "; break;
    case ' ': res = "' '  "; break;
    case 'i': res = "'i'  "; break;
    case 's': res = "'s'  "; break;
    //
    case 'a': res = "'a'  "; break;
    case 'b': res = "'b'  "; break;
    case 'c': res = "'c'  "; break;
    case 'd': res = "'d'  "; break;
    case 'e': res = "'e'  "; break;
    case 'f': res = "'f'  "; break;
    //
    case 'w': res = "'w'  "; break;
    case 'o': res = "'o'  "; break;
    case 'm': res = "'m'  "; break;
    case '!': res = "'!'  "; break;
    //
    case 'А': res = "'А'  "; break;
    case 'л': res = "'л'  "; break;
    case 'о': res = "'о'  "; break;
    case 'к': res = "'к'  "; break;
    case 'ц': res = "'ц'  "; break;
    case 'и': res = "'и'  "; break;
    case 'я': res = "'я'  "; break;
    case 'п': res = "'п'  "; break;
    case 'а': res = "'а'  "; break;
    case 'м': res = "'м'  "; break;
    case 'т': res = "'т'  "; break;
    //
    default: printf("\n res = '%c'", chr); break;
    }
    return res;
}

const char * ProcAsciiChrDecodedText(const char chr)
{
    /*static*/char * res = "\0\0\0\0";
    switch (chr){
    //
    case 0: res = "\\? "; break; // Спец. символы
    case 1: res = "\\? "; break; // Спец. символы
    case 2: res = "\\02"; break; // Спец. символы
    case 3: res = "\\03"; break; // Спец. символы
    case 4: res = "\\04"; break; // Спец. символы
    case 5: res = "\\05"; break; // Спец. символы
    case 6: res = "\\06"; break; // Спец. символы
    case 7: res = "\\07"; break; // Спец. символы
    case 8: res = "\\08"; break; // Спец. символы
    case 9: res = "\\09"; break; // Спец. символы
    case 0x0A: res = "\\0A"; break; // Спец. символы
    case 0x0B: res = "\\0B"; break; // Спец. символы
    case 0x0C: res = "\\0C"; break; // Спец. символы
    case 0x0D: res = "\\0D"; break; // Спец. символы
    case 0x0E: res = "\\0E"; break; // Спец. символы
    case 0x0F: res = "\\? "; break; // Спец. символы
    case 16: res = "\\10"; break; // Спец. символы
    case 17: res = "\\? "; break; // Спец. символы
    case 18: res = "\\12"; break; // Спец. символы
    case 19: res = "\\13"; break; // Спец. символы
    case 20: res = "\\14"; break; // Спец. символы
    case 21: res = "\\15"; break; // Спец. символы
    case 22: res = "\\16"; break; // Спец. символы
    case 23: res = "\\17"; break; // Спец. символы
    case 24: res = "\\18"; break; // Спец. символы
    case 25: res = "\\19"; break; // Спец. символы
    case 0x1A: res = "\\1A"; break; // Спец. символы
    case 0x1B: res = "\\1B"; break; // Спец. символы
    case 0x1C: res = "\\1C"; break; // Спец. символы
    case 0x1D: res = "\\1D"; break; // Спец. символы
    case 0x1E: res = "\\1E"; break; // Спец. символы
    case 0x1F: res = "\\1F"; break; // Спец. символы
    case ' ': res = "   "; break;
    //
    case 'A': res = " A "; break;
    case 'B': res = " B "; break;
    case 'C': res = " C "; break;
    case 'D': res = " D "; break;
    case 'E': res = " E "; break;
    case 'F': res = " F "; break;
    //
    case '$': res = " $ "; break;
    case 'i': res = " i "; break;
    case 's': res = " s "; break;
    //
    case 'a': res = " a "; break;
    case 'b': res = " b "; break;
    case 'c': res = " c "; break;
    case 'd': res = " d "; break;
    case 'e': res = " e "; break;
    case 'f': res = " f "; break;
    //
    case 'w': res = " w "; break;
    case 'o': res = " o "; break;
    case 'm': res = " m "; break;
    case '!': res = " ! "; break;
    //
    case 'А': res = " А "; break;
    case 'л': res = " л "; break;
    case 'о': res = " о "; break;
    case 'к': res = " к "; break;
    case 'ц': res = " ц "; break;
    case 'и': res = " и "; break;
    case 'я': res = " я "; break;
    case 'п': res = " п "; break;
    case 'а': res = " а "; break;
    case 'м': res = " м "; break;
    case 'т': res = " т "; break;
    //
    default: printf("\n res = %c", chr); break;
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