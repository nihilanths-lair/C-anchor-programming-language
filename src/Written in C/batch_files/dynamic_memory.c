// correspondence_table // таблица соответствия
//int allocation_table; // таблица распределения
//heap memory allocation

// Объём доступной памяти: 256 byte's.
#define MACRO__UPPER_LIMIT (1 << 8)
/// memory tape // лента памяти
static unsigned char m[MACRO__UPPER_LIMIT] = {0};
static unsigned char *__m = m;
//static unsigned char c = 255;
// Unsafe (небезопасная, но максимально производительная реализация).
// Программист должен следить сам, за тем, чтобы не выйти за границу буфера кучи (segmentation fault).
unsigned char * heap_mem_alloc(const unsigned char cell)
{
    __m = m; // При каждом вызове делаем сброс на начальное состояние
    __while_run:
    // Поиск подходящего участка
    while (*__m) // Пока участок занят, продолжаем искать свободный
    {
        #if !defined DEBUG
        printf("\n Поиск незанятого фрагмента памяти: <%p> | H-D [%02X-%03d]=%d", __m, ((uintptr_t) __m) % 0x100, ((uintptr_t) __m) % 0x100, *m);
        printf("\n  <HEX> [%02X]=·%02X  [%02X]=·%02X", ((uintptr_t) m) % 0x100, *__m, ((uintptr_t) m) % 0x100, *(__m + 1));
        printf("\n <DEC> [%03d]=%03d [%03d]=%03d\n", ((uintptr_t) m) % 0x1000, *__m, ((uintptr_t) m) % 0x1000, *(__m + 1));
        #endif
        __m = __m + 2 + *(__m + 1); // Заглянем в след. байт-заголовок (размер блока) и сдвинем указатель на эту длину
    }
    if (*(__m + 1) >= cell) // Если блок под новые данные достаточного размера, займём его
    {
        #if !defined DEBUG
        printf("\n Найден свободный фрагмент памяти: <%p> | H-D [%02X-%03d]=%d >= %d", __m, ((uintptr_t) __m) % 0x100, ((uintptr_t) __m) % 0x100, *m, cell);
        printf("\n  <HEX> [%02X]=·%02X  [%02X]=·%02X", ((uintptr_t) m) % 0x100, *__m, ((uintptr_t) m) % 0x100, *(__m + 1));
        printf("\n <DEC> [%03d]=%03d [%03d]=%03d\n", ((uintptr_t) m) % 0x1000, *__m, ((uintptr_t) m) % 0x1000, *(__m + 1));
        #endif
        // Обновим мета-данные (2 байт-заголовка: флаг доступности и размер блока данных
        *(__m + 1) = cell; // Размер блока данных
        *(__m) = 1;        // Доступность блока данных (занимаем слот)
        return __m + 2;    // Возвращаем указатель на данные
    }
    // Если блок под новые данные недостаточного размера
    /*
    printf("\n =========: %d[%d]=%d, %d[%d]=%d < %d",
        ((uintptr_t) __m) % 0x100, (((uintptr_t) __m) % 0x100) - (((uintptr_t) m) % 0x100), *__m,
        ((uintptr_t) __m) % 0x100, (((uintptr_t) __m) % 0x100) - (((uintptr_t) m) % 0x100), *(__m + 1), cell
    );
    */
    printf("\n Full address (64-bit's): %ph == %ud", __m, (unsigned int)((uintptr_t)__m)); // Адрес в HEX/DEC (полный)
    printf("\n =========: \\d%03d [IDX:%tu] | BUSY:%u, SIZE:%u < REQ:%d",
        (unsigned int)((uintptr_t)__m & 0xFF), // Адрес в HEX (младший байт)
        // ?                                   // Адрес в DEC (младший байт)
        // ?                                   // Адрес в DEC (короткий - 1 байт)
        __m - m,                               // Индекс в массиве
        *__m,                                  // Флаг занятости
        *(__m + 1),                            // Размер блока
        cell                                   // Сколько ищем
    );
    //__m = __m + *(__m + 1);
    goto __while_run;
}
//
void heap_mem_free(unsigned char *m)
{
    //__m = m; // При каждом вызове делаем сброс на начальное состояние
    // Пометим память свободной, при этом размер данных оставим, чтобы залатать дыру при необходимости
    #if !defined DEBUG
    printf("\n Пометим фрагмент памяти на доступность: <%p> | H-D [%02X-%03d]=%d", m, ((uintptr_t) m) % 0x100, ((uintptr_t) m) % 0x100, *m);
    printf("\n  <HEX> [%02X]=·%02X  [%02X]=·%02X", ((uintptr_t) m-2) % 0x100, *(m-2), ((uintptr_t) m-1) % 0x100, *(m-1));
    printf("\n <DEC> [%03d]=%03d [%03d]=%03d\n", ((uintptr_t) m-2) % 0x1000, *(m-2), ((uintptr_t) m-1) % 0x1000, *(m-1));
    printf("\n state: %d", *(m-2));
    #endif
    *(m-2) = 0;
    #if !defined DEBUG
    printf("\n state: %d", *(m-2));
    #endif
}
//
unsigned char * mem_alloc() {/* future code */} // предпочтительней, отсутствие фрагментации, быстрое нахождение свободной зоны памяти
void mem_free(void *mem) {/* future code */}

// %[flags][width][.precision][size]type
/*
/// В байт-представлении (удобном для машины)
    printf("\n %03d"/*, ?/);                                     // 8-bit's address число разбитое на байты (dec)
    printf("\n %03d'%03d"/*, ?/);                                // 16-bit's address число разбитое на байты (dec)
    // также интересует в 24, ..., 40, 48, 56 bit's
    printf("\n %03d'%03d'%03d'%03d"/*, ?/);                      // 32-bit's address число разбитое на байты (dec)
    printf("\n %03d'%03d'%03d'%03d'%03d'%03d'%03d'%03d'"/*, ?/); // 64-bit's address число разбитое на байты (dec)

    printf("\n %02X"/*, ?/);                                     // 8-bit's address число разбитое на байты (hex)
    printf("\n %02X'%02X"/*, ?/);                                // 16-bit's address число разбитое на байты (hex)
    // также интересует в 24, ..., 40, 48, 56 bit's
    printf("\n %02X'%02X'%02X'%02X"/*, ?/);                      // 32-bit's address число разбитое на байты (hex)
    printf("\n %02X'%02X'%02X'%02X'%02X'%02X'%02X'%02X'"/*, ?/); // 64-bit's address число разбитое на байты (hex)

    /// В читаемом представлении (удобном для человека)
    printf("\n %03d"/*, ?/); // 8-bit's address целиком число (dec)
    printf("\n %03d"/*, ?/); // 16-bit's address целиком число (dec)
    // также интересует в 24, ..., 40, 48, 56 bit's
    printf("\n %03d"/*, ?/); // 32-bit's address целиком число (dec)
    printf("\n %03d"/*, ?/); // 64-bit's address целиком число (dec)

    printf("\n %02X"/*, ?/); // 8-bit's address целиком число (hex)
    printf("\n %02X"/*, ?/); // 16-bit's address целиком число (hex)
    // также интересует в 24, ..., 40, 48, 56 bit's
    printf("\n %02X"/*, ?/); // 32-bit's address целиком число (hex)
    printf("\n %02X"/*, ?/); // 64-bit's address целиком число (hex)
*/