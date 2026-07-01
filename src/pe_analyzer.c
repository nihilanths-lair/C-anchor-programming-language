#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>

// Функция превращает байт в печатный символ, а непечатные заменяет точкой
char to_ascii(uint8_t b) { return (b >= 32 && b <= 126) ? (char)b : '·'; }

int main()
{
    setlocale(0, "");
    FILE * f = fopen("test_compiled.exe", "rb");
    if (!f)
    {
        printf("\n [Ошибка] Не удалось прочитать файл test_compiled.exe!\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t * exe = (uint8_t *) malloc(file_size);
    fread(exe, 1, file_size, f);
    fclose(f);

    printf("=============================================================================================================\n");
    printf(" Смещ.(DEC) | Сырые байты (DEC)   | Смещ.(HEX) | Сырые байты (HEX) | ASCII | Описание поля\n");
    printf("-------------------------------------------------------------------------------------------------------------\n");

    // БЛОК 1: DOS ЗАГОЛОВОК
    printf("    000     | %03d %03d             |     00     | %02X %02X             |  %c%c   | MZ Signature\n", 
           exe[0], exe[1], exe[0], exe[1], to_ascii(exe[0]), to_ascii(exe[1]));

    printf(" 002...059  | 000...000           |   02...3B  | 00...00           | ..... | Reserved (DOS Stub)\n");

    uint32_t pe_offset = exe[60] | (exe[61] << 8) | (exe[62] << 16) | (exe[63] << 24);
    printf("    060     | %03d %03d %03d %03d     |     3C     | %02X %02X %02X %02X     |  .... | PE Header Offset (e_lfanew)\n", 
           exe[60], exe[61], exe[62], exe[63], exe[60], exe[61], exe[62], exe[63]);

    // БЛОК 2: PE HEADER (COFF)
    int p = pe_offset;
    printf("    %03d     | %03d %03d %03d %03d     |     %02X     | %02X %02X %02X %02X     |  %c%c.. | PE Signature\n", 
           p, exe[p], exe[p+1], exe[p+2], exe[p+3], p, exe[p], exe[p+1], exe[p+2], exe[p+3], to_ascii(exe[p]), to_ascii(exe[p+1]));

    uint16_t sections = exe[p+6] | (exe[p+7] << 8);
    uint16_t opt_size = exe[p+20] | (exe[p+21] << 8);

    // БЛОК 3: OPTIONAL HEADER
    int o = p + 24;
    printf("    %03d     | %03d %03d             |     %02X     | %02X %02X             |  ..   | PE32+ Magic\n", 
           o, exe[o], exe[o+1], o, exe[o], exe[o+1]);

    // БЛОК 4: ТАБЛИЦА СЕКЦИЙ
    int s = o + opt_size;
    printf("-------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < sections; i++) {
        int sec_curr = s + (i * 40);
        printf("    %03d     | ", sec_curr);
        for(int b=0; b<8; b++) printf("%03d ", exe[sec_curr + b]);
        printf("|     %02X     | ", sec_curr);
        for(int b=0; b<8; b++) printf("%02X ", exe[sec_curr + b]);
        printf("| ");
        for(int b=0; b<8; b++) printf("%c", to_ascii(exe[sec_curr + b]));
        printf(" | Section %d Name\n", i + 1);
    }
    printf("=============================================================================================================");

    free(exe);
    return 0;
}