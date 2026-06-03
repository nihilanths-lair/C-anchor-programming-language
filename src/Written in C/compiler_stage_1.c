#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define  OP__END_OF_FILE          0
#define  OP__MATCH_STRING         1  // Используется в target_segment (с длиной строки)
#define  OP__JUMP_IF_NOT_EQUAL    2  // Условный переход по флагу is_match
#define  OP__STEP_FORWARD         3  // Двигает курсор строго на +1
#define  OP__JUMP                 4  // Безусловный переход
#define  OP__INJECTION_UNTIL_TAG  5  // Инъекция Си-кода
#define  OP__GENERATE_CODE        6
#define  OP__MOVE_BY              7
#define  OP__GENERATE_NUMERIC_ARGUMENT 8
#define  OP__GENERATE_STRING_ARGUMENT  9
#define  OP__BOOT_MATCH           10 // Специальная изолированная команда для загрузчика
#define  OP__REGISTER_LABEL       11 // Наша новая команда для сборщика меток!
#define  OP__GENERATE_LABEL_ARGUMENT   12 // Генерация адреса метки

int dp = 0;           // Указатель на данные, которые парсим
char data[0xFFFFFF];  // Сами данные
int ip = 0;           // Указатель на мета-инструкцию
int is_match = 1;     // Флаг совпадения

int target_segment[1024]; // Наш чистый выходной буфер для нового байт-кода
int rules_idx = 0;
int is_generation_pass = 0; // 0 - собираем метки, 1 - честно генерируем код

char label_names[32][64];  // Имена собранных текстовых меток
int label_addresses[32];   // Физические адреса этих меток в байт-коде
int label_count = 0;       // Общий счетчик зарегистрированных меток

void execute_meta_core(const int *code_segment)
{
    repeat: if (data[dp] == '\0') { return; }

    switch (code_segment[ip]) {
    case OP__END_OF_FILE: break;

    case OP__BOOT_MATCH:
    {
        const char *pattern = (const char *)(intptr_t)code_segment[ip+1];
        if (!strncmp(&data[dp], pattern, strlen(pattern))) { is_match = 1; }
        else { is_match = 0; }
        
        fprintf(stderr, "\n [Загрузчик]: Проверяем слово \"%s\". Результат = %d", pattern, is_match);
        ip += 2;
        goto repeat;
    }

    case OP__MATCH_STRING:
    {
        const char *pattern = (const char *)(intptr_t)code_segment[ip+1];
        int pattern_length = code_segment[ip+2];
        
        if (!strncmp(&data[dp], pattern, pattern_length)) { is_match = 1; }
        else { is_match = 0; }
        
        fprintf(stderr, "\n [Рантайм-Тест]: Проверяем паттерн \"%.*s\". Результат = %d", pattern_length, pattern, is_match);
        ip += 3; 
        goto repeat;
    }

    case OP__JUMP_IF_NOT_EQUAL:
    {
        if (!is_match)
        {
            is_match = 1;
            ip = code_segment[ip+1];
        }
        else { ip += 2; }
        goto repeat;
    }

    case OP__STEP_FORWARD:
    {
        dp++;
        ip++;
        goto repeat;
    }

    case OP__JUMP:
    {
        ip = code_segment[ip+1];
        goto repeat;
    }

    case OP__INJECTION_UNTIL_TAG:
    {
        dp += 14;
        while (data[dp] != '\0')
        {
            if (data[dp] == '/')
            {
                if (data[dp + 1] == '>')
                {
                    dp += 2; 
                    break;   
                }
            }
            if (data[dp] != '\r') { putchar(data[dp]); }
            dp++;
        }
        ip++; 
        goto repeat;
    }

    case OP__GENERATE_CODE:
    {
        int command_to_write = code_segment[ip+1];
        if (is_generation_pass) { target_segment[rules_idx] = command_to_write; }
        rules_idx++; 
        ip += 2;
        goto repeat;
    }

    case OP__MOVE_BY:
    {
        dp += code_segment[ip+1];
        ip += 2; 
        goto repeat;
    }

    case OP__GENERATE_NUMERIC_ARGUMENT:
    {
        char *num_ptr = &data[dp];
        char *end_ptr;
        int value = (int)strtol(num_ptr, &end_ptr, 10);
        
        if (is_generation_pass) { target_segment[rules_idx] = value; }
        rules_idx++; 
        
        dp += (end_ptr - num_ptr);
        ip++;
        goto repeat;
    }

    case OP__GENERATE_STRING_ARGUMENT:
    {
        while (data[dp] != '"' && data[dp] != '\0') { dp++; }
        if (data[dp] == '"')
        {
            dp++; 
            char *str_start = &data[dp]; 
            while (data[dp] != '"' && data[dp] != '\0') { dp++; }
            if (data[dp] == '"')
            {
                int length = (int)(&data[dp] - str_start);
                if (is_generation_pass)
                {
                    target_segment[rules_idx] = (intptr_t)str_start;
                    target_segment[rules_idx + 1] = length;
                }
                rules_idx += 2; 
                dp++; 
            }
        }
        ip++; 
        goto repeat;
    }

    case OP__REGISTER_LABEL:
    {
        int len = 0;
        while (data[dp + len] != ' ' && data[dp + len] != '\r' && data[dp + len] != '\n' && data[dp + len] != ':' && data[dp + len] != '\0') { len++; }
        
        if (data[dp + len] == ':')
        {
            if (!is_generation_pass)
            {
                strncpy(label_names[label_count], &data[dp], len);
                label_names[label_count][len] = '\0';
                label_addresses[label_count] = rules_idx;
                label_count++;
            }
            dp += (len + 1);
            is_match = 1; 
        }
        else 
        { 
            is_match = 0; 
        }
        ip++;
        goto repeat;
    }

    case OP__GENERATE_LABEL_ARGUMENT:
    {
        char name[64];
        int len = 0;
        while (data[dp + len] != ' ' && data[dp + len] != '\r' && data[dp + len] != '\n' && data[dp + len] != '\0') 
        { 
            name[len] = data[dp + len];
            len++; 
        }
        name[len] = '\0';
        
        int found_addr = -1;
        for (int i = 0; i < label_count; i++)
        {
            if (!strcmp(label_names[i], name))
            {
                found_addr = label_addresses[i];
                break;
            }
        }
        
        if (found_addr != -1)
        {
            if (is_generation_pass) { target_segment[rules_idx] = found_addr; }
        }
        else
        {
            if (is_generation_pass) { fprintf(stderr, "\n [Ошибка]: Метка \"%s\" не найдена!", name); return; }
        }
        rules_idx++; 
        dp += len;   
        ip++;
        goto repeat;
    }

    default:
    {
        ip++;
        goto repeat;
    }}
}

