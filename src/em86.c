#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h> // для atoll()
#include <string.h>

#include "em86.txt"

int main()
{
    setlocale(0, "");

    // Открываем наш файл с мета-кодом
    FILE *file = fopen("parser.meta", "r");
    if (file == NULL)
    {
        printf("\n [Ошибка] Не удалось открыть файл parser.meta! Создай его рядом с em86.exe\n");
        return 1;
    }

    // Временный строковый буфер для чтения беспробельных мнемоник
    char token[128];

    // Сбрасываем RIP в ноль перед началом загрузки, чтобы использовать его как индекс ОЗУ
    rip = 0;

    // Читаем токены, разделенные пробелами или переносами строк, до конца файла (EOF)
    while (fscanf(file, "%127s", token) != EOF)
    {
        // Защита от выхода за границы ОЗУ
        if (rip >= 0x1000)
        {
            printf("\n [Ошибка] Файл parser.meta слишком большой для памяти эмулятора!\n");
            fclose(file);
            return 1;
        }

        // Строгое маппинг-декодирование беспробельных мнемоник Intel в наши гомогенные i64 опкоды
        if (strcmp(token, "hlt") == 0)              { memory[rip++] = 0; }
        else if (strcmp(token, "mov_rcx,i64") == 0) { memory[rip++] = 1; }
        else if (strcmp(token, "jmp_abs") == 0)     { memory[rip++] = 2; }
        else if (strcmp(token, "cmp_rcx,i64") == 0) { memory[rip++] = 3; }
        else if (strcmp(token, "je_abs") == 0)      { memory[rip++] = 4; }
        else if (strcmp(token, "add_rcx,i64") == 0) { memory[rip++] = 5; }
        else if (strcmp(token, "syscall") == 0)     { memory[rip++] = 6; }
        else if (strcmp(token, "mov_rax,i64") == 0) { memory[rip++] = 7; }
        else if (strcmp(token, "mov_rcx,[rsi]") == 0) { memory[rip++] = 8; }
        else if (strcmp(token, "mov_rsi,rcx") == 0) { memory[rip++] = 9; }
        else if (strcmp(token, "mov_[rsi],rcx") == 0) { memory[rip++] = 10; }
        else if (strcmp(token, "jmp_rel64") == 0)   { memory[rip++] = 11; }
        else if (strcmp(token, "je_rel64") == 0)    { memory[rip++] = 12; }
        else {
            // Если это не мнемоника, значит это просто числовое значение (аргумент или данные)
            memory[rip++] = atoll(token);
        }
    }
    fclose(file);

    // Сбрасываем RIP обратно в 0, чтобы физический процессор стартовал с первой считанной команды
    rip = 0;

    // Выводим начальное состояние регистров до запуска
    output_registers();
    putchar('\n');

    // Передаем управление нашему аппаратно-точному винту диспетчеризации
    executor();

    putchar('\n');
    return 0;
}