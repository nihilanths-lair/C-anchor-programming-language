#include <stdint.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

// [#] БЛОК 1: DOS ЗАГОЛОВОК (DOS Header) [#]
uint8_t e_magic[2]; // 000~001 | 00~01 | Магическое число

void pe_builder()
{
    FILE * descriptor = fopen("test_subject.exe", "wb");
    fwrite("MZ", 1, 2, descriptor); // e_magic | 000: 077 090 | 00: 4D 5A | MZ
    fclose(descriptor);
}

void pe_analyzer()
{
    FILE * descriptor = fopen("test_subject.exe", "rb");
    if (!descriptor) return;
    fread(e_magic, 1, 2, descriptor); // e_magic | 000: 077 090 | 00: 4D 5A | MZ
    if (strcmp(e_magic, "MZ")) { printf("\n e_magic != MZ"); return; }
    fclose(descriptor);

    printf("\n [#] БЛОК 1: DOS ЗАГОЛОВОК (DOS Header) [#]");
    printf("\n  e_magic | %02X: %02X %02X | %03d: %03d %03d | %c%c",
     0, e_magic[0], e_magic[1], 0, e_magic[0], e_magic[1], e_magic[0], e_magic[1]
    );
}

int main()
{
    setlocale(0, "");

    pe_builder();
    pe_analyzer();

    putchar('\n');
    return 0;
}