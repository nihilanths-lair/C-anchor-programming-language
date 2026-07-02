#include <stdint.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

// БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)
//  Размер: Всегда строго 64 байта.
uint8_t e_magic[2];      // 000~001 | 00~01 | Магическое число
uint8_t e_res[56] = {0}; // 002~058 | 02~3A | Зарезервировано: Обычно забито нулями (0). Сюда можно спрятать кастомные метаданные компилятора, Windows их игнорирует.
uint8_t e_lfanew[4];     //

void pe_builder()
{
    FILE * descriptor = fopen("test_subject.exe", "wb");
    fwrite("MZ", 1, 2, descriptor);   // e_magic | 000: 077 090 | 00: 4D 5A | MZ
    fwrite(e_res, 1, 56, descriptor); // e_res   | 002~058: 000 | 02~3A: 00
    fclose(descriptor);
}

void pe_analyzer()
{
    FILE * descriptor = fopen("test_subject.exe", "rb");
    if (!descriptor) return;
    printf("\n БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)");
    fread(e_magic, 1, 2, descriptor); // e_magic | 000: 077 090 | 00: 4D 5A | MZ
    if (strcmp(e_magic, "MZ")) { /**/printf("\n e_magic != MZ");/**/ return; }
    printf("\n ---------------------------------");
    printf("\n  e_magic | 000: %03d | 00: %02X | %c", e_magic[0], e_magic[0], e_magic[0]);
    printf("\n          | 001: %03d | 01: %02X | %c", e_magic[1], e_magic[1], e_magic[1]);
    printf("\n ---------------------------------");
    if (fread(e_res, 1, 56, descriptor) != 56) { /**/printf("\n  e_res != 56");/**/ return; }  // e_res   | 002: ???     | 02: ??
    printf("\n  e_res   | %03d: %03d | %02X: %02X | ·", 2, e_res[0], 2, e_res[0]);
    for (unsigned char offset = 1; offset < 56; offset++) printf("\n          | %03d: %03d | %02X: %02X | ·", offset+2, e_res[offset], offset+2, e_res[offset]);
    printf("\n ---------------------------------");
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