#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "em86.txt"

// Вспомогательная функция очистки строк
char* trim_and_clean(char *str)
{
    char *comment = strchr(str, ';');
    if (comment) *comment = '\0';
    while (isspace((unsigned char)*str)) str++;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return str;
}

// Структура для хранения найденных меток (Таблица символов)
typedef struct {
    char name[64];
    int64_t address;
} Label;

Label label_table[100];
int label_count = 0;

// Поиск адреса метки в таблице
int64_t find_label(const char *name)
{
    for (int i = 0; i < label_count; i++) { if (strcmp(label_table[i].name, name) == 0) { return label_table[i].address; }}
    printf("\n [Ошибка] Метка '%s' не найдена!\n", name);
    exit(1);
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
    int64_t virtual_rip = 0;

    // --- ПРОХОД 1: Сбор адресов меток ---
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *cleaned = trim_and_clean(line);
        if (strlen(cleaned) == 0) continue;

        // Если строка заканчивается на двоеточие — это объявление метки!
        if (cleaned[strlen(cleaned) - 1] == ':')
        {
            cleaned[strlen(cleaned) - 1] = '\0'; // Отрезаем ':'
            strcpy(label_table[label_count].name, cleaned);
            label_table[label_count].address = virtual_rip;
            label_count++;
            continue; // Метка не занимает места в ОЗУ
        }

        // Считаем виртуальный размер команд для точного определения адресов
        if (strcmp(cleaned, "hlt") == 0 || strcmp(cleaned, "syscall") == 0 ||
            strcmp(cleaned, "mov rsi, rcx") == 0 || strcmp(cleaned, "mov [rsi], rcx") == 0 ||
            strcmp(cleaned, "inc rcx") == 0 || strcmp(cleaned, "inc rsi") == 0 ||
            strcmp(cleaned, "mov rcx, [rsi]") == 0)
        { virtual_rip += 1; }
        else { virtual_rip += 2; } // Команды с аргументами занимают 2 ячейки
    }

    // --- ПРОХОД 2: Генерация кода и автоподстановка смещений ---
    fseek(file, 0, SEEK_SET);
    rip = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *cleaned = trim_and_clean(line);
        if (strlen(cleaned) == 0 || cleaned[strlen(cleaned) - 1] == ':') continue;

        if (strcmp(cleaned, "hlt") == 0) { memory[rip++] = 0; }
        else if (strncmp(cleaned, "mov rcx, ", 9) == 0)
        {
            if (strcmp(cleaned + 9, "[rsi]") == 0) { memory[rip++] = 8; } 
            else
            {
                memory[rip++] = 1;
                // Проверяем, аргумент — это число или имя метки
                if (isalpha((unsigned char)cleaned[9])) memory[rip++] = find_label(cleaned + 9);
                else memory[rip++] = atoll(cleaned + 9);
            }
        }
        else if (strncmp(cleaned, "jmp ", 4) == 0)
        {
            memory[rip++] = 2;
            if (isalpha((unsigned char)cleaned[4])) memory[rip++] = find_label(cleaned + 4);
            else memory[rip++] = atoll(cleaned + 4);
        }
        else if (strncmp(cleaned, "cmp rcx, ", 9) == 0)
        {
            memory[rip++] = 3;
            memory[rip++] = atoll(cleaned + 9);
        }
        else if (strncmp(cleaned, "je ", 3) == 0)
        {
            memory[rip++] = 4;
            if (isalpha((unsigned char)cleaned[3])) memory[rip++] = find_label(cleaned + 3);
            else memory[rip++] = atoll(cleaned + 3);
        }
        else if (strncmp(cleaned, "add rcx, ", 9) == 0)
        {
            memory[rip++] = 5;
            memory[rip++] = atoll(cleaned + 9);
        }
        else if (strcmp(cleaned, "syscall") == 0) { memory[rip++] = 6; }
        else if (strncmp(cleaned, "mov rax, ", 9) == 0)
        {
            memory[rip++] = 7;
            memory[rip++] = atoll(cleaned + 9);
        }
        else if (strcmp(cleaned, "mov rsi, rcx") == 0) { memory[rip++] = 9; }
        else if (strcmp(cleaned, "mov [rsi], rcx") == 0) { memory[rip++] = 10; }
        else if (strncmp(cleaned, "jmp_rel ", 8) == 0)
        {
            memory[rip++] = 11;
            // АВТО-РАСЧЕТ ОТНОСИТЕЛЬНОГО СМЕЩЕНИЯ ДЛЯ JMP
            int64_t target = find_label(cleaned + 8);
            memory[rip++] = target - (rip + 1); 
        }
        else if (strncmp(cleaned, "je_rel ", 7) == 0)
        {
            memory[rip++] = 12;
            // АВТО-РАСЧЕТ ОТНОСИТЕЛЬНОГО СМЕЩЕНИЯ ДЛЯ JE
            int64_t target = find_label(cleaned + 7);
            memory[rip++] = target - (rip + 1);
        }
        else if (strcmp(cleaned, "inc rcx") == 0) { memory[rip++] = 13; }
        else if (strcmp(cleaned, "inc rsi") == 0) { memory[rip++] = 14; }
        else { memory[rip++] = atoll(cleaned); } // Чистые данные
    }
    fclose(file);

    // Интерактивный запуск
    rip = 0;
    output_registers();
    executor();
    return 0;
}