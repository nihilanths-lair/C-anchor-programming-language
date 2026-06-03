#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define  OP__END_OF_FILE                0
#define  OP__MATCH_STRING               1
#define  OP__JUMP_IF_NOT_EQUAL          2  // Условный переход по флагу is_match
#define  OP__STEP_FORWARD               3  // Двигает курсор строго на +1
#define  OP__JUMP                       4
#define  OP__INJECTION_UNTIL_TAG        5  // Инъекция Си-кода
#define  OP__GENERATE_CODE              6
#define  OP__MOVE_BY                    7
#define  OP__GENERATE_NUMERIC_ARGUMENT  8  // Генерация числового аргумента
#define  OP__GENERATE_STRING_ARGUMENT   9  // Генерация строкового аргумента

int dp = 0;           // Указатель на данные, которые парсим
char data[0xFFFFFF];  // Сами данные
int ip = 0;           // Указатель на мета-инструкцию
int is_match = 1;     // Флаг совпадения

int out_segment[1024]; // Наш чистый выходной буфер для нового байт-кода
int rules_idx = 0;

void execute_meta_core(const int *code_segment)
{
    repeat: if (data[dp] == '\0') { return; }

    switch (code_segment[ip]) {
    case OP__END_OF_FILE: break;

    case OP__MATCH_STRING:
    {
        const char *pattern = (const char *)(intptr_t)code_segment[ip+1];
        if (!strncmp(&data[dp], pattern, strlen(pattern))) { is_match = 1; }
        else { is_match = 0; }
        
        fprintf(stderr, "\n [Тест]: Проверяем паттерн \"%s\". Результат is_match = %d", pattern, is_match);
        
        ip += 2; 
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
        out_segment[rules_idx] = command_to_write;
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
        
        out_segment[rules_idx] = value;
        rules_idx++;
        
        dp += (end_ptr - num_ptr);
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

    int test_rules[] =
    {
        // === БЛОК 1: Ищем и обрабатываем команду "STEP" ===
        // Индекс 0: Проверяем "STEP"
        OP__MATCH_STRING, (intptr_t)"STEP",
        // Индекс 2: Если не "STEP" — прыгаем на Индекс 10 (к проверке "JUMP_IF_NOT")
        OP__JUMP_IF_NOT_EQUAL, 10,
        // Индекс 4: Генерируем код OP__STEP_FORWARD
        OP__GENERATE_CODE, OP__STEP_FORWARD,
        // Индекс 6: Перешагиваем 4 символа слова STEP
        OP__MOVE_BY, 4,
        // Индекс 8: Возврат на начало
        OP__JUMP, 0,

        // === БЛОК 2: Ищем и обрабатываем команду "JUMP_IF_NOT" ===
        // Индекс 10: Проверяем слово "JUMP_IF_NOT"
        OP__MATCH_STRING, (intptr_t)"JUMP_IF_NOT",
        // Индекс 12: Если не "JUMP_IF_NOT" — прыгаем строго на Индекс 21 (на пропуск символа)
        OP__JUMP_IF_NOT_EQUAL, 21,
        // Индекс 14: Генерируем в out_segment код условного перехода (число 2)
        OP__GENERATE_CODE, OP__JUMP_IF_NOT_EQUAL,
        // Индекс 16: Парсим число из текста и вшиваем аргументом вслед за командой
        OP__GENERATE_NUMERIC_ARGUMENT,
        // Индекс 17: Перешагиваем 11 символов слова "JUMP_IF_NOT"
        OP__MOVE_BY, 11,
        // Индекс 19: Возврат на начало
        OP__JUMP, 0,

        // === БЛОК 3: Пропуск обычного текста ===
        // Индекс 21: Шаг вперед по мусорному символу (\r, \n или пробелы)
        OP__STEP_FORWARD,
        // Индекс 22: Безусловный возврат на начало
        OP__JUMP, 0
    };

    execute_meta_core(test_rules);
    
    // ОТЛАДОЧНЫЙ ЛОГ: смотрим, сколько ВСЕГО ячеек (команд + аргументов) сгенерировало ядро!
    fprintf(stderr, "\n [Генератор]: Всего ячеек записано в out_segment: %d. Первая команда: %d", rules_idx, out_segment[0]);
    
    return 0;
}