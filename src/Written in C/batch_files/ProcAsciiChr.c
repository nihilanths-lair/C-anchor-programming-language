const char ProcAsciiChr(const unsigned char chr)
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
const unsigned char * ProcAsciiChrEx(const unsigned char chr)
{
    unsigned char * res = "\0\0";
    switch (chr)
    {
    case 0  : res = ""; break; // 000 - 00 - \0
    //
    case 1  : res = ""; break; // 001 - 01 - ☺
    case 2  : res = ""; break; // 002 - 02 - ☻
    case 3  : res = ""; break; // 003 - 03 - ♥
    case 4  : res = ""; break; // 004 - 04 - ♦
    case 5  : res = ""; break; // 005 - 05 - ♣
    case 6  : res = ""; break; // 006 - 06 - ♠
    //
    case 7  : res = ""; break; // 007 - 07 -
    case 8  : res = ""; break; // 008 - 08 -
    case 9  : res = ""; break; // 009 - 09 -
    //
    case 10 : res = ""; break; // 010 - 0A - \n
    //
    case 11 : res = "♂"; break; // 011 - 0B - ♂
    case 12 : res = "♀"; break; // 012 - 0C - ♀
    //
    case 13 : res = ""; break; // 013 - 0D - \r
    //
    case 14 : res = "♫"; break; // 014 - 0E - ♫
    case 15 : res = ""; break; // 015 - 0F - ☼
    case 16 : res = ""; break; // 016 - 10 - ►
    case 17 : res = ""; break; // 017 - 11 - ◄
    case 18 : res = ""; break; // 018 - 12 - ↕
    case 19 : res = ""; break; // 019 - 13 - ‼
    case 20 : res = "¶"; break; // 020 - 14 - ¶
    case 21 : res = "§"; break; // 021 - 15 - §
    case 22 : res = ""; break; // 022 - 16 - ▬
    case 23 : res = "↨"; break; // 023 - 17 - ↨
    case 24 : res = ""; break; // 024 - 18 - ↑
    case 25 : res = "↓"; break; // 025 - 19 - ↓
    case 26 : res = "→"; break; // 026 - 1A - →
    case 27 : res = "←"; break; // 027 - 1B - ←
    case 28 : res = "∟"; break; // 028 - 1C - ∟
    case 29 : res = "↔"; break; // 029 - 1D - ↔
    case 30 : res = "▲"; break; // 030 - 1E - ▲
    case 31 : res = "▼"; break; // 031 - 1F - ▼
    //
    case 32 : res = " "; break; // 032 - 20 -
    //
    case 33 : res = "!"; break; // 033 - 21 - !
    case 34: res = "\""; break; // 034 - 22 - "
    case 35 : res = "#"; break; // 035 - 23 - #
    case 36 : res = "$"; break; // 036 - 24 - $
    case 37 : res = "%"; break; // 037 - 25 - %
    case 38 : res = "&"; break; // 038 - 26 - &
    case 39 : res = "'"; break; // 039 - 27 - '
    case 40 : res = "("; break; // 040 - 28 - (
    case 41 : res = ")"; break; // 041 - 29 - )
    case 42 : res = "*"; break; // 042 - 2A - *
    case 43 : res = "+"; break; // 043 - 2B - +
    case 44 : res = ","; break; // 044 - 2C - ,
    case 45 : res = "-"; break; // 045 - 2D - -
    case 46 : res = "."; break; // 046 - 2E - .
    case 47 : res = "/"; break; // 047 - 2F - /
    //
    case 48 : res = "0"; break; // 048 - 30 - 0
    case 49 : res = "1"; break; // 049 - 31 - 1
    case 50 : res = "2"; break; // 050 - 32 - 2
    case 51 : res = "3"; break; // 051 - 33 - 3
    case 52 : res = "4"; break; // 052 - 34 - 4
    case 53 : res = "5"; break; // 053 - 35 - 5
    case 54 : res = "6"; break; // 054 - 36 - 6
    case 55 : res = "7"; break; // 055 - 37 - 7
    case 56 : res = "8"; break; // 056 - 38 - 8
    case 57 : res = "9"; break; // 057 - 39 - 9
    //
    case 58 : res = ":"; break; // 058 - 3A - :
    case 59 : res = ";"; break; // 059 - 3B - ;
    case 60 : res = "<"; break; // 060 - 3C - <
    case 61 : res = "="; break; // 061 - 3D - =
    case 62 : res = ">"; break; // 062 - 3E - >
    case 63 : res = "?"; break; // 063 - 3F - ?
    case 64 : res = "@"; break; // 064 - 40 - @
    //
    case 65: res = "A"; break; // 065 - 41 - A
    case 66: res = "B"; break; // 066 - 42 - B
    case 67: res = "C"; break; // 067 - 43 - C
    case 68: res = "D"; break; // 068 - 44 - D
    case 69: res = "E"; break; // 069 - 45 - E
    case 70: res = "F"; break; // 070 - 46 - F
    case 71: res = "G"; break; // 071 - 47 - G
    case 72: res = "H"; break; // 072 - 48 - H
    case 73: res = "I"; break; // 073 - 49 - I
    case 74: res = "J"; break; // 074 - 4A - J
    case 75: res = "K"; break; // 075 - 4B - K
    case 76: res = "L"; break; // 076 - 4C - L
    case 77: res = "M"; break; // 077 - 4D - M
    case 78: res = "N"; break; // 078 - 4E - N
    case 79: res = "O"; break; // 079 - 4F - O
    case 80: res = "P"; break; // 080 - 50 - P
    case 81: res = "Q"; break; // 081 - 51 - Q
    case 82: res = "R"; break; // 082 - 52 - R
    case 83: res = "S"; break; // 083 - 53 - S
    case 84: res = "T"; break; // 084 - 54 - T
    case 85: res = "U"; break; // 085 - 55 - U
    case 86: res = "V"; break; // 086 - 56 - V
    case 87: res = "W"; break; // 087 - 57 - W
    case 88: res = "X"; break; // 088 - 58 - X
    case 89: res = "Y"; break; // 089 - 59 - Y
    case 90: res = "Z"; break; // 090 - 5A - Z
    //
    case 91: res = "["; break; // 091 - 5B - [
    //
    case 92: res = "\\"; break; // 092 - 5C - \
    //
    case 93: res = "]"; break; // 093 - 5D - ]
    case 94: res = "^"; break; // 094 - 5E - ^
    case 95: res = "_"; break; // 095 - 5F - _
    case 96: res = "`"; break; // 096 - 60 - `
    //
    case 97 : res = "a"; break; // 097 - 61 - a
    case 98 : res = "b"; break; // 098 - 62 - b
    case 99 : res = "c"; break; // 099 - 63 - c
    case 100: res = "d"; break; // 100 - 64 - d
    case 101: res = "e"; break; // 101 - 65 - e
    case 102: res = "f"; break; // 102 - 66 - f
    case 103: res = "g"; break; // 103 - 67 - g
    case 104: res = "h"; break; // 104 - 68 - h
    case 105: res = "i"; break; // 105 - 69 - i
    case 106: res = "j"; break; // 106 - 6A - j
    case 107: res = "k"; break; // 107 - 6B - k
    case 108: res = "l"; break; // 108 - 6C - l
    case 109: res = "m"; break; // 109 - 6D - m
    case 110: res = "n"; break; // 110 - 6E - n
    case 111: res = "o"; break; // 111 - 6F - o
    case 112: res = "p"; break; // 112 - 70 - p
    case 113: res = "q"; break; // 113 - 71 - q
    case 114: res = "r"; break; // 114 - 72 - r
    case 115: res = "s"; break; // 115 - 73 - s
    case 116: res = "t"; break; // 116 - 74 - t
    case 117: res = "u"; break; // 117 - 75 - u
    case 118: res = "v"; break; // 118 - 76 - v
    case 119: res = "w"; break; // 119 - 77 - w
    case 120: res = "x"; break; // 120 - 78 - x
    case 121: res = "y"; break; // 121 - 79 - y
    case 122: res = "z"; break; // 122 - 7A - z
    //
    case 123: res = "{"; break; // 123 - 7B - {
    case 124: res = "|"; break; // 124 - 7C - |
    case 125: res = "}"; break; // 125 - 7D - }
    case 126: res = "~"; break; // 126 - 7E - ~
    //
    case 127: res = ""; break; // 127 - 7F - 
    // (обновлено) ?
    case 128: res = ""; break; // 128 - 80 - ?
    case 129: res = ""; break; // 129 - 81 - ?
    case 130: res = "'"; break; // 130 - 82 - '
    case 131: res = ""; break; // 131 - 83 - ?
    //
    case 132: res = "\""; break; // 132 - 84 - "
    // (обновлено) ?
    case 133: res = ":"; break; // 133 - 85 - :
    case 134: res = "┼"; break; // 134 - 86 - ┼
    case 135: res = "╪"; break; // 135 - 87 - ╪
    case 136: res = ""; break; // 136 - 88 - ?
    case 137: res = "%"; break; // 137 - 89 - %
    case 138: res = ""; break; // 138 - 8A - ?
    case 139: res = "<"; break; // 139 - 8B - <
    case 140: res = ""; break; // 140 - 8C - ?
    case 141: res = ""; break; // 141 - 8D - ?
    case 142: res = ""; break; // 142 - 8E - ?
    case 143: res = ""; break; // 143 - 8F - ?
    case 144: res = ""; break; // 144 - 90 - ?
    case 145: res = "'"; break; // 145 - 91 - '
    case 146: res = "'"; break; // 146 - 92 - '
    //
    case 147: res = "\""; break; // 147 - 93 - "
    case 148: res = "\""; break; // 148 - 94 - "
    //
    case 149: res = ""; break; // 149 - 95 -
    // (обновлено) ?
    case 150: res = "-"; break; // 150 - 96 - -
    case 151: res = "-"; break; // 151 - 97 - -
    case 152: res = ""; break; // 152 - 98 - ?
    case 153: res = "T"; break; // 153 - 99 - T
    case 154: res = ""; break; // 154 - 9A - ?
    case 155: res = ">"; break; // 155 - 9B - >
    case 156: res = ""; break; // 156 - 9C - ?
    case 157: res = ""; break; // 157 - 9D - ?
    case 158: res = ""; break; // 158 - 9E - ?
    case 159: res = ""; break; // 159 - 9F - ?
    //
    case 160: res = ""; break; // 160 - A0 -  
    //
    case 161: res = "Ў"; break; // 161 - A1 - Ў
    case 162: res = "ў"; break; // 162 - A2 - ў
    case 163: res = "?"; break; // 163 - A3 - ?
    case 164: res = "¤"; break; // 164 - A4 - ¤
    case 165: res = "?"; break; // 165 - A5 - ?
    case 166: res = "│"; break; // 166 - A6 - │
    case 167: res = "§"; break; // 167 - A7 - §
    //
    case 168: res = "Ё"; break; // 168 - A8 - Ё
    // (обновлено) ?
    case 169: res = "c"; break; // 169 - A9 - c
    case 170: res = "Є"; break; // 170 - AA - Є
    case 171: res = "<"; break; // 171 - AB - <
    case 172: res = "┐"; break; // 172 - AC - ┐
    case 173: res = "-"; break; // 173 - AD - -
    case 174: res = "R"; break; // 174 - AE - R
    case 175: res = "Ї"; break; // 175 - AF - Ї
    case 176: res = "°"; break; // 176 - B0 - °
    case 177: res = "+"; break; // 177 - B1 - +
    case 178: res = ""; break; // 178 - B2 - ?
    case 179: res = ""; break; // 179 - B3 - ?
    case 180: res = ""; break; // 180 - B4 - ?
    case 181: res = "ч"; break; // 181 - B5 - ч
    case 182: res = "¶"; break; // 182 - B6 - ¶
    case 183: res = "·"; break; // 183 - B7 - ·
    //
    case 184: res = "ё"; break; // 184 - B8 - ё
    // (обновлено) ?
    case 185: res = "№"; break; // 185 - B9 - №
    case 186: res = "є"; break; // 186 - BA - є
    case 187: res = ">"; break; // 187 - BB - >
    case 188: res = ""; break; // 188 - BC - ?
    case 189: res = ""; break; // 189 - BD - ?
    case 190: res = ""; break; // 190 - BE - ?
    case 191: res = "ї"; break; // 191 - BF - ї
    //
    case 192: res = "А"; break; // 192 - C0 - А
    case 193: res = "Б"; break; // 193 - C1 - Б
    case 194: res = "В"; break; // 194 - C2 - В
    case 195: res = "Г"; break; // 195 - C3 - Г
    case 196: res = "Д"; break; // 196 - C4 - Д
    case 197: res = "Е"; break; // 197 - C5 - Е
    case 198: res = "Ж"; break; // 198 - C6 - Ж
    case 199: res = "З"; break; // 199 - C7 - З
    case 200: res = "И"; break; // 200 - C8 - И
    case 201: res = "Й"; break; // 201 - C9 - Й
    case 202: res = "К"; break; // 202 - CA - К
    case 203: res = "Л"; break; // 203 - CB - Л
    case 204: res = "М"; break; // 204 - CC - М
    case 205: res = "Н"; break; // 205 - CD - Н
    case 206: res = "О"; break; // 206 - CE - О
    case 207: res = "П"; break; // 207 - CF - П
    case 208: res = "Р"; break; // 208 - D0 - Р
    case 209: res = "С"; break; // 209 - D1 - С
    case 210: res = "Т"; break; // 210 - D2 - Т
    case 211: res = "У"; break; // 211 - D3 - У
    case 212: res = "Ф"; break; // 212 - D4 - Ф
    case 213: res = "Х"; break; // 213 - D5 - Х
    case 214: res = "Ц"; break; // 214 - D6 - Ц
    case 215: res = "Ч"; break; // 215 - D7 - Ч
    case 216: res = "Ш"; break; // 216 - D8 - Ш
    case 217: res = "Щ"; break; // 217 - D9 - Щ
    //
    // Слов, начинающихся с заглавного твёрдого знака не существует!
    case 218: res = "Ъ"; break; // 218 - DA - Ъ
    //
    case 219: res = "Ы"; break; // 219 - DB - Ы
    //
    // Слов, начинающихся с заглавного мягкого знака не существует!
    case 220: res = "Ь"; break; // 220 - DC - Ь
    //
    case 221: res = "Э"; break; // 221 - DD - Э
    case 222: res = "Ю"; break; // 222 - DE - Ю
    case 223: res = "Я"; break; // 223 - DF - Я
    //
    case 224: res = "а"; break; // 224 - E0 - а
    case 225: res = "б"; break; // 225 - E1 - б
    case 226: res = "в"; break; // 226 - E2 - в
    case 227: res = "г"; break; // 227 - E3 - г
    case 228: res = "д"; break; // 228 - E4 - д
    case 229: res = "е"; break; // 229 - E5 - е
    case 230: res = "ж"; break; // 230 - E6 - ж
    case 231: res = "з"; break; // 231 - E7 - з
    case 232: res = "и"; break; // 232 - E8 - и
    case 233: res = "й"; break; // 233 - E9 - й
    case 234: res = "к"; break; // 234 - EA - к
    case 235: res = "л"; break; // 235 - EB - л
    case 236: res = "м"; break; // 236 - EC - м
    case 237: res = "н"; break; // 237 - ED - н
    case 238: res = "о"; break; // 238 - EE - о
    case 239: res = "п"; break; // 239 - EF - п
    case 240: res = "р"; break; // 240 - F0 - р
    case 241: res = "с"; break; // 241 - F1 - с
    case 242: res = "т"; break; // 242 - F2 - т
    case 243: res = "у"; break; // 243 - F3 - у
    case 244: res = "ф"; break; // 244 - F4 - ф
    case 245: res = "х"; break; // 245 - F5 - х
    case 246: res = "ц"; break; // 246 - F6 - ц
    case 247: res = "ч"; break; // 247 - F7 - ч
    case 248: res = "ш"; break; // 248 - F8 - ш
    case 249: res = "щ"; break; // 249 - F9 - щ
    case 250: res = "ъ"; break; // 250 - FA - ъ
    case 251: res = "ы"; break; // 251 - FB - ы
    case 252: res = "ь"; break; // 252 - FC - ь
    case 253: res = "э"; break; // 253 - FD - э
    case 254: res = "ю"; break; // 254 - FE - ю
    case 255: res = "я"; break; // 255 - FF - я
    //
    default: printf("\n ProcAsciiChrEx = '%c'", chr); break;
    }
    return res;
}

