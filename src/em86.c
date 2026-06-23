#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "em86.txt"

// Функция очистки строки от начальных пробелов и комментариев
char* trim_and_clean(char *str)
{
    // Отрезаем комментарий, если он есть
    char *comment = strchr(str, ';');
    if (comment) *comment = '\0';

    // Пропускаем начальные пробелы и табуляции
    while (isspace((unsigned char)*str)) str++;

    // Отрезаем пробелы с конца строки
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    return str;
}

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

    char line[256];
    rip = 0;

    // Читаем файл строго построчно
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *cleaned = trim_and_clean(line);
        
        // Пропускаем пустые строки
        if (strlen(cleaned) == 0) continue;

        if (rip >= 0x1000)
        {
            printf("\n [Ошибка] Переполнение ОЗУ эмулятора!\n");
            fclose(file);
            return 1;
        }

        // Аппаратно-точный разбор взрослого синтаксиса Intel x86-64
        if (strcmp(cleaned, "hlt") == 0) { memory[rip++] = 0; }
        else if (strncmp(cleaned, "mov rcx, ", 9) == 0)
        {
            if (strcmp(cleaned + 9, "[rsi]") == 0) { memory[rip++] = 8; } // mov rcx, [rsi]
            else
            {
                memory[rip++] = 1; // mov rcx, i64
                memory[rip++] = atoll(cleaned + 9);
            }
        }
        else if (strncmp(cleaned, "jmp ", 4) == 0)
        {
            memory[rip++] = 2; // jmp i64 (абсолютный)
            memory[rip++] = atoll(cleaned + 4);
        }
        else if (strncmp(cleaned, "cmp rcx, ", 9) == 0)
        {
            memory[rip++] = 3; // cmp rcx, i64
            memory[rip++] = atoll(cleaned + 9);
        }
        else if (strncmp(cleaned, "je ", 3) == 0)
        {
            memory[rip++] = 4; // je i64 (абсолютный)
            memory[rip++] = atoll(cleaned + 3);
        }
        else if (strncmp(cleaned, "add rcx, ", 9) == 0)
        {
            memory[rip++] = 5; // add rcx, i64
            memory[rip++] = atoll(cleaned + 9);
        }
        else if (strcmp(cleaned, "syscall") == 0) { memory[rip++] = 6; }
        else if (strncmp(cleaned, "mov rax, ", 9) == 0)
        {
            memory[rip++] = 7; // mov rax, i64
            memory[rip++] = atoll(cleaned + 9);
        }
        else if (strcmp(cleaned, "mov rsi, rcx") == 0) { memory[rip++] = 9; }
        else if (strcmp(cleaned, "mov [rsi], rcx") == 0) { memory[rip++] = 10; }
        else if (strncmp(cleaned, "jmp_rel ", 8) == 0) // Относительный jmp
        {
            memory[rip++] = 11; 
            memory[rip++] = atoll(cleaned + 8);
        }
        else if (strncmp(cleaned, "je_rel ", 7) == 0) // Относительный je
        {
            memory[rip++] = 12; 
            memory[rip++] = atoll(cleaned + 7);
        }
        else if (strcmp(cleaned, "inc rcx") == 0) { memory[rip++] = 13; }
        else if (strcmp(cleaned, "inc rsi") == 0) { memory[rip++] = 14; }
        else { memory[rip++] = atoll(cleaned); } // Если строка — это просто одинокое число (например, ячейка данных в конце)
    }
    fclose(file);

    rip = 0;
    output_registers();
    putchar('\n');
    executor();

    putchar('\n');
    return 0;
}