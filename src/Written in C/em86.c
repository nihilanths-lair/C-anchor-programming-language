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
    // Жёстко заданная тестовая программа
    char * test_program[] =
    {
        "mov rsi, 15",
        "mov rcx, 4000",
        "mov [rsi], rcx",
        "hlt"
        /*
        // Следующая микропрограмма для теста
        "mov rcx, 50",    // Положим в RCX число 50
        "cmp rcx, 50",    // Опкод 6: Сравниваем RCX с числом 50 (флаг ZF должен стать 1!)
        "je 8",           // Опкод 7: Если ZF=1, абсолютно прыгаем на rip=8 (прямо на hlt)
        "mov rax, 999",   // Этот шаг должен БЫТЬ ПРОПУЩЕН, если прыжок сработает!
        "hlt"             // Точка прыжка (rip=8)
        */
    };
    int program_lines = 4;
    int v_rip = 0;
    printf("\n [Генератор] Запущена трансляция.\n");
    //fflush(stdout);
    for (int i = 0; i < program_lines; i++)
    {
        // Создаем локальный изменяемый буфер
        char local_buf[256];
        strncpy(local_buf, test_program[i], sizeof (local_buf) - 1);
        local_buf[sizeof (local_buf) - 1] = '\0';
        char * cleaned = trim_and_clean(local_buf);
        if (strlen(cleaned) == 0) continue;
        printf("\n -> Обработка: \"%s\"", cleaned);
        //fflush(stdout);
        if (!strcmp(cleaned, "hlt")) memory[v_rip++] = 0;
        else if (!strncmp(cleaned, "mov rax, ", 9))
        {
            memory[v_rip++] = 1;
            memory[v_rip++] = atoll(cleaned + 9);
        }
        else if (!strncmp(cleaned, "mov rbx, ", 9))
        {
            memory[v_rip++] = 2;
            memory[v_rip++] = atoll(cleaned + 9);
        }
        else if (!strncmp(cleaned, "mov rcx, ", 9))
        {
            memory[v_rip++] = 3;
            memory[v_rip++] = atoll(cleaned + 9);
        }
        else if (!strncmp(cleaned, "mov rdx, ", 9))
        {
            memory[v_rip++] = 4;
            memory[v_rip++] = atoll(cleaned + 9);
        }
        else if (!strncmp(cleaned, "mov rsi, ", 9))
        {
            memory[v_rip++] = 5;
            memory[v_rip++] = atoll(cleaned + 9);
        }
        else if (!strncmp(cleaned, "cmp rcx, ", 9))
        {
            memory[v_rip++] = 7; // cmp rcx, i64
            memory[v_rip++] = atoll(cleaned + 9);
        }
        else if (!strncmp(cleaned, "je ", 3))
        {
            memory[v_rip++] = 8; // je i64 (абсолютный)
            memory[v_rip++] = atoll(cleaned + 3);
        }
        /*
        else if (!strncmp(cleaned, "add rcx, ", 9))
        {
            memory[v_rip++] = 8; // Наш опкод 8 — add rcx, i64
            memory[v_rip++] = atoll(cleaned + 9);
        }
        */
        else if (!strcmp(cleaned, "mov rsi, rcx")) memory[v_rip++] = 12;
        else if (!strcmp(cleaned, "mov [rsi], rcx")) memory[v_rip++] = 13;
    }
    printf("\n\n [Генератор] Трансляция завершена.");
    //fflush(stdout);
    Driver();
    return 0;
}