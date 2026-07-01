#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>

// Функция-помощник для записи 16-битного числа (2 байта) в массив по индексу
void write16(uint8_t *buffer, int offset, uint16_t value)
{
    buffer[offset]     = (uint8_t)(value & 0xFF);
    buffer[offset + 1] = (uint8_t)((value >> 8) & 0xFF);
}

// Функция-помощник для записи 32-битного числа (4 байта) в массив по индексу
void write32(uint8_t *buffer, int offset, uint32_t value)
{
    buffer[offset]     = (uint8_t)(value & 0xFF);
    buffer[offset + 1] = (uint8_t)((value >> 8) & 0xFF);
    buffer[offset + 2] = (uint8_t)((value >> 16) & 0xFF);
    buffer[offset + 3] = (uint8_t)((value >> 24) & 0xFF);
}

int main()
{
    setlocale(0, "");
    uint8_t exe[1024]; // Выделяем 1024 байта под наш будущий EXE-файл (этого хватит с головой)
    memset(exe, 0, sizeof (exe));
    printf("\n [PE Builder] Начинаем сборку бинарника на голом массиве...");
    // ==========================================
    // 1. DOS ЗАГОЛОВОК (Смещения 0 - 63)
    // ==========================================
    // Записываем сигнатуру 'MZ' (0x5A4D в формате Little-Endian)
    write16(exe, 0, 0x5A4D); 
    // Записываем e_lfanew на смещение 60. 
    // Говорим Windows, что PE-заголовок начнется ровно с 64-го байта.
    write32(exe, 60, 64); 
    // ==========================================
    // 2. PE СИГНАТУРА И COFF-ЗАГОЛОВОК (Смещения 64 - 87)
    // ==========================================
    // На смещении 64 пишем маркер 'PE\0\0' (0x00004550)
    write32(exe, 64, 0x00004550);
    // Смещение 68: Machine Type. 0x8664 — это код архитектуры x86-64 (AMD64)
    write16(exe, 68, 0x8664);
    // Смещение 70: NumberOfSections. Укажем пока 1 секцию (только под код)
    write16(exe, 70, 1);
    // Смещение 72: TimeDateStamp (Время создания, можно влепить заглушку)
    write32(exe, 72, 0x12345678);
    // Смещение 84: SizeOfOptionalHeader. 
    // Для 64-битных систем размер Optional Header всегда равен 240 байтам (0xF0)!
    write16(exe, 84, 0x00F0);
    // Смещение 86: Characteristics. Флаги файла.
    // 0x0022 означает: 0x0002 (EXE файл) + 0x0020 (Приложение может обрабатывать адреса > 2ГБ)
    write16(exe, 86, 0x0022);
    // ==========================================
    // 3. OPTIONAL HEADER (Смещения 88 - 199)
    // ==========================================
    // Смещение 88: Magic. 0x020B означает формат PE32+ (нативная 64-битная программа)
    write16(exe, 88, 0x020B);
    // Смещение 104: AddressOfEntryPoint. Виртуальный адрес первой инструкции
    // Наш код начнется в памяти ровно с отметки 0x1000
    write32(exe, 104, 0x1000);
    // Смещение 112: ImageBase. Куда загружать EXE в память.
    // Так как поле 64-битное (8 байт), запишем его через две функции write32
    write32(exe, 112, 0x00400000); // Младшие 4 байта
    write32(exe, 116, 0x00000000); // Старшие 4 байта
    // Смещение 120: SectionAlignment. Выравнивание секций в ОЗУ (4096 байт)
    write32(exe, 120, 0x1000);
    // Смещение 124: FileAlignment. Выравнивание данных в файле на диске (512 байт)
    write32(exe, 124, 0x0200);
    // Смещение 144: SizeOfHeaders. Размер всех заголовков вместе взятых (512 байт)
    write32(exe, 144, 0x0200);
    // Смещение 156: Subsystem. 3 — Консольное приложение Windows
    write16(exe, 156, 3);
    FILE *f = fopen("test_compiled.exe", "wb");
    if (!f)
    {
        printf("Ошибка создания файла!\n");
        return 1;
    }
    // Запишем первые 200 байт нашего сформированного файла
    fwrite(exe, 1, 200, f);
    fclose(f);
    printf("\n [PE Builder] Первые 200 байт успешно сформированы в test_compiled.exe!\n");
    return 0;
}
/*
// 1. DOS Заголовок (Минимум для Windows)
#pragma pack(push, 1)
typedef struct {
    uint16_t e_magic;    // Магическое число 'MZ'
    uint16_t e_cblp;     // Байт на последней странице файла
    uint16_t e_cp;       // Страницы в файле
    uint16_t e_crlc;     // Релокации
    uint16_t e_cparhdr;  // Размер заголовка в параграфах
    uint16_t e_minalloc; // Минимальная доп. память
    uint16_t e_maxalloc; // Максимальная доп. память
    uint16_t e_ss;       // Начальное значение SS
    uint16_t e_sp;       // Начальное значение SP
    uint16_t e_csum;     // Контрольная сумма
    uint16_t e_ip;       // Начальное значение IP
    uint16_t e_cs;       // Начальное значение CS
    uint16_t e_lfarlc;   // Адрес таблицы релокаций
    uint16_t e_ovno;     // Номер оверлея
    uint16_t e_res[4];   // Зарезервировано
    uint16_t e_oemid;    // OEM идентификатор
    uint16_t e_oeminfo;  // OEM информация
    uint16_t e_res2[10]; // Зарезервировано
    uint32_t e_lfanew;   // Смещение до PE-заголовка (Вот это ключевое поле!)
} DOS_HEADER;

// 2. COFF Заголовок файла (Характеристики процессора)
typedef struct {
    uint16_t Machine;              // Архитектура (0x8664 для AMD64/x64)
    uint16_t NumberOfSections;     // Сколько секций в файле
    uint32_t TimeDateStamp;        // Время создания
    uint32_t PointerToSymbolTable; // Символьная таблица (для дебага, у нас 0)
    uint32_t NumberOfSymbols;      // Количество символов
    uint16_t SizeOfOptionalHeader; // Размер следующего (Optional) заголовка
    uint16_t Characteristics;      // Флаги файла (что это EXE, а не DLL)
} COFF_HEADER;
#pragma pack(pop)

int main()
{
    printf("[PE Builder] Каркас структур успешно инициализирован!\n");
    // Сюда мы будем по байтам собирать наш первый настоящий EXE
    return 0;
}
*/