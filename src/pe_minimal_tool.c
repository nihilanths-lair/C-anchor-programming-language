#include <stdio.h>
#include <locale.h>
//#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Заполнитель
void file_aggregate(FILE * file_descriptor, const char ascii, int quantity)
{
    while (--quantity >= 0) putc(ascii, file_descriptor);
}
void print_aggregate(const char ascii, int quantity) { while (--quantity >= 0) putchar(ascii); }

char charf(char ascii)
{
    switch (ascii){
    case '\0': ascii = '.';
    }
    return ascii;
}

#define macro__putcharf \
 switch (pe_file[file_offset]){ \
 case '\0': '.'; \
 default: putchar(pe_file[file_offset]); \
 }

// Замер точного физического размера файла на диске
#define macro__file_size \
 fseek(file_descriptor, 0, SEEK_END); \
 long file_size = ftell(file_descriptor); \
 fseek(file_descriptor, 0, SEEK_SET);

void pe_minimal_builder(const char * file_name)
{
    FILE * file_descriptor = fopen(file_name, "wb");
    fprintf(file_descriptor, "MZ"); // 2 байта (0-1)
    // По спецификации: 60 байт до поля lfanew. 60 - 2 = 58 байт нулей.
    file_aggregate(file_descriptor, '\0', 58); // 58 байт (2-59)
    // Записываем lfanew = 64 (0x00000040) в Little-Endian на байты 60, 61, 62, 63
    fprintf( file_descriptor, "%c%c%c%c", 64, 0, 0, 0);
    fclose(file_descriptor);
}
void pe_minimal_analyzer(const char * file_name)
{
    FILE * file_descriptor;

    // 1. Открываем файл в бинарном режиме
    file_descriptor = fopen(file_name, "rb");
    if (!file_descriptor) { printf("\n /!\\: Файл %s не был открыт", file_name); return; }

    // 2. Измеряем точный физический размер файла на диске
    fseek(file_descriptor, 0, SEEK_END);
    long file_size = ftell(file_descriptor);
    fseek(file_descriptor, 0, SEEK_SET);

    if (!file_size) { printf("\n /!\\: Размер файла %s не определён (пуст)", file_name); fclose(file_descriptor); return; }
    
    // 3. Выделяем память под весь файл
    // Выделяем беззнаковую память (uint8_t вместо char)
    uint8_t * file = (uint8_t *) malloc(file_size);
    if (!file) { printf("\n /!\\: Недостаточно памяти под буфер файла %s", file_name); fclose(file_descriptor); return; }

    // 4. Считываем весь файл в память одним монолитным блоком и закрываем дескриптор
    long bytes_read = fread(file, 1, file_size, file_descriptor); fclose(file_descriptor);

    if (bytes_read != file_size) { printf("\n /!\\: Файл %s не был прочитан полностью", file_name); free(file); return; }

    printf(" --");
    printf("\n magic");
    printf("\n %08llu: %03d | %02X | %c", 0, file[0], file[0], charf(file[0]));
    printf("\n %08llu: %03d | %02X | %c", 1, file[1], file[1], charf(file[1]));
    printf("\n --");
    for (long offset = 2; offset <= 63; offset++) printf("\n %08llu: %03d | %02X | %c", offset, file[offset], file[offset], charf(file[offset]));
    printf("\n --");
    // Читаем lfanew из ПРАВИЛЬНЫХ ячеек (60, 61, 62, 63)
    printf("\n lfanew = %u", // Little-endian
     ((uint32_t) file[60]) | ((uint32_t) file[61] << 8) | ((uint32_t) file[62] << 16) | ((uint32_t) file[63] << 24)
    );
    printf("\n %08llu: %03d | %02X | %c", 60, file[60], file[60], charf(file[60]));
    printf("\n %08llu: %03d | %02X | %c", 61, file[61], file[61], charf(file[61]));
    printf("\n %08llu: %03d | %02X | %c", 62, file[62], file[62], charf(file[62]));
    printf("\n %08llu: %03d | %02X | %c", 63, file[63], file[63], charf(file[63]));
    printf("\n --");
}

// Потоковый стрим?: Нет.
// pe_minimal_tool.exe
// pe_minimal_tool.exe > pe_minimal.dmp
int main()
{
    setlocale(0, "");
    pe_minimal_builder("__.exe");
    pe_minimal_analyzer("__.exe");
    return 0;
}