#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>

// 1 Мегабайт плоской памяти RAM (1024*1024 байт)
#define RAM_SIZE 1048576
unsigned char RAM[RAM_SIZE];

// Нативные 64-битные регистры x86-64 (Глобальный уровень)
uint64_t rax = 0;
uint64_t rbx = 0;
uint64_t rip = 0x1000;   // Точка входа (Entry Point) для нашего кода
uint64_t rsp = 0xF0000;  // Вершина стека
uint64_t rbp = 0x0000;
unsigned char zf = 0;    // Флаг нуля (Zero Flag)

uint64_t get_set_reg(uint8_t reg_code, int mode, uint64_t value);
char * numf(long long num);
char * bin64(uint64_t num);
void dbg_RegisterState(uint64_t step);

int main()
{
    setlocale(LC_ALL, ""); // Инициализация локали
    
    // --- ПРОШИВКА ТЕСТОВОЙ ПРОГРАММЫ ---
    // Цель: загрузить число в RAX, скопировать в RBX, установить адрес в RBX и сбросить RAX в память!

    // 1. [0x1000] MOV EAX, 0x1234 (Опкод 0xB8). RAX станет равен 0x1234
    RAM[0x1000] = 0xB8;
    RAM[0x1001] = 0x34; RAM[0x1002] = 0x12; RAM[0x1003] = 0x00; RAM[0x1004] = 0x00;
    
    // 2. [0x1005] MOV RBX, RAX (Опкод 0x89, ModR/M = 0xC3) -> MOD=3 (регистры), REG=0 (RAX), R/M=3 (RBX)
    RAM[0x1005] = 0x89;
    RAM[0x1006] = 0xC3;

    // 3. [0x1007] MOV EAX, 0x5000 (Опкод 0xB8). Загрузим в RAX адрес памяти 0x5000
    RAM[0x1007] = 0xB8;
    RAM[0x1008] = 0x00; RAM[0x1009] = 0x50; RAM[0x100A] = 0x00; RAM[0x100B] = 0x00;

    // 4. [0x100C] MOV RBX, RAX (Опкод 0x89, ModR/M = 0xC3). Перенесем этот адрес в RBX. Теперь RBX = 0x5000
    RAM[0x100C] = 0x89;
    RAM[0x100D] = 0xC3;

    // 5. [0x100E] MOV EAX, 0x7777 (Опкод 0xB8). Запишем в RAX новые тестовые данные "7777"
    RAM[0x100E] = 0xB8;
    RAM[0x100F] = 0x77; RAM[0x1010] = 0x77; RAM[0x1011] = 0x00; RAM[0x1012] = 0x00;

    // 6. [0x1013] MOV [RBX], RAX (Опкод 0x89, ModR/M = 0x03) -> MOD=0 (память), REG=0 (RAX), R/M=3 (RBX)
    // Процессор должен взять число 0x7777 из RAX и положить в RAM по адресу, который в RBX (0x5000)
    RAM[0x1013] = 0x89;
    RAM[0x1014] = 0x03;

    // 7. [0x1015] HALT (Опкод 0x00)
    RAM[0x1015] = 0x00;

    uint64_t step = 0;
    int running = 1;
    
    printf("--- Запуск 64-битного Мета-Ядра x86-64 с ModR/M ---\n");

    while (running)
    {
        dbg_RegisterState(step++);

        printf("(Enter - шаг) ");
        rewind(stdin); 
        getchar();

        uint8_t opcode = RAM[rip];
        rip++; // Сдвигаем RIP на 1 байт опкода вперед (FETCH)

        switch (opcode) {
        case 0xB8: // MOV EAX, imm32 (Прямая загрузка константы)
            rax = RAM[rip] |
                ((uint32_t)RAM[rip + 1] << 8) |
                ((uint32_t)RAM[rip + 2] << 16) |
                ((uint32_t)RAM[rip + 3] << 24);
            rip += 4; // Сдвигаем RIP на 4 байта числа
            break;

        case 0x89: // MOV r/m64, r64 (Основная магия ModR/M)
        {
            uint8_t modrm = RAM[rip];
            rip++; // Сдвигаем RIP мимо байта ModR/M

            uint8_t mod = (modrm >> 6) & 0x03;
            uint8_t reg = (modrm >> 3) & 0x07;
            uint8_t rm  = modrm & 0x07;

            uint64_t source_value = get_set_reg(reg, 0, 0); // Читаем регистр-источник (REG)

            if (mod == 3) { // Режим: Регистр -> Регистр (Например: MOV RBX, RAX)
                get_set_reg(rm, 1, source_value); // Перезаписываем регистр-приемник (R/M)
            }
            else if (mod == 0)
            { // Режим: Регистр -> Память по указателю (Например: MOV [RBX], RAX)
                uint64_t target_addr = get_set_reg(rm, 0, 0); // Получаем адрес из регистра R/M
                
                // Побайтовая запись 64-битного значения в RAM (Little Endian)
                for (int i = 0; i < 8; i++) {
                    RAM[target_addr + i] = (source_value >> (i * 8)) & 0xFF;
                }
                printf("\n [MEM WRITE] Запись в память RAM по адресу 0x%016llX значения: 0x%016llX\n", target_addr, source_value);
            }
            break;
        }

        case 0x00: // HALT
            printf("\n Процессор остановлен по инструкции HALT (0x00).\n");
            running = 0;
            break;

        default:
            printf("\n Архитектурный тупик: Неизвестный опкод 0x%02X на RIP 0x%016llX\n", opcode, rip - 1);
            running = 0;
            break;
        }
    }

    // Проверка: прочитаем из нашей RAM по адресу 0x5000, появились ли там данные?
    printf("\n --- Дамп ячеек памяти RAM по адресу 0x5000 ---");
    printf("\n Адрес 0x5000: %02X %02X %02X %02X %02X %02X %02X %02X\n", 
        RAM[0x5000], RAM[0x5001], RAM[0x5002], RAM[0x5003],
        RAM[0x5004], RAM[0x5005], RAM[0x5006], RAM[0x5007]
    );
    return 0;
}

