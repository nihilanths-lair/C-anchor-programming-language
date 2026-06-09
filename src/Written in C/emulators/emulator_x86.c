#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>

// 1 Мегабайт плоской памяти RAM (1024 * 1024 байт)
#define RAM_SIZE 1048576
unsigned char RAM[RAM_SIZE];

// Нативные 64-битные регистры x86-64 (Глобальный уровень)
uint64_t rax = 0x7FFFFFFFFFFFFFFF; // Максимальное положительное число (для теста)
uint64_t rbx = 0x7FFFFFFFFFFFFFFF; // Максимальное положительное число (для теста)
uint64_t rip = 0x1000;   // Точка входа (Entry Point) для нашего кода
uint64_t rsp = 0xF0000;  // Вершина стека (выделено ~1 МБ, растет вниз)
uint64_t rbp = 0x0000;
unsigned char zf = 0;    // Флаг нуля (Zero Flag)

char * numf(long long num);
char * bin64(uint64_t num);
void print_segmented_reg(const char* name, uint64_t reg_val);
void dbg_RegisterState(uint64_t step);

int main()
{
    setlocale(LC_ALL, ""); // Инициализация локали для вывода тысяч

    // Временная "прошивка" тестовой программы в память RAM по адресу RIP (0x1000)
    // 0xB8 -> MOV EAX, imm32 (Опкод x86-64). Запишем число 4660 (0x1234)
    RAM[0x1000] = 0xB8; 
    RAM[0x1001] = 0x34; // Little Endian порядок байт
    RAM[0x1002] = 0x12;
    RAM[0x1003] = 0x00;
    RAM[0x1004] = 0x00;

    // 0x00 -> Временная команда HALT для остановки нашего цикла
    RAM[0x1005] = 0x00; 

    uint64_t step = 0;
    int running = 1;

    printf("\n [<|>] Запуск 64-битного Мета-Ядра x86-64 [<|>]\n");

    // Главный цикл процессора (Fetch - Decode - Execute)
    while (running)
    {
        // 1. Показываем текущее состояние процессора ПЕРЕД выполнением инструкции
        dbg_RegisterState(step++);

        // Пошаговый режим: ждем нажатия Enter в консоли для симуляции такта
        printf("(Enter - шаг) ");
        rewind(stdin);
        getchar();

        // 2. FETCH: Извлекаем текущий опкод из RAM по адресу RIP
        uint8_t opcode = RAM[rip];

        // 3. DECODE & EXECUTE: Анализ и выполнение
        switch (opcode) {
        case 0xB8: // Реальный опкод x86-64: MOV EAX, imm32
            // Читаем следующие 4 байта из RAM (Little Endian)
            rax = RAM[rip + 1] | 
                ((uint32_t) RAM[rip+2] << 8) | 
                ((uint32_t) RAM[rip+3] << 16) | 
                ((uint32_t) RAM[rip+4] << 24);
            
            // В x86-64 запись в младшую 32-битную часть (EAX) автоматически зануляет верхнюю часть RAX!
            rip += 5; // Сдвигаем RIP вперед на длину инструкции (1 байт опкода + 4 байта числа)
            break;
        
        case 0x89:
            // ... //
            break;
        
        case 0x00: // Наш временный HALT
            printf("\n Процессор остановлен по инструкции HALT (0x00).\n");
            running = 0;
            break;

        default:
            printf("\n Архитектурный тупик: Неизвестный опкод 0x%02X на RIP 0x%016llX\n", opcode, rip);
            running = 0;
            break;
        }
    }
    return 0;
}

// Функция форматирования чисел с разделением тысяч апострофом
char * numf(long long num)
{
    static char buffers[4][64];
    static int buf_idx = 0;
    
    char * str = buffers[buf_idx];
    buf_idx = (buf_idx + 1) % 4;
    
    char temp[64];
    sprintf(temp, "%lld", num);
    
    int len = strlen(temp);
    int is_negative = (num < 0) ? 1 : 0;
    int digits_len = len - is_negative;
    
    int insert_quotes = (digits_len - 1) / 3; 
    int new_len = len + insert_quotes;
    
    str[new_len] = '\0';
    
    int src = len - 1;
    int dst = new_len - 1;
    int digit_count = 0;
    
    while (src >= is_negative)
    {
        if (digit_count == 3)
        {
            str[dst--] = '\''; 
            digit_count = 0;
        }
        str[dst--] = temp[src--];
        digit_count++;
    }
    
    if (is_negative) str[dst] = '-';
    return str;
}

