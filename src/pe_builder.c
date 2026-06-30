#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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