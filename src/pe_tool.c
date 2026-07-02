#include <stdint.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

// БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)
//  Размер: Всегда строго 64 байта.
//  Природа: Статичный исторический балласт. Изменяется только одно поле -> e_lfanew.
uint8_t e_magic[] = "MZ";  // 000~001      | 00~01      #  Магическое число
//  (size: 56)       // 002~059      | 02~3B      #  Зарезервировано: Обычно забито нулями (0). Сюда можно спрятать кастомные метаданные компилятора, Windows их игнорирует.
uint8_t e_lfanew[4]; // 060~063: 064 | 3C~3F: 40  #  Динамическое поле: Указывает смещение (в байтах от начала файла), где начнется Блок 2 (PE). • Минимум: 64 (если DOS-код заглушки отсутствует).• Динамика: Если ты решишь вставить туда реальную DOS-программу (которая пишет "This program cannot be run in DOS mode"), это поле сдвинется вперед на размер этого DOS-кода (обычно 128 или 248).
uint32_t _e_lfanew = 64;

// БЛОК 2: PE ЗАГОЛОВОК (COFF File Header)
//  Размер: Всегда строго 24 байта (4 байта сигнатура + 20 байт заголовок).
//  Природа: Задает базовые свойства файла.
uint8_t pe_signature[] = "PE\0\0";
uint8_t Machine[2];

// БЛОК 3: OPTIONAL HEADER (Только для 64-бит / PE32+)
//  Размер: Базовый — 112 байт.
//  Природа: Самый сложный блок. Конфигурирует подсистему памяти Windows.
// ... ... ...


// БЛОК 4: КАТАЛОГИ ДАННЫХ (Data Directories)
//  Размер: Динамический. Зависит от NumberOfRvaAndSizes. Каждый каталог занимает 8 байт (4 байта адрес + 4 байта размер). Если каталогов 16 \(\rightarrow \) размер строго 128 байт.
//  Природа: Указатели на сложные системные таблицы.
// ... ... ...


uint8_t program[2048];

//uint64_t offset = 0;

// Порядок байт: little-endian

