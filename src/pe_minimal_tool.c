#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Заполнитель
void file_aggregate(FILE * file_descriptor, const char ascii, int quantity) { while (--quantity >= 0) putc(ascii, file_descriptor); }
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

void pe_minimal_builder(const char * file_name)
{
    FILE * file_descriptor = fopen(file_name, "wb");
    if (!file_descriptor) return;
    fprintf(file_descriptor, "MZ");                                  // magic = MZ (2 байта)
    file_aggregate(file_descriptor, '\0', 58);                       // 58 байт (2-59)
    fwrite(&(uint32_t){64}, sizeof (uint32_t), 1, file_descriptor);  // lfanew = 64 (4 байта) ; влияет на последующее смещение в файле
    fprintf(file_descriptor, "PE%c%c"  ,  0, 0);                     // signature = PE\0\0 (4 байта)
    // === БЛОК: IMAGE_FILE_HEADER ===
    fprintf(file_descriptor, "%c%c"    , 0x64, 0x86);                // 1. Machine = 0x8664 (2 байта) ; AMD64
    fwrite(&(uint16_t){1}, sizeof (uint16_t), 1, file_descriptor);   // 2. NumberOfSections     = 1 (2 байта)
    fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor);   // 3. TimeDateStamp        = 0 (4 байта)
    fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor);   // 4. PointerToSymbolTable = 0 (4 байта)
    fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor);   // 5. NumberOfSymbols      = 0 (4 байта)
    fwrite(&(uint16_t){240}, sizeof (uint16_t), 1, file_descriptor); // 6. SizeOfOptionalHeader = 0x00F0 (2 байта)
    fprintf(file_descriptor, "%c%c", 0x22, 0x00);                    // 7. Characteristics = 0x0022 (EXECUTABLE_IMAGE | LARGE_ADDRESS_AWARE) (2 байта)
    // === БЛОК: IMAGE_OPTIONAL_HEADER64 (Стандартные поля) ===
    // Начинается со смещения 88 (если lfanew = 64)
    fwrite(&(uint16_t){0x020B}, sizeof (uint16_t), 1, file_descriptor); // 1. Magic = PE32+ (64-битный файл)
    fputc(1, file_descriptor);                                          // 2.1 MajorLinkerVersion
    fputc(0, file_descriptor);                                          // 2.2 MinorLinkerVersion
    fwrite(&(uint32_t) {512}, sizeof (uint32_t), 1, file_descriptor);   // 3. SizeOfCode (4 байта) ; Выровнен по FileAlignment
    fwrite(&(uint32_t)   {0}, sizeof (uint32_t), 1, file_descriptor);   // 4. SizeOfInitializedData (4 байта)
    fwrite(&(uint32_t)   {0}, sizeof (uint32_t), 1, file_descriptor);   // 5. SizeOfUninitializedData (4 байта)
    fwrite(&(uint32_t){4096}, sizeof (uint32_t), 1, file_descriptor);   // 6. AddressOfEntryPoint — укажем RVA = 4096 (0x1000). Это стандартное начало первой секции в памяти
    fclose(file_descriptor);
}
void pe_minimal_analyzer(const char * file_name, FILE * stream)
{
    FILE * file_descriptor;
    file_descriptor = fopen(file_name, "rb"); // Открываем файл в бинарном режиме
    if (!file_descriptor) { printf("\n /!\\: Файл %s не был открыт", file_name); return; }
    fseek(file_descriptor, 0, SEEK_END);
    long file_size = ftell(file_descriptor); // Измеряем точный физический размер файла на диске
    fseek(file_descriptor, 0, SEEK_SET);
    if (!file_size) { printf("\n /!\\: Размер файла %s не определён (пуст)", file_name); fclose(file_descriptor); return; }
    uint8_t * file = (uint8_t *) malloc(file_size); // Выделяем беззнаковую память под весь файл
    if (!file) { printf("\n /!\\: Недостаточно памяти под буфер файла %s", file_name); fclose(file_descriptor); return; }
    long bytes_read = fread(file, 1, file_size, file_descriptor); fclose(file_descriptor); // Считываем весь файл в память одним монолитным блоком и закрываем дескриптор
    if (bytes_read != file_size) { printf("\n /!\\: Файл %s не был прочитан полностью", file_name); free(file); return; }
    //printf(" Анализ начат.");
    fprintf(stream, " --");
    fprintf(stream, "\n magic = %u :: %u",
     (file[0])      | (file[1] <<  8),
     (file[0]) << 8 | (file[1]      )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 0, file[0], file[0], charf(file[0]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 1, file[1], file[1], charf(file[1]));
    fprintf(stream, "\n --");
    for (long offset = 2; offset <= 59; offset++) fprintf(stream, "\n %08llu: %03d | %02X | %c", offset, file[offset], file[offset], charf(file[offset]));
    fprintf(stream, "\n --");
    // Читаем lfanew из ПРАВИЛЬНЫХ ячеек (60, 61, 62, 63)
    uint32_t lfanew = (file[60]) | (file[61] << 8) | (file[62] << 16) | (file[63] << 24);
    fprintf(stream, "\n lfanew = %u :: %u", lfanew, // (4 байта)
     (file[60]) << 24 | (file[61] << 16) | (file[62] <<  8) | (file[63])
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 60, file[60], file[60], charf(file[60]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 61, file[61], file[61], charf(file[61]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 62, file[62], file[62], charf(file[62]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 63, file[63], file[63], charf(file[63]));
    fprintf(stream, "\n --");
    // --- ЧИТАЕМ СИГНАТУРУ NT_HEADER (Начиная со смещения lfanew) ---
    // Вычисляем смещения для 4 байт сигнатуры
    fprintf(stream, "\n signature = %u :: %u", // (4 байта)
     (file[lfanew])       | (file[lfanew+1] <<  8) | (file[lfanew+2] << 16) | (file[lfanew+3] << 24),
     (file[lfanew]) << 24 | (file[lfanew+1] << 16) | (file[lfanew+2] <<  8) | (file[lfanew+3])
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew  , file[lfanew  ], file[lfanew  ], charf(file[lfanew  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+1, file[lfanew+1], file[lfanew+1], charf(file[lfanew+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+2, file[lfanew+2], file[lfanew+2], charf(file[lfanew+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+3, file[lfanew+3], file[lfanew+3], charf(file[lfanew+3]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n machine = %u :: %u", // (2 байта)
     (file[lfanew+4])      | (file[lfanew+5] << 8),
     (file[lfanew+4]) << 8 | (file[lfanew+5]     )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+4, file[lfanew+4], file[lfanew+4], charf(file[lfanew+4]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+5, file[lfanew+5], file[lfanew+5], charf(file[lfanew+5]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n number_of_sections = %u :: %u", // (2 байта)
     (file[lfanew+6])      | (file[lfanew+7] << 8),
     (file[lfanew+6]) << 8 | (file[lfanew+7]     )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+6, file[lfanew+6], file[lfanew+6], charf(file[lfanew+6]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+7, file[lfanew+7], file[lfanew+7], charf(file[lfanew+7]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n time_date_stamp = %u :: %u", // (4 байта)
     (file[lfanew+8])       | (file[lfanew+9] <<  8) | (file[lfanew+10] << 16) | (file[lfanew+11] << 24),
     (file[lfanew+8]) << 24 | (file[lfanew+9] << 16) | (file[lfanew+10] <<  8) | (file[lfanew+11]      )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+ 8, file[lfanew+ 8], file[lfanew+ 8], charf(file[lfanew+ 8]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+ 9, file[lfanew+ 9], file[lfanew+ 9], charf(file[lfanew+ 9]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+10, file[lfanew+10], file[lfanew+10], charf(file[lfanew+10]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+11, file[lfanew+11], file[lfanew+11], charf(file[lfanew+11]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n pointer_to_symbol_table = %u :: %u", // (4 байта)
     (file[lfanew+12])       | (file[lfanew+13] <<  8) | (file[lfanew+14] << 16) | (file[lfanew+15] << 24),
     (file[lfanew+12]) << 24 | (file[lfanew+13] << 16) | (file[lfanew+14] <<  8) | (file[lfanew+15]      )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+12, file[lfanew+12], file[lfanew+12], charf(file[lfanew+12]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+13, file[lfanew+13], file[lfanew+13], charf(file[lfanew+13]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+14, file[lfanew+14], file[lfanew+14], charf(file[lfanew+14]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+15, file[lfanew+15], file[lfanew+15], charf(file[lfanew+15]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n number_of_symbols = %u :: %u", // (4 байта)
     (file[lfanew+16])       | (file[lfanew+17] <<  8) | (file[lfanew+18] << 16) | (file[lfanew+19] << 24),
     (file[lfanew+16]) << 24 | (file[lfanew+17] << 16) | (file[lfanew+18] <<  8) | (file[lfanew+19]      )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+16, file[lfanew+16], file[lfanew+16], charf(file[lfanew+16]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+17, file[lfanew+17], file[lfanew+17], charf(file[lfanew+17]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+18, file[lfanew+18], file[lfanew+18], charf(file[lfanew+18]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+19, file[lfanew+19], file[lfanew+19], charf(file[lfanew+19]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_optional_header = %u :: %u", // (2 байта)
     (file[lfanew+20])      | (file[lfanew+21] << 8),
     (file[lfanew+20]) << 8 | (file[lfanew+21]     )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+20, file[lfanew+20], file[lfanew+20], charf(file[lfanew+20]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+21, file[lfanew+21], file[lfanew+21], charf(file[lfanew+21]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n characteristics = %u :: %u", // (2 байта)
     (file[lfanew+22])      | (file[lfanew+23] << 8),
     (file[lfanew+22]) << 8 | (file[lfanew+23]     )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+22, file[lfanew+22], file[lfanew+22], charf(file[lfanew+22]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+23, file[lfanew+23], file[lfanew+23], charf(file[lfanew+23]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n magic = %u :: %u", // (2 байта)
     (file[lfanew+24])      | (file[lfanew+25] << 8),
     (file[lfanew+24]) << 8 | (file[lfanew+25]     )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+24, file[lfanew+24], file[lfanew+24], charf(file[lfanew+24]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+25, file[lfanew+25], file[lfanew+25], charf(file[lfanew+25]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n major_linker_version = %u :: %u", // (1 байт)
     (file[lfanew+26]),
     (file[lfanew+26])
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+26, file[lfanew+26], file[lfanew+26], charf(file[lfanew+26]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n minor_linker_version = %u :: %u", // (1 байт)
     (file[lfanew+27]),
     (file[lfanew+27])
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+27, file[lfanew+27], file[lfanew+27], charf(file[lfanew+27]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_code = %u :: %u", // (4 байта)
     (file[lfanew+28])       | (file[lfanew+29] <<  8) | (file[lfanew+30] << 16) | (file[lfanew+31] << 24),
     (file[lfanew+28]) << 24 | (file[lfanew+29] << 16) | (file[lfanew+30] <<  8) | (file[lfanew+31]      )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+28, file[lfanew+28], file[lfanew+28], charf(file[lfanew+28]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+29, file[lfanew+29], file[lfanew+29], charf(file[lfanew+29]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+30, file[lfanew+30], file[lfanew+30], charf(file[lfanew+30]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+31, file[lfanew+31], file[lfanew+31], charf(file[lfanew+31]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_initialized_data = %u :: %u", // (4 байта)
     (file[lfanew+32])       | (file[lfanew+33] <<  8) | (file[lfanew+34] << 16) | (file[lfanew+35] << 24),
     (file[lfanew+32]) << 24 | (file[lfanew+33] << 16) | (file[lfanew+34] <<  8) | (file[lfanew+35]      )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+32, file[lfanew+32], file[lfanew+32], charf(file[lfanew+32]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+33, file[lfanew+33], file[lfanew+33], charf(file[lfanew+33]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+34, file[lfanew+34], file[lfanew+34], charf(file[lfanew+34]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+35, file[lfanew+35], file[lfanew+35], charf(file[lfanew+35]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_uninitialized_data = %u :: %u", // (4 байта)
     (file[lfanew+36])       | (file[lfanew+37] <<  8) | (file[lfanew+38] << 16) | (file[lfanew+39] << 24),
     (file[lfanew+36]) << 24 | (file[lfanew+37] << 16) | (file[lfanew+38] <<  8) | (file[lfanew+39]      )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+36, file[lfanew+36], file[lfanew+36], charf(file[lfanew+36]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+37, file[lfanew+37], file[lfanew+37], charf(file[lfanew+37]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+38, file[lfanew+38], file[lfanew+38], charf(file[lfanew+38]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+39, file[lfanew+39], file[lfanew+39], charf(file[lfanew+39]));
    fprintf(stream, "\n --");
    fprintf(stream, "\n address_of_entry_point = %u :: %u", // (4 байта)
     (file[lfanew+40])       | (file[lfanew+41] <<  8) | (file[lfanew+42] << 16) | (file[lfanew+43] << 24),
     (file[lfanew+40]) << 24 | (file[lfanew+41] << 16) | (file[lfanew+42] <<  8) | (file[lfanew+43]      )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+40, file[lfanew+40], file[lfanew+40], charf(file[lfanew+40]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+41, file[lfanew+41], file[lfanew+41], charf(file[lfanew+41]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+42, file[lfanew+42], file[lfanew+42], charf(file[lfanew+42]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", lfanew+43, file[lfanew+43], file[lfanew+43], charf(file[lfanew+43]));
    fprintf(stream, "\n --");
    //printf("\n Конец анализа.");
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
    pe_minimal_builder("__.exe");
    //pe_minimal_analyzer("__.exe");
    char buffer[128];
    char buffer_2[64];
    printf("\n Введите путь к файлу, который необходимо проанализировать!\n>>> ");
    fgets(buffer, sizeof (buffer), stdin); // Считывает строку вместе с пробелами (максимум 99 символов + '\0')
    buffer[strcspn(buffer, "\n")] = '\0'; // fgets сохраняет символ переноса строки '\n' в конце, удаляем его, если он мешает
    __start:
    printf("\n Куда бы вы хотели получить результат?\n  В консоль\n  В файл\n  Оба варианта\n>>> ");
    fgets(buffer_2, sizeof (buffer_2), stdin); // Считывает строку вместе с пробелами (максимум 99 символов + '\0')
    buffer_2[strcspn(buffer_2, "\n")] = '\0';
    //printf("```\n%s\n```", buffer_2);
    if (!strcmp(buffer_2, "В консоль")) 
    {
        pe_minimal_analyzer(buffer, stdout); // Вывод в консоль
        putchar('\n');
        system("pause");
    }
    else if (!strcmp(buffer_2, "В файл"))
    {
        FILE * file_descriptor = fopen("__.dmp", "wb");
        if (!file_descriptor) return 0;
        pe_minimal_analyzer(buffer, file_descriptor); // Вывод в файл
        fclose(file_descriptor);
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