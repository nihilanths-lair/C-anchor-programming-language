// <!-- Encoding: Windows-1251 -->

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define macro__reverse_16_bit_number(value) (uint16_t) (((value)>>8) | ((value)<<8))
#define macro__reverse_32_bit_number(value) \
 (uint32_t) ( \
 (((value)>>24)&0x000000FFu) | \
 (((value)>>8 )&0x0000FF00u) | \
 (((value)<<8 )&0x00FF0000u) | \
 (((value)<<24)&0xFF000000u)   \
)
#define macro__reverse_64_bit_number(value) \
 (uint64_t) ( \
 (((value)>>56)&0x00000000000000FFull) | \
 (((value)>>40)&0x000000000000FF00ull) | \
 (((value)>>24)&0x0000000000FF0000ull) | \
 (((value)>>8 )&0x00000000FF000000ull) | \
 (((value)<<8 )&0x000000FF00000000ull) | \
 (((value)<<24)&0x0000FF0000000000ull) | \
 (((value)<<40)&0x00FF000000000000ull) | \
 (((value)<<56)&0xFF00000000000000ull)   \
)
 
// Заполнитель
void file_aggregate(FILE * file_descriptor, const uint8_t ascii, int16_t quantity) { while (--quantity >= 0) fprintf(file_descriptor, "%c", ascii); }
//void print_aggregate(const char ascii, int quantity) { while (--quantity >= 0) putchar(ascii); }

int8_t charf(uint8_t ascii)
{
    switch (ascii){
    case '\0': ascii = ' '; // [NUL]
    case    1: ascii = ' '; // [SOH]
    case    2: ascii = ' '; // [STX]
    case    3: ascii = ' '; // [ETX]
    case    6: ascii = ' '; // [ACK]
    case '\v': ascii = ' '; //  [VT]
    case   16: ascii = ' '; // [DLE]
    case  134: ascii = ' '; // †
    }
    return ascii;
}

uint32_t rva_to_raw(uint32_t number_of_sections, uint32_t address_of_entry_point, const uint32_t * virtual_size, const uint32_t * virtual_address, const uint32_t * pointer_to_raw_data)
{
    for (uint32_t i = 0; i < number_of_sections; i++)
    {
        uint32_t _virtual_address = virtual_address[i];
        uint32_t _virtual_size = virtual_size[i];
        // Проверяем, попадает ли искомый RVA в диапазон текущей секции
        if (
            address_of_entry_point >= _virtual_address
            &&
            address_of_entry_point < (_virtual_address + _virtual_size)
        )
        { return address_of_entry_point - _virtual_address + pointer_to_raw_data[i]; }
    }
    return 0; // Если адрес указывает на заголовки или поврежден
}

