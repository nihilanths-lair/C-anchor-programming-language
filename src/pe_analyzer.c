#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>

// Функция для безопасного чтения 16-битного числа из массива байт (Little-Endian)
uint16_t read16(uint8_t * buffer, int offset)
{
    return (uint16_t)(buffer[offset] | (buffer[offset + 1] << 8));
}

// Функция для безопасного чтения 32-битного числа из массива байт (Little-Endian)
uint32_t read32(uint8_t * buffer, int offset)
{
    return (uint32_t)(buffer[offset] | 
                     (buffer[offset + 1] << 8) | 
                     (buffer[offset + 2] << 16) | 
                     (buffer[offset + 3] << 24));
}

int main()
{
    setlocale(0, "");
    FILE * f = fopen("test_compiled.exe", "rb");
    if (!f)
    {
        printf("\n [Ошибка] Не удалось прочитать файл test_compiled.exe!");
        printf("\n Сначала скомпилируй и запусти pe_builder.c\n");
        return 1;
    }

    // Узнаем реальный размер файла на диске
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Выделяем массив под байты файла
    uint8_t * buffer = (uint8_t *) malloc(file_size);
    fread(buffer, 1, file_size, f);
    fclose(f);

    printf("====================================================================");
    printf("\n      АНАЛИЗАТОР И HEX/DEC-ДАМП WINDOWS PE (РАЗМЕР: %ld байт)        ", file_size);
    printf("\n====================================================================");
    printf("\n  Offset(h|d)");
    // ====================================================================
    // 1. ДЕКОДИРОВАНИЕ DOS-ЗАГОЛОВКА
    // ====================================================================
    printf("\n---------------------");
    printf("\n << DOS ЗАГОЛОВОК >>");
    printf("\n---------------------");
    // Читаем сигнатуру MZ (смещение 0, размер 2 байта)
    uint16_t mz_magic = read16(buffer, 0);
    printf("\n           [00|000]:       %02X %02X | %03d %03d         ; Сигнатура MZ: 0x%04X (Символы: %c%c)",
        buffer[0], buffer[1],
        buffer[0], buffer[1],
        mz_magic, buffer[0], buffer[1]
    );
    //printf("\n               ----");
    // Временная заглушка - выводим всё вплоть до смещения 60
    printf("\n  [02|002]~[3B|059]:          00 | 000             ; Заглушка");
    // Читаем e_lfanew (смещение 60, размер 4 байта)
    uint32_t pe_offset = read32(buffer, 60);
    //printf("\n               ----------");
    printf("\n           [3C|060]: %02X %02X %02X %02X | %03d %03d %03d %03d ; Смещение до PE-заголовка (%u байт): 0x%08X",
        buffer[60], buffer[61], buffer[62], buffer[63],
        buffer[60], buffer[61], buffer[62], buffer[63],
        pe_offset, pe_offset
    );
    // ====================================================================
    // 2. ДЕКОДИРОВАНИЕ PE SIGNATURE & COFF HEADER
    // ====================================================================
    // Проверяем, хватает ли размера файла, чтобы прочитать PE-заголовок
    if (file_size < pe_offset + 24)
    {
        printf("[-] Файл слишком мал для анализа COFF заголовка. Допиши билдер!\n");
        free(buffer);
        return 0;
    }
    printf("\n-------------------------------------");
    printf("\n << PE СИГНАТУРА И COFF-ЗАГОЛОВОК >>");
    printf("\n-------------------------------------");
    // Сигнатура PE (4 байта)
    uint32_t pe_magic = read32(buffer, pe_offset);
    printf("\n  [%02X|%03d]: %02X %02X %02X %02X | %03d %03d %03d %03d ; Сигнатура PE: 0x%08X (Символы: %c%c\\0\\0)",
     pe_offset, pe_offset, // смещение
     buffer[pe_offset], buffer[pe_offset+1], buffer[pe_offset+2], buffer[pe_offset+3], // hex
     buffer[pe_offset], buffer[pe_offset+1], buffer[pe_offset+2], buffer[pe_offset+3], // dec
     pe_magic, buffer[pe_offset], buffer[pe_offset+1]
    );
    // Архитектура процессора (2 байта, смещение pe_offset + 4)
    uint16_t machine = read16(buffer, pe_offset+4);
    printf("\n  [%02X|%03d]:       %02X %02X | %03d %03d         ; Архитектура ЦП: 0x%04X ",
     pe_offset+4, pe_offset+4, // смещение
     buffer[pe_offset+4], buffer[pe_offset+5], // hex
     buffer[pe_offset+4], buffer[pe_offset+5], // dec
     machine
    );
    if (machine == 0x8664) printf("(x86-64/AMD64 — Нативный 64-битный режим)");
    else printf("(Неизвестная архитектура)");
    // Количество секций (2 байта, смещение pe_offset + 6)
    uint16_t sections = read16(buffer, pe_offset+6);
    printf("\n  [%02X|%03d]:       %02X %02X | %03d %03d         ; Кол-во секций: %u",
     pe_offset+6, pe_offset+6, // смещение
     buffer[pe_offset+6], buffer[pe_offset+7], // hex
     buffer[pe_offset+6], buffer[pe_offset+7], // dec
     sections
    );
    // Таймштамп (4 байта, смещение pe_offset + 8)
    uint32_t timestamp = read32(buffer, pe_offset+8);
    printf("\n  [%02X|%03d]: %02X %02X %02X %02X | %03d %03d %03d %03d ; Временная метка: 0x%08X",
     pe_offset+8, pe_offset+8,
     buffer[pe_offset+8], buffer[pe_offset+9], buffer[pe_offset+10], buffer[pe_offset+11],
     buffer[pe_offset+8], buffer[pe_offset+9], buffer[pe_offset+10], buffer[pe_offset+11],
     timestamp
    );
    // Размер Optional Header (2 байта, смещение pe_offset + 20)
    uint16_t opt_hdr_size = read16(buffer, pe_offset+20);
    printf("\n  [%02X|%03d]:       %02X %02X | %03d %03d         ; Размер OptionalHdr: %u байт (0x%04X)",
     pe_offset+20, pe_offset+20,
     buffer[pe_offset+20], buffer[pe_offset+21],
     buffer[pe_offset+20], buffer[pe_offset+21],
     opt_hdr_size, opt_hdr_size
    );
    // Характеристики файла (2 байта, смещение pe_offset + 22)
    uint16_t chars = read16(buffer, pe_offset + 22);
    printf("\n  [%02X|%03d]:       %02X %02X | %03d %03d         ; Характеристики: 0x%04X ",
     pe_offset+22, pe_offset+22,
     buffer[pe_offset+22], buffer[pe_offset+23],
     buffer[pe_offset+22], buffer[pe_offset+23],
     chars
    );
    if (chars & 0x0002) printf("(Исполняемый EXE-файл)");
    else printf("(Флаг EXE отсутствует)");
    printf("\n====================================================================");
    free(buffer);
    return 0;
}