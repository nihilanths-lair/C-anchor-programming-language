#ifdef _WIN32
 #include <windows.h>
#endif

#include <stdint.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Разворот 16-битного числа (WORD)
#define macro__to_le16(x) \
 ((((x) & 0x00FF) << 8) | \
 (((x) & 0xFF00) >> 8))

// Разворот 32-битного числа (DWORD)
#define macro__to_le32(x) \
 ((((x) & 0x000000FF) << 24) | \
 (((x) & 0x0000FF00) << 8) | \
 (((x) & 0x00FF0000) >> 8) | \
 (((x) & 0xFF000000) >> 24))

// Для 2 символов (например, 'M', 'Z') -> преобразует в uint16_t в Little-Endian
#define macro__str_le16(b1, b2) \
( \
 (uint16_t)(uint8_t)(b1) | \
 ((uint16_t)(uint8_t)(b2) << 8) \
)
// Для 4 символов (например, 'P', 'E', '\0', '\0') -> преобразует в uint32_t в Little-Endian
#define macro__str_le32(b1, b2, b3, b4) \
( \
 (uint32_t)(uint8_t)(b1) | \
 ((uint32_t)(uint8_t)(b2) << 8) | \
 ((uint32_t)(uint8_t)(b3) << 16) | \
 ((uint32_t)(uint8_t)(b4) << 24) \
)
// Для 8 символов (например, '.', 't', 'e', 'x', 't') -> преобразует в uint64_t в Little-Endian
#define macro__str_le64(b1, b2, b3, b4, b5, b6, b7, b8) \
( \
 (uint64_t)(uint8_t)(b1) | \
 ((uint64_t)(uint8_t)(b2) << 8) | \
 ((uint64_t)(uint8_t)(b3) << 16) | \
 ((uint64_t)(uint8_t)(b4) << 24) | \
 ((uint64_t)(uint8_t)(b5) << 32) | \
 ((uint64_t)(uint8_t)(b6) << 40) | \
 ((uint64_t)(uint8_t)(b7) << 48) | \
 ((uint64_t)(uint8_t)(b8) << 56) \
)

#define macro__align_up(size, align) (((size) + (align) - 1) & ~((align) - 1))

static inline uint32_t str_to_le32(const char * str)
{
    return
     ((uint32_t)(uint8_t) str[0]) |
     ((uint32_t)(uint8_t) str[1] << 8) |
     ((uint32_t)(uint8_t) str[2] << 16) |
     ((uint32_t)(uint8_t) str[3] << 24)
    ;
}
static inline uint64_t str_to_le64(const char * str)
{
    return
     ((uint64_t)(uint8_t) str[0]) |
     ((uint64_t)(uint8_t) str[1] << 8) |
     ((uint64_t)(uint8_t) str[2] << 16) |
     ((uint64_t)(uint8_t) str[3] << 24) |
     ((uint64_t)(uint8_t) str[4] << 32) |
     ((uint64_t)(uint8_t) str[5] << 40) |
     ((uint64_t)(uint8_t) str[6] << 48) |
     ((uint64_t)(uint8_t) str[7] << 56)
    ;
}

typedef union { uint8_t value; uint8_t bytes[1]; } union__uint8_t;
typedef union { uint16_t value; uint8_t bytes[2]; } union__uint16_t;
typedef union { uint32_t value; uint8_t bytes[4]; } union__uint32_t;
typedef union { uint64_t value; uint8_t bytes[8]; } union__uint64_t;

#pragma pack(push, 1)
// БЛОК 1: DOS HEADER.
//  Размер: Всегда строго 64 байта.
//  Природа: Статичный исторический балласт. Изменяется только одно поле `lfanew`.
typedef struct {
    uint16_t magic;        // 2 байта (000~001: 077 090 | 00~01: 4D 5A): LE 23'117 | MZ  @  Магическое число
    // Обычно забито нулями (0). Сюда можно спрятать кастомные метаданные компилятора, Windows их игнорирует.
    uint8_t  reserved[58]; // 58 байт (002~059 | 02~39)  @  Зазор (бесполезные поля, не несут нагрузку)
    //uint16_t reserved[29]; // 58 байт (002~059 | 02~39)  @  Зазор (бесполезные поля, не несут нагрузку)
    // Динамическое поле: Указывает смещение (в байтах от начала файла), где начнется Блок 2 (PE).
    // • Минимум: 64 (если DOS-код заглушки отсутствует).
    // • Динамика: Если ты решишь вставить туда реальную DOS-программу (которая пишет "This program cannot be run in DOS mode"),
    //  это поле сдвинется вперед на размер этого DOS-кода (обычно 128 или 248).
    uint32_t lfanew; // 4 байта (060~063: 064 | 3C~3F: 40): 64
} DosHeader;
#pragma pack(pop)
union__uint16_t dos_header__magic; // 2 байта (000~001: 077 090 | 00~01: 4D 5A): LE 23'117 | MZ  @  Магическое число
union__uint16_t dos_header__reserved[29]; // 58 байт (002~059 | 02~39)  @  Зазор (бесполезные поля, не несут нагрузку)
union__uint16_t dos_header__res[4];
union__uint16_t dos_header__reserved_2[2]; // 58 байт (002~059 | 02~39)  @  Зазор (бесполезные поля, не несут нагрузку)
union__uint16_t dos_header__res2[10];
union__uint32_t dos_header__lfanew; // 4 байта (060~063: 064 | 3C~3F: 40): 64
#pragma pack(push, 1)
typedef struct {
    union__uint16_t reserved[29]; // 58 байт (002~059 | 02~39)  @  Зазор (бесполезные поля, не несут нагрузку)
    union__uint16_t cblp;
    union__uint16_t cp;
    union__uint16_t crlc;
    union__uint16_t cparhdr;
    union__uint16_t minalloc;
    union__uint16_t maxalloc;
    union__uint16_t ss;
    union__uint16_t sp;
    union__uint16_t csum;
    union__uint16_t ip;
    union__uint16_t cs;
    union__uint16_t lfarlc;
    union__uint16_t ovno;
    union__uint16_t res[4];
    union__uint16_t oemid;
    union__uint16_t oeminfo;
    union__uint16_t res2[10];
} Analyzer__DosHeader;
#pragma pack(pop)
// Сигнатура PE в виде 32-битного числа (0x00004550)
// В памяти типа Little-Endian она запишется на диск как 'P' 'E' '\0' '\0'
const char pe_signature[4] = "PE\0\0"; // macro__str_le32('P', 'E', '\0', '\0'); // 0x00004550;
#pragma pack(push, 1)
typedef struct {
    uint16_t machine;                 // Архитектура процессора (для 32-бит x86 это 0x014C)
    uint16_t number_of_sections;      // Количество секций в файле
    uint32_t time_date_stamp;         // Время создания файла (POSIX timestamp)
    uint32_t pointer_to_symbol_table; // Смещение таблицы символов (для EXE обычно 0)
    uint32_t number_of_symbols;       // Количество символов (для EXE обычно 0)
    uint16_t size_of_optional_header; // Размер следующего за ним Опционального заголовка
    uint16_t characteristics;         // Флаги файла (что это исполняемый EXE, 32-бит и т.д.)
} FileHeader;
typedef struct { // Кирпичик для каталога данных (Занимает ровно 8 байт)
    uint32_t virtual_address; // Виртуальный адрес начала таблицы (RVA)
    uint32_t size;            // Размер таблицы в байтах
} DataDirectory;
typedef struct {
    // --- Стандартные поля (Standard Fields) ---
    uint16_t magic;         
    uint8_t  major_linker_version; 
    uint8_t  minor_linker_version;
    uint32_t size_of_code; 
    uint32_t size_of_initialized_data;
    uint32_t size_of_uninitialized_data;
    uint32_t address_of_entry_point;
    uint32_t base_of_code;
    // --- Дополнительные поля Windows (Windows-Specific Fields) ---
    uint64_t image_base;      
    uint32_t section_alignment;     
    uint32_t file_alignment;
    uint16_t major_operating_system_version;
    uint16_t minor_operating_system_version;
    uint16_t major_image_version;
    uint16_t minor_image_version;
    uint16_t major_subsystem_version;
    uint16_t minor_subsystem_version;
    uint32_t win32_version_value;   
    uint32_t size_of_image;          
    uint32_t size_of_headers;        
    uint32_t check_sum;            
    uint16_t subsystem;    
    uint16_t dll_characteristics;  
    uint64_t size_of_stack_reserve; 
    uint64_t size_of_stack_commit;
    uint64_t size_of_heap_reserve; 
    uint64_t size_of_heap_commit;   
    uint32_t loader_flags;
    uint32_t number_of_rva_and_sizes;  // Тут всегда будет записано число 16
    // --- НАШ ВЛОЖЕННЫЙ МАССИВ (Data Directories) ---
    // Физически продолжает заголовок. Занимает: 16 директорий * 8 байт = 128 байт.
    DataDirectory data_directories[16];
} OptionalHeader64;
typedef struct {
    union__uint64_t name;             // Имя секции (8 байт, например ".text\0\0\0")
    uint32_t virtual_size;            // Размер секции в оперативной памяти
    uint32_t virtual_address;         // Виртуальный адрес начала секции в памяти (RVA)
    uint32_t size_of_raw_data;        // Физический размер секции на жестком диске
    uint32_t pointer_to_raw_data;     // Физическое смещение начала секции в файле (RAW)
    uint32_t pointer_to_relocations;  // Смещение таблицы релокаций (для EXE обычно 0)
    uint32_t pointer_to_line_numbers; // Смещение номеров строк (всегда 0)
    uint16_t number_of_relocations;   // Количество релокаций (обычно 0)
    uint16_t number_of_line_numbers;  // Количество номеров строк (всегда 0)
    uint32_t characteristics;         // Флаги доступа (чтение, запись, выполнение)
} SectionHeader;
#pragma pack(pop)

