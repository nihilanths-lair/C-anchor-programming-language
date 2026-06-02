#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define  OP__END_OF_FILE          0
#define  OP__MATCH_STRING         1
#define  OP__JUMP_IF_NOT_EQUAL    2  // Условный переход по флагу is_match
#define  OP__STEP_FORWARD         3  // Двигает курсор строго на +1
#define  OP__JUMP                 4
#define  OP__INJECTION_UNTIL_TAG  5  // Инъекция Си-кода
#define  OP__GENERATE_CODE        6
#define  OP__MOVE_BY              7  // Двигает курсор на X

int dp = 0;           // Указатель на данные, которые парсим
char data[0xFFFFFF];  // Сами данные
int ip = 0;           // Указатель на мета-инструкцию
int is_match = 1;     // Флаг совпадения

int out_segment[1024]; // Наш чистый выходной буфер для нового байт-кода
int rules_idx = 0;

void execute_meta_core(const int *code_segment)
{
    repeat: switch (code_segment[ip]) {
    case OP__END_OF_FILE: break;
    case OP__MATCH_STRING:
    {
        // 1. Вытаскиваем адрес строки из следующей ячейки массива и приводим обратно к типу char*
        const char *pattern = (const char *)(intptr_t)code_segment[ip+1];
        
        // 2. Сверяем текст в памяти с паттерном на всю его длину через стандартную функцию strncmp
        if (!strncmp(&data[dp], pattern, strlen(pattern))) { is_match = 1; }
        else { is_match = 0; }
        
        // ТОЧЕЧНЫЙ ТЕСТ: выводим результат проверки прямо на экран консоли!
        fprintf(stderr, "\n [Тест]: Проверяем паттерн \"%s\". Результат is_match = %d", pattern, is_match);

        // 3. Так как эта команда теперь занимает 2 ячейки (команда + адрес строки),
        // мы сдвигаем ip на +2, чтобы перешагнуть аргумент. Курсор данных dp по-прежнему на месте!
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
        // 1. Ручным прыжком перешагиваем длину тега "</c-injection:" (14 символов)
        dp += 14;
        // 2. Внутренний цикл: слепо копируем код Си
        while (data[dp] != '\0')
        {
            // Используем твою двухэтапную проверку закрывающего тега
            if (data[dp] == '/')
            {
                if (data[dp + 1] == '>')
                {
                    dp += 2; // Перешагиваем сам закрывающий тег "/>"
                    break;   // Выходим из режима инъекции Си-кода
                }
            }

            // Защита от Windows-переносов строк \r\n, чтобы код не разрывало
            if (data[dp] != '\r') { putchar(data[dp]); }
            dp++;
        }
        ip++; // Шагаем к следующей мета-команде
        goto repeat;
    }
    case OP__GENERATE_CODE:
    {
        // Читаем число-аргумент из сегмента кода
        int command_to_write = code_segment[ip+1];
        
        // Безопасно пишем его в наш глобальный выходной сегмент!
        out_segment[rules_idx] = command_to_write;
        rules_idx++;
        
        ip += 2;
        goto repeat;
    }
    case OP__MOVE_BY:
    {
        // Прибавляем число-аргумент к нашему указателю данных dp
        dp += code_segment[ip+1];
        ip += 2; // Перешагиваем команду и ее аргумент
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

    // Переводим указатель на строку в число и кладем аргументом прямо в правила!
    int test_rules[] =
    {
        // Индекс 0: Проверяем "STEP" (занимает 2 ячейки)
        OP__MATCH_STRING, (intptr_t)"STEP",
        // Индекс 2: Если не "STEP" — прыгаем строго на Индекс 9 (к проверке "JUMP")!
        OP__JUMP_IF_NOT_EQUAL, 9,
        // Индекс 4: Генерируем код (занимает 2 ячейки)
        OP__GENERATE_CODE, OP__STEP_FORWARD,
        // Индекс 6: Перешагиваем 4 символа слова STEP (занимает 2 ячейки)
        OP__MOVE_BY, 4,
        // Индекс 8: Безусловный возврат на Индекс 0
        OP__JUMP, 0,
        // Индекс 9: Проверяем "JUMP" (занимает 2 ячейки)
        OP__MATCH_STRING, (intptr_t)"JUMP",
        // Индекс 11: Если не "JUMP" — прыгаем строго на Индекс 18 (на пропуск символа)!
        OP__JUMP_IF_NOT_EQUAL, 18,
        // Индекс 13: Генерируем код (занимает 2 ячейки)
        OP__GENERATE_CODE, OP__JUMP,
        // Индекс 15: Перешагиваем 4 символа слова JUMP (занимает 2 ячейки)
        OP__MOVE_BY, 4,
        // Индекс 17: Безусловный возврат на Индекс 0
        OP__JUMP, 0,
        // Индекс 18: Шаг вперед по обычному символу (\r, \n или пробелы)
        OP__STEP_FORWARD,
        // Индекс 19: Безусловный возврат на Индекс 0
        OP__JUMP, 0
    };
    execute_meta_core(test_rules);
    // ОТЛАДОЧНЫЙ ЧЕК: смотрим, записало ли ядро хоть что-то в наш out_segment!
    fprintf(stderr, "\n [Генератор]: Всего команд записано в out_segment: %d. Первая команда: %d", rules_idx, out_segment[0]);
    return 0;
}