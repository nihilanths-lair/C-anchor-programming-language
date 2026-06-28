#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>

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

// Помощник для подсчета размера директив данных (db/dq)
int64_t parse_data_directive(const char *cleaned, int64_t *out_array, int store)
{
    int64_t count = 0;
    if (strncmp(cleaned, "db ", 3) == 0)
    {
        const char *p = cleaned + 3;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '"')
        { // Если это текстовая строка в кавычках
            p++;
            while (*p && *p != '"')
            {
                if (store && out_array) out_array[count] = (int64_t)*p;
                count++;
                p++;
            }
            if (*p == '"') p++;
            // Проверяем, есть ли после строки запятая и другие элементы (например, , 0)
            while (*p)
            {
                if (*p == ',' || isspace((unsigned char)*p)) { p++; continue; }
                char tmp[64]; int j = 0;
                while (*p && *p != ',' && !isspace((unsigned char)*p)) tmp[j++] = *p++;
                tmp[j] = '\0';
                if (strlen(tmp) > 0)
                {
                    if (store && out_array) out_array[count] = atoll(tmp);
                    count++;
                }
            }
        }
        else
        { // Если это просто числа через запятую
            char buf[512]; strcpy(buf, p);
            char *tok = strtok(buf, ", \t");
            while (tok)
            {
                if (store && out_array) out_array[count] = atoll(tok);
                count++;
                tok = strtok(NULL, ", \t");
            }
        }
    }
    else if (strncmp(cleaned, "dq ", 3) == 0)
    {
        const char *p = cleaned + 3;
        char buf[512]; strcpy(buf, p);
        char *tok = strtok(buf, ", \t");
        while (tok)
        {
            if (store && out_array) out_array[count] = atoll(tok);
            count++;
            tok = strtok(NULL, ", \t");
        }
    }
    return count;
}
int main()
{
    setlocale(0, "");
/*
    FILE *source = fopen("main.meta", "r");
    if (source == NULL)
    {
        printf("\n [Ошибка] Не удалось открыть main.meta!\n");
        return 1;
    }

    FILE *c_file = fopen("injection.c", "w");

    char line[256];
    int in_c_injection = 0;
    rip = 0;

    // --- НАШ ГЛУПЫЙ И КОРЯВЫЙ ПЕРЕВОДЧИК СТРОК ---
    while (fgets(line, sizeof (line), source) != NULL)
    {
        // 1. ЕСЛИ МЫ ВНУТРИ ИНЪЕКЦИИ — КОПИРУЕМ СТРОКУ СРАЗУ, ДО ОЧИСТКИ ОТ КОММЕНТАРИЕВ СИ!
        if (in_c_injection)
        {
            // Проверяем на маркер конца, не очищая строку полностью
            if (strstr(line, "c_injection end") != NULL)
            {
                in_c_injection = 0;
                continue;
            }
            fprintf(c_file, "%s", line); // Сохраняем оригинальные отступы и точки с запятой
            continue;
        }

        // 2. ВНЕ ИНЪЕКЦИИ — ЧИСТИТЬ СТРОКУ ПОД АССЕМБЛЕР МОЖНО
        char * cleaned = trim_and_clean(line);
        if (strlen(cleaned) == 0) continue;

        // Ловим маркер старта Си-инъекции
        if (strcmp(cleaned, "c_injection start") == 0)
        {
            in_c_injection = 1;
            continue;
        }

        // Вне инъекции — парсим наш примитивный Pawn-байткод (пока сделаем простую заглушку)
        if (strcmp(cleaned, "call_c") == 0)
        {
            memory[rip++] = 7;  // mov rax, i64
            memory[rip++] = 99; // 99 — наш маркер вызова Си-кода
            memory[rip++] = 6;  // syscall
        }
        else if (strcmp(cleaned, "hlt") == 0) memory[rip++] = 0;
    }

    // Закрываем файл
    fclose(c_file);
    fclose(source);

    // --- КВАНТОВЫЙ ШАГ: Авто-компиляция Си-кода на лету через GCC или TCC ---
    printf("\n [Компилятор] Собираем injection.c в DLL...");
    // Вызываем системный компилятор (у тебя в системе должен быть доступен gcc или tcc в PATH)
    system("gcc -shared -o injection.dll injection.c");

    // Загружаем получившуюся DLL в память нашего эмулятора
    HMODULE hDll = LoadLibraryA("injection.dll");
    if (hDll != NULL)
    {
        injected_function = (InjectedFunction) GetProcAddress(hDll, "run_injection");
        if (injected_function == NULL) printf("\n [Ошибка] Не удалось найти функцию run_injection в DLL!\n");
    }
    else printf("\n [Ошибка] Не удалось загрузить injection.dll! Убедись, что GCC установлен и добавлен в PATH.\n");

    // Запуск железного исполнителя
    rip = 0;
*/
    Driver();
/*
    if (hDll) FreeLibrary(hDll);
*/
    return 0;
}
/*
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
*/