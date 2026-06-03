#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define  OP__END_OF_FILE                0
#define  OP__MATCH_STRING               1  // Используется в target_segment (с длиной строки)
#define  OP__JUMP_IF_NOT_EQUAL          2  // Условный переход по флагу is_match
#define  OP__STEP_FORWARD               3  // Двигает курсор строго на +1
#define  OP__JUMP                       4  // Безусловный переход
#define  OP__INJECTION_UNTIL_TAG        5  // Инъекция Си-кода
#define  OP__GENERATE_CODE              6
#define  OP__MOVE_BY                    7
#define  OP__GENERATE_NUMERIC_ARGUMENT  8
#define  OP__GENERATE_STRING_ARGUMENT   9
#define  OP__BOOT_MATCH                 10 // Специальная изолированная команда для загрузчика
#define  OP__REGISTER_LABEL             11
#define  OP__GENERATE_LABEL_ARGUMENT    12

int dp = 0;           // Указатель на данные, которые парсим
char data[0xFFFFFF];  // Сами данные
int ip = 0;           // Указатель на мета-инструкцию
int is_match = 1;     // Флаг совпадения

int target_segment[1024]; // Наш чистый выходной буфер для нового байт-кода
int rules_idx = 0;

int is_generation_pass = 0; // 0 - собираем метки, 1 - честно генерируем код

char label_names[32][64];  // Имена собранных текстовых меток (например, "start", "skip")
int label_addresses[32];   // Физические адреса этих меток в байт-коде
int label_count = 0;       // Общий счетчик зарегистрированных меток

