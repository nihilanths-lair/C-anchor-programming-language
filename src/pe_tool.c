#ifdef _WIN32
 #include <windows.h>
#endif

#include <stdint.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//typedef union { uint8_t value; uint8_t bytes[1]; } union__uint8_t;
typedef union { uint16_t value; uint8_t bytes[2]; } union__uint16_t;
typedef union { uint32_t value; uint8_t bytes[4]; } union__uint32_t;
typedef union { uint64_t value; uint8_t bytes[8]; } union__uint64_t;

// БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)
//  Размер: Всегда строго 64 байта.
//  Природа: Статичный исторический балласт. Изменяется только одно поле -> e_lfanew.
union__uint16_t e_magic;    // 000~001      | 00~01      #  Магическое число
union__uint16_t e_cblp;
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

// БЛОК 3: OPTIONAL HEADER
// [Примечание] Этот заголовок бывает двух видов:
// 1| PE32 (для 32-битных файлов) — базовый размер 96 байт.
// 2| PE32+ (для 64-битных файлов) — базовый размер 112 байт.
//  Размер: Базовый — 112 байт (Только для 64-бит / PE32+).
//  Природа: Самый сложный блок. Конфигурирует подсистему памяти Windows.
// Часть 1: Стандартные поля COFF (Standard Fields) — одинаковые для 32/64 бит
union__uint16_t Magic; // PE32 :: 0x010B, PE32+ :: 0x020B
uint8_t MajorLinkerVersion; // Необязательное поле (можно выставить 0)
uint8_t MinorLinkerVersion; // Необязательное поле (можно выставить 0)
union__uint32_t SizeOfCode;            // Суммарный размер всех секций с кодом (размер поля .text)
union__uint32_t SizeOfInitializedData;     // Размер инициализированных данных (поле .data)
union__uint32_t SizeOfUninitializedData; // Размер неинициализированных данных (поле .bss)
union__uint32_t AddressOfEntryPoint; // Точка входа (RVA). Адрес (относительно ImageBase), с которого Windows начинает выполнять код программы.
union__uint32_t BaseOfCode; // Относительный адрес начала кода.
// [Примечание]: Только в PE32 здесь идёт ещё 4 байта uint32_t BaseOfData. В 64-битном PE32+ этого поля вообще нет!
// Часть 2: Специфичные поля Windows (Windows-Specific Fields) — размеры различаются!
union__uint64_t ImageBase; // Базовый адрес загрузки. Предпочтительный адрес в виртуальной памяти, куда EXE отображается при старте (обычно 0x00400000 для 32-бит или 0x000140000000 для 64-бит). (В PE32 это поле занимает 4 байта, в PE32+ — 8 байт).
union__uint32_t SectionAlignment; // Выравнивание секций в виртуальной памяти (обычно 4096 байт / 4 КБ страница).
union__uint32_t FileAlignment; // Выравнивание секций на диске (обычно 512 байт / размер сектора).
union__uint16_t MajorOperatingSystemVersion; // Требуемая мажорная версия ОС.
union__uint16_t MinorOperatingSystemVersion; // Требуемая минорная версия ОС.
union__uint16_t MajorImageVersion; // Мажорная версия самого бинарника.
union__uint16_t MinorImageVersion; // Минорная версия самого бинарника.
union__uint16_t MajorSubsystemVersion; // Мажорная версия подсистемы.
union__uint16_t MinorSubsystemVersion; // Минорная версия подсистемы.
union__uint32_t Win32VersionValue; // Зарезервировано, всегда 0.
union__uint32_t SizeOfImage; // Общий размер файла в памяти. Сумма всех секций, округленная до SectionAlignment.
union__uint32_t SizeOfHeaders; // Размер всех заголовков вместе взятых (включая таблицу секций), округленный до FileAlignment.
union__uint32_t CheckSum; // Контрольная сумма (критично для драйверов .sys, в обычных EXE часто равна 0).
union__uint16_t Subsystem; // Тип приложения. Например: 2 (Windows GUI — графическое) или 3 (Windows CUI — консольное).
union__uint16_t DllCharacteristics; // Флаги защиты (поддержка ASLR, DEP, безопасной обработки исключений и т.д.).
union__uint64_t SizeOfStackReserve; // Сколько памяти зарезервировать под стек (В PE32 — 4 байта, в PE32+ — 8 байт).
union__uint64_t SizeOfStackCommit; // Сколько памяти выделить под стек сразу (В PE32 — 4 байта, в PE32+ — 8 байт).
union__uint64_t SizeOfHeapReserve; // Сколько памяти зарезервировать под кучу (В PE32 — 4 байта, в PE32+ — 8 байт).
union__uint64_t SizeOfHeapCommit; // Сколько памяти выделить под кучу сразу (В PE32 — 4 байта, в PE32+ — 8 байт).
union__uint32_t LoaderFlags; // Устаревшее поле, всегда 0.
union__uint32_t NumberOfRvaAndSizes; // Количество каталогов данных. Показывает, сколько элементов будет в следующем (4-м) блоке. По стандарту их всегда 16.


// БЛОК 4: КАТАЛОГИ ДАННЫХ (Data Directories)
//  Размер: Динамический. Зависит от NumberOfRvaAndSizes. Каждый каталог занимает 8 байт (4 байта адрес + 4 байта размер). Если каталогов 16 \(\rightarrow \) размер строго 128 байт.
//  Природа: Указатели на сложные системные таблицы.
//typedef struct {
union__uint32_t virtual_address[16];
union__uint32_t size[16];
//} DataDirectory;