// Продолжение ->
#pragma pack(push, 1)
// Описание одной импортируемой DLL (20 байт)
typedef struct {
    uint32_t import_lookup_table_rva;   // RVA на массив ILT (с именами)
    uint32_t time_date_stamp;           // 0
    uint32_t forwarder_chain;           // 0
    uint32_t name_rva;                  // RVA на строку с именем DLL (например, "kernel32.dll")
    uint32_t import_address_table_rva;  // RVA на массив IAT (куда ОС запишет адрес)
} ImportDescriptor;

// Структура Hint/Name для функции
typedef struct {
    uint16_t hint;                      // Очередь перебора (можно 0)
    char name[12];                      // "ExitProcess\0" (выровнено)
} ImportHintName;
#pragma pack(pop)

// --- //


// БЛОК 2: PE ЗАГОЛОВОК (COFF File Header) / 2. COFF Заголовок файла (Характеристики процессора)
//  Размер: Всегда строго 24 байта (4 байта сигнатура + 20 байт заголовок).
//  Природа: Задает базовые свойства файла.
union__uint32_t Signature;

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
union__uint8_t MajorLinkerVersion; // Необязательное поле (можно выставить 0)
union__uint8_t MinorLinkerVersion; // Необязательное поле (можно выставить 0)
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
typedef struct {
    uint8_t Name[8];
    union__uint32_t VirtualSize;
    union__uint32_t VirtualAddress;
    union__uint32_t SizeOfRawData;
    union__uint32_t PointerToRawData;
    uint8_t Reserved[12]; // Пропускаем легаси-поля (3 поля по 4 байта)
    union__uint32_t Characteristics;
} _SectionHeader;
_SectionHeader section_header[96]; // Ограничимся пока 96-ю секциями


uint8_t program[2048];
uint64_t offset = 0;

void print_offset_in_bytes(uint64_t offset)
{
    // Распиливаем 64-битное число на массив из 8 байт через указатель
    uint8_t *b = (uint8_t*) &offset;
    printf("\n Числовое значение: %lld (0x%016llX)", offset, offset);
    // 1. Выводим побайтово в Десятичной СС (8 байт по 3 цифры)
    printf("\n Десятичные байты: ");
    for (int i = 0; i < 8; i++) printf("%03d ", b[i]); // %03d делает "005" вместо "5"
    // 2. Выводим побайтово в Шестнадцатеричной СС (8 байт по 2 знака)
    printf("\n HEX-байты (Little): ");
    for (int i = 0; i < 8; i++) printf("%02X ", b[i]); // %02X делает "0A" вместо "A"
    printf("\n ------------------------------------------------");
}

/*

 1. Управляющие символы (0~31, 127)


 2. Печатные символы (32~126)


 3. Расширенные символы ASCII Win-1251 кириллица (128~255)
 149 = 0x95 = 10010101	•	&#149;	&bull;

*/
// Функция превращает байт в печатный символ, а непечатные заменяет точкой
char to_ascii(uint8_t ascii)
{
    switch (ascii){
    case '\0': return '·'; // 0: 000 = 00 = NUL
    case '\a': return '·'; // 7: 007 = 07 = BEL
    case '\b': return '·'; // 8: 008 = 08 = BS
    case '\t': return '·'; // 9: 009 = 09 = TAB
    case '\n': return '·'; // 10: 010 = 0A = LF
    case '\r': return '·'; // 13: 013 = 0D = CR
    }
    return ascii;
    //return (ascii >= 32 && ascii <= 126) ? (char) ascii : '·';
}

