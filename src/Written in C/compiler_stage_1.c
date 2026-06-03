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
                // 1. Вычисляем точную физическую длину строки в кавычках
                int length = (int)(&data[dp] - str_start);
                // 2. Вшиваем в out_segment СНАЧАЛА адрес начала строки...
                out_segment[rules_idx] = (intptr_t)str_start;
                rules_idx++;
                // 3. ...а СРАЗУ СЛЕДОМ вшиваем её длину!
                out_segment[rules_idx] = length;
                rules_idx++;
                dp++;
            }
        }
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

    // НАШ ИСТИННЫЙ ПЕРВИЧНЫЙ ЗАГРУЗЧИК (BOOTLOADER)
    int boot_rules[] =
    {
        // === БЛОК 1: Ищем и обрабатываем команду "STEP" ===
        // Индекс 0: Проверяем "STEP" (занимает 2 ячейки)
        OP__MATCH_STRING, (intptr_t)"STEP",
        // Индекс 2: Если не "STEP" — прыгаем строго на Блок 2 (на Индекс 10!)
        OP__JUMP_IF_NOT_EQUAL, 10,
        // Индекс 4: Генерируем код (занимает 2 ячейки)
        OP__GENERATE_CODE, OP__STEP_FORWARD,
        // Индекс 6: Перешагиваем 4 символа слова STEP (занимает 2 ячейки)
        OP__MOVE_BY, 4,
        // Индекс 8: Безусловный возврат на Индекс 0
        OP__JUMP, 0,

        // === БЛОК 2: Ищем и обрабатываем команду "JUMP_IF_NOT" ===
        // Индекс 10: Проверяем слово "JUMP_IF_NOT" (занимает 2 ячейки)
        OP__MATCH_STRING, (intptr_t)"JUMP_IF_NOT",
        // Индекс 12: Если не "JUMP_IF_NOT" — прыгаем строго на Блок 3 (на Индекс 21!)
        OP__JUMP_IF_NOT_EQUAL, 21,
        // Индекс 14: Генерируем код условного перехода (занимает 2 ячейки)
        OP__GENERATE_CODE, OP__JUMP_IF_NOT_EQUAL,
        // Индекс 16: Парсим число из текста и вшиваем аргументом (занимает 1 ячейку)
        OP__GENERATE_NUMERIC_ARGUMENT,
        // Индекс 17: Перешагиваем 11 символов слова "JUMP_IF_NOT" (занимает 2 ячейки)
        OP__MOVE_BY, 11,
        // Индекс 19: Безусловный возврат на Индекс 0
        OP__JUMP, 0,

        // === БЛОК 3: Ищем и обрабатываем команду "MATCH" ===
        // Индекс 21: Проверяем слово "MATCH" (занимает 2 ячейки)
        OP__MATCH_STRING, (intptr_t)"MATCH",
        // Индекс 23: Если не "MATCH" — прыгаем строго на Блок 4 (на Индекс 33!)
        OP__JUMP_IF_NOT_EQUAL, 33,
        // Индекс 25: Генерируем код строкового сопоставления (занимает 2 ячейки)
        OP__GENERATE_CODE, OP__MATCH_STRING,
        // Индекс 27: Парсим кавычки и вшиваем адрес строки аргументом (занимает 1 ячейку)
        OP__GENERATE_STRING_ARGUMENT,
        // Индекс 28: Перешагиваем 5 символов слова "MATCH" (занимает 2 ячейки)
        OP__MOVE_BY, 5,
        // Индекс 30: Безусловный возврат на Индекс 0
        OP__JUMP, 0,

        // === БЛОК 4: Ищем и обрабатываем команду "INJECTION" ===
        // Индекс 33: Проверяем слово "INJECTION" (занимает 2 ячейки)
        OP__MATCH_STRING, (intptr_t)"INJECTION",
        // Индекс 35: Если не "INJECTION" — прыгаем строго на Блок 5 (на Индекс 44!)
        OP__JUMP_IF_NOT_EQUAL, 44,
        // Индекс 37: Генерируем код слепого Си-вывода (занимает 2 ячейки)
        OP__GENERATE_CODE, OP__INJECTION_UNTIL_TAG,
        // Индекс 39: Перешагиваем 9 символов слова "INJECTION" (занимает 2 ячейки)
        OP__MOVE_BY, 9,
        // Индекс 41: Безусловный возврат на Индекс 0
        OP__JUMP, 0,

        // === БЛОК 5: Пропуск обычного текста ===
        // Индекс 44: Шаг вперед по обычному символу (\r, \n или пробелы)
        OP__STEP_FORWARD,
        // Индекс 45: Безусловный возврат на Индекс 0
        OP__JUMP, 0
    };
    // Запускаем первичный загрузчик!
    execute_meta_core(boot_rules);
    // ОТЛАДОЧНЫЙ ЛОГ: смотрим, сколько ВСЕГО ячеек (команд + аргументов) сгенерировал загрузчик!
    fprintf(stderr, "\n [Генератор]: Всего ячеек записано в out_segment: %d. Первая команда: %d", rules_idx, out_segment[0]);
    // ВЕЛИКИЙ ВТОРОЙ ПРОХОД: ПЕРЕДАЕМ УПРАВЛЕНИЕ СГЕНЕРИРОВАННОМУ БАЙТ-КОДУ!
    if (rules_idx > 0)
    {
        // 1. Обязательно добавляем команду OP__END_OF_FILE в самый конец нашего нового кода,
        // чтобы мета-процессор знал, где остановиться!
        out_segment[rules_idx] = OP__END_OF_FILE;
        // 2. Полностью сбрасываем указатели "железа" в ноль, как при перезагрузке процессора
        dp = 0;
        ip = 0;
        is_match = 1;
        fprintf(stderr, "\n [Рантайм]: Запуск программы из out_segment силами текстового DSL...");
        // 3. Запускаем ядро ВТОРОЙ РАЗ, но скармливаем ему наш динамический out_segment!
        execute_meta_core(out_segment);
    }
    return 0;
}