void pe_builder()
{
    FILE * descriptor = fopen("test_subject.exe", "wb");
    fwrite("MZ", 1, 2, descriptor); // e_magic  | 000: 077 090 | 00: 4D 5A | MZ
    fwrite("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, 56, descriptor); // 002~058: 000 | 02~3A: 00
    //fputc(64, descriptor);          // e_lfanew | 060~063: 064 | 3C~3F: 40 | @
    fwrite("\x40\0\0\0", 1, 4, descriptor); //fwrite("@\0\0\0", 1, 4, descriptor);
    fwrite("PE\0\0", 1, 4, descriptor);
    fwrite("\x64\x86", 1, 2, descriptor);
    fclose(descriptor);
}

void pe_analyzer()
{
    FILE * descriptor = fopen("test_subject.exe", "rb");
    if (!descriptor) return;
    printf("\n БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)");
    if (fread(e_magic, 1, 2, descriptor) != 2) { printf("\n  e_magic != size: 2");/**/ return; } // e_magic | 000: 077 090 | 00: 4D 5A | MZ
    if (strcmp(e_magic, "MZ")) { /**/printf("\n  e_magic != MZ");/**/ return; }
    printf("\n -------------------------------------------------------------");
    printf("\n  000: %03d | 00: %02X | %c | uint16_t e_magic = \"%s\"", e_magic[0], e_magic[0], e_magic[0], e_magic);
    printf("\n  001: %03d | 01: %02X | %c |", e_magic[1], e_magic[1], e_magic[1]);
    printf("\n -------------------------------------------------------------");
    if (fread(program, 1, 56, descriptor) != 56) { /**/printf("\n  merged_fields != size: 56");/**/ return; }  // 002: ???     | 02: ??
    printf("\n  002: %03d | 02: %02X | · | uint16_t e_cblp = '\\0',", program[0], program[0]);
    printf("\n  003: %03d | 03: %02X | · |                 = '\\0'",  program[1], program[1]);
    printf("\n -------------------------------------------------------------");
    printf("\n  004: %03d | 04: %02X | · | uint16_t e_cp = '\\0',", program[2], program[2]);
    printf("\n  005: %03d | 05: %02X | · |               = '\\0'",  program[3], program[3]);
    printf("\n -------------------------------------------------------------");
    printf("\n  006: %03d | 06: %02X | · | uint16_t e_crlc = '\\0',", program[4], program[4]);
    printf("\n  007: %03d | 07: %02X | · |                 = '\\0'",  program[5], program[5]);
    printf("\n -------------------------------------------------------------");
    printf("\n  008: %03d | 08: %02X | · | uint16_t e_cparhdr = '\\0',", program[6], program[6]);
    printf("\n  009: %03d | 09: %02X | · |                    = '\\0'",  program[7], program[7]);
    printf("\n -------------------------------------------------------------");
    printf("\n  010: %03d | 0A: %02X | · | uint16_t e_minalloc = '\\0',", program[8], program[8]);
    printf("\n  011: %03d | 0B: %02X | · |                     = '\\0'",  program[9], program[9]);
    printf("\n -------------------------------------------------------------");
    printf("\n  012: %03d | 0C: %02X | · | uint16_t e_maxalloc = '\\0',", program[10], program[10]);
    printf("\n  013: %03d | 0D: %02X | · |                     = '\\0'",  program[11], program[11]);
    printf("\n -------------------------------------------------------------");
    printf("\n  014: %03d | 0E: %02X | · | uint16_t e_ss = '\\0',", program[12], program[12]);
    printf("\n  015: %03d | 0F: %02X | · |               = '\\0'",  program[13], program[13]);
    printf("\n -------------------------------------------------------------");
    printf("\n  016: %03d | 10: %02X | · | uint16_t e_sp = '\\0',", program[14], program[14]);
    printf("\n  017: %03d | 11: %02X | · |               = '\\0'",  program[15], program[15]);
    printf("\n -------------------------------------------------------------");
    printf("\n  018: %03d | 12: %02X | · | uint16_t e_csum = '\\0',", program[16], program[16]);
    printf("\n  019: %03d | 13: %02X | · |                 = '\\0'",  program[17], program[17]);
    printf("\n -------------------------------------------------------------");
    printf("\n  020: %03d | 14: %02X | · | uint16_t e_ip = '\\0',", program[18], program[18]);
    printf("\n  021: %03d | 15: %02X | · |               = '\\0'",  program[19], program[19]);
    printf("\n -------------------------------------------------------------");
    printf("\n  022: %03d | 16: %02X | · | uint16_t e_cs = '\\0',", program[20], program[20]);
    printf("\n  023: %03d | 17: %02X | · |               = '\\0'",  program[21], program[21]);
    printf("\n -------------------------------------------------------------");
    printf("\n  024: %03d | 18: %02X | · | uint16_t e_lfarlc = '\\0',", program[22], program[22]);
    printf("\n  025: %03d | 19: %02X | · |                   = '\\0'",  program[23], program[23]);
    printf("\n -------------------------------------------------------------");
    printf("\n  026: %03d | 1A: %02X | · | uint16_t e_ovno = '\\0',", program[24], program[24]);
    printf("\n  027: %03d | 1B: %02X | · |                 = '\\0'",  program[25], program[25]);
    printf("\n -------------------------------------------------------------");
    printf("\n  028: %03d | 1C: %02X | · | uint16_t e_res[4] = '\\0', №1", program[26], program[26]);
    printf("\n  029: %03d | 1D: %02X | · |                     '\\0',",    program[27], program[27]);
    printf("\n  030: %03d | 1E: %02X | · |                     '\\0', №2", program[28], program[28]);
    printf("\n  031: %03d | 1F: %02X | · |                     '\\0',",    program[29], program[29]);
    printf("\n  032: %03d | 20: %02X | · |                     '\\0', №3", program[30], program[30]);
    printf("\n  033: %03d | 21: %02X | · |                     '\\0',",    program[31], program[31]);
    printf("\n  034: %03d | 22: %02X | · |                     '\\0', №4", program[32], program[32]);
    printf("\n  035: %03d | 23: %02X | · |                     '\\0'.",    program[33], program[33]);
    printf("\n -------------------------------------------------------------");
    printf("\n  036: %03d | 24: %02X | · | uint16_t e_oemid = '\\0', №1",  program[34], program[34]);
    printf("\n  037: %03d | 25: %02X | · |                    '\\0'.",     program[35], program[35]);
    printf("\n -------------------------------------------------------------");
    printf("\n  038: %03d | 26: %02X | · | uint16_t e_oeminfo = '\\0', №1", program[36], program[36]);
    printf("\n  039: %03d | 27: %02X | · |                      '\\0'.",    program[37], program[37]);
    printf("\n -------------------------------------------------------------");
    printf("\n  040: %03d | 28: %02X | · | uint16_t e_res2[10] = '\\0', №1", program[38], program[38]);
    printf("\n  041: %03d | 29: %02X | · |                       '\\0',",    program[39], program[39]);
    printf("\n  042: %03d | 2A: %02X | · |                       '\\0', №2", program[40], program[40]);
    printf("\n  043: %03d | 2B: %02X | · |                       '\\0',",    program[41], program[41]);
    printf("\n  044: %03d | 2C: %02X | · |                       '\\0', №3", program[42], program[42]);
    printf("\n  045: %03d | 2D: %02X | · |                       '\\0',",    program[43], program[43]);
    printf("\n  046: %03d | 2E: %02X | · |                       '\\0', №4", program[44], program[44]);
    printf("\n  047: %03d | 2F: %02X | · |                       '\\0',",    program[45], program[45]);
    printf("\n  048: %03d | 30: %02X | · |                       '\\0', №5", program[46], program[46]);
    printf("\n  049: %03d | 31: %02X | · |                       '\\0',",    program[47], program[47]);
    printf("\n  050: %03d | 32: %02X | · |                       '\\0', №6", program[48], program[48]);
    printf("\n  051: %03d | 33: %02X | · |                       '\\0',",    program[49], program[49]);
    printf("\n  052: %03d | 34: %02X | · |                       '\\0', №7", program[50], program[50]);
    printf("\n  053: %03d | 35: %02X | · |                       '\\0',",    program[51], program[51]);
    printf("\n  054: %03d | 36: %02X | · |                       '\\0', №8", program[52], program[52]); // 54 - 2 = 52
    printf("\n  055: %03d | 37: %02X | · |                       '\\0',",    program[53], program[53]); // 55 - 2 = 53
    printf("\n  056: %03d | 38: %02X | · |                       '\\0', №9", program[54], program[54]); // 56 - 2 = 54
    printf("\n  057: %03d | 39: %02X | · |                       '\\0',",    program[55], program[55]); // 57 - 2 = 55
    // Внимание! Для адресов 058 и 059 мы НЕ МОЖЕМ читать массив program дальше.
    // По спецификации PE, эти последние 2 байта DOS-заголовка перед e_lfanew 
    // остаются неиспользованными, поэтому мы просто выводим их как жесткие нули:
    printf("\n  058: 000 | 3A: 00 | · |                       '\\0', №10");
    printf("\n  059: 000 | 3B: 00 | · |                       '\\0'.");
    printf("\n -------------------------------------------------------------");
    if (fread(e_lfanew, 1, 4, descriptor) != 4) { /**/printf("\n  e_lfanew != 4");/**/ return; } // e_lfanew | 060: ??? ??? ??? ??? | ??: ?? ?? ?? ??
    //if (fread(e_lfanew, 4, 1, descriptor) != 1)
    printf("\n  060: %03d | 3C: %02X |  '%c' | uint32_t e_lfanew = %ld", e_lfanew[0], e_lfanew[0], e_lfanew[0], _e_lfanew);
    printf("\n  061: %03d | 3D: %02X | '\\0' |", e_lfanew[1], e_lfanew[1]);
    printf("\n  062: %03d | 3E: %02X | '\\0' |", e_lfanew[2], e_lfanew[2]);
    printf("\n  063: %03d | 3F: %02X | '\\0' |", e_lfanew[3], e_lfanew[3]);
    printf("\n -------------------------------------------------------------");
    printf("\n БЛОК 2: PE ЗАГОЛОВОК (COFF File Header)");
    printf("\n -------------------------------------------------------------");
    if (fread(pe_signature, 1, 4, descriptor) != 4) { /**/printf("\n  pe_signature != 4");/**/ return; } // pe_signature | 060: ??? ??? 000 000 | 40: ?? ?? 00 00
    //if (fread(pe_signature, 4, 1, descriptor) != 1)
    printf("\n  064: %03d | 40: %02X |  '%c' | uint32_t pe_signature = \"%s\\0\\0\"", pe_signature[0], pe_signature[0], pe_signature[0], pe_signature);
    printf("\n  065: %03d | 41: %02X |  '%c' |", pe_signature[1], pe_signature[1], pe_signature[1]);
    printf("\n  066: %03d | 42: %02X | '\\0' |", pe_signature[2], pe_signature[2]);
    printf("\n  067: %03d | 43: %02X | '\\0' |", pe_signature[3], pe_signature[3]);
    printf("\n -------------------------------------------------------------");
    if (fread(Machine, 1, 2, descriptor) != 2) { /**/printf("\n  Machine != 2");/**/ return; } // Machine | 060: ??? ??? 000 000 | 40: ?? ?? 00 00
    //if (fread(Machine, 2, 1, descriptor) != 1)
    printf("\n  068: %03d | 44: %02X |  '%c' | uint16_t Machine = \\x%X%X", Machine[0], Machine[0], Machine[0], Machine[1], Machine[0]);
    printf("\n  069: %03d | 45: %02X |  '%c' |",  Machine[1], Machine[1], Machine[1]);
    printf("\n -------------------------------------------------------------");
    fclose(descriptor);
}

int main()
{
    setlocale(0, "");

    pe_builder();
    pe_analyzer();

    putchar('\n');
    return 0;
}