// Вспомогательная функция-диспетчер для доступа к регистрам по их кодам x86-64
// mode: 0 - чтение, 1 - запись. Возвращает значение регистра или записывает value.
uint64_t get_set_reg(uint8_t reg_code, int mode, uint64_t value)
{
    switch (reg_code) {
    case 0: // RAX
        if (mode == 1) rax = value;
        return rax;
    case 3: // RBX
        if (mode == 1) rbx = value;
        return rbx;
    case 5: // RBP
        if (mode == 1) rbp = value;
        return rbp;
    default:
        printf("\n [ERROR] Попытка доступа к нереализованному коду регистра: %d\n", reg_code);
        return 0;
    }
}

// Функция форматирования чисел с разделением тысяч апострофом
char * numf(long long num)
{
    static char buffers[4][32];
    static int buf_idx = 0;
    char * str = buffers[buf_idx];
    buf_idx = (buf_idx + 1) % 4;
    char temp[32];
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
        if (digit_count == 3) { str[dst--] = '\''; digit_count = 0; }
        str[dst--] = temp[src--];
        digit_count++;
    }
    if (is_negative) str[dst] = '-';
    return str;
}

// Функция для 64-битных регистров (Вывод: 8 тетрад, разделенных подчёркиванием)
char * bin64(uint64_t num)
{
    static char sbin[74]; 
    sbin[73] = '\0';
    int pos = 72;
    for (int i = 0; i < 64; i++)
    {
        if (pos == 36) sbin[pos--] = ' ';
        else if (pos == 63 || pos == 54 || pos == 45 || pos == 27 || pos == 18 || pos == 9) sbin[pos--] = '_';
        sbin[pos--] = ((num >> i) & 1) ? '1' : '0';
    }
    return sbin;
}

void print_segmented_reg(const char* name, uint64_t reg_val)
{
    uint8_t b7 = (reg_val >> 56) & 0xFF; uint8_t b6 = (reg_val >> 48) & 0xFF;
    uint8_t b5 = (reg_val >> 40) & 0xFF; uint8_t b4 = (reg_val >> 32) & 0xFF;
    uint8_t b3 = (reg_val >> 24) & 0xFF; uint8_t b2 = (reg_val >> 16) & 0xFF;
    uint8_t b1 = (reg_val >> 8)  & 0xFF; uint8_t b0 = reg_val         & 0xFF;
    printf("\n  %4s | %03d %03d %03d %03d %03d %03d %03d %03d | %02X %02X %02X %02X %02X %02X %02X %02X | %s", 
        name, b7, b6, b5, b4, b3, b2, b1, b0, b7, b6, b5, b4, b3, b2, b1, b0, numf((long long)reg_val));
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
    print_segmented_reg("       i", rip);
    print_segmented_reg("       s", rsp);
    print_segmented_reg("       b", rbp);
    printf("\n -------------------------------------------------------------------------------------------------------");
    printf("\n");
}