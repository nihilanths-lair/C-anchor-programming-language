#include <stdint.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

// БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)
//  Размер: Всегда строго 64 байта.
uint8_t e_magic[2];         // 000~001 | 00~01 | Магическое число
//  (size: 56) // 002~058 | 02~3A | Зарезервировано: Обычно забито нулями (0). Сюда можно спрятать кастомные метаданные компилятора, Windows их игнорирует.
uint8_t e_lfanew[4];        // 059~??? | ??~?? | Динамическое поле: Указывает смещение (в байтах от начала файла), где начнется Блок 2 (PE). • Минимум: 64 (если DOS-код заглушки отсутствует).• Динамика: Если ты решишь вставить туда реальную DOS-программу (которая пишет "This program cannot be run in DOS mode"), это поле сдвинется вперед на размер этого DOS-кода (обычно 128 или 248).

uint8_t program[2048];

//uint64_t offset = 0;

void pe_builder()
{
    FILE * descriptor = fopen("test_subject.exe", "wb");
    fwrite("MZ", 1, 2, descriptor);     // e_magic  | 000: 077 090 | 00: 4D 5A | MZ
    fwrite("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, 56, descriptor); // 002~058: 000 | 02~3A: 00
    //fwrite(e_lfanew, 1, 4, descriptor); // e_lfanew | 059~???: ??? | ??~??: ??
    fclose(descriptor);
}

void pe_analyzer()
{
    FILE * descriptor = fopen("test_subject.exe", "rb");
    if (!descriptor) return;
    printf("\n БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)");
    if (fread(e_magic, 1, 2, descriptor) != 2) { printf("\n  e_magic != size: 2");/**/ return; } // e_magic | 000: 077 090 | 00: 4D 5A | MZ
    if (strcmp(e_magic, "MZ")) { /**/printf("\n  e_magic != MZ");/**/ return; }
    printf("\n -------------------------------------------");
    printf("\n  000: %03d | 00: %02X | %c | e_magic = 'M'", e_magic[0], e_magic[0], e_magic[0]);
    printf("\n  001: %03d | 01: %02X | %c |         = 'Z'", e_magic[1], e_magic[1], e_magic[1]);
    printf("\n -------------------------------------------");
    if (fread(program, 1, 56, descriptor) != 56) { /**/printf("\n  merged_fields != size: 56");/**/ return; }  // 002: ???     | 02: ??
    printf("\n  002: %03d | 02: %02X | · | e_cblp = '\\0'", program[0], program[0]);
    printf("\n  003: %03d | 03: %02X | · |        = '\\0'", program[1], program[1]);
    printf("\n -------------------------------------------");
    printf("\n  004: %03d | 04: %02X | · | e_cp = '\\0'", program[2], program[2]);
    printf("\n  005: %03d | 05: %02X | · |      = '\\0'", program[3], program[3]);
    printf("\n -------------------------------------------");
    printf("\n  006: %03d | 06: %02X | · | e_crlc = '\\0'", program[4], program[4]);
    printf("\n  007: %03d | 07: %02X | · |        = '\\0'", program[5], program[5]);
    printf("\n -------------------------------------------");
    printf("\n  008: %03d | 08: %02X | · | e_cparhdr = '\\0'", program[6], program[6]);
    printf("\n  009: %03d | 09: %02X | · |           = '\\0'", program[7], program[7]);
    printf("\n -------------------------------------------");
    printf("\n  010: %03d | 0A: %02X | · | e_minalloc = '\\0'", program[8], program[8]);
    printf("\n  011: %03d | 0B: %02X | · |            = '\\0'", program[9], program[9]);
    printf("\n -------------------------------------------");
    printf("\n  012: %03d | 0C: %02X | · | e_maxalloc = '\\0'", program[10], program[10]);
    printf("\n  013: %03d | 0D: %02X | · |            = '\\0'", program[11], program[11]);
    printf("\n -------------------------------------------");
    printf("\n  014: %03d | 0E: %02X | · | e_ss = '\\0'", program[12], program[12]);
    printf("\n  015: %03d | 0F: %02X | · |      = '\\0'", program[13], program[13]);
    printf("\n -------------------------------------------");
    printf("\n  016: %03d | 10: %02X | · | e_sp = '\\0'", program[14], program[14]);
    printf("\n  017: %03d | 11: %02X | · |      = '\\0'", program[15], program[15]);
    printf("\n -------------------------------------------");
    for (unsigned char offset = 18; offset <= 40; offset++) printf("\n  %03d: %03d | %02X: %02X | · |", offset, program[offset-2], offset, program[offset-2]);
    for (unsigned char offset = 41; offset <= 56; offset++) printf("\n  %03d: %03d | %02X: %02X | $ |", offset, program[offset-2], offset, program[offset-2]);
    printf("\n -------------------------------------------");
    if (fread(e_lfanew, 1, 4, descriptor) != 4) { /**/printf("\n  e_lfanew != 4");/**/ return; } // e_lfanew | 059: ??? ??? ??? ??? | ??: ?? ?? ?? ??
    printf("\n  %03d: %03d | %02X: %02X | · | e_lfanew = ?", 61, e_lfanew[0], 61, e_lfanew[0]);
    printf("\n  %03d: %03d | %02X: %02X | · |", 62, e_lfanew[1], 62, e_lfanew[1]);
    printf("\n  %03d: %03d | %02X: %02X | · |", 63, e_lfanew[2], 63, e_lfanew[2]);
    printf("\n  %03d: %03d | %02X: %02X | · |", 64, e_lfanew[3], 64, e_lfanew[3]);
    printf("\n -------------------------------------------");
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