// Функция превращает непечатный байт в экранированную ascii-последовательность
const char * to_str_ascii(uint8_t ascii)
{
    // static сохраняет массив в памяти после выхода из функции. 5 байт хватит для любой строки.
    static char buffer[5];
    // Обрабатываем непечатные спецсимволы
    switch (ascii) {
    case '\0': return "\\0"; // 0x00: Нуль-терминатор
    case '\a': return "\\a"; // 0x07: Сигнал (Bell)
    case '\b': return "\\b"; // 0x08: Забой (Backspace)
    case '\t': return "\\t"; // 0x09: Табуляция
    case '\n': return "\\n"; // 0x0A: Перевод строки (Line Feed)
    //case '\v': return "\\v"; // 0x0B: Вертикальная табуляция
    //case '\f': return "\\f"; // 0x0C: Смена страницы (Form Feed)
    case '\r': return "\\r"; // 0x0D: Возврат каретки
    //case 27:   return "\\e"; // 0x1B: Escape-символ
    }
    // Для всех остальных непечатных байт (например, 0x01, 0x86, 0xFF)
    // возвращаем красивую универсальную заглушку с кодом символа
    buffer[0] = '\\';
    buffer[1] = '?';
    buffer[2] = '\0';
    return buffer;
    //return (ascii >= 32 && ascii <= 126) ? (char) ascii : '·';
}

bool is_this_printable_character(uint8_t ascii)
{
    switch (ascii){
    case '\0': return false; // 0: 000 = 00 = NUL
    case '\a': return false; // 7: 007 = 07 = BEL
    case '\b': return false; // 8: 008 = 08 = BS
    case '\t': return false; // 9: 009 = 09 = TAB
    case '\n': return false; // 10: 010 = 0A = LF
    case '\r': return false; // 13: 013 = 0D = CR
    }
    return true;
    //return (ascii >= 32 && ascii <= 126) ? true : false;
}

uint8_t symbol_adjustment(uint8_t ascii)
{
    switch (ascii){
    case '\0': // 0: 000 = 00 = NUL
    case '\a': // 7: 007 = 07 = BEL
    case '\b': // 8: 008 = 08 = BS
    case '\t': // 9: 009 = 09 = TAB
    case '\n': // 10: 010 = 0A = LF
    case '\r': // 13: 013 = 0D = CR
    case 149:  //     149 = 95 = •
        // Если символ нечитаемый (управляющий или нулевой)
        #ifdef _WIN32
         HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 1. Получаем дескриптор консоли
         CONSOLE_SCREEN_BUFFER_INFO consoleInfo; // 2. Сохраняем текущий цвет (чтобы не испортить последующий вывод)
         WORD saved_attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // дефолтный белый
         if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) { saved_attributes = consoleInfo.wAttributes; }
         // 3. Устанавливаем ярко-красный цвет текста (FOREGROUND_INTENSITY делает цвет сочным)
         SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
         //putchar(95); //putchar(35); //putchar(36); //putchar(21); //putchar(137); //putchar(20); //putchar(127);
         printf("%s", to_str_ascii(ascii)); //putchar(19); //putchar(14); //putchar(2); //putchar(164); //putchar(184);
         SetConsoleTextAttribute(hConsole, saved_attributes); // 5. Возвращаем исходный цвет обратно
        #else
         // Для Linux/macOS оставляем ANSI-коды, там они работают из коробки
         printf("\033[1;31m.\033[0m");
        #endif
        return 2;
    default:
        putchar(ascii); // Обычный читаемый ASCII символ печатаем как есть
        return 1;
    }
}
/*
 Object 1: Offset, параметры:
 Вывод: включён [по умолчанию].
 Как отображать смещение (в человекочитаемом виде/в машинном виде)?
 1: Без разбивки (непрерывная цепочка), с форматированием: недостающие цифры спереди забиваются нулями [по умолчанию].
 2: Бить число на отдельные десятичные байты (без форматирования).
 4: Бить число на отдельные шестнадцатеричные байты (без форматирования).
 8, 16, 32, 64, 128: - ?
*/
uint8_t regulator = 0; // configurator
uint8_t checksum = 1; // Контрольная сумма