const char * directory_names[16] =
{
    "EXPORT Table            ", // 0: Таблица экспорта (нужна для DLL)
    "IMPORT Table            ", // 1: Таблица импорта (какие DLL и функции нужны EXE)
    "RESOURCE Table          ", // 2: Ресурсы (иконки, манифест, меню, строки)
    "EXCEPTION Table         ", // 3: Таблица исключений (триггеры для x64 try/catch)
    "SECURITY Table          ", // 4: Сертификаты и цифровые подписи (смещение файла, а не RVA!)
    "BASE RELOCATION Table   ", // 5: Таблица релокаций (нужна для ASLR и DLL)
    "DEBUG Debug Directory   ", // 6: Отладочная информация (пути к .pdb файлам)
    "ARCHITECTURE Data       ", // 7: Зарезервировано под специфичную архитектуру процессора
    "GLOBAL POINTER Register ", // 8: Относительный адрес глобального указателя (RVA)
    "THREAD LOCAL STORAGE    ", // 9: Таблица TLS (потоковые локальные переменные)
    "LOAD CONFIG Directory   ", // 10: Конфигурация загрузки (важно для защиты Control Flow Guard)
    "BOUND IMPORT Table      ", // 11: Связанный импорт (оптимизация старых ОС, сейчас редко)
    "IMPORT ADDRESS Table    ", // 12: Таблица IAT (сюда Windows пишет реальные адреса функций)
    "DELAY IMPORT Descriptors", // 13: Отложенный импорт (загрузка DLL только при первом вызове)
    "CLR RUNTIME Header      ", // 14: Заголовок .NET / CLR (если приложение написано на C#/.NET)
    "RESERVED                "  // 15: Зарезервировано Microsoft, всегда нули
};


// БЛОК 5: ТАБЛИЦА СЕКЦИЙ (Section Table)
//  Размер: Динамический. Равен NumberOfSections \(\times \) 40 байт.
//  Природа: Массив "паспортов" для каждого блока данных. Каждый паспорт имеет строго фиксированную структуру из 40 байт.
// ... ... ...


uint8_t program[2048];

uint64_t offset = 0;

// Функция превращает байт в печатный символ, а непечатные заменяет точкой
char to_ascii(uint8_t ascii)
{
   //#ifdef _WIN32
    //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 1. Получаем дескриптор консоли
    //CONSOLE_SCREEN_BUFFER_INFO consoleInfo; // 2. Сохраняем текущий цвет (чтобы не испортить последующий вывод)
    //WORD saved_attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // дефолтный белый
    //if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) { saved_attributes = consoleInfo.wAttributes; }
   //#endif
    switch (ascii){
    case '\0': return '·';
    case '\t': return '·';
    case '\n': return '·';
    case '\r': return '·';
    case 0x13: return '·'; // ?
    }
    return ascii;
    //return (b >= 32 && b <= 126) ? (char) b : '·';
}

bool is_this_printable_character(uint8_t ascii)
{
    switch (ascii){
    case '\0': return false;
    case '\t': return false;
    case '\n': return false;
    case '\r': return false;
    case 0x13: return false; // ?
    }
    return true;
}

void print_colored_char(uint8_t b)
{
    // Если символ нечитаемый (управляющий или нулевой)
    if (b < 32 || b > 126)
    {
        #ifdef _WIN32
         HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 1. Получаем дескриптор консоли
         CONSOLE_SCREEN_BUFFER_INFO consoleInfo; // 2. Сохраняем текущий цвет (чтобы не испортить последующий вывод)
         WORD saved_attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // дефолтный белый
         if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) { saved_attributes = consoleInfo.wAttributes; }
         // 3. Устанавливаем ярко-красный цвет текста (FOREGROUND_INTENSITY делает цвет сочным)
         SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
         printf("."); // 4. Печатаем точку
         SetConsoleTextAttribute(hConsole, saved_attributes); // 5. Возвращаем исходный цвет обратно
        #else
         // Для Linux/macOS оставляем ANSI-коды, там они работают из коробки
         printf("\033[1;31m.\033[0m");
        #endif
    }
    else putchar(b); // Обычный читаемый ASCII символ печатаем как есть
}