int main(int argc, char *argv[])
{
    setlocale(0, "");
    if (argc < 2)
    {
        printf("\n compile: %s source_code.meta\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        printf("\n Не удалось открыть %s", argv[1]);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    fread(data, sizeof (char), file_size, file);
    data[file_size] = '\0';
    fclose(file);

    int boot_rules[] =
    {
        // === БЛОК 0: Автоматический перехват и сборка ассемблерных меток ===
        OP__REGISTER_LABEL,
        OP__JUMP_IF_NOT_EQUAL, 5, 
        OP__JUMP, 0,              

        // === БЛОК 1: Ищем и обрабатываем команду "STEP" ===
        OP__BOOT_MATCH, (intptr_t)"STEP",
        OP__JUMP_IF_NOT_EQUAL, 14, 
        OP__GENERATE_CODE, OP__STEP_FORWARD,
        OP__MOVE_BY, 4,
        OP__JUMP, 0,

        // === БЛОК 2: Ищем и обрабатываем команду "JUMP_IF_NOT" ===
        OP__BOOT_MATCH, (intptr_t)"JUMP_IF_NOT",
        OP__JUMP_IF_NOT_EQUAL, 25,
        OP__GENERATE_CODE, OP__JUMP_IF_NOT_EQUAL,
        OP__GENERATE_LABEL_ARGUMENT, 
        OP__MOVE_BY, 11,
        OP__JUMP, 0,

        // === БЛОК 3: Ищем и обрабатываем команду "MATCH" ===
        OP__BOOT_MATCH, (intptr_t)"MATCH",
        OP__JUMP_IF_NOT_EQUAL, 37,
        OP__GENERATE_CODE, OP__MATCH_STRING, 
        OP__GENERATE_STRING_ARGUMENT,
        OP__MOVE_BY, 5,
        OP__JUMP, 0,

        // === БЛОК 4: Ищем и обрабатываем команду "INJECTION" ===
        OP__BOOT_MATCH, (intptr_t)"INJECTION",
        OP__JUMP_IF_NOT_EQUAL, 48,
        OP__GENERATE_CODE, OP__INJECTION_UNTIL_TAG,
        OP__MOVE_BY, 9,
        OP__JUMP, 0,

        // === БЛОК 5: Пропуск обычного текста ===
        OP__STEP_FORWARD,
        OP__JUMP, 0
    };

    fprintf(stderr, "\n [Загрузчик - Проход 1]: Сканируем bootstrap.meta и собираем адреса меток...\n");
    execute_meta_core(boot_rules);
    
    dp = 0;
    ip = 0;
    is_match = 1;
    rules_idx = 0; 
    is_generation_pass = 1; 
    
    fprintf(stderr, "\n [Загрузчик - Проход 2]: Генерируем чистый байт-код в target_segment...\n");
    execute_meta_core(boot_rules);
    
    fprintf(stderr, "\n [Генератор]: Всего ячеек записано в target_segment: %d.", rules_idx);
    
    if (rules_idx > 0)
    {
        target_segment[rules_idx] = OP__END_OF_FILE;
        
        dp = 0;
        ip = 0;
        is_match = 1;
        is_generation_pass = 0; 
        
        fprintf(stderr, "\n [Рантайм]: Запуск программы из target_segment силами текстового DSL...\n");
        execute_meta_core(target_segment);
    }
    
    return 0;
}