// Функция для 64-битных регистров (Вывод: 8 тетрад, разделенных подчёркиванием)
char * bin64(uint64_t num)
{
    // 64 бита + 7 разделителей тетрад + 1 пробел в центре + 1 нуль-терминал = 74 байта
    static char sbin[74]; 
    sbin[73] = '\0';
    
    int pos = 72;
    for (int i = 0; i < 64; i++)
    {
        if (pos == 36) // Разделитель между старшим и младшим двойным словом (Dword)
        {
            sbin[pos--] = ' ';
        }
        else if (pos == 63 || pos == 54 || pos == 45 || pos == 27 || pos == 18 || pos == 9) 
        {
            sbin[pos--] = '_'; // Разделитель тетрад внутри байта
        }
        
        sbin[pos--] = ((num >> i) & 1) ? '1' : '0';
    }
    return sbin;
}

// Вспомогательная функция для печати байт регистра раздельно
void print_segmented_reg(const char* name, uint64_t reg_val)
{
    // Извлекаем каждый байт отдельно (от старшего к младшему)
    uint8_t b7 = (reg_val >> 56) & 0xFF;
    uint8_t b6 = (reg_val >> 48) & 0xFF;
    uint8_t b5 = (reg_val >> 40) & 0xFF;
    uint8_t b4 = (reg_val >> 32) & 0xFF;
    uint8_t b3 = (reg_val >> 24) & 0xFF; // Старший байт EAX
    uint8_t b2 = (reg_val >> 16) & 0xFF;
    uint8_t b1 = (reg_val >> 8)  & 0xFF; // AH
    uint8_t b0 = reg_val         & 0xFF; // AL (Младший байт)

    // Выводим имя регистра, затем 8 раздельных байт в DEC, затем 8 байт в HEX
    // Младшие 4 байта (b3, b2, b1, b0) - это подрегистр "Ex" (например, EAX)
    // Младшие 2 байта (b1, b0) - это подрегистр "x" (например, AX)
    printf("\n  %4s | %03d %03d %03d %03d %03d %03d %03d %03d | %02X %02X %02X %02X %02X %02X %02X %02X | %s",
        name, 
        b7, b6, b5, b4,  // Старшая половина (64-бит)
        b3, b2, b1, b0,  // Младшая половина (32-бит: EAX/EBX/RIP...)
        b7, b6, b5, b4, b3, b2, b1, b0,
        numf((long long)reg_val)
    );
}

void dbg_RegisterState(uint64_t step)
{
    printf("\n -----------\n Такт: %llu", step);
    printf("\n -------------------------------------------------------------------------------------------------------");
    printf("\n  REGISTER |               DEC               |           HEX           | TEXT");
    printf("\n           |                                 |                         |");
    printf("\n        zf |                                                           | %d", zf);
    putchar('\n');
    printf("\n            <------------- r?x -------------> <--------- r?x --------->");
    printf("\n                            <----- e?x ----->             <--- e?x --->");
    printf("\n                                    <- ?x -->                   < ?x ->");
    printf("\n                                    < ?h ?l >                   <?h ?l>");
    // Печатаем каждый регистр через нашу новую сегментированную функцию
    print_segmented_reg("       a", rax);
    print_segmented_reg("       b", rbx);
    putchar('\n');
    printf("\n            <------------- r?p -------------> <--------- r?p --------->");
    //printf("\n                            <----- e?p ----->             <--- e?p --->");
    //printf("\n                                    <- ?p -->                   < ?p ->");
    print_segmented_reg("       i", rip);
    print_segmented_reg("       s", rsp);
    print_segmented_reg("       b", rbp);
    printf("\n -------------------------------------------------------------------------------------------------------");
    printf("\n");
}