void console_log(uint8_t size, uint32_t loc_offset, const uint8_t * bytes, uint64_t value, const char * abbreviation)
{
    switch (checksum){
    case 1:
    {
        // 1. Выводим числовой адрес в текстовом виде и сырые байты в десятичной системе счисления
        printf("\n %010lld:", loc_offset); // 0 ~ 4'294'967'295
        for (int i = 0; i < size; i++) printf(" %03d", bytes[i]);
        for (int i = size; i < 8; i++)
        {
            putchar(' ');
            symbol_adjustment(22);
            symbol_adjustment(22);
            symbol_adjustment(22);
        } // Выравнивание
        printf(" |");
        for (int i = 0; i < size; i++) printf(" %02X", bytes[i]);
        for (int i = size; i < 8; i++)
        {
            putchar(' ');
            symbol_adjustment(22);
            symbol_adjustment(22);
        } // Выравнивание
        // 3. Выводим символы с цветной обработкой
        printf(" | ");
        uint8_t printed_chars = 0; 
        for (int i = 0; i < size; i++) printed_chars += symbol_adjustment(bytes[i]);
        // === МАТЕМАТИЧЕСКИ ИДЕАЛЬНЫЙ ВАРИАНТ ===
        // Нам не важен размер size и сколько байт пропущено. 
        // Мы просто смотрим, сколько знаков (включая двойные \0) уже вывелось на экран,
        // и принудительно добиваем текстовое поле до фиксированной ширины в 16 знаков.
        int dots_to_print = 16 - printed_chars;
        // Если dots_to_print уходит в минус (на всякий случай защита), обнуляем
        if (dots_to_print < 0) dots_to_print = 0;
        // Забиваем оставшееся пространство знаками выравнивания
        for (int i = 0; i < dots_to_print; i++) putchar(22); // Выводим ваш символ '?' через putchar
        // 4. Печатаем тип данных и значение
        //const char * type_str = (size == 1) ? "" : (size == 2) ? "" : (size == 4) ? "" : " uint64_t";
        if (size != 1)
        {
            if (abbreviation[0] != '\0') printf(" | %s = %llu; // 0x", abbreviation, value); // "%s", type_str
            else printf(" |");
        } 
        // Красиво выводим HEX значение нужной разрядности
        //if (size == 1) printf("%02llX", value);
        if (size == 2) printf("%04llX", value);
        else if (size == 4) printf("%08llX", value);
        else if (size == 8)
        {
            if (abbreviation[0] != '\0') printf("%016llX", value);
            //else putchar('|');
        }
        else printf(" |");
    }
    break;
    default:
        for (int i = 0; i < size; i++) printf("%03d ", bytes[i]);
        for (int i = size; i < 8; i++) printf("    "); // Выравнивание
        // Распиливаем адрес loc_offset на массив из 8 байт
        uint8_t * bo = (uint8_t*) &loc_offset;
        // Определяем реальное количество байт, необходимых для отображения текущего адреса.
        // Для смещений в начале файла (до 255) хватит 1 байта, до 65535 - 2 байт, для PE обычно хватает 4 байт.
        // Сделаем фиксированно 4 байта (32-битный адрес), чтобы колонки в консоли не съезжали.
        int addr_size = 4;
        // 2. Выводим Byte offset (dec) в формате Big-Endian (от старшего байта к младшему)
        for (int i = addr_size - 1; i >= 0; i--) printf("%03d", bo[i]); // липкие байты
        printf("(be) = ");
        // 3. Выводим Byte offset (dec) в формате Little-Endian (от младшего байта к старшему)
        for (int i = 0; i < addr_size; i++) printf("%03d", bo[i]); // липкие байты
        printf("(le) :: ");
        // 4. Выводим Byte offset (hex) в формате Big-Endian
        for (int i = addr_size - 1; i >= 0; i--) printf("%02X ", bo[i]);
        printf("(be) = ");
        // 5. Выводим Byte offset (hex) в формате Little-Endian
        for (int i = 0; i < addr_size; i++) printf("%02X ", bo[i]);
        printf("(le): ");
        // 6. ПРАВАЯ ЧАСТЬ (Вывод самих данных, которые мы прочитали из файла)
        // Печатаем сырые байты считанного значения в десятичном виде
        
        // Переходим к вашему шагу №2 (Вывод HEX-значений считанных данных)
        printf(" | ");
        for (int i = 0; i < size; i++) printf("%02X ", bytes[i]);
        for (int i = size; i < 8; i++) printf("   "); // Выравнивание
        // 3. Печатаем текстовое представление с цветными точками
        printf("| ");//printf("| \"");
        for (int i = 0; i < size; i++) symbol_adjustment(bytes[i]);
        printf("  ");//printf("\" ");
        for (int i = size; i < 8*2; i++) putchar(' '); // выравнивание закрывающей кавычки
        // 4. Печатаем тип данных и значение
        const char * type_str = (size == 1) ? " uint8_t " : (size == 2) ? "" : (size == 4) ? " uint32_t" : " uint64_t";
        printf("|%s %s = %llu; // 0x", type_str, abbreviation, value);
        // Красиво выводим HEX значение нужной разрядности
        if (size == 1) printf("%02llX", value);
        else if (size == 2) printf("%04llX", value);
        else if (size == 4) printf("%08llX", value);
        else printf("%016llX", value);
    }
}

