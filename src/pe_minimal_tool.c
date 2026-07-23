#include <stdio.h>
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
    // === БЛОК: IMAGE_FILE_HEADER ===
    fprintf(file_descriptor, "%c%c", 0x64, 0x86); // 1. Поле Machine = 0x8664 (AMD64). В LE: сначала младший 0x64 (100), затем старший 0x86 (134)
    fprintf(file_descriptor, "%c%c", 1, 0);       // 2. Поле NumberOfSections = 1. В LE: сначала 1, затем 0
    fclose(file_descriptor);
}
void pe_minimal_analyzer(const char * file_name, uint8_t stream)
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
     (file[60]) << 24 | (file[61] << 16) | (file[62] <<  8) | (file[63])  // Big-endian (склеиваем байты слева направо)
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
     (file[lfanew]) << 24 | (file[lfanew+1] << 16) | (file[lfanew+2] <<  8) | (file[lfanew+3])  // Big-endian (склеиваем байты слева направо)
    );
    printf("\n %08llu: %03d | %02X | %c", lfanew  , file[lfanew  ], file[lfanew  ], charf(file[lfanew  ]));
    printf("\n %08llu: %03d | %02X | %c", lfanew+1, file[lfanew+1], file[lfanew+1], charf(file[lfanew+1]));
    printf("\n %08llu: %03d | %02X | %c", lfanew+2, file[lfanew+2], file[lfanew+2], charf(file[lfanew+2]));
    printf("\n %08llu: %03d | %02X | %c", lfanew+3, file[lfanew+3], file[lfanew+3], charf(file[lfanew+3]));
    printf("\n --");
    uint16_t                        machine = (file[lfanew+4])      | (file[lfanew+5] << 8);
    printf("\n machine = %u :: %u", machine,  (file[lfanew+4]) << 8 | (file[lfanew+5]    ));
    printf("\n %08llu: %03d | %02X | %c", lfanew+4, file[lfanew+4], file[lfanew+4], charf(file[lfanew+4]));
    printf("\n %08llu: %03d | %02X | %c", lfanew+5, file[lfanew+5], file[lfanew+5], charf(file[lfanew+5]));
    printf("\n --");
}
//#include <locale.h>
#include <string.h>
#include <windows.h>
// Потоковый стрим?: Нет.
// pe_minimal_tool.exe
// pe_minimal_tool.exe > pe_minimal.dmp
int main(/*int argc, char * argv[]*/)
{
    //setlocale(0, "");
    SetConsoleCP(1251);       // Кодировка ввода
    SetConsoleOutputCP(1251); // Кодировка вывода
    //pe_minimal_builder("__.exe");
    //pe_minimal_analyzer("__.exe");
    char buffer[128]; char buffer_2[64];
    printf("\n Введите путь к файлу, который необходимо проанализировать!\n>>> ");
    fgets(buffer, sizeof (buffer), stdin); // Считывает строку вместе с пробелами (максимум 99 символов + '\0')
    buffer[strcspn(buffer, "\n")] = '\0'; // fgets сохраняет символ переноса строки '\n' в конце, удаляем его, если он мешает
    __start:
    printf("\n Куда бы вы хотели получить результат:\n В консоль\n В файл\n Оба варианта\n>>> ");
    fgets(buffer_2, sizeof (buffer_2), stdin); // Считывает строку вместе с пробелами (максимум 99 символов + '\0')
    buffer_2[strcspn(buffer_2, "\n")] = '\0';
    //printf("```\n%s\n```", buffer_2);
    if (!strcmp(buffer_2, "В консоль"))
    {
        printf("\n Анализ начат.");
        pe_minimal_analyzer(buffer);
        printf("\n Анализ окончен.\n");
        system("pause");
    }
    else if (!strcmp(buffer_2, "В файл"))
    {
        FILE * file_descriptor = fopen("__.dmp", "wb");
        if (!file_descriptor) return 0;
        fprintf(file_descriptor, " Анализ начат.");
        pe_minimal_analyzer(buffer);
        fprintf(file_descriptor, "\n Анализ окончен.");
        system("pause");
    }
    else
    {
        printf("\n /!\\ Временно недоступно, выберите другое...\n");
        goto __start;
    }
    //printf("\n argc = %d", argc);
    //for (int i = 0; i < argc; i++) printf("\n argv[%d] = %d", i+1, argv[i]);
    return 0;
}