const char * ProcAsciiChrDecodedText(const char chr)
{
    /*static*/char * res = "\0\0\0\0";
    printf("\n ProcAsciiChrDecodedText = %c", chr);// break;
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
void mem_dbg(const unsigned char *m)
{
    //static buf[0xFF];
    printf("\n <DEC>");
    for (int i = 0; i < 8; i++) printf(" %03d", i);
    printf("\t <HEX>");
    for (int i = 0; i < 8; i++) printf(" %02X", i);
    printf("\t <ASCII> "); // графа ASCII
    for (int i = 0; i < 8; i++) printf("%s", ProcAsciiChrEx(i));
    //putchar('\n');
    //printf("\n ------------------------------------------------------------------------------------------------------------------------\n");
    // Данные в памяти
    for (int j = 0, j2; j < 16; j++)
    {
        j2 = j*16;
        printf("\n\n");
        if (j2 >= 0 && j2 <= 255) printf("  %03d:", j2); // графа в данных DEC (1-я часть)
        for (int i = 0; i < 8; i++) printf(" %03d", m[j2+i]); // сами данные
        putchar('\t');
        if (j2 >= 0 && j2 <= 255) printf("   %02X:", j2); // графа в данных HEX (1-я часть)
        for (int i = 0; i < 8; i++) printf(" %02X", m[j2+i]); // сами данные
        putchar('\t');
        if (j2 >= 0 && j2 <= 255) printf("  %s: ", ProcAsciiChrEx(j2)); // графа в данных ASCII (1-я часть)
        for (int i = 0; i < 8; i++) printf("%s", ProcAsciiChrEx(m[j2+i])); // сами данные
        putchar('\n');
        //printf("\n\n");
        //printf("\n ------------------------------------------------------------------------------------------------------------------------\n");
        if (j2 >= 0 && j2 <= 255) printf("  %03d:", j2+8); // графа в данных DEC (2-я часть)
        for (int i = 8; i < 16; i++) printf(" %03d", m[j2+i]); // сами данные
        putchar('\t');
        if (j2 >= 0 && j2 <= 255) printf("   %02X:", j2+8); // графа в данных HEX (2-я часть)
        for (int i = 8; i < 16; i++) printf(" %02X", m[j2+i]); // сами данные
        putchar('\t');
        if (j2 >= 0 && j2 <= 255) printf("  %s: ", ProcAsciiChrEx(j2+8)); // графа в данных ASCII (2-я часть)
        for (int i = 8; i < 16; i++) printf("%s", ProcAsciiChrEx(m[j2+i])); // сами данные
        //putchar('\n');
        //printf("\n\n");
        //printf("\n ------------------------------------------------------------------------------------------------------------------------\n");
    }
    putchar('\n');
    /*
    // Generator DUMP памяти
    printf("\nconst unsigned char * ProcAsciiChrEx(const unsigned char chr)");
    printf("\n{");
    printf("\n    static unsigned char * res = \"\\0\\0\";");
    printf("\n    switch (chr)");
    printf("\n    {");
    printf("\n    case %-3d: res = \"\"; break; // %03d - %02X - \\0", 0, 0, 0); // Спец. символ (невидимый)
    printf("\n    //");
    for (int i = 1; i <= 6; i++) printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Спец. символы (отображаемые)
    printf("\n    //");
    for (int i = 7; i <= 9; i++) printf("\n    case %-3d: res = \"\"; break; // %03d - %02X - %c", i, i, i, i); // Спец. символы (невидимые)
    printf("\n    //");
    printf("\n    case %-3d: res = \"\"; break; // %03d - %02X - \\n", 10, 10, 10); // Спец. символ (невидимый)
    printf("\n    //");
    for (int i = 11; i <= 12; i++) printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Спец. символы (отображаемые)
    printf("\n    //");
    printf("\n    case %-3d: res = \"\"; break; // %03d - %02X - \\r", 13, 13, 13); // Спец. символ (невидимый)
    printf("\n    //");
    for (int i = 14; i <= 31; i++) printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Спец. символы (отображаемые)
    printf("\n    //");
    printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", 32, 32, 32, 32, 32); // Спец. символ - разделитель (невидимый)
    printf("\n    //");
    printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", 33, 33, 33, 33, 33);
    printf("\n    case %d: res = \"\\%c\"; break; // %03d - %02X - %c", 34, 34, 34, 34, 34);
    for (int i = 35; i <= 47; i++) printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Знаки
    printf("\n    //");
    for (int i = 48; i <= 57; i++) printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Цифры
    printf("\n    //");
    for (int i = 58; i <= 64; i++) printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Знаки
    printf("\n    //");
    for (int i = 65; i <= 90; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Заглавные английские буквы
    printf("\n    //");
    printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", 91, 91, 91, 91, 91);
    printf("\n    //");
    printf("\n    case %d: res = \"\\%c\"; break; // %03d - %02X - %c", 92, 92, 92, 92, 92);
    printf("\n    //");
    for (int i = 93; i <= 96; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i);
    printf("\n    //");
    for (int i = 97; i <= 99; i++) printf("\n    case %-3d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Строчные английские буквы
    for (int i = 100; i <= 122; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Строчные английские буквы
    printf("\n    //");
    for (int i = 123; i <= 126; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // {|}~
    printf("\n    //");
    printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", 127, 127, 127, 127, 127); // Спец. символ: 
    printf("\n    //");
    for (int i = 128; i <= 131; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i);
    printf("\n    //");
    printf("\n    case %d: res = \"\\%c\"; break; // %03d - %02X - %c", 132, 132, 132, 132, 132);
    printf("\n    //");
    for (int i = 133; i <= 146; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i);
    printf("\n    //");
    printf("\n    case %d: res = \"\\%c\"; break; // %03d - %02X - %c", 147, 147, 147, 147, 147);
    printf("\n    case %d: res = \"\\%c\"; break; // %03d - %02X - %c", 148, 148, 148, 148, 148);
    printf("\n    //");
    printf("\n    case %d: res = \"\"; break; // %03d - %02X - %c", 149, 149, 149, 149); // Спец. символ (невидимый)
    printf("\n    //");
    for (int i = 150; i <= 159; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i);
    printf("\n    //");
    printf("\n    case %d: res = \"\"; break; // %03d - %02X - %c", 160, 160, 160, 160); // Спец. символ (невидимый)
    printf("\n    //");
    for (int i = 161; i <= 167; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i);
    printf("\n    //");
    printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", 168, 168, 168, 168, 168); // Ё
    printf("\n    //");
    for (int i = 169; i <= 183; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i);
    printf("\n    //");
    printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", 184, 184, 184, 184, 184); // ё
    printf("\n    //");
    for (int i = 185; i <= 191; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i);
    printf("\n    //");
    for (int i = 192; i <= 217; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Заглавные русские буквы А~Щ
    printf("\n    //");
    printf("\n    // Слов, начинающихся с заглавного твёрдого знака не существует!");
    printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", 218, 218, 218, 218, 218); // Ъ
    printf("\n    //");
    printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", 219, 219, 219, 219, 219); // Ы
    printf("\n    //");
    printf("\n    // Слов, начинающихся с заглавного мягкого знака не существует!");
    printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", 220, 220, 220, 220, 220); // Ь
    printf("\n    //");
    for (int i = 221; i <= 223; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i);
    printf("\n    //");
    for (int i = 224; i <= 255; i++) printf("\n    case %d: res = \"%c\"; break; // %03d - %02X - %c", i, i, i, i, i); // Строчные русские буквы
    printf("\n    //");
    printf("\n    default: printf(\"\\n ProcAsciiChrEx = '%%c'\", chr); break;");
    printf("\n    }");
    printf("\n    return res;");
    printf("\n}");
    */
}