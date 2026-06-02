#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define  OP__END_OF_FILE    0
#define  OP_ADVANCE         1  // Двигает курсор строго на +1
#define  OP_MOVE_BY         2  // Двигает курсор на X
#define  OP__MATCH_STRING   3  // Зоркая проверка целой строки без сдвига курсора
#define  OP_JUMP_IF_NOT     4  // Условный переход по флагу is_match
#define  OP_EMIT_UNTIL_TAG  5  // Инъекция Си-кода

int dp = 0; // Указатель на данные, которые парсим
char data[0xFFFFFF]; // Сами данные
int ip = 0; // Указатель на мета-инструкцию
int is_match = 1; // Флаг совпадения

void execute_meta_core(const int *rules_table)
{
    repeat: switch (rules_table[ip]) {
    case OP__END_OF_FILE: break;

    case OP__MATCH_STRING:
    {
        // 1. Вытаскиваем адрес строки из следующей ячейки массива и приводим обратно к типу char*
        const char *pattern = (const char *)(intptr_t)rules_table[ip+1];
        
        // 2. Сверяем текст в памяти с паттерном на всю его длину через стандартную функцию strncmp
        if (!strncmp(&data[dp], pattern, strlen(pattern))) { is_match = 1; }
        else { is_match = 0; }
        
        // ТОЧЕЧНЫЙ ТЕСТ: выводим результат проверки прямо на экран консоли!
        fprintf(stderr, "\n [Тест]: Проверяем паттерн \"%s\". Результат is_match = %d", pattern, is_match);

        // 3. Так как эта команда теперь занимает 2 ячейки (команда + адрес строки),
        // мы сдвигаем ip на +2, чтобы перешагнуть аргумент. Курсор данных dp по-прежнему на месте!
        ip += 2; 
        break;
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
        OP__MATCH_STRING, (intptr_t)"</c-injection:", 
        OP__END_OF_FILE
    };
    execute_meta_core(test_rules);
    return 0;
}