void pe_minimal_builder(const char * file_name)
{
    FILE * file_descriptor = fopen(file_name, "wb");
    if (!file_descriptor) return;
    fprintf(file_descriptor, "MZ");                                  // magic = MZ (2 байта)
    //file_aggregate(file_descriptor, '\0', 58);                     // 58 байт (2-59)
    for (int i = 0; i < 58; i++) fputc('\0', file_descriptor);
    fwrite(&(uint32_t){64}, sizeof (uint32_t), 1, file_descriptor);  // lfanew = 64 (4 байта) ; влияет на последующее смещение в файле
    fprintf(file_descriptor, "PE%c%c"  ,  0, 0);                     // signature = PE\0\0 (4 байта)
    // === БЛОК: IMAGE_FILE_HEADER ===
    fprintf(file_descriptor, "%c%c"    , 0x64, 0x86);                // 1. Machine = 0x8664 (2 байта) ; AMD64
    fwrite(&(uint16_t){  1}, sizeof (uint16_t), 1, file_descriptor); // 2. NumberOfSections     = 1 (2 байта)
    fwrite(&(uint32_t){  0}, sizeof (uint32_t), 1, file_descriptor); // 3. TimeDateStamp        = 0 (4 байта)
    fwrite(&(uint32_t){  0}, sizeof (uint32_t), 1, file_descriptor); // 4. PointerToSymbolTable = 0 (4 байта)
    fwrite(&(uint32_t){  0}, sizeof (uint32_t), 1, file_descriptor); // 5. NumberOfSymbols      = 0 (4 байта)
    fwrite(&(uint16_t){240}, sizeof (uint16_t), 1, file_descriptor); // 6. SizeOfOptionalHeader = 0x00F0 (2 байта)
    fprintf(file_descriptor, "%c%c", 0x22, 0x00);                    // 7. Characteristics = 0x0022 (EXECUTABLE_IMAGE | LARGE_ADDRESS_AWARE) (2 байта)
    // === БЛОК: IMAGE_OPTIONAL_HEADER64 (Стандартные поля) ===
    // Начинается со смещения 88 (если lfanew = 64)
    fwrite(&(uint16_t){0x020B}, sizeof (uint16_t), 1, file_descriptor); // 1. Magic = PE32+ (64-битный файл)
    fputc(1, file_descriptor);                                          // 2.1 MajorLinkerVersion
    fputc(0, file_descriptor);                                          // 2.2 MinorLinkerVersion
    fwrite(&(uint32_t){ 512}, sizeof (uint32_t), 1, file_descriptor);   // 3. SizeOfCode (4 байта) ; Выровнен по FileAlignment
    fwrite(&(uint32_t){   0}, sizeof (uint32_t), 1, file_descriptor);   // 4. SizeOfInitializedData (4 байта)
    fwrite(&(uint32_t){   0}, sizeof (uint32_t), 1, file_descriptor);   // 5. SizeOfUninitializedData (4 байта)
    fwrite(&(uint32_t){4096}, sizeof (uint32_t), 1, file_descriptor);   // 6. AddressOfEntryPoint — укажем RVA = 4096 (0x1000). Это стандартное начало первой секции в памяти
    fwrite(&(uint32_t){4096}, sizeof (uint32_t), 1, file_descriptor);   // 7. BaseOfCode (Обычно совпадает с началом кода)
    // === БЛОК: IMAGE_OPTIONAL_HEADER64 (Windows-Specific Fields) ===
    // Начинается со смещения lfanew + 48 (112-й байт в файле)
    fwrite(&(uint64_t){0x00400000}, sizeof (uint64_t), 1, file_descriptor); // 8. ImageBase (8 байт)
    fwrite(&(uint32_t){      4096}, sizeof (uint32_t), 1, file_descriptor); // 9. SectionAlignment = 4096 (4 байта)
    fwrite(&(uint32_t){       512}, sizeof (uint32_t), 1, file_descriptor); // 10. FileAlignment = 512 (4 байта)
    // === БЛОК: IMAGE_OPTIONAL_HEADER64 (Размеры и версии) ===
    // Начинается со смещения lfanew + 64 (128-й байт в файле)
    fwrite(&(uint16_t){   6}, sizeof (uint16_t), 1, file_descriptor); // 11. MajorOperatingSystemVersion
    fwrite(&(uint16_t){   0}, sizeof (uint16_t), 1, file_descriptor); // 12. MinorOperatingSystemVersion
    fwrite(&(uint16_t){   0}, sizeof (uint16_t), 1, file_descriptor); // 13. MajorImageVersion
    fwrite(&(uint16_t){   0}, sizeof (uint16_t), 1, file_descriptor); // 14. MinorImageVersion
    fwrite(&(uint16_t){   6}, sizeof (uint16_t), 1, file_descriptor); // 15. MajorSubsystemVersion
    fwrite(&(uint16_t){   0}, sizeof (uint16_t), 1, file_descriptor); // 16. MinorSubsystemVersion
    fwrite(&(uint32_t){   0}, sizeof (uint32_t), 1, file_descriptor); // 17. Win32VersionValue (Всегда 0)
    fwrite(&(uint32_t){8192}, sizeof (uint32_t), 1, file_descriptor); // 18. SizeOfImage = 8192 (Размер в памяти, кратен SectionAlignment)
    fwrite(&(uint32_t){ 512}, sizeof (uint32_t), 1, file_descriptor); // 19. SizeOfHeaders = 512 (Размер заголовков на диске, кратен FileAlignment)
    // === БЛОК: IMAGE_OPTIONAL_HEADER64 (Подсистема и размеры памяти) ===
    // Начинается со смещения lfanew + 88 (152-й байт в файле)
    fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor); // 20. CheckSum
    fwrite(&(uint16_t){3}, sizeof (uint16_t), 1, file_descriptor); // 21. Subsystem = 3 (Консоль)
    fwrite(&(uint16_t){0}, sizeof (uint16_t), 1, file_descriptor); // 22. DllCharacteristics
    // SizeOfStackReserve, SizeOfStackCommit, SizeOfHeapReserve, SizeOfHeapCommit
    for (int i = 0; i < 4; i++) fwrite(&(uint64_t){0}, sizeof (uint64_t), 1, file_descriptor); // 23~26. Память (забиваем нулями для дефолтов Windows)
    fwrite(&(uint32_t){ 0}, sizeof (uint32_t), 1, file_descriptor); // 27. LoaderFlags
    fwrite(&(uint32_t){16}, sizeof (uint32_t), 1, file_descriptor); // 28. NumberOfRvaAndSizes (количество каталогов)
    // === БЛОК: IMAGE_OPTIONAL_HEADER64 (DATA DIRECTORIES) ===
    // Начинается со смещения lfanew + 136. Всего 16 директорий по 8 байт = 128 байт.
    for (int i = 0; i < 16; i++)
    {
        fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor); // VirtualAddress = 0
        fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor); // Size = 0
    }
    // === БЛОК №3: IMAGE_SECTION_HEADER (Секция .text) ===
    // Начинается со смещения lfanew + 264 (328-й байт в файле)
    fprintf(file_descriptor, ".text%c%c%c", 0, 0, 0);                 // 1. Name = ".text" (8 байт)
    fwrite(&(uint32_t){ 512}, sizeof (uint32_t), 1, file_descriptor); // 2. VirtualSize = 512 (4 байта)
    fwrite(&(uint32_t){4096}, sizeof (uint32_t), 1, file_descriptor); // 3. VirtualAddress (RVA) = 4096 (4 байта) — Точка привязки EntryPoint в памяти
    fwrite(&(uint32_t){ 512}, sizeof (uint32_t), 1, file_descriptor); // 4. SizeOfRawData = 512 (4 байта) — Физический размер кода на диске
    fwrite(&(uint32_t){ 512}, sizeof (uint32_t), 1, file_descriptor); // 5. PointerToRawData = 512 (4 байта) — Физическое смещение кода в файле
    // 6. Оставшиеся указатели и счетчики релокаций (12 байт нулей)
    fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor);
    fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor);
    fwrite(&(uint32_t){0}, sizeof (uint32_t), 1, file_descriptor);
    //
    fwrite(&(uint32_t){0xE0000020}, sizeof (uint32_t), 1, file_descriptor); // 7. Characteristics = 0xE0000020 (CODE | EXECUTE | READ | WRITE) (4 байта)
    // К этому моменту записано ровно 368 байт.
    // 1. ПАДДИНГ ЗАГОЛОВКОВ: Добиваем нулями до 512 байт (512 - 368 = 144 байта)
    file_aggregate(file_descriptor, '\0', 144);
    // === СМЕЩЕНИЕ 512: НАЧАЛО СЕКЦИИ .text (RAW DATA) ===
    // Записываем ассемблерную инструкцию 'ret' (0xC3) — это код нашей программы!
    fprintf(file_descriptor, "%c", 0xC3);
    // 2. ПАДДИНГ СЕКЦИИ: Секция на диске должна занимать строго 512 байт.
    // Мы записали 1 байт кода, значит добиваем еще 511 байт нулями
    file_aggregate(file_descriptor, '\0', 511);
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
    fprintf(stream, " --------------------------");
    fprintf(stream, "\n /!\\ Анализ PE-файла начат.");
    fprintf(stream, "\n --------------------------");
    fprintf(stream, "\n magic = %u :: %u", // (2 байта)
     ((uint16_t) file[0]   ) | ((uint16_t) file[1]<<8),
     ((uint16_t) file[0]<<8) | ((uint16_t) file[1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 0ull, file[0], file[0], charf(file[0]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 1ull, file[1], file[1], charf(file[1]));
    fprintf(stream, "\n --");
    for (uint8_t offset = 2; offset <= 59; offset++) fprintf(stream, "\n %08llu: %03d | %02X | %c", offset, file[offset], file[offset], charf(file[offset]));
    fprintf(stream, "\n --");
    uint32_t lfanew = ((uint32_t) file[60]) | ((uint32_t) file[61]<<8 ) | ((uint32_t) file[62]<<16) | ((uint32_t) file[63]<<24); // (4 байта)
    fprintf(stream, "\n lfanew = %u :: %u", lfanew, macro__reverse_32_bit_number(lfanew));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 60ull, file[60], file[60], charf(file[60]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 61ull, file[61], file[61], charf(file[61]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 62ull, file[62], file[62], charf(file[62]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", 63ull, file[63], file[63], charf(file[63]));
    uint64_t offset;
    if (lfanew > 64) // Если lfanew > 64, значит между DOS-заголовком и NT-заголовком есть зазор (DOS STUB / Заглушка)
    {
        fprintf(stream, "\n --");
        for (offset = 64; offset < lfanew; offset++) fprintf(stream, "\n %08llu: %03d | %02X | %c", offset, file[offset], file[offset], charf(file[offset]));
    }
    offset = lfanew; // Принудительно ставим offset на начало NT-заголовка
    fprintf(stream, "\n --");
    fprintf(stream, "\n signature = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4; // IMAGE_FILE_HEADER (COFF)
    fprintf(stream, "\n --");
    fprintf(stream, "\n machine = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    uint16_t number_of_sections = ((uint16_t) file[offset]) | ((uint16_t) file[offset+1]<<8);
    fprintf(stream, "\n number_of_sections = %u :: %u", number_of_sections, macro__reverse_16_bit_number(number_of_sections)); // (2 байта)
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n time_date_stamp = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n pointer_to_symbol_table = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n number_of_symbols = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_optional_header = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n characteristics = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    uint16_t magic = ((uint16_t) file[offset]) | ((uint16_t) file[offset+1]<<8);
    fprintf(stream, "\n magic = %u :: %u", magic, macro__reverse_16_bit_number(magic)); // (2 байта)
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n major_linker_version = %u :: %u", // (1 байт)
     ((uint8_t) file[offset]),
     ((uint8_t) file[offset])
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset, file[offset], file[offset], charf(file[offset]));
    offset += 1;
    fprintf(stream, "\n --");
    fprintf(stream, "\n minor_linker_version = %u :: %u", // (1 байт)
     ((uint8_t) file[offset]),
     ((uint8_t) file[offset])
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset, file[offset], file[offset], charf(file[offset]));
    offset += 1;
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_code = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_initialized_data = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_uninitialized_data = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    uint32_t address_of_entry_point = ((uint32_t) file[offset]) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24);
    fprintf(stream, "\n address_of_entry_point = %u :: %u", address_of_entry_point, macro__reverse_32_bit_number(address_of_entry_point)); // (4 байта)
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n base_of_code = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    // === РАЗВИЛКА АРХИТЕКТУР (PE32 vs PE32+) ===
    uint64_t image_base = 0;
    if (magic == 0x010B) // PE32 (32-бит)
    {
        fprintf(stream, "\n base_of_data = %u :: %u", // (4 байта)
         ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
         ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
        image_base =
         ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
         ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
        ;
        fprintf(stream, "\n image_base = %u :: %u", image_base, macro__reverse_32_bit_number(image_base)); // (4 байта)
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
    } 
    else if (magic == 0x020B) // PE32+ (64-бит)
    {
        image_base =
         ((uint64_t) file[offset  ]    ) | ((uint64_t) file[offset+1]<<8 ) | ((uint64_t) file[offset+2]<<16) | ((uint64_t) file[offset+3]<<24) |
         ((uint64_t) file[offset+4]<<32) | ((uint64_t) file[offset+5]<<40) | ((uint64_t) file[offset+6]<<48) | ((uint64_t) file[offset+7]<<56),

         ((uint64_t) file[offset  ]<<56) | ((uint64_t) file[offset+1]<<48) | ((uint64_t) file[offset+2]<<40) | ((uint64_t) file[offset+3]<<32) |
         ((uint64_t) file[offset+4]<<24) | ((uint64_t) file[offset+5]<<16) | ((uint64_t) file[offset+6]<<8 ) | ((uint64_t) file[offset+7]    )
        ;
        fprintf(stream, "\n image_base = %llu :: %llu", image_base, macro__reverse_64_bit_number(image_base)); // (8 байт)
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+4, file[offset+4], file[offset+4], charf(file[offset+4]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+5, file[offset+5], file[offset+5], charf(file[offset+5]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+6, file[offset+6], file[offset+6], charf(file[offset+6]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+7, file[offset+7], file[offset+7], charf(file[offset+7]));
        offset += 8;
    }
    fprintf(stream, "\n --");
    fprintf(stream, "\n section_alignment = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n file_alignment = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n major_operating_system_version = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n minor_operating_system_version = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n major_image_version = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n minor_image_version = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n major_subsystem_version = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n minor_subsystem_version = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n win_32_version_value = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_image = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    uint32_t size_of_headers = ((uint32_t) file[offset]) | ((uint32_t) file[offset+1]<<8) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24);
    fprintf(stream, "\n size_of_headers = %u :: %u", size_of_headers, size_of_headers); // (4 байта)
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n check_sum = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    fprintf(stream, "\n sub_system = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    fprintf(stream, "\n dll_characteristics = %u :: %u", // (2 байта)
     ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
     ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    offset += 2;
    fprintf(stream, "\n --");
    // SizeOfStackReserve, SizeOfStackCommit, SizeOfHeapReserve, SizeOfHeapCommit
    fprintf(stream, "\n size_of_stack_reserve = %llu :: %llu", // (8 байт)
     ((uint64_t) file[offset  ]    ) | ((uint64_t) file[offset+1]<<8 ) | ((uint64_t) file[offset+2]<<16) | ((uint64_t) file[offset+3]<<24) |
     ((uint64_t) file[offset+4]<<32) | ((uint64_t) file[offset+5]<<40) | ((uint64_t) file[offset+6]<<48) | ((uint64_t) file[offset+7]<<56),

     ((uint64_t) file[offset  ]<<56) | ((uint64_t) file[offset+1]<<48) | ((uint64_t) file[offset+2]<<40) | ((uint64_t) file[offset+3]<<32) |
     ((uint64_t) file[offset+4]<<24) | ((uint64_t) file[offset+5]<<16) | ((uint64_t) file[offset+6]<<8 ) | ((uint64_t) file[offset+7]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+4, file[offset+4], file[offset+4], charf(file[offset+4]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+5, file[offset+5], file[offset+5], charf(file[offset+5]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+6, file[offset+6], file[offset+6], charf(file[offset+6]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+7, file[offset+7], file[offset+7], charf(file[offset+7]));
    offset += 8;
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_stack_commit = %llu :: %llu", // (8 байт)
     ((uint64_t) file[offset  ]    ) | ((uint64_t) file[offset+1]<<8 ) | ((uint64_t) file[offset+2]<<16) | ((uint64_t) file[offset+3]<<24) |
     ((uint64_t) file[offset+4]<<32) | ((uint64_t) file[offset+5]<<40) | ((uint64_t) file[offset+6]<<48) | ((uint64_t) file[offset+7]<<56),

     ((uint64_t) file[offset  ]<<56) | ((uint64_t) file[offset+1]<<48) | ((uint64_t) file[offset+2]<<40) | ((uint64_t) file[offset+3]<<32) |
     ((uint64_t) file[offset+4]<<24) | ((uint64_t) file[offset+5]<<16) | ((uint64_t) file[offset+6]<<8 ) | ((uint64_t) file[offset+7]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+4, file[offset+4], file[offset+4], charf(file[offset+4]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+5, file[offset+5], file[offset+5], charf(file[offset+5]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+6, file[offset+6], file[offset+6], charf(file[offset+6]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+7, file[offset+7], file[offset+7], charf(file[offset+7]));
    offset += 8;
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_heap_reserve = %llu :: %llu", // (8 байт)
     ((uint64_t) file[offset  ]    ) | ((uint64_t) file[offset+1]<<8 ) | ((uint64_t) file[offset+2]<<16) | ((uint64_t) file[offset+3]<<24) |
     ((uint64_t) file[offset+4]<<32) | ((uint64_t) file[offset+5]<<40) | ((uint64_t) file[offset+6]<<48) | ((uint64_t) file[offset+7]<<56),

     ((uint64_t) file[offset  ]<<56) | ((uint64_t) file[offset+1]<<48) | ((uint64_t) file[offset+2]<<40) | ((uint64_t) file[offset+3]<<32) |
     ((uint64_t) file[offset+4]<<24) | ((uint64_t) file[offset+5]<<16) | ((uint64_t) file[offset+6]<<8 ) | ((uint64_t) file[offset+7]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+4, file[offset+4], file[offset+4], charf(file[offset+4]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+5, file[offset+5], file[offset+5], charf(file[offset+5]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+6, file[offset+6], file[offset+6], charf(file[offset+6]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+7, file[offset+7], file[offset+7], charf(file[offset+7]));
    offset += 8;
    fprintf(stream, "\n --");
    fprintf(stream, "\n size_of_heap_commit = %llu :: %llu", // (8 байт)
     ((uint64_t) file[offset  ]    ) | ((uint64_t) file[offset+1]<<8 ) | ((uint64_t) file[offset+2]<<16) | ((uint64_t) file[offset+3]<<24) |
     ((uint64_t) file[offset+4]<<32) | ((uint64_t) file[offset+5]<<40) | ((uint64_t) file[offset+6]<<48) | ((uint64_t) file[offset+7]<<56),

     ((uint64_t) file[offset  ]<<56) | ((uint64_t) file[offset+1]<<48) | ((uint64_t) file[offset+2]<<40) | ((uint64_t) file[offset+3]<<32) |
     ((uint64_t) file[offset+4]<<24) | ((uint64_t) file[offset+5]<<16) | ((uint64_t) file[offset+6]<<8 ) | ((uint64_t) file[offset+7]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+4, file[offset+4], file[offset+4], charf(file[offset+4]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+5, file[offset+5], file[offset+5], charf(file[offset+5]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+6, file[offset+6], file[offset+6], charf(file[offset+6]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+7, file[offset+7], file[offset+7], charf(file[offset+7]));
    offset += 8;
    fprintf(stream, "\n --");
    fprintf(stream, "\n loader_flags = %u :: %u", // (4 байта)
     ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
     ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
    );
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    uint32_t number_of_rva_and_sizes = ((uint32_t) file[offset]) | ((uint32_t) file[offset+1]<<8) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24);
    fprintf(stream, "\n number_of_rva_and_sizes = %u :: %u", number_of_rva_and_sizes, macro__reverse_32_bit_number(number_of_rva_and_sizes)); // (4 байта)
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
    fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
    offset += 4;
    fprintf(stream, "\n --");
    // === ФИНАЛ: ЧТЕНИЕ КАТАЛОГОВ ДАННЫХ (DATA DIRECTORIES) ===
    for (uint32_t i = 1; i <= number_of_rva_and_sizes; i++)
    {
        //fprintf(stream, "\n");
        fprintf(stream, "\n virtual_address[%d] = %u :: %u", i, // (4 байта)
         ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
         ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        fprintf(stream, "\n size[%d] = %u :: %u", i, // (4 байта)
         ((uint32_t) file[offset+4]    ) | ((uint32_t) file[offset+5]<<8 ) | ((uint32_t) file[offset+6]<<16) | ((uint32_t) file[offset+7]<<24),
         ((uint32_t) file[offset+4]<<24) | ((uint32_t) file[offset+5]<<16) | ((uint32_t) file[offset+6]<<8 ) | ((uint32_t) file[offset+7]    )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+4, file[offset+4], file[offset+4], charf(file[offset+4]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+5, file[offset+5], file[offset+5], charf(file[offset+5]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+6, file[offset+6], file[offset+6], charf(file[offset+6]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+7, file[offset+7], file[offset+7], charf(file[offset+7]));
        offset += 8;
        //fprintf(stream, "\n");
    }
    //fprintf(stream, "\n Количество секций: %u", number_of_sections);
    // Заводим массивы (или переменные), которые нам ЖИЗНЕННО НЕОБХОДИМЫ дальше для борьбы с хаосом.
    // Мы сохраним физические и виртуальные адреса секций.
    // Для универсальности выделим память под максимум 96 секций (ограничение PE спецификации)
    uint64_t section_heading[96][8+1] = {'\0'}; // Заголовок раздела
    uint32_t virtual_size[96] = {0};
    uint32_t virtual_address[96] = {0};
    uint32_t size_of_raw_data[96] = {0};
    uint32_t pointer_to_raw_data[96] = {0};
    uint32_t characteristics[96] = {0};
    for (int i = 0; i < number_of_sections; i++)
    {
        //for (int j = 0; j < 8; j++) section_heading[i][j] = file[offset+j];
        //section_heading[i][8] = '\0';
        //fprintf(stream, "\n Заголовок раздела: %s", section_heading[i]);
        fprintf(stream, "\n --");
        fprintf(stream, "\n name[%d] = %llu :: %llu", i+1, // (8 байт)
         ((uint64_t) file[offset  ]    ) | ((uint64_t) file[offset+1]<<8 ) | ((uint64_t) file[offset+2]<<16) | ((uint64_t) file[offset+3]<<24) |
         ((uint64_t) file[offset+4]<<32) | ((uint64_t) file[offset+5]<<40) | ((uint64_t) file[offset+6]<<48) | ((uint64_t) file[offset+7]<<56),

         ((uint64_t) file[offset  ]<<56) | ((uint64_t) file[offset+1]<<48) | ((uint64_t) file[offset+2]<<40) | ((uint64_t) file[offset+3]<<32) |
         ((uint64_t) file[offset+4]<<24) | ((uint64_t) file[offset+5]<<16) | ((uint64_t) file[offset+6]<<8 ) | ((uint64_t) file[offset+7]    )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+4, file[offset+4], file[offset+4], charf(file[offset+4]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+5, file[offset+5], file[offset+5], charf(file[offset+5]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+6, file[offset+6], file[offset+6], charf(file[offset+6]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+7, file[offset+7], file[offset+7], charf(file[offset+7]));
        offset += 8;
        fprintf(stream, "\n --");
        /*uint32_t*/virtual_size[i] = ((uint32_t) file[offset]) | ((uint32_t) file[offset+1]<<8) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24);
        fprintf(stream, "\n virtual_size[%d] = %u :: %u", i+1, virtual_size[i], macro__reverse_32_bit_number(virtual_size[i])); // (4 байта)
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
        fprintf(stream, "\n --");
        /*uint32_t*/virtual_address[i] = ((uint32_t) file[offset]) | ((uint32_t) file[offset+1]<<8) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24);
        fprintf(stream, "\n virtual_address[%d] = %u :: %u", i+1, virtual_address[i], macro__reverse_32_bit_number(virtual_address[i])); // (4 байта)
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
        fprintf(stream, "\n --");
        fprintf(stream, "\n size_of_raw_data[%d] = %u :: %u", i+1, // (4 байта)
         ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
         ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
        fprintf(stream, "\n --");
        /*uint32_t*/pointer_to_raw_data[i] = ((uint32_t) file[offset]) | ((uint32_t) file[offset+1]<<8) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24);
        fprintf(stream, "\n pointer_to_raw_data[%d] = %u :: %u", i+1, pointer_to_raw_data[i], macro__reverse_32_bit_number(pointer_to_raw_data[i])); // (4 байта)
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
        fprintf(stream, "\n --");
        fprintf(stream, "\n pointer_to_relocations[%d] = %u :: %u", i+1, // (4 байта)
         ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
         ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
        fprintf(stream, "\n --");
        fprintf(stream, "\n pointer_to_line_numbers[%d] = %u :: %u", i+1, // (4 байта)
         ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
         ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
        fprintf(stream, "\n --");
        fprintf(stream, "\n number_of_relocations[%d] = %u :: %u", i+1, // (2 байта)
         ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
         ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        offset += 2;
        fprintf(stream, "\n --");
        fprintf(stream, "\n number_of_line_numbers[%d] = %u :: %u", i+1, // (2 байта)
         ((uint16_t) file[offset]   ) | ((uint16_t) file[offset+1]<<8),
         ((uint16_t) file[offset]<<8) | ((uint16_t) file[offset+1]   )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        offset += 2;
        fprintf(stream, "\n --");
        fprintf(stream, "\n characteristics[%d] = %u :: %u", i+1, // (4 байта)
         ((uint32_t) file[offset]    ) | ((uint32_t) file[offset+1]<<8 ) | ((uint32_t) file[offset+2]<<16) | ((uint32_t) file[offset+3]<<24),
         ((uint32_t) file[offset]<<24) | ((uint32_t) file[offset+1]<<16) | ((uint32_t) file[offset+2]<<8 ) | ((uint32_t) file[offset+3]    )
        );
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset  , file[offset  ], file[offset  ], charf(file[offset  ]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+1, file[offset+1], file[offset+1], charf(file[offset+1]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+2, file[offset+2], file[offset+2], charf(file[offset+2]));
        fprintf(stream, "\n %08llu: %03d | %02X | %c", offset+3, file[offset+3], file[offset+3], charf(file[offset+3]));
        offset += 4;
        fprintf(stream, "\n --");
    }
    // === БЛОК №4: ПЕРВЫЙ ПРЫЖОК В ХАОС ДАННЫХ ===
    // entry_point у нас равен 4096. Переводим его в физическое смещение в файле:
    // Передаем значение, количество секций и наши массивы-карты
    uint32_t entry_point_raw = rva_to_raw(number_of_sections, address_of_entry_point, virtual_size, virtual_address, pointer_to_raw_data);
    if (entry_point_raw != 0)
    {
        fprintf(stream, "\n Точка входа в программу (RVA): %u = 0x%08X", address_of_entry_point, address_of_entry_point);
        fprintf(stream, "\n Физическое смещение в файле (RAW): %u = 0x%08X", entry_point_raw, entry_point_raw);
    }
    fprintf(stream, "\n -----------------------------");
    fprintf(stream, "\n /!\\ Анализ PE-файла завершён.");
    fprintf(stream, "\n -----------------------------");
    //printf("\n Конец анализа.");
}
//#include <locale.h>
#include <string.h>
#include <windows.h>
// Потоковый стрим?: Нет.
// gcc -s pe_mtool.c -o pe_mtool.exe / Strip (Удаление отладочной информации/лишнего мусора)
// gcc -Os -s pe_mtool.c -o pe_mtool.exe
// pe_mtool.exe
// pe_mtool.exe > pe_mtool.dmp
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

// <winsock2.h> (для Windows) или <arpa/inet.h> (для Linux)
// htons(число) для 16-bit's или htonll(число) для 64-bit's
//
// Интринсики
// __builtin_bswap16(число)
// __builtin_bswap32(число)
// __builtin_bswap64(число)