void console_log(
 uint8_t size, uint64_t loc_offset,
 uint8_t cell_1, uint8_t cell_2, uint8_t cell_3, uint8_t cell_4,
  uint8_t cell_5, uint8_t cell_6, uint8_t cell_7, uint8_t cell_8,
 uint64_t value, const char * abbreviation
)
{
   #ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 1. Получаем дескриптор консоли
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo; // 2. Сохраняем текущий цвет (чтобы не испортить последующий вывод)
    WORD saved_attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // дефолтный белый
    if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) { saved_attributes = consoleInfo.wAttributes; }
   #endif
    switch (size){
    case 1:
    {
        printf("\n  %d: %03d | %X: %02X | '", loc_offset, cell_1, loc_offset, cell_1);
        if (is_this_printable_character(cell_1))
        {
            printf("\n СИМВОЛ ЯВЛЯЕТСЯ ПЕЧАТНЫМ.\n ");
            putchar(cell_1);
        }
        else
        {
            printf("\n СИМВОЛ НЕ ЯВЛЯЕТСЯ ПЕЧАТНЫМ!!!\n ");
            // 3. Устанавливаем ярко-красный цвет текста (FOREGROUND_INTENSITY делает цвет сочным)
           #ifdef _WIN32
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
           #endif
            putchar(cell_1);
           #ifdef _WIN32
            SetConsoleTextAttribute(hConsole, saved_attributes); // 5. Возвращаем исходный цвет обратно
           #endif
        }
        printf("' | uint8_t %s = %u; // 0x%02X", abbreviation, value, value);
        break;
    }
    case 2:
    {
        printf("\n  %d: %03d %03d | %X: %02X %02X | \"", loc_offset, cell_1, cell_2, loc_offset, cell_1, cell_2);
        if (!is_this_printable_character(cell_1))
        {
            printf("\n СИМВОЛ-1 НЕ ПЕЧАТНЫЙ!!!\n ");
            // 3. Устанавливаем ярко-красный цвет текста (FOREGROUND_INTENSITY делает цвет сочным)
           #ifdef _WIN32
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
           #endif
            putchar(cell_1);
           #ifdef _WIN32
            SetConsoleTextAttribute(hConsole, saved_attributes); // 5. Возвращаем исходный цвет обратно
           #endif
        }
        else
        {
            //printf("\n СИМВОЛ-1 ЯВЛЯЕТСЯ ПЕЧАТНЫМ.\n ");
           #ifdef _WIN32
            SetConsoleTextAttribute(hConsole, saved_attributes); // 5. Возвращаем исходный цвет обратно
           #endif
           putchar(cell_1);
        }
        if (!is_this_printable_character(cell_2))
        {
            //printf("\n СИМВОЛ-2 НЕ ПЕЧАТНЫЙ!!!\n ");
            // 3. Устанавливаем ярко-красный цвет текста (FOREGROUND_INTENSITY делает цвет сочным)
           #ifdef _WIN32
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
           #endif
            putchar('·');
           #ifdef _WIN32
            SetConsoleTextAttribute(hConsole, saved_attributes); // 5. Возвращаем исходный цвет обратно
           #endif
        }
        else
        {
            //printf("\n СИМВОЛ-2 ЯВЛЯЕТСЯ ПЕЧАТНЫМ.\n ");
           #ifdef _WIN32
            SetConsoleTextAttribute(hConsole, saved_attributes); // 5. Возвращаем исходный цвет обратно
           #endif
           putchar(cell_2);
        }
        printf("\" | uint16_t %s = %u; // 0x%04X", abbreviation, value, value);
        break;
    }
    case 4:
    {
        printf("\n  %d: %03d %03d %03d %03d | %X: %02X %02X %02X %02X | \"%c%c%c%c\" | uint32_t %s = %u; // 0x%08X",
         loc_offset, cell_1, cell_2, cell_3, cell_4,
         loc_offset, cell_1, cell_2, cell_3, cell_4,
         to_ascii(cell_1), to_ascii(cell_2), to_ascii(cell_3), to_ascii(cell_4),
         abbreviation, value, value
        );
        break;
    }
    case 8:
    {
        printf("\n  %d: %03d %03d %03d %03d %03d %03d %03d %03d | %X: %02X %02X %02X %02X %02X %02X %02X %02X | \"%c%c%c%c%c%c%c%c\" | uint64_t %s = %u; // 0x%016X",
         loc_offset, cell_1, cell_2, cell_3, cell_4, cell_5, cell_6, cell_7, cell_8,
         loc_offset, cell_1, cell_2, cell_3, cell_4, cell_5, cell_6, cell_7, cell_8,
         to_ascii(cell_1), to_ascii(cell_2), to_ascii(cell_3), to_ascii(cell_4),
          to_ascii(cell_5), to_ascii(cell_6), to_ascii(cell_7), to_ascii(cell_8),
         abbreviation, value, value
        );
        break;
    }}
}

void pe_builder()
{
    FILE * descriptor = fopen("test_subject.exe", "wb");
    if (!descriptor) return;
    fwrite("MZ", 1, 2, descriptor); // e_magic  | 000: 077 090 | 00: 4D 5A | MZ
    uint8_t dos_reserved[58] = {0}; // Гарантируем ровно 58 байт нулей в зарезервированной зоне DOS
    fwrite(dos_reserved, 1, 58, descriptor);
    fwrite("\x40\x00\x00\x00", 1, 4, descriptor); // uint32_t e_lfanew              -|-  Адрес PE-сигнатуры (0x00000040 = 64 в десятичной)
    fwrite("PE\0\0", 1, 4, descriptor);           // uint32_t pe_signature          -|-  PE-сигнатура
    fwrite("\x64\x86", 1, 2, descriptor);         // uint16_t Machine               -|-  Архитектура процессора
    fwrite("\x00\x01", 1, 2, descriptor);         // uint16_t NumberOfSections      -|-  Количество секций
    fwrite("\x00\x00\x00\x00", 1, 4, descriptor); // uint32_t TimeDateStamp         -|-  Время создания файла
    fwrite("\x00\x00\x00\x00", 1, 4, descriptor); // uint32_t PointerToSymbolTable  -|-  Символьная таблица (для дебага, у нас 0)
    //fwrite("\x00\x00\x00\x00", 1, 4, descriptor); // uint32_t NumberOfSymbols;      -|-  Количество символов
    fclose(descriptor);
}

