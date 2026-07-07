#include <stdint.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

typedef union { uint16_t value; uint8_t bytes[2]; } union__uint16_t;
typedef union { uint32_t value; uint8_t bytes[4]; } union__uint32_t;

// БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)
//  Размер: Всегда строго 64 байта.
//  Природа: Статичный исторический балласт. Изменяется только одно поле -> e_lfanew.
union__uint16_t e_magic;    // 000~001      | 00~01      #  Магическое число
//uint8_t dos_reserved[38];   // 002~039      | 02~27      #  Зарезервировано: Обычно забито нулями (0). Сюда можно спрятать кастомные метаданные компилятора, Windows их игнорирует.
union__uint16_t e_res2[10];
union__uint32_t e_lfanew;   // 060~063: 064 | 3C~3F: 40  #  Динамическое поле: Указывает смещение (в байтах от начала файла), где начнется Блок 2 (PE). • Минимум: 64 (если DOS-код заглушки отсутствует).• Динамика: Если ты решишь вставить туда реальную DOS-программу (которая пишет "This program cannot be run in DOS mode"), это поле сдвинется вперед на размер этого DOS-кода (обычно 128 или 248).

// БЛОК 2: PE ЗАГОЛОВОК (COFF File Header) / 2. COFF Заголовок файла (Характеристики процессора)
//  Размер: Всегда строго 24 байта (4 байта сигнатура + 20 байт заголовок).
//  Природа: Задает базовые свойства файла.
union__uint32_t pe_signature;
union__uint16_t Machine;              // Архитектура (0x8664 для AMD64/x64)
union__uint16_t NumberOfSections;     // Сколько секций в файле
union__uint32_t TimeDateStamp;        // Время создания файла
union__uint32_t PointerToSymbolTable; // Символьная таблица (для дебага, у нас 0)
union__uint32_t NumberOfSymbols;      // Количество символов
union__uint16_t SizeOfOptionalHeader; // Размер следующего (Optional) заголовка
union__uint16_t Characteristics;      // Флаги файла (что это EXE, а не DLL)

// БЛОК 3: OPTIONAL HEADER (Только для 64-бит / PE32+)
//  Размер: Базовый — 112 байт.
//  Природа: Самый сложный блок. Конфигурирует подсистему памяти Windows.
// ... ... ...


// БЛОК 4: КАТАЛОГИ ДАННЫХ (Data Directories)
//  Размер: Динамический. Зависит от NumberOfRvaAndSizes. Каждый каталог занимает 8 байт (4 байта адрес + 4 байта размер). Если каталогов 16 \(\rightarrow \) размер строго 128 байт.
//  Природа: Указатели на сложные системные таблицы.
// ... ... ...


// БЛОК 5: ТАБЛИЦА СЕКЦИЙ (Section Table)
//  Размер: Динамический. Равен NumberOfSections \(\times \) 40 байт.
//  Природа: Массив "паспортов" для каждого блока данных. Каждый паспорт имеет строго фиксированную структуру из 40 байт.
// ... ... ...


uint8_t program[2048];

uint64_t offset = 0;

// Функция превращает байт в печатный символ, а непечатные заменяет точкой
char to_ascii(uint8_t b)
{
    switch (b){
    case '\0': return '·';
    case '\t': return '·';
    case '\n': return '·';
    case '\r': return '·';
    }
    //return (b >= 32 && b <= 126) ? (char) b : '·';
}

// Порядок байт: little-endian

void pe_builder()
{
    FILE * descriptor = fopen("test_subject.exe", "wb");
    if (!descriptor) return;
    fwrite("MZ", 1, 2, descriptor); // e_magic  | 000: 077 090 | 00: 4D 5A | MZ
    uint8_t dos_reserved[58] = {0}; // Гарантируем ровно 58 байт нулей в зарезервированной зоне DOS
    fwrite(dos_reserved, 1, 58, descriptor);
    fwrite("\x40\x00\x00\x00", 1, 4, descriptor); // uint32_t e_lfanew         | Адрес PE-сигнатуры (0x00000040 = 64 в десятичной)
    fwrite("PE\0\0", 1, 4, descriptor);           // uint32_t pe_signature     | PE-сигнатура
    fwrite("\x64\x86", 1, 2, descriptor);         // uint16_t Machine          | Архитектура процессора
    fwrite("\x00\x01", 1, 2, descriptor);         // uint16_t NumberOfSections | Количество секций
    fclose(descriptor);
}

