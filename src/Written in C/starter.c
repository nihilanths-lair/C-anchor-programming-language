#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

// Константы типов токенов
#define TOK_EOF   0
#define TOK_WHILE 1
#define TOK_ID    2
#define TOK_NUM   3
#define TOK_OP    4
#define TOK_DEC   5

// Глобальное состояние текущего токена (наши "структуры")
int tok_type;
int tok_value;
char tok_text[64];

// Указатель на текущую позицию в строке исходного кода
const char *src_ptr;
char *file_buffer = NULL;
int indent_level = 1; // По умолчанию 1 таб для тела main()

void print_indent() { for (int i = 0; i < indent_level; i++) { printf("\x20\x20\x20\x20"); } }

// Поточная функция: считывает ровно ОДИН следующий токен
void next_token()
{
    // 1. Пропускаем пробелы
    while (*src_ptr != '\0' && isspace(*src_ptr)) { src_ptr++; }

    // Если дошли до конца текста
    if (*src_ptr == '\0')
    {
        tok_type = TOK_EOF;
        return;
    }

    // 2. Разбираем числа
    if (isdigit(*src_ptr))
    {
        tok_value = 0;
        while (isdigit(*src_ptr))
        {
            tok_value = tok_value * 10 + (*src_ptr - '0');
            src_ptr++;
        }
        tok_type = TOK_NUM;
        return;
    }

    // 3. Разбираем имена переменных и ключевые слова
    if (isalpha(*src_ptr) || *src_ptr == '_')
    {
        int len = 0;
        while (isalnum(*src_ptr) || *src_ptr == '_')
        {
            if (len < 63) { tok_text[len++] = *src_ptr; }
            src_ptr++;
        }
        tok_text[len] = '\0'; // Закрываем строку

        // Проверяем на ключевое слово "while"
        if (strcmp(tok_text, "while") == 0) { tok_type = TOK_WHILE; }
        else { tok_type = TOK_ID; } // Просто переменная
        return;
    }

    // 4. Разбираем операторы и знаки
    if (*src_ptr == '-' && *(src_ptr + 1) == '-')
    {
        tok_text[0] = '-'; tok_text[1] = '-'; tok_text[2] = '\0';
        tok_type = TOK_DEC;
        src_ptr += 2; // Шагаем сразу через два символа
        return;
    }

    // Все остальные одиночные символы (=, +, {, }, (, ))
    tok_text[0] = *src_ptr;
    tok_text[1] = '\0';
    tok_type = TOK_OP;
    src_ptr++;
}

// Прототипы функций, чтобы Си не ругался на порядок их объявления
void parse_statements();
void parse_assignment();
void parse_while();

// Функция разбора блока команд (внутри фигурных скобок или до конца файла)
void parse_statements()
{
    // Крутим цикл, пока не встретим закрывающую скобку или конец текста
    while (tok_type != TOK_EOF && (tok_type != TOK_OP || tok_text[0] != '}'))
    {
        if (tok_type == TOK_WHILE) { parse_while(); } 
        else if (tok_type == TOK_ID) { parse_assignment(); } 
        else { next_token(); } // Если встретили что-то непонятное (например, случайную точку с запятой), просто пропускаем
    }
}

// Разбор присваивания вида: x = 42
void parse_assignment()
{
    char var_name[64];
    strcpy(var_name, tok_text);
    next_token();
    if (tok_type == TOK_DEC)
    {
        print_indent(); // Печатаем правильный отступ динамически
        printf("%s--;\n", var_name);
        next_token();
        return;
    }
    if (tok_type != TOK_OP || tok_text[0] != '=')
    {
        printf("// Ошибка синтаксиса: Ожидался знак '=' или '--'\n");
        return;
    }
    next_token();
    if (tok_type != TOK_NUM)
    {
        printf("// Ошибка синтаксиса: Ожидалось число\n");
        return;
    }
    print_indent(); // Печатаем правильный отступ динамически
    printf("%s = %d;\n", var_name, tok_value);
    next_token();
}

// Разбор цикла вида: while x { ... }
void parse_while()
{
    next_token(); // Пропускаем "while"
    if (tok_type != TOK_ID)
    {
        printf("// Ошибка синтаксиса: Ожидалось условие\n");
        return;
    }
    print_indent(); // Отступ для самого ключевого слова while
    printf("while (%s)\n", tok_text);
    print_indent(); // Отступ для открывающей скобки на отдельной строке
    printf("{\n");
    next_token(); // Шагаем к '{'
    if (tok_type != TOK_OP || tok_text[0] != '{')
    {
        printf("// Ошибка синтаксиса: Ожидалась скобка '{'\n");
        return;
    }
    next_token(); // Заходим внутрь цикла
    indent_level++; // Магия: увеличиваем уровень отступа для внутренностей цикла!
    parse_statements(); // Парсим внутренности (они напечатаются со смещением +1 таб)
    indent_level--; // Возвращаем уровень отступа назад
    if (tok_type != TOK_OP || tok_text[0] != '}')
    {
        printf("// Ошибка синтаксиса: Ожидалась закрывающая скобка '}'\n");
        return;
    }
    print_indent(); // Отступ для закрывающей скобки на отдельной строке
    printf("}\n");
    next_token(); // Пропускаем '}'
}

int main(int argc, char *argv[])
{
    setlocale(0, "");
    // 1. Проверяем, передал ли пользователь имя файла
    if (argc < 2)
    {
        printf("\n Использование: %s <имя_файла.cdlr>\n", argv[0]);
        return 1;
    }
    // 2. Открываем файл для чтения в бинарном режиме
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        printf("Ошибка: Не удалось открыть файл %s\n", argv[1]);
        return 1;
    }
    // 3. Узнаем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    // 4. Выделяем память под текст файла (+1 байт для финала строки '\0')
    file_buffer = (char *) malloc(file_size + 1);
    if (file_buffer == NULL)
    {
        printf("Ошибка: Не удалось выделить память\n");
        fclose(file);
        return 1;
    }
    // 5. Считываем весь файл целиком в наш буфер
    fread(file_buffer, 1, file_size, file);
    file_buffer[file_size] = '\0'; // Обязательно закрываем строку null-терминатором
    fclose(file);
    // 6. Направляем указатель лексера на считанный из файла текст
    src_ptr = file_buffer;
    // 7. Выводим каркас будущего Си-файла в стиле Allman
    printf("#include <stdio.h>");
    putchar('\n');
    printf("\nint main()");
    printf("\n{");
    printf("\n    // Наша спартанская память");
    printf("\n    int x = 0;");
    printf("\n    int y = 0;");
    printf("\n    /***/\n");
    next_token();       // Заряжаем первый токен из файла
    parse_statements(); // Запускаем парсер
    printf("    /***/");
    printf("\n    return 0;");
    printf("\n}");
    // Освобождаем память
    free(file_buffer);
    return 0;
}