void pe_builder(const char * output_filename)
{
    FILE * descriptor = fopen(output_filename, "wb");
    if (!descriptor) return;

    uint32_t code_size   = 16;  // Пока заглушка: пусть код занимает 16 байт
    uint32_t import_size = 128; // Пока заглушка: пусть импорт занимает 128 байт

    uint32_t code_rva   = 4096; // Первое доступное место в памяти (0x1000)
    uint32_t import_rva = code_rva + code_size;

    uint32_t code_raw   = 512;  // Первое доступное место в файле (0x200)
    uint32_t import_raw = code_raw + code_size;

    // Массив, куда мы побайтово упакуем весь наш импорт
    uint8_t import_buffer[256] = {0};
    uint32_t import_ptr = 0; // Наш внутренний курсор для записи в буфер

    // --- НАШ ХАРДКОДНЫЙ ШЕЛЛКОД (x64) ---
    // Вызывает ExitProcess(42)
    uint8_t shellcode[64] =
    {
        0xB9, 0x2A, 0x00, 0x00, 0x00,       // mov ecx, 42
        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp qword ptr [RIP+0]
        0xC3                                // ret
    };
    uint32_t code_size = 64;

    // --- РАСЧЕТ АДРЕСОВ ИМПОРТА ---
    uint32_t code_rva = 4096; // 0x1000
    uint32_t import_base_rva = code_rva + code_size; // 0x1040

    // Расчет RVA блоков внутри импорта
    uint32_t rva_descriptors = import_base_rva;
    uint32_t rva_ilt         = rva_descriptors + 40; // 2 дескриптора (наш + нулевой)
    uint32_t rva_iat         = rva_ilt + 24;         // 2 функции + нуль-терминатор
    uint32_t rva_fn_exit     = rva_iat + 24;
    uint32_t rva_fn_write    = rva_fn_exit + 14;     // sizeof(Hint) + "ExitProcess\0" + padding
    uint32_t rva_dll_name    = rva_fn_write + 12;    // sizeof(Hint) + "WriteFile\0" + padding

    // Прошиваем смещение в шеллкод
    uint32_t rip_offset = rva_iat - (code_rva + 5 + 6);
    memcpy(&shellcode[7], &rip_offset, 4);

    // --- СБОРКА БУФЕРА ИМПОРТА (IB) ---
    uint8_t ib[256] = {0};
    
    // 1. Заполняем Import Descriptor
    ImportDescriptor import_descriptor = {0};
    import_descriptor.import_lookup_table_rva = rva_ilt;
    import_descriptor.name_rva = rva_dll_name;
    import_descriptor.import_address_table_rva = rva_iat;
    memcpy(&ib[0], &desc, sizeof (ImportDescriptor));

    // 2. ILT (Указатели на имена функций)
    uint64_t entry_exit = rva_fn_exit;
    uint64_t entry_write = rva_fn_write;
    memcpy(&ib[40], &entry_exit, 8);
    memcpy(&ib[48], &entry_write, 8);

    // 3. IAT (На диске дублирует ILT)
    memcpy(&ib[64], &entry_exit, 8);
    memcpy(&ib[72], &entry_write, 8);

    // 4. Имена функций (Hint/Name) и DLL
    uint16_t hint = 0;
    memcpy(&ib[88], &hint, 2);
    memcpy(&ib[90], "ExitProcess\0", 12);
    memcpy(&ib[102], &hint, 2);
    memcpy(&ib[104], "WriteFile\0", 10);
    memcpy(&ib[114], "kernel32.dll\0", 13);
    
    uint32_t import_size = 114 + 13;

    // --- ЗАПИСЬ ЗАГОЛОВКОВ И СЕКЦИИ ---

    // 1. Создаем пустые структуры в памяти (зануляем их через {0})
    DosHeader dos_header = {0};
    FileHeader file_header = {0};
    OptionalHeader64 optional_header_64 = {0};
    SectionHeader section_header = {0};

    // 2. ЗАПОЛНЯЕМ DOS ЗАГОЛОВОК
    // Используем обычные числа — процессор сам уложит их в Little-Endian!
    dos_header.magic = 0x5A4D; // Это символы 'MZ'
    dos_header.lfanew = 64;    // Сигнатура PE начнется строго на 64-м байте (сразу за DOS-заголовком)

    // 3. ЗАПОЛНЯЕМ COFF FILE HEADER
    file_header.machine = 0x8664;                                    // Архитектура x86_64 (AMD64)
    file_header.number_of_sections = 1;                              // У нас будет всего 1 секция (.text)
    file_header.time_date_stamp = 0xFFFFFFFF;                        // Пока заглушка
    file_header.size_of_optional_header = sizeof (OptionalHeader64); // Компилятор сам посчитает (240 байт)!
    // Флаги: IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LARGE_ADDRESS_AWARE
    //  EXE файл + приложение может обрабатывать адреса > 2 ГБ
    file_header.characteristics = 0x0022;

    /////////////////////////////
    // OPTIONAL_HEADER [START] //

    optional_header_64.magic = 0x020B; // PE32+
    optional_header_64.major_linker_version = 1;
    optional_header_64.minor_linker_version = 0;

    // Размеры секций (пока у нас только .text, её физический размер на диске будет 512 байт)
    optional_header_64.size_of_code = 10;
    optional_header_64.size_of_initialized_data = 0;
    optional_header_64.size_of_uninitialized_data = 0;

    // ТОЧКА ВХОДА: Укажем RVA (Relative Virtual Address). 
    // Первая секция в памяти всегда начинается со смещения 4096 (0x1000).
    // Пусть наша точка входа указывает прямо на самое начало этой секции!
    optional_header_64.address_of_entry_point = code_rva; // 4096
    optional_header_64.base_of_code           = code_rva; // 4096

    // Базовый адрес, куда Windows попытается загрузить программу в памяти
    optional_header_64.image_base = 0x00400000;

    // Выравнивания (Жесткий закон PE-формата)
    optional_header_64.section_alignment = 4096; // В памяти выравниваем по 4 КБ
    optional_header_64.file_alignment = 512;     // В файле выравниваем по 512 байт

    // Версии операционной системы (ставим Windows 7/10 совместимость)
    optional_header_64.major_operating_system_version = 6;
    optional_header_64.minor_operating_system_version = 0;
    optional_header_64.major_image_version = 0;
    optional_header_64.minor_image_version = 0;
    optional_header_64.major_subsystem_version = 6;
    optional_header_64.minor_subsystem_version = 0;
    optional_header_64.win32_version_value = 0;

    // Общий размер файла при развертывании в оперативной памяти.
    // Заголовки занимают 4096 в памяти + секция .text занимает 4096. Итого: 8192!
    optional_header_64.size_of_image = 8192;

    // Размер всех заголовков на диске вместе с зазором (Padding)
    optional_header_64.size_of_headers = 512; // Заголовки займут ровно 1 сектор

    optional_header_64.check_sum = 0;
    optional_header_64.subsystem = 3; // IMAGE_SUBSYSTEM_WINDOWS_CUI (Консольное приложение)
    optional_header_64.dll_characteristics = 0;
    optional_header_64.size_of_stack_reserve = 0x100000; // Стандартный 1 МБ стека
    optional_header_64.size_of_stack_commit = 0x1000;
    optional_header_64.size_of_heap_reserve = 0x100000;
    optional_header_64.size_of_heap_commit = 0x1000;
    optional_header_64.loader_flags = 0;
    optional_header_64.number_of_rva_and_sizes = 16; // 16 каталогов данных

    optional_header_64.data_directories[0].virtual_address = 0;
    optional_header_64.data_directories[0].size = 0;
    // ВАЖНО: Выставляем Data Directory для Импорта
    optional_header_64.data_directories[1].virtual_address = rva_descriptors;
    optional_header_64.data_directories[1].size = 40; 
    //optional_header_64.data_directories[1].virtual_address = import_rva;
    //optional_header_64.data_directories[1].size = import_size;
    for (int i = 2; i < 16; i++)
    {
        optional_header_64.data_directories[i].virtual_address = 0;
        optional_header_64.data_directories[i].size = 0;
    }

    // OPTIONAL_HEADER [END] //
    ///////////////////////////

    //memcpy(section_header.name, ".text", 5);  // Скопирует 5 символов: '.', 't', 'e', 'x', 't'
    //section_header.name.value = macro__str_le64('.', 't', 'e', 'x', 't', '\0', '\0', '\0');
    section_header.name.value = str_to_le64(".text\0\0\0");
    section_header.virtual_size = code_size + import_size; // Укажем реальный размер кода (пока 10 байт)
    section_header.virtual_address = code_rva; // В памяти секция начнется с RVA 0x1000 (4096)
    section_header.size_of_raw_data = macro__align_up(code_size + import_size, 512); // На диске округляем до минимальных 512 байт
    section_header.pointer_to_raw_data = 512;  // Код начнется сразу после 512-байтных заголовков

    // 4. ПОСЛЕДОВАТЕЛЬНО ЗАПИСЫВАЕМ ВСЁ НА ДИСК
    // Каждая структура улетает монолитным идеальным блоком
    fwrite(&dos_header, sizeof (DosHeader), 1, descriptor);   // Блок 1: DOS (64 байта)
    fwrite(pe_signature, 1, 4, descriptor);                   // Блок 2: Сигнатура "PE\0\0" (4 байта)
    fwrite(&file_header, sizeof (FileHeader), 1, descriptor); // Блок 3: File Header (20 байт)

    // Записываем Опциональный заголовок вместе со всеми 16 директориями данных (240 байт)
    fwrite(&optional_header_64, sizeof (OptionalHeader64), 1, descriptor);

    // Записываем структуру единственной секции .text (40 байт)
    fwrite(&section_header, sizeof (SectionHeader), 1, descriptor);

    uint32_t headers_real_size =
     sizeof (DosHeader) +        // 64 байта
     sizeof (pe_signature) +     // 4 байта
     sizeof (FileHeader) +       // 20 байт
     sizeof (OptionalHeader64) + // 240 байт
     sizeof (SectionHeader)      // 40 байт
    ;
    // Вычисляем, сколько нулей нужно добить до границы 512
    for (uint32_t padding_needed = 512 - headers_real_size; padding_needed != 0; padding_needed--) fputc('\0', descriptor);
    // Начало секции .text (точка входа в программу)
    fputc(0xC3, descriptor); // RET
    // Выравнивание самого тела секции .text до 512 байт (итого файл 1024)
    for (uint32_t padding_needed = 512 - 1; padding_needed != 0; padding_needed--) fputc('\0', descriptor);

    fclose(descriptor); // Временная заглушка, чтобы файл пока просто закрывался
}