void pe_analyzer()
{
    FILE * descriptor = fopen("test_subject.exe", "rb");
    //FILE * descriptor = fopen("pe_tool.exe", "rb");
    if (!descriptor) return;
    printf("\n БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)");
    if (fread(&e_magic, 2, 1, descriptor) != 1) { /*printf("\n Ошибка чтения e_magic");*/ return; }
    printf("\n -------------------------------------------------------------");
    printf("\n  000: %03d | 00: %02X | '%c' | uint16_t e_magic = %u", e_magic.bytes[0], e_magic.bytes[0], to_ascii(e_magic.bytes[0]), e_magic.value);
    printf("\n  001: %03d | 01: %02X | '%c' |",                       e_magic.bytes[1], e_magic.bytes[1], to_ascii(e_magic.bytes[1]));
    printf("\n -------------------------------------------------------------");
    // Размер массива должен быть строго 38 байт (40 - 2 байта MZ)
    uint8_t dos_reserved[38];
    // Читаем ровно 38 байт, чтобы каретка файла остановилась строго на позиции 40
    if (fread(dos_reserved, 1, 38, descriptor) != 38) { /*printf("\n  Ошибка чтения по смещению 002~039 | 02~27");*/ return; }
    printf("\n  002: %03d | 02: %02X | '%c' | uint16_t e_cblp = '\\0',",         dos_reserved[0], dos_reserved[0], to_ascii(dos_reserved[0]));
    printf("\n  003: %03d | 03: %02X | '%c' |                 = '\\0'",          dos_reserved[1], dos_reserved[1], to_ascii(dos_reserved[1]));
    printf("\n -------------------------------------------------------------");
    printf("\n  004: %03d | 04: %02X | '%c' | uint16_t e_cp = '\\0',",           dos_reserved[2], dos_reserved[2], to_ascii(dos_reserved[2]));
    printf("\n  005: %03d | 05: %02X | '%c' |               = '\\0'",            dos_reserved[3], dos_reserved[3], to_ascii(dos_reserved[3]));
    printf("\n -------------------------------------------------------------");
    printf("\n  006: %03d | 06: %02X | '%c' | uint16_t e_crlc = '\\0',",         dos_reserved[4], dos_reserved[4], to_ascii(dos_reserved[4]));
    printf("\n  007: %03d | 07: %02X | '%c' |                 = '\\0'",          dos_reserved[5], dos_reserved[5], to_ascii(dos_reserved[5]));
    printf("\n -------------------------------------------------------------");
    printf("\n  008: %03d | 08: %02X | '%c' | uint16_t e_cparhdr = '\\0',",      dos_reserved[6], dos_reserved[6], to_ascii(dos_reserved[6]));
    printf("\n  009: %03d | 09: %02X | '%c' |                    = '\\0'",       dos_reserved[7], dos_reserved[7], to_ascii(dos_reserved[7]));
    printf("\n -------------------------------------------------------------");
    printf("\n  010: %03d | 0A: %02X | '%c' | uint16_t e_minalloc = '\\0',",     dos_reserved[8], dos_reserved[8], to_ascii(dos_reserved[8]));
    printf("\n  011: %03d | 0B: %02X | '%c' |                     = '\\0'",      dos_reserved[9], dos_reserved[9], to_ascii(dos_reserved[9]));
    printf("\n -------------------------------------------------------------");
    printf("\n  012: %03d | 0C: %02X | '%c' | uint16_t e_maxalloc = '\\0',",     dos_reserved[10], dos_reserved[10], to_ascii(dos_reserved[10]));
    printf("\n  013: %03d | 0D: %02X | '%c' |                     = '\\0'",      dos_reserved[11], dos_reserved[11], to_ascii(dos_reserved[11]));
    printf("\n -------------------------------------------------------------");
    printf("\n  014: %03d | 0E: %02X | '%c' | uint16_t e_ss = '\\0',",           dos_reserved[12], dos_reserved[12], to_ascii(dos_reserved[12]));
    printf("\n  015: %03d | 0F: %02X | '%c' |               = '\\0'",            dos_reserved[13], dos_reserved[13], to_ascii(dos_reserved[13]));
    printf("\n -------------------------------------------------------------");
    printf("\n  016: %03d | 10: %02X | '%c' | uint16_t e_sp = '\\0',",           dos_reserved[14], dos_reserved[14], to_ascii(dos_reserved[14]));
    printf("\n  017: %03d | 11: %02X | '%c' |               = '\\0'",            dos_reserved[15], dos_reserved[15], to_ascii(dos_reserved[15]));
    printf("\n -------------------------------------------------------------");
    printf("\n  018: %03d | 12: %02X | '%c' | uint16_t e_csum = '\\0',",         dos_reserved[16], dos_reserved[16], to_ascii(dos_reserved[16]));
    printf("\n  019: %03d | 13: %02X | '%c' |                 = '\\0'",          dos_reserved[17], dos_reserved[17], to_ascii(dos_reserved[17]));
    printf("\n -------------------------------------------------------------");
    printf("\n  020: %03d | 14: %02X | '%c' | uint16_t e_ip = '\\0',",           dos_reserved[18], dos_reserved[18], to_ascii(dos_reserved[18]));
    printf("\n  021: %03d | 15: %02X | '%c' |               = '\\0'",            dos_reserved[19], dos_reserved[19], to_ascii(dos_reserved[19]));
    printf("\n -------------------------------------------------------------");
    printf("\n  022: %03d | 16: %02X | '%c' | uint16_t e_cs = '\\0',",           dos_reserved[20], dos_reserved[20], to_ascii(dos_reserved[20]));
    printf("\n  023: %03d | 17: %02X | '%c' |               = '\\0'",            dos_reserved[21], dos_reserved[21], to_ascii(dos_reserved[21]));
    printf("\n -------------------------------------------------------------");
    printf("\n  024: %03d | 18: %02X | '%c' | uint16_t e_lfarlc = '\\0',",       dos_reserved[22], dos_reserved[22], to_ascii(dos_reserved[22]));
    printf("\n  025: %03d | 19: %02X | '%c' |                   = '\\0'",        dos_reserved[23], dos_reserved[23], to_ascii(dos_reserved[23]));
    printf("\n -------------------------------------------------------------");
    printf("\n  026: %03d | 1A: %02X | '%c' | uint16_t e_ovno = '\\0',",         dos_reserved[24], dos_reserved[24], to_ascii(dos_reserved[24]));
    printf("\n  027: %03d | 1B: %02X | '%c' |                 = '\\0'",          dos_reserved[25], dos_reserved[25], to_ascii(dos_reserved[25]));
    printf("\n -------------------------------------------------------------");
    printf("\n  028: %03d | 1C: %02X | '%c' | uint16_t e_res[4] = '\\0', №1",    dos_reserved[26], dos_reserved[26], to_ascii(dos_reserved[26]));
    printf("\n  029: %03d | 1D: %02X | '%c' |                     '\\0',",       dos_reserved[27], dos_reserved[27], to_ascii(dos_reserved[27]));
    printf("\n  030: %03d | 1E: %02X | '%c' |                     '\\0', №2",    dos_reserved[28], dos_reserved[28], to_ascii(dos_reserved[28]));
    printf("\n  031: %03d | 1F: %02X | '%c' |                     '\\0',",       dos_reserved[29], dos_reserved[29], to_ascii(dos_reserved[29]));
    printf("\n  032: %03d | 20: %02X | '%c' |                     '\\0', №3",    dos_reserved[30], dos_reserved[30], to_ascii(dos_reserved[30]));
    printf("\n  033: %03d | 21: %02X | '%c' |                     '\\0',",       dos_reserved[31], dos_reserved[31], to_ascii(dos_reserved[31]));
    printf("\n  034: %03d | 22: %02X | '%c' |                     '\\0', №4",    dos_reserved[32], dos_reserved[32], to_ascii(dos_reserved[32]));
    printf("\n  035: %03d | 23: %02X | '%c' |                     '\\0'.",       dos_reserved[33], dos_reserved[33], to_ascii(dos_reserved[33]));
    printf("\n -------------------------------------------------------------");
    printf("\n  036: %03d | 24: %02X | '%c' | uint16_t e_oemid = '\\0', №1",     dos_reserved[34], dos_reserved[34], to_ascii(dos_reserved[34]));
    printf("\n  037: %03d | 25: %02X | '%c' |                    '\\0'.",        dos_reserved[35], dos_reserved[35], to_ascii(dos_reserved[35]));
    printf("\n -------------------------------------------------------------");
    printf("\n  038: %03d | 26: %02X | '%c' | uint16_t e_oeminfo = '\\0', №1",   dos_reserved[36], dos_reserved[36], to_ascii(dos_reserved[36]));
    printf("\n  039: %03d | 27: %02X | '%c' |                      '\\0'.",      dos_reserved[37], dos_reserved[37], to_ascii(dos_reserved[37]));
    printf("\n -------------------------------------------------------------");
    // ЖЕСТКИЙ СБРОС КАРЕТКИ НА СМЕЩЕНИЕ 40 (0x28)
    // Это полностью исправит сдвиг в 10 байт, накопленный выше по коду
    //fseek(descriptor, 40, SEEK_SET);
    //printf("\n ТЕКУЩАЯ ПОЗИЦИЯ В ФАЙЛЕ: %ld", ftell(descriptor));
    // Явно указываем адрес поля value самого первого элемента. 
    // fread запишет 20 байт подряд во все последующие .value элементы массива
    if (fread(&e_res2[0].value, 2, 10, descriptor) != 10) { /*printf("\n Ошибка чтения e_res2[10]");*/ return; }
    printf("\n  040, 041: %03d %03d | 28, 29: %02X %02X | '%c', '%c' | uint16_t e_res2[10] = {%u,",
     e_res2[0].bytes[0], e_res2[0].bytes[1],
     e_res2[0].bytes[0], e_res2[0].bytes[1],
     to_ascii(e_res2[0].bytes[0]), to_ascii(e_res2[0].bytes[1]),
     e_res2[0].value
    );
    offset = 42;
    for (unsigned char i = 1; i <= 8; i++)
    {
        //offset += 2;
        printf("\n  %03d, %03d: %03d %03d | %02X, %02X: %02X %02X | '%c', '%c' | \t\t\t%u,",
         offset, offset+1, e_res2[i].bytes[0], e_res2[i].bytes[1],
         offset, offset+1, e_res2[i].bytes[0], e_res2[i].bytes[1],
         to_ascii(e_res2[i].bytes[0]), to_ascii(e_res2[i].bytes[1]),
         e_res2[i].value
        );
        offset += 2;
    }
    printf("\n  058, 059: %03d %03d | 3A, 3B: %02X %02X | '%c', '%c' | \t\t\t%u};",
     e_res2[9].bytes[0], e_res2[9].bytes[1],
     e_res2[9].bytes[0], e_res2[9].bytes[1],
     to_ascii(e_res2[9].bytes[0]), to_ascii(e_res2[9].bytes[1]),
     e_res2[9].value
    );
    printf("\n -------------------------------------------------------------");
    //printf("\n %zu", sizeof (union__uint16_t)); // 2
    if (fread(&e_lfanew.value, 4, 1, descriptor) != 1) { /*printf("\n Ошибка чтения e_lfanew");*/ return; }
    printf("\n  060: %03d | 3C: %02X | '%c' | uint32_t e_lfanew = %u (0x%08X)", e_lfanew.bytes[0], e_lfanew.bytes[0], e_lfanew.bytes[0], e_lfanew.value, e_lfanew.value);
    printf("\n  061: %03d | 3D: %02X | '%c' |",                                 e_lfanew.bytes[1], e_lfanew.bytes[1], e_lfanew.bytes[1]);
    printf("\n  062: %03d | 3E: %02X | '%c' |",                                 e_lfanew.bytes[2], e_lfanew.bytes[2], e_lfanew.bytes[2]);
    printf("\n  063: %03d | 3F: %02X | '%c' |",                                 e_lfanew.bytes[3], e_lfanew.bytes[3], e_lfanew.bytes[3]);
    printf("\n -------------------------------------------------------------");
    // С этого момента байты могут смещаться (иметь разную длину) ?
    if (e_lfanew.value > 64)
    {
        for (int i = 64, byte = 0; i < e_lfanew.value; i++)
        {
            byte = getc(descriptor);
            printf("\n  %03d: %03d | %02X: %02X | '%c' |", i, byte, i, byte, to_ascii(byte));
        }
    }
    offset = e_lfanew.value;
    //printf("\n -------------------------------------------------------------");
    printf("\n БЛОК 2: PE ЗАГОЛОВОК (COFF File Header)");
    printf("\n -------------------------------------------------------------");
    if (fread(&pe_signature.value, 4, 1, descriptor) != 1) { /*printf("\n Ошибка чтения pe_signature");*/ return; }
    printf("\n  %03d: %03d | %02X: %02X | '%c' | uint32_t pe_signature = %u (0x%08X)", offset, pe_signature.bytes[0], offset, pe_signature.bytes[0], to_ascii(pe_signature.bytes[0]), pe_signature.value, pe_signature.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                           offset, pe_signature.bytes[1], offset, pe_signature.bytes[1], to_ascii(pe_signature.bytes[1]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                           offset, pe_signature.bytes[2], offset, pe_signature.bytes[2], to_ascii(pe_signature.bytes[2]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                           offset, pe_signature.bytes[3], offset, pe_signature.bytes[3], to_ascii(pe_signature.bytes[3]));
    printf("\n -------------------------------------------------------------");
    if (fread(&Machine.value, 2, 1, descriptor) != 1) { /*printf("\n Ошибка чтения Machine");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint16_t Machine = %u (0x%04X)", offset, Machine.bytes[0], offset, Machine.bytes[0], to_ascii(Machine.bytes[0]), Machine.value, Machine.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                offset, Machine.bytes[1], offset, Machine.bytes[1], to_ascii(Machine.bytes[1]));
    printf("\n -------------------------------------------------------------");
    if (fread(&NumberOfSections.value, 2, 1, descriptor) != 1) { /*printf("\n Ошибка чтения NumberOfSections");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint16_t NumberOfSections = %u (0x%04X)", offset, NumberOfSections.bytes[0], offset, NumberOfSections.bytes[0], to_ascii(NumberOfSections.bytes[0]), NumberOfSections.value, NumberOfSections.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                         offset, NumberOfSections.bytes[1], offset, NumberOfSections.bytes[1], to_ascii(NumberOfSections.bytes[1]));
    printf("\n -------------------------------------------------------------");
    if (fread(&TimeDateStamp.value, 4, 1, descriptor) != 1) { /*printf("\n Ошибка чтения TimeDateStamp");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint32_t TimeDateStamp = %u (0x%08X)", offset, TimeDateStamp.bytes[0], offset, TimeDateStamp.bytes[0], to_ascii(TimeDateStamp.bytes[0]), TimeDateStamp.value, TimeDateStamp.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                         offset, TimeDateStamp.bytes[1], offset, TimeDateStamp.bytes[1], to_ascii(TimeDateStamp.bytes[1]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                         offset, TimeDateStamp.bytes[2], offset, TimeDateStamp.bytes[2], to_ascii(TimeDateStamp.bytes[2]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                         offset, TimeDateStamp.bytes[3], offset, TimeDateStamp.bytes[3], to_ascii(TimeDateStamp.bytes[3]));
    printf("\n -------------------------------------------------------------");
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