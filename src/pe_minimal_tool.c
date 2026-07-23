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

 // Для 4 символов (например, 'P', 'E', '\0', '\0') -> преобразует в uint32_t в Little-Endian
// (file[60]) | (file[61] << 8) | (file[62] << 16) | (file[63] << 24)
#define macro__splicing_be32(_1, _2, _3, _4) ((_4) << 24) | ((_3) << 16) | ((_2) << 8) | (_1)
#define macro__splicing_le32(_1, _2, _3, _4) (_1) | ((_2) << 8) | ((_3) << 16) | ((_4) << 24)

//#define macro__left_to_right_byte_order()
//#define macro__right_to_left_byte_order()

//#define _using_left_to_right_notation()
//#define _using_right_to_left_notation()
//#define macro__converting_bytes_to_number_(_0, _1, _2, _3, _4) /* преобразование байтов в число */

// macro__converting_number_to_bytes() // преобразование числа в байты

// Замер точного физического размера файла на диске
#define macro__file_size \
 fseek(file_descriptor, 0, SEEK_END); \
 long file_size = ftell(file_descriptor); \
 fseek(file_descriptor, 0, SEEK_SET);

void pe_minimal_builder(const char * file_name)
{
    FILE * file_descriptor = fopen(file_name, "wb");
    if (!file_descriptor) return;
    fprintf(file_descriptor, "MZ"); // 2 байта (0-1)
    file_aggregate(file_descriptor, '\0', 58); // 58 байт (2-59)
    fprintf(file_descriptor, "%c%c%c%c", 64, 0, 0, 0); // Записываем lfanew = 64 строго как 4 отдельных байта (60-63)
    fprintf(file_descriptor, "PE%c%c", 0, 0); // Записываем сигнатуру PE\0\0 строго как 4 отдельных байта (64-67)
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
    printf("\n magic = %u :: %u", // Little-endian :: Big-endian
     (file[0])      | (file[1] <<  8), // Little-endian (склеиваем байты справа налево, реверсируем)
     (file[0]) << 8 | (file[1]      )  // Big-endian (склеиваем байты слева направо)
    );
    printf("\n %08llu: %03d | %02X | %c", 0, file[0], file[0], charf(file[0]));
    printf("\n %08llu: %03d | %02X | %c", 1, file[1], file[1], charf(file[1]));
    printf("\n --");
    for (long offset = 2; offset <= 59; offset++) printf("\n %08llu: %03d | %02X | %c", offset, file[offset], file[offset], charf(file[offset]));
    printf("\n --");
    // Читаем lfanew из ПРАВИЛЬНЫХ ячеек (60, 61, 62, 63)
    uint32_t lfanew = (file[60]) | (file[61] << 8) | (file[62] << 16) | (file[63] << 24); // Little-endian (склеиваем байты справа налево, реверсируем)
    printf("\n lfanew = %u :: %u", lfanew,
     //(file[60])       | (file[61] <<  8) | (file[62] << 16) | (file[63] << 24), // Little-endian (склеиваем байты справа налево, реверсируем)
     (file[60]) << 24 | (file[61] << 16) | (file[62] <<  8) | (file[63]      )  // Big-endian (склеиваем байты слева направо)
    );
    printf("\n %08llu: %03d | %02X | %c", 60, file[60], file[60], charf(file[60]));
    printf("\n %08llu: %03d | %02X | %c", 61, file[61], file[61], charf(file[61]));
    printf("\n %08llu: %03d | %02X | %c", 62, file[62], file[62], charf(file[62]));
    printf("\n %08llu: %03d | %02X | %c", 63, file[63], file[63], charf(file[63]));
    printf("\n --");
    // --- ЧИТАЕМ СИГНАТУРУ NT_HEADER (Начиная со смещения lfanew) ---
    // Вычисляем смещения для 4 байт сигнатуры
    uint32_t signature = (file[lfanew]) | (file[lfanew+1] << 8) | (file[lfanew+2] << 16) | (file[lfanew+3] << 24); // Little-endian (склеиваем байты справа налево, реверсируем)
    printf("\n signature = %u :: %u", signature,
     //(file[lfanew])       | (file[lfanew+1] <<  8) | (file[lfanew+2] << 16) | (file[lfanew+3] << 24), // Little-endian (склеиваем байты справа налево, реверсируем)
     (file[lfanew]) << 24 | (file[lfanew+1] << 16) | (file[lfanew+2] <<  8) | (file[lfanew+3]      )  // Big-endian (склеиваем байты слева направо)
    );
    printf("\n %08llu: %03d | %02X | %c", lfanew  , file[lfanew  ], file[lfanew  ], charf(file[lfanew  ]));
    printf("\n %08llu: %03d | %02X | %c", lfanew+1, file[lfanew+1], file[lfanew+1], charf(file[lfanew+1]));
    printf("\n %08llu: %03d | %02X | %c", lfanew+2, file[lfanew+2], file[lfanew+2], charf(file[lfanew+2]));
    printf("\n %08llu: %03d | %02X | %c", lfanew+3, file[lfanew+3], file[lfanew+3], charf(file[lfanew+3]));
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