const char dos_header__reserved_name[13][24+1] =
{
    "cblp", "cp", "crlc", "cparhdr", "minalloc", "maxalloc",
    "ss"  , "sp", "csum", "ip"     , "cs"      , "lfarlc"  ,
    "ovno"
};

void pe_analyzer()
{
    FILE * descriptor = fopen("test_subject.exe", "rb");
    //FILE * descriptor = fopen("pe_tool.exe", "rb");
    if (!descriptor) return;
    //printf("\n ------------------------------------------------------------------------------------------");
    //printf("\n  Offset(text): dec byte | Byte offset(dec/hex): hex byte");
    printf("\n ------------------------------------------------------------------------------------------");
    printf("\n  ___________________");
    printf("\n /                   \\");
    printf("\n %c №1.1 | DOS HEADER %c", 16, 17);
    printf("\n \\___________________/");
    printf("\n ------------------------------------------------------------------------------------------");

    if (fread(&dos_header__magic.value, 2, 1, descriptor) != 1) return;
    console_log(2, 0, dos_header__magic.bytes, dos_header__magic.value, "/!\\ magic");

    if (fread(&dos_header__reserved[0].value, 2, 13, descriptor) != 13) return;
    for (uint8_t i = 0; i < 13; i++) {
        console_log(2,  i*2+2, dos_header__reserved[i].bytes, dos_header__reserved[i].value, dos_header__reserved_name[i]);
    }

    if (fread(&dos_header__res[0].value, 2, 4, descriptor) != 4) return;
    printf("\n  __________________________");
    printf("\n / dw res[4];");
    console_log(2, 28, dos_header__res[0].bytes, dos_header__res[0].value, "res[0]");
    console_log(2, 30, dos_header__res[1].bytes, dos_header__res[1].value, "res[1]");
    console_log(2, 32, dos_header__res[2].bytes, dos_header__res[2].value, "res[2]");
    console_log(2, 34, dos_header__res[3].bytes, dos_header__res[3].value, "res[3]");

    if (fread(&dos_header__reserved_2[0].value, 2, 2, descriptor) != 2) return;
    console_log(2, 36, dos_header__reserved_2[0].bytes, dos_header__reserved_2[0].value, "oemid");
    console_log(2, 38, dos_header__reserved_2[1].bytes, dos_header__reserved_2[1].value, "oeminfo");

    offset = 40;

    if (fread(&dos_header__res2[0].value, 2, 10, descriptor) != 10) return;
    printf("\n  __________________________");
    printf("\n / dw res2[10];");

    char _e_res2[32];
    for (uint8_t i = 0; i < 10; i++)
    {
        sprintf(_e_res2, "res2[%d]", i);
        console_log(2, offset, dos_header__res2[i].bytes, dos_header__res2[i].value, _e_res2);
        offset += 2;
    }
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&dos_header__lfanew.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, dos_header__lfanew.bytes, dos_header__lfanew.value, "/!\\ lfanew");
    offset += 4;
    printf("\n ------------------------------------------------------------------------------------------");
    if (dos_header__lfanew.value > 64)
    {
        printf("\n  _________________");
        printf("\n /                 \\");
        printf("\n %c №1.2 | DOS STUB %c", 16, 17);
        printf("\n \\_________________/");
        printf("\n ------------------------------------------------------------------------------------------");
        uint8_t bytes[8];
        uint8_t chunk_size;
        while (offset < dos_header__lfanew.value)
        {
            chunk_size = 8; // 1. Вычисляем, сколько байт выдать в этой строке (максимум 8)
            if (dos_header__lfanew.value - offset < 8) chunk_size = dos_header__lfanew.value - offset; // Если у финиша осталось меньше 8 байт
            // 2. Набиваем буфер байтами из файла
            for (uint8_t i = 0; i < chunk_size; i++) bytes[i] = getc(descriptor);
            // 3. Отдаем всю пачку из 8 байт в вашу console_log за один раз!
            // Передаем: chunk_size (размер), offset (текущий адрес), bytes (указатель на массив)
            // Для четвертого аргумента (value) при чтении 8 байт обычно передают 0, 
            // либо первый байт буфера, так как выравниватель внутри console_log у вас уже отлажен.
            console_log(chunk_size, offset, bytes, bytes[0], "");
            // 4. Двигаем offset сразу на размер прочитанной группы байт
            offset += chunk_size;
        }
        printf("\n ------------------------------------------------------------------------------------------");
    }
    else if (dos_header__lfanew.value < 64) return;
    printf("\n  ________________");
    printf("\n /                \\");
    printf("\n %c №2 | NT HEADER %c", 16, 17);
    printf("\n \\________________/");
    printf("\n ------------------------------------------------------------------------------------------");
    offset = dos_header__lfanew.value;
    if (fread(&Signature.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, Signature.bytes, Signature.value, "Signature");
    offset += 4;
    printf("\n ------------------------------------------------------------------------------------------");
    printf("\n  ________________________________");
    printf("\n /                                \\");
    printf("\n %c №2.1 | NT HEADER / FILE HEADER %c", 16, 17);
    printf("\n \\________________________________/");
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&Machine.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, Machine.bytes, Machine.value, "Machine");
    offset += 2;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&NumberOfSections.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, NumberOfSections.bytes, NumberOfSections.value, "NumberOfSections");
    offset += 2;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&TimeDateStamp.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, TimeDateStamp.bytes, TimeDateStamp.value, "TimeDateStamp");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&PointerToSymbolTable.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, PointerToSymbolTable.bytes, PointerToSymbolTable.value, "PointerToSymbolTable");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&NumberOfSymbols.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, NumberOfSymbols.bytes, NumberOfSymbols.value, "NumberOfSymbols");
    offset += 4;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfOptionalHeader.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, SizeOfOptionalHeader.bytes, SizeOfOptionalHeader.value, "SizeOfOptionalHeader");
    offset += 2;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&Characteristics.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, Characteristics.bytes, Characteristics.value, "Characteristics");
    offset += 2;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&Magic.value, 2, 1, descriptor) != 1) return;
    //Magic.value = ?;
    if (Magic.value == 0x020B)
    {
        printf("\n  ___________________________________________________");
        printf("\n /                                                   \\");
        printf("\n %c №2.2 | NT HEADER / OPTIONAL HEADER (PE32+/64-bit) %c", 16, 17);
        printf("\n \\___________________________________________________/");
    }
    else if (Magic.value == 0x010B)
    {
        printf("\n  __________________________________________________");
        printf("\n /                                                  \\");
        printf("\n %c №2.2 | NT HEADER / OPTIONAL HEADER (PE32/32-bit) %c", 16, 17);
        printf("\n \\__________________________________________________/");
    }
    else
    {
        printf("\n  ____________________________________");
        printf("\n /                                    \\");
        printf("\n %c №2.2 | NT HEADER / OPTIONAL HEADER %c", 16, 17);
        printf("\n \\____________________________________/");
    }
    // Часть 1: Стандартные поля COFF (Standard Fields) — одинаковые для 32/64 бит
    printf("\n ------------------------------------------------------------------------------------------");
    // Далее произведён рефакторинг кода (функция printf перемещена в console_log). Сделано это для уменьшения объёма (дублирования) кода.
    console_log(2, offset, Magic.bytes, Magic.value, "Magic");
    offset += 2;
    printf("\n ------------------------------------------------------------------------------------------");
    MajorLinkerVersion.value = getc(descriptor);
    console_log(1, offset, MajorLinkerVersion.bytes, MajorLinkerVersion.value, "MajorLinkerVersion");
    offset++;
    //printf("\n ------------------------------------------------------------------------------------------");
    MinorLinkerVersion.value = getc(descriptor);
    console_log(1, offset, MinorLinkerVersion.bytes, MinorLinkerVersion.value, "MinorLinkerVersion");
    offset++;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfCode.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, SizeOfCode.bytes, SizeOfCode.value, "SizeOfCode");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfInitializedData.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, SizeOfInitializedData.bytes, SizeOfInitializedData.value, "SizeOfInitializedData");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfUninitializedData.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, SizeOfUninitializedData.bytes, SizeOfUninitializedData.value, "SizeOfUninitializedData");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&AddressOfEntryPoint.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, AddressOfEntryPoint.bytes, AddressOfEntryPoint.value, "AddressOfEntryPoint");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&BaseOfCode.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, BaseOfCode.bytes, BaseOfCode.value, "BaseOfCode");
    offset += 4;
    printf("\n ------------------------------------------------------------------------------------------");
    // [Примечание]: Только в PE32 здесь идёт ещё 4 байта uint32_t BaseOfData. В 64-битном PE32+ этого поля вообще нет!
    //if (Magic.value == 0x010B) { ... }
    // Часть 2: Специфичные поля Windows (Windows-Specific Fields) — размеры различаются!
    if (fread(&ImageBase.value, 8, 1, descriptor) != 1) return;
    console_log(8, offset, ImageBase.bytes, ImageBase.value, "ImageBase");
    offset += 8;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SectionAlignment.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, SectionAlignment.bytes, SectionAlignment.value, "SectionAlignment");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&FileAlignment.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, FileAlignment.bytes, FileAlignment.value, "FileAlignment");
    offset += 4;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&MajorOperatingSystemVersion.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, MajorOperatingSystemVersion.bytes, MajorOperatingSystemVersion.value, "MajorOperatingSystemVersion");
    offset += 2;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&MinorOperatingSystemVersion.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, MinorOperatingSystemVersion.bytes, MinorOperatingSystemVersion.value, "MinorOperatingSystemVersion");
    offset += 2;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&MajorImageVersion.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, MajorImageVersion.bytes, MajorImageVersion.value, "MajorImageVersion");
    offset += 2;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&MinorImageVersion.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, MinorImageVersion.bytes, MinorImageVersion.value, "MinorImageVersion");
    offset += 2;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&MajorSubsystemVersion.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, MajorSubsystemVersion.bytes, MajorSubsystemVersion.value, "MajorSubsystemVersion");
    offset += 2;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&MinorSubsystemVersion.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, MinorSubsystemVersion.bytes, MinorSubsystemVersion.value, "MinorSubsystemVersion");
    offset += 2;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&Win32VersionValue.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, Win32VersionValue.bytes, Win32VersionValue.value, "Win32VersionValue");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfImage.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, SizeOfImage.bytes, SizeOfImage.value, "SizeOfImage");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeaders.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, SizeOfHeaders.bytes, SizeOfHeaders.value, "SizeOfHeaders");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&CheckSum.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, CheckSum.bytes, CheckSum.value, "CheckSum");
    offset += 4;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&Subsystem.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, Subsystem.bytes, Subsystem.value, "Subsystem");
    offset += 2;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&DllCharacteristics.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, DllCharacteristics.bytes, DllCharacteristics.value, "DllCharacteristics");
    offset += 2;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfStackReserve.value, 8, 1, descriptor) != 1) return;
    console_log(8, offset, SizeOfStackReserve.bytes, SizeOfStackReserve.value, "SizeOfStackReserve");
    offset += 8;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfStackCommit.value, 8, 1, descriptor) != 1) return;
    console_log(8, offset, SizeOfStackCommit.bytes, SizeOfStackCommit.value, "SizeOfStackCommit");
    offset += 8;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeapReserve.value, 8, 1, descriptor) != 1) return;
    console_log(8, offset, SizeOfHeapReserve.bytes, SizeOfHeapReserve.value, "SizeOfHeapReserve");
    offset += 8;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeapCommit.value, 8, 1, descriptor) != 1) return;
    console_log(8, offset, SizeOfHeapCommit.bytes, SizeOfHeapCommit.value, "SizeOfHeapCommit");
    offset += 8;
    printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&LoaderFlags.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, LoaderFlags.bytes, LoaderFlags.value, "LoaderFlags");
    offset += 4;
    //printf("\n ------------------------------------------------------------------------------------------");
    if (fread(&NumberOfRvaAndSizes.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, NumberOfRvaAndSizes.bytes, NumberOfRvaAndSizes.value, "NumberOfRvaAndSizes");
    offset += 4;
    printf("\n ------------------------------------------------------------------------------------------");
    printf("\n  _______________________________________________________");
    printf("\n /                                                       \\");
    printf("\n %c №2.2.1 | NT HEADER / OPTIONAL HEADER / DATA DIRECTORY %c", 16, 17);
    printf("\n \\_______________________________________________________/");
    printf("\n ------------------------------------------------------------------------------------------");
    char abbreviation[32];
    //uint32_t dir_count = NumberOfRvaAndSizes.value;
    for (uint32_t i = 0; i < NumberOfRvaAndSizes.value; i++)
    {
        printf("\n  __________________________");
        printf("\n / %s", directory_names[i]);
        sprintf(abbreviation, "virtual_address[%d]", i);
        //}
        if (fread(&virtual_address[i].value, 4, 1, descriptor) != 1) return;
        console_log(4, offset, virtual_address[i].bytes, virtual_address[i].value, abbreviation);
        offset += 4;
        sprintf(abbreviation, "size[%d]", i);
        if (fread(&size[i].value, 4, 1, descriptor) != 1) return;
        console_log(4, offset, size[i].bytes, size[i].value, abbreviation);
        offset += 4;
        //printf("\n ------------------------------------------------------------------------------------------");
    }
    printf("\n ------------------------------------------------------------------------------------------");
    printf("\n  _____________________");
    printf("\n /                     \\");
    printf("\n %c №3 | SECTION HEADER %c", 16, 17);
    printf("\n \\_____________________/");
    printf("\n ------------------------------------------------------------------------------------------");
    for (uint16_t i = 0; i < NumberOfSections.value; i++)
    {
        // 1. Читаем Имя секции (8 байт)
        if (fread(section_header[i].Name, 1, 8, descriptor) != 8) return;
        printf("\n  __________________________");
        printf("\n / SECTION: %d, NAME: ", i);
        for (int j = 0; j < 8; j++) symbol_adjustment(section_header[i].Name[j]);
        offset += 8;

        if (fread(&section_header[i].VirtualSize.value, 4, 1, descriptor) != 1) return;
        console_log(4, offset, section_header[i].VirtualSize.bytes, section_header[i].VirtualSize.value, "VirtualSize");
        offset += 4;

        if (fread(&section_header[i].VirtualAddress.value, 4, 1, descriptor) != 1) return;
        console_log(4, offset, section_header[i].VirtualAddress.bytes, section_header[i].VirtualAddress.value, "VirtualAddress");
        offset += 4;

        if (fread(&section_header[i].SizeOfRawData.value, 4, 1, descriptor) != 1) return;
        console_log(4, offset, section_header[i].SizeOfRawData.bytes, section_header[i].SizeOfRawData.value, "SizeOfRawData");
        offset += 4;

        if (fread(&section_header[i].PointerToRawData.value, 4, 1, descriptor) != 1) return;
        console_log(4, offset, section_header[i].PointerToRawData.bytes, section_header[i].PointerToRawData.value, "PointerToRawData");
        offset += 4;

        // 6. Пропускаем 12 байт неиспользуемых легаси-полей
        if (fread(section_header[i].Reserved, 1, 12, descriptor) != 12) return;
        offset += 12;

        if (fread(&section_header[i].Characteristics.value, 4, 1, descriptor) != 1) return;
        console_log(4, offset, section_header[i].Characteristics.bytes, section_header[i].Characteristics.value, "Characteristics");
        offset += 4;
    }
    printf("\n ------------------------------------------------------------------------------------------");
    printf("\n  _________________________________");
    printf("\n /                                 \\");
    printf("\n %c №4 | HEADER PADDING (ALIGNMENT) %c", 16, 17); // Зазор выравнивания
    printf("\n \\_________________________________/");
    printf("\n ------------------------------------------------------------------------------------------");
    // Записываем максимально возможное число (4'294'967'295)
    // Любой реальный адрес в файле гарантированно окажется меньше него!
    uint32_t sort = 0xFFFFFFFF;
    for (uint16_t i = 0; i < NumberOfSections.value; i++)
    {
        if (sort > section_header[i].PointerToRawData.value && section_header[i].PointerToRawData.value > 0)
        {
            sort = section_header[i].PointerToRawData.value;
        }
    }
    // anonymous scope //
    {
        uint8_t bytes[8];
        uint8_t chunk_size;
        // 2. Набиваем буфер байтами из файла
        for (uint8_t i = 0; i < chunk_size; i++)
        {
            int ch = getc(descriptor);
            if (ch == EOF) // Если файл внезапно кончился — принудительно останавливаем всё!
            {
                sort = offset; // Схлопываем границу, чтобы внешний while (offset < sort) сразу завершился
                chunk_size = i; // Корректируем размер реально прочитанного хвостика
                break;
            }
            bytes[i] = (uint8_t) ch;
        }
        // Вызываем console_log только если мы реально прочитали хоть один байт
        if (chunk_size > 0)
        {
            console_log(chunk_size, offset, bytes, bytes[0], "");
            offset += chunk_size;
        }
    }
    printf("\n ------------------------------------------------------------------------------------------");
    printf("\n  _______________________________");
    printf("\n /                               \\");
    printf("\n %c №5 | SECTIONS RAW DATA STREAM %c", 16, 17);
    printf("\n \\_______________________________/");
    printf("\n ------------------------------------------------------------------------------------------");
    // Формула вычисления точки входа на диске
    uint32_t raw_entry_point =
     AddressOfEntryPoint.value - section_header[0].VirtualAddress.value + section_header[0].PointerToRawData.value;
    uint8_t bytes[8]; // Буфер для чтения пачками по 8 байт
    int read_bytes;
    while (true)
    {
        read_bytes = fread(bytes, 1, 8, descriptor);
        if (read_bytes <= 0) break;
        if (raw_entry_point >= offset && raw_entry_point < offset + read_bytes)
        {
            printf("\n ------------------------------------------------------------------------------------------");
            printf("\n  __________________________");
            printf("\n /                          \\");
            printf("\n %c №6 | PROGRAM ENTRY POINT %c", 16, 17);
            printf("\n \\__________________________/");
            printf("\n ------------------------------------------------------------------------------------------");
        }
        console_log(read_bytes, offset, bytes, bytes[0], "");
        offset += read_bytes;
    }
    while (true)
    {
        read_bytes = fread(bytes, 1, 8, descriptor);
        if (read_bytes <= 0) break;
        // Точка входа в программу уже ранее была пройдена, опускаем проверку
        console_log(read_bytes, offset, bytes, bytes[0], "");
        // Точка входа в программу уже ранее была пройдена, опускаем проверку
        offset += read_bytes;
    }
    printf("\n ------------------------------------------------------------------------------------------");
    // gcc -s pe_tool.c -o pe_tool.exe / Strip (Удаление отладочной информации/лишнего мусора)
    // gcc -Os -s pe_tool.c -o pe_tool.exe
    fclose(descriptor);
}

int main()
{
    setlocale(0, "");

    pe_builder("test_subject.exe");
    pe_analyzer();

    putchar('\n');
    return 0;
}
/*
putchar('\n');
for (short i = 0; i < 256; i++)
{
    switch (i){
    case '\0': printf("\n №%d: \\0 | ", i+1); break;
    case '\a': printf("\n №%d: \\a | ", i+1); break;
    case '\b': printf("\n №%d: \\b | ", i+1); break;
    case '\t': printf("\n №%d: \\t | ", i+1); break;
    case '\n': printf("\n №%d: \\n | ", i+1); break;
    case '\r': printf("\n №%d: \\r | ", i+1); break;
    case 149: printf("\n №%d: \\? | ", i+1); break;
    default: printf("\n №%d: %c | ", i+1, i);
    }
    symbol_adjustment(i);
    //putchar(',');
}
*/