#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

// Константы типов токенов (в Pawn это были бы дефайны)
#define TOK_EOF   0
#define TOK_WHILE 1
#define TOK_ID    2
#define TOK_NUM   3
#define TOK_OP    4

// Глобальное состояние текущего токена (наши "структуры")
int tok_type;
int tok_value;
char tok_text[64];

// Указатель на текущую позицию в строке исходного кода
const char *src_ptr;

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

    // 4. Разбираем операторы (=, +, {, }, (, ))
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
    strcpy(var_name, tok_text); // Запомнили имя переменной
    next_token(); // Шагаем к '='

    if (tok_type != TOK_OP || tok_text[0] != '=')
    {
        printf("// Ошибка синтаксиса: Ожидался знак '='\n");
        return;
    }
    next_token(); // Шагаем к числу

    if (tok_type != TOK_NUM)
    {
        printf("// Ошибка синтаксиса: Ожидалось число\n");
        return;
    }

    // ГЕНЕРАЦИЯ: Сразу выводим Си-строку
    printf("    %s = %d;\n", var_name, tok_value);
    next_token(); // Переходим к следующему токену
}

// Разбор цикла вида: while x { ... }
void parse_while()
{
    next_token(); // Пропускаем само слово "while"
    if (tok_type != TOK_ID)
    {
        printf("// Ошибка синтаксиса: Ожидалось условие (имя переменной)\n");
        return;
    }
    // ГЕНЕРАЦИЯ: Пишем начало си-шного цикла с нашим условием
    printf("    while (%s) {\n", tok_text);
    next_token(); // Шагаем к открывающей скобке '{'

    if (tok_type != TOK_OP || tok_text[0] != '{')
    {
        printf("// Ошибка синтаксиса: Ожидалась скобка '{'\n");
        return;
    }
    next_token(); // Пропускаем '{' и заходим внутрь цикла
    parse_statements(); // Рекурсивно парсим все команды внутри тела цикла

    if (tok_type != TOK_OP || tok_text[0] != '}')
    {
        printf("// Ошибка синтаксиса: Ожидалась закрывающая скобка '}'\n");
        return;
    }
    // ГЕНЕРАЦИЯ: Закрываем цикл в Си
    printf("    }\n");
    next_token(); // Пропускаем '}'
}

int main()
{
    setlocale(0, "");
    // Первая подопытная программа на DSL, имеющая чистый синтаксис!
    const char *code = 
     "x = 5 "
     "while x { "
     "    y = 10 "
     "}"
    ;
    src_ptr = code;

    // Выводим каркас будущего Си-файла
    printf("\n #include <stdio.h>\n");
    printf("\n int main()");
    printf("\n {");
    printf("\n    // Наша спартанская память");
    printf("\n    int x = 0;");
    printf("\n    int y = 0;\n");

    next_token();       // Заряжаем первый токен в лексер
    parse_statements(); // Запускаем парсер

    printf("\n    return 0;");
    printf("\n }\n");

    return 0;
}