void execute_meta_core(const int *code_segment)
{
    repeat: if (data[dp] == '\0') { return; }

    switch (code_segment[ip]) {
    case OP__END_OF_FILE: break;

    case OP__BOOT_MATCH:
    {
        // Логика загрузчика: работает БЕЗ ячейки длины, чтобы не ломать boot_rules
        const char *pattern = (const char *)(intptr_t)code_segment[ip+1];
        if (!strncmp(&data[dp], pattern, strlen(pattern))) { is_match = 1; }
        else { is_match = 0; }
        fprintf(stderr, "\n [Загрузчик]: Проверяем слово \"%s\". Результат = %d", pattern, is_match);
        ip += 2;
        goto repeat;
    }

    case OP__MATCH_STRING:
    {
        // Логика рантайма: берет адрес и точную длину из ТРЕХ ячеек!
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
        // ТОЧЕЧНЫЙ ФИКС: пишем в память только на втором проходе!
        if (is_generation_pass) { target_segment[rules_idx] = command_to_write; }
        // Но индекс rules_idx мы ОБЯЗАНЫ наращивать на обоих проходах,
        // чтобы правильно высчитать виртуальные адреса будущих меток!
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
        rules_idx++; // Виртуально растим размер программы
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
                rules_idx += 2; // Строковый аргумент занимает ровно 2 ячейки в памяти!
                dp++;
            }
        }
        ip++;
        goto repeat;
    }

    case OP__REGISTER_LABEL:
    {
        int len = 0;
        // Бежим по слову, пока не встретим пробельные символы или двоеточие
        while (data[dp + len] != ' ' && data[dp + len] != '\r' && data[dp + len] != '\n' && data[dp + len] != ':' && data[dp + len] != '\0') 
        {
            len++;
        }
        // Если слово закончилось на двоеточие — это НАША МЕТКА!
        if (data[dp + len] == ':')
        {
            // Копируем имя метки в таблицу символов
            strncpy(label_names[label_count], &data[dp], len);
            label_names[label_count][len] = '\0'; // Не забываем терминатор строки!
            // Запоминаем текущую позицию генерации байт-кода для этой метки!
            label_addresses[label_count] = rules_idx;
            label_count++;
            // Продвигаем dp за само слово и символ двоеточия
            dp += (len + 1);
        }
        ip++;
        goto repeat;
    }

    case OP__GENERATE_LABEL_ARGUMENT:
    {
        char name[64];
        int len = 0;
        // 1. Выкусываем имя метки-аргумента из текста bootstrap.meta
        while (data[dp + len] != ' ' && data[dp + len] != '\r' && data[dp + len] != '\n' && data[dp + len] != '\0')
        {
            name[len] = data[dp + len];
            len++;
        }
        name[len] = '\0';
        // 2. Ищем адрес этой метки в нашей таблице символов
        int found_addr = -1;
        for (int i = 0; i < label_count; i++)
        {
            if (!strcmp(label_names[i], name))
            {
                found_addr = label_addresses[i];
                break;
            }
        }
        // 3. Если нашли — пишем адрес в target_segment строго на ВТОРОМ проходе!
        if (found_addr != -1)
        {
            if (is_generation_pass) { target_segment[rules_idx] = found_addr; }
        }
        else
        {
            fprintf(stderr, "\n [Ошибка]: Метка \"%s\" не найдена!", name);
            return;
        }
        rules_idx++; // Виртуально растим размер программы
        dp += len;   // Перешагиваем имя метки в тексте файла
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
        // Индекс 0: Если на текущей позиции стоит метка (например, start:) — она зарегистрируется в таблице
        OP__REGISTER_LABEL,

        // === БЛОК 1: Ищем и обрабатываем команду "STEP" ===
        // Индекс 1: Проверяем "STEP"
        OP__BOOT_MATCH, (intptr_t)"STEP",
        OP__JUMP_IF_NOT_EQUAL, 11, // Смещения пересчитаны строго на +1
        OP__GENERATE_CODE, OP__STEP_FORWARD,
        OP__MOVE_BY, 4,
        OP__JUMP, 0,

        // === БЛОК 2: Ищем и обрабатываем команду "JUMP_IF_NOT" ===
        // Индекс 11: Проверяем слово "JUMP_IF_NOT"
        OP__BOOT_MATCH, (intptr_t)"JUMP_IF_NOT",
        OP__JUMP_IF_NOT_EQUAL, 22,
        OP__GENERATE_CODE, OP__JUMP_IF_NOT_EQUAL,
        // ИСПРАВЛЕНО: вместо чтения жестких чисел заставляем ядро САМО искать адрес текстовой метки!
        OP__GENERATE_LABEL_ARGUMENT, 
        OP__MOVE_BY, 11,
        OP__JUMP, 0,

        // === БЛОК 3: Ищем и обрабатываем команду "MATCH" ===
        // Индекс 22: Проверяем слово "MATCH"
        OP__BOOT_MATCH, (intptr_t)"MATCH",
        OP__JUMP_IF_NOT_EQUAL, 34,
        OP__GENERATE_CODE, OP__MATCH_STRING,
        OP__GENERATE_STRING_ARGUMENT,
        OP__MOVE_BY, 5,
        OP__JUMP, 0,

        // === БЛОК 4: Ищем и обрабатываем команду "INJECTION" ===
        // Индекс 34: Проверяем слово "INJECTION"
        OP__BOOT_MATCH, (intptr_t)"INJECTION",
        OP__JUMP_IF_NOT_EQUAL, 45,
        OP__GENERATE_CODE, OP__INJECTION_UNTIL_TAG,
        OP__MOVE_BY, 9,
        OP__JUMP, 0,

        // === БЛОК 5: Пропуск обычного текста ===
        // Индекс 45: Шаг вперед по мусорному символу (\r, \n или пробелы)
        OP__STEP_FORWARD,
        // Индекс 46: Безусловный возврат на начало
        OP__JUMP, 0
    };
    // =========================================================================
    // ПРОХОД 1: СБОР МЕТОК (is_generation_pass = 0 по умолчанию)
    // =========================================================================
    fprintf(stderr, "\n [Загрузчик - Проход 1]: Сканируем bootstrap.meta и собираем адреса меток...\n");
    execute_meta_core(boot_rules);
    // =========================================================================
    // ПРОХОД 2: ЧЕСТНАЯ ГЕНЕРАЦИЯ КОДА С МЕТКАМИ
    // =========================================================================
    // Аппаратно сбрасываем указатели данных и инструкций, как при перезагрузке
    dp = 0;
    ip = 0;
    is_match = 1;
    rules_idx = 0; // Сбрасываем индекс записи, теперь мы будем честно писать с нуля!
    is_generation_pass = 1; // ВЗВОДИМ ФЛАГ ГЕНЕРАЦИИ!
    fprintf(stderr, "\n [Загрузчик - Проход 2]: Генерируем чистый байт-код в target_segment...\n");
    execute_meta_core(boot_rules);
    fprintf(stderr, "\n [Генератор]: Всего ячеек записано в target_segment: %d.", rules_idx);
    // ПЕРЕДАЕМ УПРАВЛЕНИЕ СГЕНЕРИРОВАННОМУ БАЙТ-КОДУ (РАНТАЙМ)
    if (rules_idx > 0)
    {
        target_segment[rules_idx] = OP__END_OF_FILE;
        dp = 0;
        ip = 0;
        is_match = 1;
        is_generation_pass = 0; // Для рантайма сбрасываем флаг в ноль
        fprintf(stderr, "\n [Рантайм]: Запуск программы из target_segment силами текстового DSL...\n");
        execute_meta_core(target_segment);
    }
    return 0;
}