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
    case 0: res = " /!\\ "; break; // Спец. символы
    case 1: res = " /!\\ "; break; //
    case 2: res = " /!\\ "; break; //
    case 3: res = " /!\\ "; break; //
    case 4: res = " /!\\ "; break; //
    case 5: res = " /!\\ "; break; //
    case 6: res = " /!\\ "; break; //
    case 7: res = " /!\\ "; break; //
    case 8: res = " /!\\ "; break; //
    case 9: res = " /!\\ "; break; //
    case 10: res = " /!\\ "; break; //
    case 11: res = " /!\\ "; break; //
    case 12: res = " /!\\ "; break; //
    case 13: res = " /!\\ "; break; //
    case 14: res = " /!\\ "; break; //
    case 15: res = " /!\\ "; break; //
    case 16: res = " /!\\ "; break; //
    case 17: res = " /!\\ "; break; //
    case 18: res = " /!\\ "; break; //
    case 19: res = " /!\\ "; break; //
    case 20: res = " /!\\ "; break; //
    case 21: res = " /!\\ "; break; //
    case 22: res = " /!\\ "; break; //
    case 23: res = " /!\\ "; break; //
    case 24: res = " /!\\ "; break; //
    case 25: res = " /!\\ "; break; //
    case 26: res = " /!\\ "; break; //
    case 27: res = " /!\\ "; break; //
    case 28: res = " /!\\ "; break; //
    case 29: res = " /!\\ "; break; //
    case 30: res = " /!\\ "; break; //
    case 31: res = " /!\\ "; break; // Спец. символы
    case ' ': res = " ' ' "; break; // SP
    //
    case 'A': res = " 'A' "; break;
    case 'B': res = " 'B' "; break;
    case 'C': res = " 'C' "; break;
    case 'D': res = " 'D' "; break;
    case 'E': res = " 'E' "; break;
    case 'F': res = " 'F' "; break;
    case 'G': res = " 'G' "; break;
    case 'H': res = " 'H' "; break;
    case 'I': res = " 'I' "; break;
    case 'J': res = " 'J' "; break;
    case 'K': res = " 'K' "; break;
    case 'L': res = " 'L' "; break;
    case 'M': res = " 'M' "; break;
    case 'N': res = " 'N' "; break;
    case 'O': res = " 'O' "; break;
    case 'P': res = " 'P' "; break;
    case 'Q': res = " 'Q' "; break;
    case 'R': res = " 'R' "; break;
    case 'S': res = " 'S' "; break;
    case 'T': res = " 'T' "; break;
    case 'U': res = " 'U' "; break;
    case 'V': res = " 'V' "; break;
    case 'W': res = " 'W' "; break;
    case 'X': res = " 'X' "; break;
    case 'Y': res = " 'Y' "; break;
    case 'Z': res = " 'Z' "; break;
    //
    case 'a': res = " 'a' "; break;
    case 'b': res = " 'b' "; break;
    case 'c': res = " 'c' "; break;
    case 'd': res = " 'd' "; break;
    case 'e': res = " 'e' "; break;
    case 'f': res = " 'f' "; break;
    case 'g': res = " 'g' "; break;
    case 'h': res = " 'h' "; break;
    case 'i': res = " 'i' "; break;
    case 'j': res = " 'j' "; break;
    case 'k': res = " 'k' "; break;
    case 'l': res = " 'l' "; break;
    case 'm': res = " 'm' "; break;
    case 'n': res = " 'n' "; break;
    case 'o': res = " 'o' "; break;
    case 'p': res = " 'p' "; break;
    case 'q': res = " 'q' "; break;
    case 'r': res = " 'r' "; break;
    case 's': res = " 's' "; break;
    case 't': res = " 't' "; break;
    case 'u': res = " 'u' "; break;
    case 'v': res = " 'v' "; break;
    case 'w': res = " 'w' "; break;
    case 'x': res = " 'x' "; break;
    case 'y': res = " 'y' "; break;
    case 'z': res = " 'z' "; break;
    //
    case '.': res = " '.' "; break;
    case '!': res = " '!' "; break;
    case '$': res = " '$' "; break;
    case '#': res = " '#' "; break; // 23h - 035d
    case '(': res = " '(' "; break; // 28h - 040d
    case '?': res = " '?' "; break; // 3Fh - 063d
    //
    case 'А': res = " 'А' "; break;
    case 'Б': res = " 'Б' "; break;
    case 'В': res = " 'В' "; break;
    case 'Г': res = " 'Г' "; break;
    case 'Д': res = " 'Д' "; break;
    case 'Е': res = " 'Е' "; break;
    case 'Ё': res = " 'Ё' "; break;
    case 'Ж': res = " 'Ж' "; break;
    case 'З': res = " 'З' "; break;
    case 'И': res = " 'И' "; break;
    case 'Й': res = " 'Й' "; break;
    case 'К': res = " 'К' "; break;
    case 'Л': res = " 'Л' "; break;
    case 'М': res = " 'М' "; break;
    case 'Н': res = " 'Н' "; break;
    case 'О': res = " 'О' "; break;
    case 'П': res = " 'П' "; break;
    case 'Р': res = " 'Р' "; break;
    case 'С': res = " 'С' "; break;
    case 'Т': res = " 'Т' "; break;
    case 'У': res = " 'У' "; break;
    case 'Ф': res = " 'Ф' "; break;
    case 'Х': res = " 'Х' "; break;
    case 'Ц': res = " 'Ц' "; break;
    case 'Ч': res = " 'Ч' "; break;
    case 'Ш': res = " 'Ш' "; break;
    case 'Щ': res = " 'Щ' "; break;
    // Слов, начинающихся с заглавного твёрдого знака не существует
    case 'Ы': res = " 'Ы' "; break; // Спорно..
    // Слов, начинающихся с заглавного мягкого знака не существует
    case 'Э': res = " 'Э' "; break;
    case 'Ю': res = " 'Ю' "; break;
    case 'Я': res = " 'Я' "; break;
    //
    case 'а': res = " 'а' "; break;
    case 'б': res = " 'б' "; break;
    case 'в': res = " 'в' "; break;
    case 'г': res = " 'г' "; break;
    case 'д': res = " 'д' "; break;
    case 'е': res = " 'е' "; break;
    case 'ё': res = " 'ё' "; break;
    case 'ж': res = " 'ж' "; break;
    case 'з': res = " 'з' "; break;
    case 'и': res = " 'и' "; break;
    case 'й': res = " 'й' "; break;
    case 'к': res = " 'к' "; break;
    case 'л': res = " 'л' "; break;
    case 'м': res = " 'м' "; break;
    case 'н': res = " 'н' "; break;
    case 'о': res = " 'о' "; break;
    case 'п': res = "  п' "; break;
    case 'р': res = " 'р' "; break;
    case 'с': res = " 'с' "; break;
    case 'т': res = " 'т' "; break;
    case 'у': res = " 'у' "; break;
    case 'ф': res = " 'ф' "; break;
    case 'х': res = " 'х' "; break;
    case 'ц': res = " 'ц' "; break;
    case 'ч': res = " 'ч' "; break;
    case 'ш': res = " 'ш' "; break;
    case 'щ': res = " 'щ' "; break;
    case 'ъ': res = " 'ъ' "; break;
    case 'ы': res = " 'ы' "; break;
    case 'ь': res = " 'ь' "; break;
    case 'э': res = " 'э' "; break;
    case 'ю': res = " 'ю' "; break;
    case 'я': res = " 'я' "; break;
    //
    default: printf("\n ProcAsciiChrEx = '%c'", chr); break;
    }
    return res;
}