void pe_analyzer()
{
    //FILE * descriptor = fopen("test_subject.exe", "rb");
    FILE * descriptor = fopen("pe_tool.exe", "rb");
    if (!descriptor) return;
    printf("\n  ____________________________________");
    printf("\n /                                    \\");
    printf("\n %c БЛОК 1: DOS ЗАГОЛОВОК (DOS Header) %c", 16, 17);
    printf("\n \\____________________________________/");
    //printf("\n    ____________________________________");
    //printf("\n __/ БЛОК 1: DOS ЗАГОЛОВОК (DOS Header) \\__");
    if (fread(&e_magic.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ e_magic");*/ return; }
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    console_log(2, 0, e_magic.bytes[0], e_magic.bytes[1], 0, 0, 0, 0, 0, 0, e_magic.value, "e_magic");
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&e_cblp.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ e_cblp");*/ return; }
    console_log(2, 2, e_cblp.bytes[0], e_cblp.bytes[1], 0, 0, 0, 0, 0, 0, e_cblp.value, "e_cblp");
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    // Размер массива должен быть строго 36 байт (40 - 2 - 2 байта MZ)
    uint8_t dos_reserved[36];
    if (fread(dos_reserved, 1, 36, descriptor) != 36) { /*printf("\n  Ошибка чтения по смещению 004~039 | 02~27");*/ return; }
    printf("\n  4: %03d %03d | 4: %02X %02X | \"%c%c\" | uint16_t e_cp = ?;",
     dos_reserved[0], dos_reserved[1], dos_reserved[0], dos_reserved[1], to_ascii(dos_reserved[0]), to_ascii(dos_reserved[1])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  6: %03d %03d | 6: %02X %02X | \"%c%c\" | uint16_t e_crlc = ?;",
     dos_reserved[2], dos_reserved[3], dos_reserved[2], dos_reserved[3], to_ascii(dos_reserved[2]), to_ascii(dos_reserved[3])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  8: %03d %03d | 8: %02X %02X | \"%c%c\" | uint16_t e_cparhdr = ?;",
     dos_reserved[4], dos_reserved[5], dos_reserved[4], dos_reserved[5], to_ascii(dos_reserved[4]), to_ascii(dos_reserved[5])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  10: %03d %03d | A: %02X %02X | \"%c%c\" | uint16_t e_minalloc = ?;",
     dos_reserved[6], dos_reserved[7], dos_reserved[6], dos_reserved[7], to_ascii(dos_reserved[6]), to_ascii(dos_reserved[7])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  12: %03d %03d | C: %02X %02X | \"%c%c\" | uint16_t e_maxalloc = ?;",
     dos_reserved[8], dos_reserved[9], dos_reserved[8], dos_reserved[9], to_ascii(dos_reserved[8]), to_ascii(dos_reserved[9])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  14: %03d %03d | E: %02X %02X | \"%c%c\" | uint16_t e_ss = ?;",
     dos_reserved[10], dos_reserved[11], dos_reserved[10], dos_reserved[11], to_ascii(dos_reserved[10]), to_ascii(dos_reserved[11])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  16: %03d %03d | 10: %02X %02X | \"%c%c\" | uint16_t e_sp = ?;",
     dos_reserved[12], dos_reserved[13], dos_reserved[12], dos_reserved[13], to_ascii(dos_reserved[12]), to_ascii(dos_reserved[13])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  18: %03d %03d | 12: %02X %02X | \"%c%c\" | uint16_t e_csum = ?;",
     dos_reserved[14], dos_reserved[15], dos_reserved[14], dos_reserved[15], to_ascii(dos_reserved[14]), to_ascii(dos_reserved[15])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  20: %03d %03d | 14: %02X %02X | \"%c%c\" | uint16_t e_ip = ?;",
     dos_reserved[16], dos_reserved[17], dos_reserved[16], dos_reserved[17], to_ascii(dos_reserved[16]), to_ascii(dos_reserved[17])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  22: %03d %03d | 16: %02X %02X | \"%c%c\" | uint16_t e_cs = ?;",
     dos_reserved[18], dos_reserved[19], dos_reserved[18], dos_reserved[19], to_ascii(dos_reserved[18]), to_ascii(dos_reserved[19])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  24: %03d %03d | 18: %02X %02X | \"%c%c\" | uint16_t e_lfarlc = ?;",
     dos_reserved[20], dos_reserved[21], dos_reserved[20], dos_reserved[21], to_ascii(dos_reserved[20]), to_ascii(dos_reserved[21])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  26: %03d %03d | 1A: %02X %02X | \"%c%c\" | uint16_t e_ovno = ?;",
     dos_reserved[22], dos_reserved[23], dos_reserved[22], dos_reserved[23], to_ascii(dos_reserved[22]), to_ascii(dos_reserved[23])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  28: %03d %03d | 1C: %02X %02X | \"%c%c\" | uint16_t e_res[4] = {?,",
     dos_reserved[24], dos_reserved[25], dos_reserved[24], dos_reserved[25], to_ascii(dos_reserved[24]), to_ascii(dos_reserved[25])
    );
    printf("\n  30: %03d %03d | 1E: %02X %02X | \"%c%c\" | \t\t\t ?,",
     dos_reserved[26], dos_reserved[27], dos_reserved[26], dos_reserved[27], to_ascii(dos_reserved[26]), to_ascii(dos_reserved[27])
    );
    printf("\n  32: %03d %03d | 20: %02X %02X | \"%c%c\" | \t\t\t ?,",
     dos_reserved[28], dos_reserved[29], dos_reserved[28], dos_reserved[29], to_ascii(dos_reserved[28]), to_ascii(dos_reserved[29])
    );
    printf("\n  34: %03d %03d | 22: %02X %02X | \"%c%c\" | \t\t\t ?};",
     dos_reserved[30], dos_reserved[31], dos_reserved[30], dos_reserved[31], to_ascii(dos_reserved[30]), to_ascii(dos_reserved[31])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  36: %03d %03d | 24: %02X %02X | \"%c%c\" | uint16_t e_oemid = ?;",
     dos_reserved[32], dos_reserved[33], dos_reserved[32], dos_reserved[33], to_ascii(dos_reserved[32]), to_ascii(dos_reserved[33])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  38: %03d %03d | 26: %02X %02X | \"%c%c\" | uint16_t e_oeminfo = ?;",
     dos_reserved[34], dos_reserved[35], dos_reserved[34], dos_reserved[35], to_ascii(dos_reserved[34]), to_ascii(dos_reserved[35])
    );
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    //fseek(descriptor, 40, SEEK_SET);
    //printf("\n ТЕКУЩАЯ ПОЗИЦИЯ В ФАЙЛЕ: %ld", ftell(descriptor));
    if (fread(&e_res2[0].value, 2, 10, descriptor) != 10) { /*printf("\n Ошибка чтения e_res2[10]");*/ return; }
    printf("\n  40: %03d %03d | 28: %02X %02X | \"%c%c\" | uint16_t e_res2[10] = {%u,",
     e_res2[0].bytes[0], e_res2[0].bytes[1],
     e_res2[0].bytes[0], e_res2[0].bytes[1],
     to_ascii(e_res2[0].bytes[0]), to_ascii(e_res2[0].bytes[1]),
     e_res2[0].value
    );
    offset = 42;
    for (unsigned char i = 1; i <= 8; i++)
    {
        //offset += 2;
        printf("\n  %d: %03d %03d | %02X: %02X %02X | \"%c%c\" | \t\t\t   %u,",
         offset, e_res2[i].bytes[0], e_res2[i].bytes[1],
         offset, e_res2[i].bytes[0], e_res2[i].bytes[1],
         to_ascii(e_res2[i].bytes[0]), to_ascii(e_res2[i].bytes[1]),
         e_res2[i].value
        );
        offset += 2;
    }
    printf("\n  58: %03d %03d | 3A: %02X %02X | \"%c%c\" | \t\t\t   %u};",
     e_res2[9].bytes[0], e_res2[9].bytes[1],
     e_res2[9].bytes[0], e_res2[9].bytes[1],
     to_ascii(e_res2[9].bytes[0]), to_ascii(e_res2[9].bytes[1]),
     e_res2[9].value
    );
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    //printf("\n %zu", sizeof (union__uint16_t)); // 2
    if (fread(&e_lfanew.value, 4, 1, descriptor) != 1) { /*printf("\n Ошибка чтения e_lfanew");*/ return; }
    printf("\n  060: %03d %03d %03d %03d | 3C: %02X %02X %02X %02X | \"%c%c%c%c\" | uint32_t e_lfanew = %u; // 0x%08X",
     e_lfanew.bytes[0], e_lfanew.bytes[1], e_lfanew.bytes[2], e_lfanew.bytes[3],
     e_lfanew.bytes[0], e_lfanew.bytes[1], e_lfanew.bytes[2], e_lfanew.bytes[3],
     to_ascii(e_lfanew.bytes[0]), to_ascii(e_lfanew.bytes[1]), to_ascii(e_lfanew.bytes[2]), to_ascii(e_lfanew.bytes[3]),
     e_lfanew.value, e_lfanew.value
    );
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    // С этого момента структура блоков (её полей) может иметь разное смещение
    if (e_lfanew.value > 64)
    {
        for (int i = 64, byte = 0; i < e_lfanew.value; i++)
        {
            byte = getc(descriptor);
            printf("\n  %03d: %03d | %02X: %02X | '%c' |", i, byte, i, byte, to_ascii(byte));
        }
        printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    }
    offset = e_lfanew.value;
    printf("\n  _________________________________________");
    printf("\n /                                         \\");
    printf("\n %c БЛОК 2: PE ЗАГОЛОВОК (COFF File Header) %c", 16, 17);
    printf("\n \\_________________________________________/");
    //printf("\n    _________________________________________");
    //printf("\n __/ БЛОК 2: PE ЗАГОЛОВОК (COFF File Header) \\__");
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&pe_signature.value, 4, 1, descriptor) != 1) { /*printf("\n Ошибка чтения pe_signature");*/ return; }
    printf("\n  %03d: %03d | %02X: %02X | '%c' | uint32_t pe_signature = %u; // 0x%08X", offset, pe_signature.bytes[0], offset, pe_signature.bytes[0], to_ascii(pe_signature.bytes[0]), pe_signature.value, pe_signature.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                             offset, pe_signature.bytes[1], offset, pe_signature.bytes[1], to_ascii(pe_signature.bytes[1]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                             offset, pe_signature.bytes[2], offset, pe_signature.bytes[2], to_ascii(pe_signature.bytes[2]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                             offset, pe_signature.bytes[3], offset, pe_signature.bytes[3], to_ascii(pe_signature.bytes[3]));
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Machine.value, 2, 1, descriptor) != 1) { /*printf("\n Ошибка чтения Machine");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint16_t Machine = %u; // 0x%04X", offset, Machine.bytes[0], offset, Machine.bytes[0], to_ascii(Machine.bytes[0]), Machine.value, Machine.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                  offset, Machine.bytes[1], offset, Machine.bytes[1], to_ascii(Machine.bytes[1]));
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&NumberOfSections.value, 2, 1, descriptor) != 1) { /*printf("\n Ошибка чтения NumberOfSections");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint16_t NumberOfSections = %u; // 0x%04X", offset, NumberOfSections.bytes[0], offset, NumberOfSections.bytes[0], to_ascii(NumberOfSections.bytes[0]), NumberOfSections.value, NumberOfSections.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                           offset, NumberOfSections.bytes[1], offset, NumberOfSections.bytes[1], to_ascii(NumberOfSections.bytes[1]));
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&TimeDateStamp.value, 4, 1, descriptor) != 1) { /*printf("\n Ошибка чтения TimeDateStamp");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint32_t TimeDateStamp = %u; // 0x%08X", offset, TimeDateStamp.bytes[0], offset, TimeDateStamp.bytes[0], to_ascii(TimeDateStamp.bytes[0]), TimeDateStamp.value, TimeDateStamp.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                        offset, TimeDateStamp.bytes[1], offset, TimeDateStamp.bytes[1], to_ascii(TimeDateStamp.bytes[1]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                        offset, TimeDateStamp.bytes[2], offset, TimeDateStamp.bytes[2], to_ascii(TimeDateStamp.bytes[2]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                        offset, TimeDateStamp.bytes[3], offset, TimeDateStamp.bytes[3], to_ascii(TimeDateStamp.bytes[3]));
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&PointerToSymbolTable.value, 4, 1, descriptor) != 1) { /*printf("\n Ошибка чтения PointerToSymbolTable");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint32_t PointerToSymbolTable = %u; // 0x%08X", offset, PointerToSymbolTable.bytes[0], offset, PointerToSymbolTable.bytes[0], to_ascii(PointerToSymbolTable.bytes[0]), PointerToSymbolTable.value, PointerToSymbolTable.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                               offset, PointerToSymbolTable.bytes[1], offset, PointerToSymbolTable.bytes[1], to_ascii(PointerToSymbolTable.bytes[1]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                               offset, PointerToSymbolTable.bytes[2], offset, PointerToSymbolTable.bytes[2], to_ascii(PointerToSymbolTable.bytes[2]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                               offset, PointerToSymbolTable.bytes[3], offset, PointerToSymbolTable.bytes[3], to_ascii(PointerToSymbolTable.bytes[3]));
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&NumberOfSymbols.value, 4, 1, descriptor) != 1) { /*printf("\n Ошибка чтения NumberOfSymbols");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint32_t NumberOfSymbols = %u; // 0x%08X", offset, NumberOfSymbols.bytes[0], offset, NumberOfSymbols.bytes[0], to_ascii(NumberOfSymbols.bytes[0]), NumberOfSymbols.value, NumberOfSymbols.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                          offset, NumberOfSymbols.bytes[1], offset, NumberOfSymbols.bytes[1], to_ascii(NumberOfSymbols.bytes[1]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                          offset, NumberOfSymbols.bytes[2], offset, NumberOfSymbols.bytes[2], to_ascii(NumberOfSymbols.bytes[2]));
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                          offset, NumberOfSymbols.bytes[3], offset, NumberOfSymbols.bytes[3], to_ascii(NumberOfSymbols.bytes[3]));
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfOptionalHeader.value, 2, 1, descriptor) != 1) { /*printf("\n Ошибка чтения SizeOfOptionalHeader");*/ return; }
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' | uint16_t SizeOfOptionalHeader = %u; // 0x%04X", offset, SizeOfOptionalHeader.bytes[0], offset, SizeOfOptionalHeader.bytes[0], to_ascii(SizeOfOptionalHeader.bytes[0]), SizeOfOptionalHeader.value, SizeOfOptionalHeader.value);
    offset++; printf("\n  %03d: %03d | %02X: %02X | '%c' |",                                               offset, SizeOfOptionalHeader.bytes[1], offset, SizeOfOptionalHeader.bytes[1], to_ascii(SizeOfOptionalHeader.bytes[1]));
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Characteristics.value, 2, 1, descriptor) != 1) { /*printf("\n Ошибка чтения Characteristics");*/ return; }
    offset++;
    printf("\n  %03d: %03d %03d | %02X: %02X %02X | \"%c%c\" | uint16_t Characteristics = %u; // 0x%04X",
     offset, Characteristics.bytes[0], Characteristics.bytes[1],
     offset, Characteristics.bytes[0], Characteristics.bytes[1],
     to_ascii(Characteristics.bytes[0]), to_ascii(Characteristics.bytes[1]),
     Characteristics.value, Characteristics.value
    );
    offset += 2;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Magic.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ Magic");*/ return; }
    switch (Magic.value){
    case 0x010B:
     printf("\n    _______________________________________");
     printf("\n __/ БЛОК 3: OPTIONAL HEADER (PE32/32-bit) \\__");
     break;
    case 0x020B:
     printf("\n  ________________________________________");
     printf("\n /                                        \\");
     printf("\n %c БЛОК 3: OPTIONAL HEADER (PE32+/64-bit) %c", 16, 17);
     printf("\n \\________________________________________/");
     //printf("\n    ________________________________________");
     //printf("\n __/ БЛОК 3: OPTIONAL HEADER (PE32+/64-bit) \\__");
     break;
    default:
     printf("\n    _________________________");
     printf("\n __/ БЛОК 3: OPTIONAL HEADER \\__");
    }
    // Часть 1: Стандартные поля COFF (Standard Fields) — одинаковые для 32/64 бит
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    // Далее произведён рефакторинг кода (функция printf перемещена в console_log). Сделано это для уменьшения объёма (дублирования) кода.
    console_log(2, offset, Magic.bytes[0], Magic.bytes[1], 0, 0, 0, 0, 0, 0, Magic.value, "Magic");
    offset += 2;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    MajorLinkerVersion = getc(descriptor);
    console_log(1, offset, MajorLinkerVersion, 0, 0, 0, 0, 0, 0, 0, MajorLinkerVersion, "MajorLinkerVersion");
    offset++;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    MinorLinkerVersion = getc(descriptor);
    console_log(1, offset, MinorLinkerVersion, 0, 0, 0, 0, 0, 0, 0, MinorLinkerVersion, "MinorLinkerVersion");
    offset++;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfCode.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfCode");*/ return; }
    console_log(4, offset,
     SizeOfCode.bytes[0], SizeOfCode.bytes[1], SizeOfCode.bytes[2], SizeOfCode.bytes[3], 0, 0, 0, 0,
     SizeOfCode.value, "SizeOfCode"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfInitializedData.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfInitializedData");*/ return; }
    console_log(4, offset,
     SizeOfInitializedData.bytes[0], SizeOfInitializedData.bytes[1], SizeOfInitializedData.bytes[2], SizeOfInitializedData.bytes[3], 0, 0, 0, 0,
     SizeOfInitializedData.value, "SizeOfInitializedData"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfUninitializedData.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfUninitializedData");*/ return; }
    console_log(4, offset,
     SizeOfUninitializedData.bytes[0], SizeOfUninitializedData.bytes[1], SizeOfUninitializedData.bytes[2], SizeOfUninitializedData.bytes[3], 0, 0, 0, 0,
     SizeOfUninitializedData.value, "SizeOfUninitializedData"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&AddressOfEntryPoint.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ AddressOfEntryPoint");*/ return; }
    console_log(4, offset,
     AddressOfEntryPoint.bytes[0], AddressOfEntryPoint.bytes[1], AddressOfEntryPoint.bytes[2], AddressOfEntryPoint.bytes[3], 0, 0, 0, 0,
     AddressOfEntryPoint.value, "AddressOfEntryPoint"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&BaseOfCode.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ BaseOfCode");*/ return; }
    console_log(4, offset,
     BaseOfCode.bytes[0], BaseOfCode.bytes[1], BaseOfCode.bytes[2], BaseOfCode.bytes[3], 0, 0, 0, 0,
     BaseOfCode.value, "BaseOfCode"
    );
    offset += 4;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    // [Примечание]: Только в PE32 здесь идёт ещё 4 байта uint32_t BaseOfData. В 64-битном PE32+ этого поля вообще нет!
    //if (Magic.value == 0x010B) { ... }
    // Часть 2: Специфичные поля Windows (Windows-Specific Fields) — размеры различаются!
    if (fread(&ImageBase.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ ImageBase");*/ return; }
    console_log(8, offset,
     ImageBase.bytes[0], ImageBase.bytes[1], ImageBase.bytes[2], ImageBase.bytes[3], ImageBase.bytes[4], ImageBase.bytes[5], ImageBase.bytes[6], ImageBase.bytes[7],
     ImageBase.value, "ImageBase"
    );
    offset += 8;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SectionAlignment.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SectionAlignment");*/ return; }
    console_log(4, offset,
     SectionAlignment.bytes[0], SectionAlignment.bytes[1], SectionAlignment.bytes[2], SectionAlignment.bytes[3], 0, 0, 0, 0,
     SectionAlignment.value, "SectionAlignment"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&FileAlignment.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ FileAlignment");*/ return; }
    console_log(4, offset,
     FileAlignment.bytes[0], FileAlignment.bytes[1], FileAlignment.bytes[2], FileAlignment.bytes[3], 0, 0, 0, 0,
     FileAlignment.value, "FileAlignment"
    );
    offset += 4;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MajorOperatingSystemVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MajorOperatingSystemVersion");*/ return; }
    console_log(2, offset, MajorOperatingSystemVersion.bytes[0], MajorOperatingSystemVersion.bytes[1], 0, 0, 0, 0, 0, 0,
     MajorOperatingSystemVersion.value, "MajorOperatingSystemVersion"
    );
    offset += 2;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MinorOperatingSystemVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MinorOperatingSystemVersion");*/ return; }
    console_log(2, offset, MinorOperatingSystemVersion.bytes[0], MinorOperatingSystemVersion.bytes[1], 0, 0, 0, 0, 0, 0,
     MinorOperatingSystemVersion.value, "MinorOperatingSystemVersion"
    );
    offset += 2;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MajorImageVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MajorImageVersion = ?");*/ return; }
    console_log(2, offset, MajorImageVersion.bytes[0], MajorImageVersion.bytes[1], 0, 0, 0, 0, 0, 0,
     MajorImageVersion.value, "MajorImageVersion"
    );
    offset += 2;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MinorImageVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MinorImageVersion = ?");*/ return; }
    console_log(2, offset, MinorImageVersion.bytes[0], MinorImageVersion.bytes[1], 0, 0, 0, 0, 0, 0,
     MinorImageVersion.value, "MinorImageVersion"
    );
    offset += 2;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MajorSubsystemVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MajorSubsystemVersion = ?");*/ return; }
    console_log(2, offset, MajorSubsystemVersion.bytes[0], MajorSubsystemVersion.bytes[1], 0, 0, 0, 0, 0, 0,
     MajorSubsystemVersion.value, "MajorSubsystemVersion"
    );
    offset += 2;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MinorSubsystemVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MinorSubsystemVersion = ?");*/ return; }
    console_log(2, offset, MinorSubsystemVersion.bytes[0], MinorSubsystemVersion.bytes[1], 0, 0, 0, 0, 0, 0,
     MinorSubsystemVersion.value, "MinorSubsystemVersion"
    );
    offset += 2;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Win32VersionValue.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ Win32VersionValue = ?");*/ return; }
    console_log(4, offset, Win32VersionValue.bytes[0], Win32VersionValue.bytes[1], Win32VersionValue.bytes[2], Win32VersionValue.bytes[3], 0, 0, 0, 0,
     Win32VersionValue.value, "Win32VersionValue"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfImage.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfImage = ?");*/ return; }
    console_log(4, offset, SizeOfImage.bytes[0], SizeOfImage.bytes[1], SizeOfImage.bytes[2], SizeOfImage.bytes[3], 0, 0, 0, 0,
     SizeOfImage.value, "SizeOfImage"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeaders.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfHeaders = ?");*/ return; }
    console_log(4, offset, SizeOfHeaders.bytes[0], SizeOfHeaders.bytes[1], SizeOfHeaders.bytes[2], SizeOfHeaders.bytes[3], 0, 0, 0, 0,
     SizeOfHeaders.value, "SizeOfHeaders"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&CheckSum.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ CheckSum = ?");*/ return; }
    console_log(4, offset, CheckSum.bytes[0], CheckSum.bytes[1], CheckSum.bytes[2], CheckSum.bytes[3], 0, 0, 0, 0,
     CheckSum.value, "CheckSum"
    );
    offset += 4;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Subsystem.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ Subsystem = ?");*/ return; }
    console_log(2, offset, Subsystem.bytes[0], Subsystem.bytes[1], 0, 0, 0, 0, 0, 0,
     Subsystem.value, "Subsystem"
    );
    offset += 2;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&DllCharacteristics.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ DllCharacteristics = ?");*/ return; }
    console_log(2, offset, DllCharacteristics.bytes[0], DllCharacteristics.bytes[1], 0, 0, 0, 0, 0, 0,
     DllCharacteristics.value, "DllCharacteristics"
    );
    offset += 2;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfStackReserve.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfStackReserve");*/ return; }
    console_log(8, offset,
     SizeOfStackReserve.bytes[0], SizeOfStackReserve.bytes[1], SizeOfStackReserve.bytes[2], SizeOfStackReserve.bytes[3],
      SizeOfStackReserve.bytes[4], SizeOfStackReserve.bytes[5], SizeOfStackReserve.bytes[6], SizeOfStackReserve.bytes[7],
     SizeOfStackReserve.value, "SizeOfStackReserve"
    );
    offset += 8;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfStackCommit.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfStackCommit");*/ return; }
    console_log(8, offset,
     SizeOfStackCommit.bytes[0], SizeOfStackCommit.bytes[1], SizeOfStackCommit.bytes[2], SizeOfStackCommit.bytes[3],
      SizeOfStackCommit.bytes[4], SizeOfStackCommit.bytes[5], SizeOfStackCommit.bytes[6], SizeOfStackCommit.bytes[7],
     SizeOfStackCommit.value, "SizeOfStackCommit"
    );
    offset += 8;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeapReserve.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfHeapReserve");*/ return; }
    console_log(8, offset,
     SizeOfHeapReserve.bytes[0], SizeOfHeapReserve.bytes[1], SizeOfHeapReserve.bytes[2], SizeOfHeapReserve.bytes[3],
      SizeOfHeapReserve.bytes[4], SizeOfHeapReserve.bytes[5], SizeOfHeapReserve.bytes[6], SizeOfHeapReserve.bytes[7],
     SizeOfHeapReserve.value, "SizeOfHeapReserve"
    );
    offset += 8;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeapCommit.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfHeapCommit");*/ return; }
    console_log(8, offset,
     SizeOfHeapCommit.bytes[0], SizeOfHeapCommit.bytes[1], SizeOfHeapCommit.bytes[2], SizeOfHeapCommit.bytes[3],
      SizeOfHeapCommit.bytes[4], SizeOfHeapCommit.bytes[5], SizeOfHeapCommit.bytes[6], SizeOfHeapCommit.bytes[7],
     SizeOfHeapCommit.value, "SizeOfHeapCommit"
    );
    offset += 8;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&LoaderFlags.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ LoaderFlags = ?");*/ return; }
    console_log(4, offset, LoaderFlags.bytes[0], LoaderFlags.bytes[1], LoaderFlags.bytes[2], LoaderFlags.bytes[3], 0, 0, 0, 0,
     LoaderFlags.value, "LoaderFlags"
    );
    offset += 4;
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    if (fread(&NumberOfRvaAndSizes.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ NumberOfRvaAndSizes = ?");*/ return; }
    console_log(4, offset,
     NumberOfRvaAndSizes.bytes[0], NumberOfRvaAndSizes.bytes[1], NumberOfRvaAndSizes.bytes[2], NumberOfRvaAndSizes.bytes[3], 0, 0, 0, 0,
     NumberOfRvaAndSizes.value, "NumberOfRvaAndSizes"
    );
    offset += 4;
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    printf("\n  ____________________________________________");
    printf("\n /                                            \\");
    printf("\n %c БЛОК 4: КАТАЛОГИ ДАННЫХ (Data Directories) %c", 16, 17);
    printf("\n \\____________________________________________/");
    //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    char abbreviation[32];
    uint32_t dir_count = NumberOfRvaAndSizes.value;
    for (uint32_t i = 0; i < dir_count; i++)
    {
        printf("\n  __________________________");
        printf("\n / %s", directory_names[i]);
        sprintf(abbreviation, "virtual_address[%d]", i);
        //}
        if (fread(&virtual_address[i].value, 4, 1, descriptor) != 1) return;
        console_log(4, offset,
         virtual_address[i].bytes[0], virtual_address[i].bytes[1], virtual_address[i].bytes[2], virtual_address[i].bytes[3], 0, 0, 0, 0,
         virtual_address[i].value, abbreviation
        );
        offset += 4;
        sprintf(abbreviation, "size[%d]", i);
        if (fread(&size[i].value, 4, 1, descriptor) != 1) { return; }
        console_log(4, offset,
            size[i].bytes[0], size[i].bytes[1], size[i].bytes[2], size[i].bytes[3], 0, 0, 0, 0, size[i].value, abbreviation
        );
        offset += 4;
        //printf("\n -----------------------------------------------------------------------------------------------------------------------------");
    }
    printf("\n -----------------------------------------------------------------------------------------------------------------------------");
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

/*
    // АКТИВАЦИЯ ЦВЕТА В WINDOWS
   #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
   #endif
*/