#ifdef _WIN32
 #include <windows.h>
#endif

#include <stdint.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef union { uint8_t value; uint8_t bytes[1]; } union__uint8_t;
typedef union { uint16_t value; uint8_t bytes[2]; } union__uint16_t;
typedef union { uint32_t value; uint8_t bytes[4]; } union__uint32_t;
typedef union { uint64_t value; uint8_t bytes[8]; } union__uint64_t;

// БЛОК 1: DOS ЗАГОЛОВОК (DOS Header)
//  Размер: Всегда строго 64 байта.
//  Природа: Статичный исторический балласт. Изменяется только одно поле -> e_lfanew.
union__uint16_t e_magic;    // 000~001      | 00~01      #  Магическое число

//uint8_t dos_reserved[38];   // 002~039      | 02~27      #  Зарезервировано: Обычно забито нулями (0). Сюда можно спрятать кастомные метаданные компилятора, Windows их игнорирует.
union__uint16_t e_cblp;     // dos reserved №1
union__uint16_t e_cp;       // dos reserved №2
union__uint16_t e_crlc;     // dos reserved №3
union__uint16_t e_cparhdr;  // dos reserved №4
union__uint16_t e_minalloc; // dos reserved №5
union__uint16_t e_maxalloc; // dos reserved №6
union__uint16_t e_ss;       // dos reserved №7
union__uint16_t e_sp;       // dos reserved №8
union__uint16_t e_csum;     // dos reserved №9
union__uint16_t e_ip;       // dos reserved №10
union__uint16_t e_cs;       // dos reserved №11
union__uint16_t e_lfarlc;   // dos reserved №12
union__uint16_t e_ovno;     // dos reserved №13
union__uint16_t e_res[4];   // dos reserved №14
union__uint16_t e_oemid;    // dos reserved №15
union__uint16_t e_oeminfo;  // dos reserved №16
union__uint16_t e_res2[10]; // dos reserved №17

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
} SectionHeader;


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
 8: - ?
 16: - ?
 32: - ?
 64: - ?
 128: - ?
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
        for (int i = size; i < 8; i++) { putchar(' '); symbol_adjustment('-'); symbol_adjustment('-'); symbol_adjustment('-'); } // Выравнивание
        printf(" |");
        for (int i = 0; i < size; i++) printf(" %02X", bytes[i]);
        for (int i = size; i < 8; i++) { putchar(' '); symbol_adjustment('-'); symbol_adjustment('-'); } // Выравнивание
        // 3. Выводим символы с цветной обработкой
        printf(" | ");
        uint8_t printed_chars = 0; 
        for (int i = 0; i < size; i++) printed_chars += symbol_adjustment(bytes[i]);
        // === ИДЕАЛЬНОЕ ВЫРАВНИВАНИЕ ДЛЯ CASE 1 ===
        // Базовая ширина текстового поля для 8 байт, если все символы печатные (8 байт * 2 символа на HEX-шаг) = 16.
        // Но так как у нас символы выводятся «липко» друг за другом, максимальное экранное поле под символы — 16 знаков.
        int max_width = 8;
        int dots_to_print = max_width - printed_chars;
        // КРИТИЧЕСКИЙ МОМЕНТ: Если из файла прочитано меньше 8 байт (например, size = 2 или 4),
        // нам нужно компенсировать пустые байты, которые мы вообще не выводили в цикле.
        // За каждый отсутствующий байт добавляем по 2 выравнивающие точки.
        int missing_bytes = 8 - size;
        dots_to_print += (missing_bytes * 2);
        // Забиваем оставшееся пространство точками
        for (int i = 0; i < dots_to_print; i++) printf("·");

        // 4. Печатаем тип данных и значение
        const char * type_str = (size == 1) ? "" : (size == 2) ? "" : (size == 4) ? "" : " uint64_t";
        if (size != 1)
        {
            if (abbreviation[0] != '\0') printf("|%s %s = %llu; // 0x", type_str, abbreviation, value);
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
        else putchar('|');
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
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n  Offset(text): dec byte | Byte offset(dec/hex): hex byte");
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n  ____________________________________");
    printf("\n /                                    \\");
    printf("\n %c БЛОК 1: DOS ЗАГОЛОВОК (DOS Header) %c", 16, 17);
    printf("\n \\____________________________________/");
    //printf("\n    ____________________________________");
    //printf("\n __/ БЛОК 1: DOS ЗАГОЛОВОК (DOS Header) \\__");
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&e_magic.value, 2, 1, descriptor) != 1) return;
    console_log(2, 0, e_magic.bytes, e_magic.value, "/!\\ dw/short e_magic");

    if (fread(&e_cblp.value, 2, 1, descriptor) != 1) return;
    console_log(2, 2, e_cblp.bytes, e_cblp.value, "e_cblp");

    if (fread(&e_cp.value, 2, 1, descriptor) != 1) return;
    console_log(2, 4, e_cp.bytes, e_cp.value, "e_cp");

    if (fread(&e_crlc.value, 2, 1, descriptor) != 1) return;
    console_log(2, 6, e_crlc.bytes, e_crlc.value, "e_crlc");

    if (fread(&e_cparhdr.value, 2, 1, descriptor) != 1) return;
    console_log(2, 8, e_cparhdr.bytes, e_cparhdr.value, "e_cparhdr");

    if (fread(&e_minalloc.value, 2, 1, descriptor) != 1) return;
    console_log(2, 10, e_minalloc.bytes, e_minalloc.value, "e_minalloc");

    if (fread(&e_maxalloc.value, 2, 1, descriptor) != 1) return;
    console_log(2, 12, e_maxalloc.bytes, e_maxalloc.value, "e_maxalloc");

    if (fread(&e_ss.value, 2, 1, descriptor) != 1) return;
    console_log(2, 14, e_ss.bytes, e_ss.value, "e_ss");

    if (fread(&e_sp.value, 2, 1, descriptor) != 1) return;
    console_log(2, 16, e_sp.bytes, e_sp.value, "e_sp");

    if (fread(&e_csum.value, 2, 1, descriptor) != 1) return;
    console_log(2, 18, e_csum.bytes, e_csum.value, "e_csum");

    if (fread(&e_ip.value, 2, 1, descriptor) != 1) return;
    console_log(2, 20, e_ip.bytes, e_ip.value, "e_ip");

    if (fread(&e_cs.value, 2, 1, descriptor) != 1) return;
    console_log(2, 22, e_cs.bytes, e_cs.value, "e_cs");

    if (fread(&e_lfarlc.value, 2, 1, descriptor) != 1) return;
    console_log(2, 24, e_lfarlc.bytes, e_lfarlc.value, "e_lfarlc");

    if (fread(&e_ovno.value, 2, 1, descriptor) != 1) return;
    console_log(2, 26, e_ovno.bytes, e_ovno.value, "e_ovno");

    offset = 28;

    if (fread(&e_res[0].value, 2, 4, descriptor) != 4) return;
    printf("\n  __________________________");
    printf("\n / dw/short e_res[4];");
    char _e_res[32];
    for (uint8_t i = 0; i < 4; i++)
    {
        sprintf(_e_res, "e_res[%d]", i);
        console_log(2, offset, e_res[i].bytes, e_res[i].value, _e_res);
        offset += 2;
    }
    putchar('\n');

    if (fread(&e_oemid.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, e_oemid.bytes, e_oemid.value, "dw/short e_oemid");
    offset += 2;

    if (fread(&e_oeminfo.value, 2, 1, descriptor) != 1) return;
    console_log(2, offset, e_oeminfo.bytes, e_oeminfo.value, "dw/short e_oeminfo");
    offset += 2;

    if (fread(&e_res2[0].value, 2, 10, descriptor) != 10) return;
    printf("\n  __________________________");
    printf("\n / dw/short e_res2[10];");
    char _e_res2[32];
    for (uint8_t i = 0; i < 10; i++)
    {
        sprintf(_e_res2, "e_res2[%d]", i);
        console_log(2, offset, e_res2[i].bytes, e_res2[i].value, _e_res2);
        offset += 2;
    }
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&e_lfanew.value, 4, 1, descriptor) != 1) return;
    console_log(4, offset, e_lfanew.bytes, e_lfanew.value, "/!\\ dd/int e_lfanew");
    offset += 4; // Теперь offset равен 64, и маркер файла на 64
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    // БЕЗОПАСНОСТЬ: Если e_lfanew некорректный (меньше конца DOS-заголовка),
    // то PE-заголовка в файле физически быть не может. Завершаем работу.
    if (e_lfanew.value < 64)
    {
        //printf("\n [ОШИБКА] Неверное смещение e_lfanew. Файл не является исполняемым PE-файлом Windows.");
        //fclose(descriptor);
        return;
    }
    // Если всё хорошо, последовательно вычитываем DOS-заглушку байт за байтом
    if (e_lfanew.value > 64)
    {
        printf("\n  ____________________");
        printf("\n /                    \\");
        printf("\n %c БЛОК 1.2: DOS STUB %c", 16, 17);
        printf("\n \\____________________/");
        printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
        union__uint64_t byte;
        for (int i = 64; i < e_lfanew.value; i += 8)
        {
            if (fread(&byte.value, 1, 8, descriptor) != 8) return;
            console_log(8, offset, byte.bytes, byte.value, "");
            offset += 8;
        }
        printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    }
    // Маркер файла СЕЙЧАС сам автоматически дошел до позиции e_lfanew.value
    // Просто синхронизируем нашу переменную offset для console_log
    offset = e_lfanew.value;
    printf("\n  _________________________________________");
    printf("\n /                                         \\");
    printf("\n %c БЛОК 2: PE ЗАГОЛОВОК (COFF File Header) %c", 16, 17);
    printf("\n \\_________________________________________/");
    //printf("\n    _________________________________________");
    //printf("\n __/ БЛОК 2: PE ЗАГОЛОВОК (COFF File Header) \\__");
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&pe_signature.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ pe_signature");*/ return; }
    console_log(4, offset, pe_signature.bytes, pe_signature.value, "pe_signature");
    offset += 4;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Machine.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ Machine");*/ return; }
    console_log(2, offset, Machine.bytes, Machine.value, "Machine");
    offset += 2;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&NumberOfSections.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ NumberOfSections");*/ return; }
    console_log(2, offset, NumberOfSections.bytes, NumberOfSections.value, "NumberOfSections");
    offset += 2;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&TimeDateStamp.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ TimeDateStamp");*/ return; }
    console_log(4, offset, TimeDateStamp.bytes, TimeDateStamp.value, "TimeDateStamp");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&PointerToSymbolTable.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ PointerToSymbolTable");*/ return; }
    console_log(4, offset, PointerToSymbolTable.bytes, PointerToSymbolTable.value, "PointerToSymbolTable");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&NumberOfSymbols.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ NumberOfSymbols");*/ return; }
    console_log(4, offset, NumberOfSymbols.bytes, NumberOfSymbols.value, "NumberOfSymbols");
    offset += 4;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfOptionalHeader.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfOptionalHeader");*/ return; }
    console_log(2, offset, SizeOfOptionalHeader.bytes, SizeOfOptionalHeader.value, "SizeOfOptionalHeader");
    offset += 2;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Characteristics.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ Characteristics");*/ return; }
    console_log(2, offset, Characteristics.bytes, Characteristics.value, "Characteristics");
    offset += 2;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
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
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    // Далее произведён рефакторинг кода (функция printf перемещена в console_log). Сделано это для уменьшения объёма (дублирования) кода.
    console_log(2, offset, Magic.bytes, Magic.value, "Magic");
    offset += 2;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    MajorLinkerVersion.value = getc(descriptor);
    console_log(1, offset, MajorLinkerVersion.bytes, MajorLinkerVersion.value, "MajorLinkerVersion");
    offset++;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    MinorLinkerVersion.value = getc(descriptor);
    console_log(1, offset, MinorLinkerVersion.bytes, MinorLinkerVersion.value, "MinorLinkerVersion");
    offset++;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfCode.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfCode");*/ return; }
    console_log(4, offset, SizeOfCode.bytes, SizeOfCode.value, "SizeOfCode");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfInitializedData.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfInitializedData");*/ return; }
    console_log(4, offset, SizeOfInitializedData.bytes, SizeOfInitializedData.value, "SizeOfInitializedData");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfUninitializedData.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfUninitializedData");*/ return; }
    console_log(4, offset, SizeOfUninitializedData.bytes, SizeOfUninitializedData.value, "SizeOfUninitializedData");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&AddressOfEntryPoint.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ AddressOfEntryPoint");*/ return; }
    console_log(4, offset, AddressOfEntryPoint.bytes, AddressOfEntryPoint.value, "AddressOfEntryPoint");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&BaseOfCode.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ BaseOfCode");*/ return; }
    console_log(4, offset, BaseOfCode.bytes, BaseOfCode.value, "BaseOfCode");
    offset += 4;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    // [Примечание]: Только в PE32 здесь идёт ещё 4 байта uint32_t BaseOfData. В 64-битном PE32+ этого поля вообще нет!
    //if (Magic.value == 0x010B) { ... }
    // Часть 2: Специфичные поля Windows (Windows-Specific Fields) — размеры различаются!
    if (fread(&ImageBase.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ ImageBase");*/ return; }
    console_log(8, offset, ImageBase.bytes, ImageBase.value, "ImageBase");
    offset += 8;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SectionAlignment.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SectionAlignment");*/ return; }
    console_log(4, offset, SectionAlignment.bytes, SectionAlignment.value, "SectionAlignment");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&FileAlignment.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ FileAlignment");*/ return; }
    console_log(4, offset, FileAlignment.bytes, FileAlignment.value, "FileAlignment");
    offset += 4;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MajorOperatingSystemVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MajorOperatingSystemVersion");*/ return; }
    console_log(2, offset, MajorOperatingSystemVersion.bytes, MajorOperatingSystemVersion.value, "MajorOperatingSystemVersion");
    offset += 2;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MinorOperatingSystemVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MinorOperatingSystemVersion");*/ return; }
    console_log(2, offset, MinorOperatingSystemVersion.bytes, MinorOperatingSystemVersion.value, "MinorOperatingSystemVersion");
    offset += 2;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MajorImageVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MajorImageVersion = ?");*/ return; }
    console_log(2, offset, MajorImageVersion.bytes, MajorImageVersion.value, "MajorImageVersion");
    offset += 2;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MinorImageVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MinorImageVersion = ?");*/ return; }
    console_log(2, offset, MinorImageVersion.bytes, MinorImageVersion.value, "MinorImageVersion");
    offset += 2;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MajorSubsystemVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MajorSubsystemVersion = ?");*/ return; }
    console_log(2, offset, MajorSubsystemVersion.bytes, MajorSubsystemVersion.value, "MajorSubsystemVersion");
    offset += 2;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&MinorSubsystemVersion.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ MinorSubsystemVersion = ?");*/ return; }
    console_log(2, offset, MinorSubsystemVersion.bytes, MinorSubsystemVersion.value, "MinorSubsystemVersion");
    offset += 2;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Win32VersionValue.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ Win32VersionValue = ?");*/ return; }
    console_log(4, offset, Win32VersionValue.bytes, Win32VersionValue.value, "Win32VersionValue");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfImage.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfImage = ?");*/ return; }
    console_log(4, offset, SizeOfImage.bytes, SizeOfImage.value, "SizeOfImage");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeaders.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfHeaders = ?");*/ return; }
    console_log(4, offset, SizeOfHeaders.bytes, SizeOfHeaders.value, "SizeOfHeaders");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&CheckSum.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ CheckSum = ?");*/ return; }
    console_log(4, offset, CheckSum.bytes, CheckSum.value, "CheckSum");
    offset += 4;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&Subsystem.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ Subsystem = ?");*/ return; }
    console_log(2, offset, Subsystem.bytes, Subsystem.value, "Subsystem");
    offset += 2;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&DllCharacteristics.value, 2, 1, descriptor) != 1) { /*printf("\n /!\\ DllCharacteristics = ?");*/ return; }
    console_log(2, offset, DllCharacteristics.bytes, DllCharacteristics.value, "DllCharacteristics");
    offset += 2;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfStackReserve.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfStackReserve");*/ return; }
    console_log(8, offset, SizeOfStackReserve.bytes, SizeOfStackReserve.value, "SizeOfStackReserve");
    offset += 8;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfStackCommit.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfStackCommit");*/ return; }
    console_log(8, offset, SizeOfStackCommit.bytes, SizeOfStackCommit.value, "SizeOfStackCommit");
    offset += 8;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeapReserve.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfHeapReserve");*/ return; }
    console_log(8, offset, SizeOfHeapReserve.bytes, SizeOfHeapReserve.value, "SizeOfHeapReserve");
    offset += 8;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&SizeOfHeapCommit.value, 8, 1, descriptor) != 1) { /*printf("\n /!\\ SizeOfHeapCommit");*/ return; }
    console_log(8, offset, SizeOfHeapCommit.bytes, SizeOfHeapCommit.value, "SizeOfHeapCommit");
    offset += 8;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&LoaderFlags.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ LoaderFlags = ?");*/ return; }
    console_log(4, offset, LoaderFlags.bytes, LoaderFlags.value, "LoaderFlags");
    offset += 4;
    //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    if (fread(&NumberOfRvaAndSizes.value, 4, 1, descriptor) != 1) { /*printf("\n /!\\ NumberOfRvaAndSizes = ?");*/ return; }
    console_log(4, offset, NumberOfRvaAndSizes.bytes, NumberOfRvaAndSizes.value, "NumberOfRvaAndSizes");
    offset += 4;
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n  ____________________________________________");
    printf("\n /                                            \\");
    printf("\n %c БЛОК 4: КАТАЛОГИ ДАННЫХ (Data Directories) %c", 16, 17);
    printf("\n \\____________________________________________/");
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
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
        if (fread(&size[i].value, 4, 1, descriptor) != 1) { return; }
        console_log(4, offset, size[i].bytes, size[i].value, abbreviation);
        offset += 4;
        //printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    }
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n  ________________________________________");
    printf("\n /                                        \\");
    printf("\n %c БЛОК 5: ТАБЛИЦА СЕКЦИЙ (Section Table) %c", 16, 17);
    printf("\n \\________________________________________/");
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    // Количество секций берем из Блока 2 (переменная NumberOfSections)
    uint16_t sec_count = NumberOfSections.value;
    SectionHeader current_sec;
    for (uint16_t i = 0; i < sec_count; i++)
    {
        // 1. Читаем Имя секции (8 байт)
        if (fread(current_sec.Name, 1, 8, descriptor) != 8) { return; }
        // Выводим имя (можно через вашу обновленную функцию console_log или напрямую)
        printf("\n  __________________________");
        printf("\n / SECTION: %d, NAME: ", i);
        for (int n = 0; n < 8; n++) { symbol_adjustment(current_sec.Name[n]); }
        //printf("\n -------------------------------------------------------------------------------------------------");
        offset += 8;

        // 2. Читаем VirtualSize (4 байта)
        if (fread(&current_sec.VirtualSize.value, 4, 1, descriptor) != 1) { return; }
        console_log(4, offset, current_sec.VirtualSize.bytes, current_sec.VirtualSize.value, "VirtualSize");
        offset += 4;

        // 3. Читаем VirtualAddress (4 байта)
        if (fread(&current_sec.VirtualAddress.value, 4, 1, descriptor) != 1) { return; }
        console_log(4, offset, current_sec.VirtualAddress.bytes, current_sec.VirtualAddress.value, "VirtualAddress");
        offset += 4;

        // 4. Читаем SizeOfRawData (4 байта)
        if (fread(&current_sec.SizeOfRawData.value, 4, 1, descriptor) != 1) { return; }
        console_log(4, offset, current_sec.SizeOfRawData.bytes, current_sec.SizeOfRawData.value, "SizeOfRawData");
        offset += 4;

        // 5. Читаем PointerToRawData (4 байта)
        if (fread(&current_sec.PointerToRawData.value, 4, 1, descriptor) != 1) { return; }
        console_log(4, offset, current_sec.PointerToRawData.bytes, current_sec.PointerToRawData.value, "PointerToRawData");
        offset += 4;

        // 6. Пропускаем 12 байт неиспользуемых легаси-полей
        if (fread(current_sec.Reserved, 1, 12, descriptor) != 12) { return; }
        // При желании можно вывести их одной строчкой, offset += 12
        offset += 12;

        // 7. Читаем Characteristics (4 байта)
        if (fread(&current_sec.Characteristics.value, 4, 1, descriptor) != 1) { return; }
        console_log(4, offset, current_sec.Characteristics.bytes, current_sec.Characteristics.value, "Characteristics");
        offset += 4;
    }
    printf("\n ---------------------------------------------------------------------------------------------------------------------------------------------------------");
    putchar('\n');
    for (short i = 0; i < 256; i++)
    {
        printf(" №%d: %c | ", i+1, i);
        symbol_adjustment(i);
        putchar(',');
    }
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