const char * ProcAsciiChrDecodedText(const char chr)
{
    /*static*/char * res = "\0\0\0\0";
    switch (chr){
    //
    case 0: res = "\\00 "; break; // Спец. символы
    case 1: res = "\\01 "; break; // Спец. символы
    case 2: res = "\\02 "; break; // Спец. символы
    case 3: res = "\\03 "; break; // Спец. символы
    case 4: res = "\\04 "; break; // Спец. символы
    case 5: res = "\\05 "; break; // Спец. символы
    case 6: res = "\\06 "; break; // Спец. символы
    case 7: res = "\\07 "; break; // Спец. символы
    case 8: res = "\\08 "; break; // Спец. символы
    case 9: res = "\\09 "; break; // Спец. символы
    case 0x0A: res = "\\0A "; break; // Спец. символы
    case 0x0B: res = "\\0B "; break; // Спец. символы
    case 0x0C: res = "\\0C "; break; // Спец. символы
    case 0x0D: res = "\\0D "; break; // Спец. символы
    case 0x0E: res = "\\0E "; break; // Спец. символы
    case 0x0F: res = "\\0F "; break; // Спец. символы
    case 16: res = "\\10"; break; // Спец. символы
    case 17: res = "\\11"; break; // Спец. символы
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
    case ' ': res = "    "; break; // SP
    //
    case 'A': res = " A "; break;
    case 'B': res = " B "; break;
    case 'C': res = " C  "; break;
    case 'D': res = " D "; break;
    case 'E': res = " E "; break;
    case 'F': res = " F "; break;
    case 'G': res = " G "; break;
    case 'H': res = " H "; break;
    case 'I': res = " I "; break;
    case 'J': res = " J "; break;
    case 'K': res = " K "; break;
    case 'L': res = " L "; break;
    case 'M': res = " M "; break;
    case 'N': res = " N "; break;
    case 'O': res = " O "; break;
    case 'P': res = " P "; break;
    case 'Q': res = " Q "; break;
    case 'R': res = " R "; break;
    case 'S': res = " S "; break;
    case 'T': res = " T "; break;
    case 'U': res = " U "; break;
    case 'V': res = " V "; break;
    case 'W': res = " W "; break;
    case 'X': res = " X "; break;
    case 'Y': res = " Y "; break;
    case 'Z': res = " Z "; break;
    //
    case 'a': res = " a "; break;
    case 'b': res = " b "; break;
    case 'c': res = " c "; break;
    case 'd': res = " d "; break;
    case 'e': res = " e "; break;
    case 'f': res = " f "; break;
    case 'g': res = " g "; break;
    case 'h': res = " h "; break;
    case 'i': res = " i  "; break;
    case 'j': res = " j "; break;
    case 'k': res = " k "; break;
    case 'l': res = " l "; break;
    case 'm': res = " m "; break;
    case 'n': res = " n "; break;
    case 'o': res = " o "; break;
    case 'p': res = " p "; break;
    case 'q': res = " q "; break;
    case 'r': res = " r "; break;
    case 's': res = " s "; break;
    case 't': res = " t "; break;
    case 'u': res = " u "; break;
    case 'v': res = " v "; break;
    case 'w': res = " w "; break;
    case 'x': res = " x "; break;
    case 'y': res = " y "; break;
    case 'z': res = " z "; break;
    //
    case '.': res = " . "; break;
    case '!': res = " ! "; break;
    case '#': res = " # "; break; // 23h - 035d
    case '$': res = " $ "; break; // 24h - 036d
    case '(': res = " ( "; break; // 28h - 040d
    //
    case 'А': res = " А "; break;
    case 'Б': res = " Б "; break;
    case 'В': res = " В "; break;
    case 'Г': res = " Г "; break;
    case 'Д': res = " Д "; break;
    case 'Е': res = " Е "; break;
    case 'Ё': res = " Ё "; break;
    case 'Ж': res = " Ж "; break;
    case 'З': res = " З "; break;
    case 'И': res = " И "; break;
    case 'Й': res = " Й "; break;
    case 'К': res = " К "; break;
    case 'Л': res = " Л "; break;
    case 'М': res = " М "; break;
    case 'Н': res = " Н "; break;
    case 'О': res = " О "; break;
    case 'П': res = " П "; break;
    case 'Р': res = " Р "; break;
    case 'С': res = " С "; break;
    case 'Т': res = " Т "; break;
    case 'У': res = " У "; break;
    case 'Ф': res = " Ф "; break;
    case 'Х': res = " Х "; break;
    case 'Ц': res = " Ц "; break;
    case 'Ч': res = " Ч "; break;
    case 'Ш': res = " Ш "; break;
    case 'Щ': res = " Щ "; break;
    // Слов, начинающихся с заглавного твёрдого знака не существует
    case 'Ы': res = " Ы "; break; // Спорно..
    // Слов, начинающихся с заглавного мягкого знака не существует
    case 'Э': res = " Э "; break;
    case 'Ю': res = " Ю "; break;
    case 'Я': res = " Я "; break;
    //
    case 'а': res = " а "; break;
    case 'б': res = " б "; break;
    case 'в': res = " в "; break;
    case 'г': res = " г "; break;
    case 'д': res = " д "; break;
    case 'е': res = " е "; break;
    case 'ё': res = " ё "; break;
    case 'ж': res = " ж "; break;
    case 'з': res = " з "; break;
    case 'и': res = " и "; break;
    case 'й': res = " й "; break;
    case 'к': res = " к "; break;
    case 'л': res = " л "; break;
    case 'м': res = " м "; break;
    case 'н': res = " н "; break;
    case 'о': res = " о "; break;
    case 'п': res = " п "; break;
    case 'р': res = " р "; break;
    case 'с': res = " с "; break;
    case 'т': res = " т "; break;
    case 'у': res = " у "; break;
    case 'ф': res = " ф "; break;
    case 'х': res = " х "; break;
    case 'ц': res = " ц "; break;
    case 'ч': res = " ч "; break;
    case 'ш': res = " ш "; break;
    case 'щ': res = " щ "; break;
    case 'ъ': res = " ъ "; break;
    case 'ы': res = " ы "; break;
    case 'ь': res = " ь "; break;
    case 'э': res = " э "; break;
    case 'ю': res = " ю "; break;
    case 'я': res = " я "; break;
    //
    default: printf("\n ProcAsciiChrDecodedText = %c", chr); break;
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
void mem_dbg(const char *m)
{
    //static buf[0xFF];
    printf("\n  DEC ");
    for (int i = 0; i < 8; i++) printf("[%03d]", i);
    printf("\t HEX ");
    for (int i = 0; i < 8; i++) printf("[%02X]", i);
    printf("\tASCII "); // графа ASCII
    for (int i = 0; i < 8; i++) printf("[%c]", i);
    putchar('\n');
    //printf("\n ------------------------------------------------------------------------------------------------------------------------\n");
    // Данные в памяти
    for (int j = 0, j2; j < 16; j++)
    {
        j2 = j*16;
        if (j2 >= 0 && j2 <= 255) printf(" [%03d]", j2); // графа в данных DEC (1-я часть)
        for (int i = 0; i < 8; i++) printf(" %03d ", (unsigned char) m[j2+i]); // сами данные
        putchar('\t');
        if (j2 >= 0 && j2 <= 255) printf(" [%02X]", j2); // графа в данных HEX (1-я часть)
        for (int i = 0; i < 8; i++) printf(" %02X ", (unsigned char) m[j2+i]); // сами данные
        putchar('\t');
        if (j2 >= 0 && j2 <= 255) printf("  [%c] ", j2); // графа в данных ASCII (1-я часть)
        for (int i = 0; i < 8; i++) printf("%c", m[j2+i]); // сами данные
        putchar('\n');
        //printf("\n ------------------------------------------------------------------------------------------------------------------------\n");
        j2 += 8;
        if (j2 >= 0 && j2 <= 255) printf(" [%03d]", j2); // графа в данных DEC (2-я часть)
        for (int i = 8; i < 16; i++) printf(" %03d ", (unsigned char) m[j2+i]); // сами данные
        putchar('\t');
        if (j2 >= 0 && j2 <= 255) printf(" [%02X]", j2); // графа в данных HEX (2-я часть)
        for (int i = 8; i < 16; i++) printf(" %02X ", (unsigned char) m[j2+i]); // сами данные
        putchar('\t');
        if (j2 >= 0 && j2 <= 255) printf("  [%c] ", (j2 != 8) ? j2 : ' '); // графа в данных ASCII (2-я часть)
        for (int i = 8; i < 16; i++) printf("%c", m[j2+i]); // сами данные
        putchar('\n');
        //printf("\n ------------------------------------------------------------------------------------------------------------------------\n");
    }
}