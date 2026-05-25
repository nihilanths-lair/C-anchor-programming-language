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
#define TOK_IF    6

// Глобальное состояние текущего токена (наши "структуры")
int tok_type;
int tok_value;
char tok_text[64];

// Указатель на текущую позицию в строке исходного кода
const char *src_ptr;
char *file_buffer = NULL;
int indent_level = 1; // По умолчанию 1 таб для тела main()
int if_counter = 0; // Счетчик для генерации уникальных имен флагов

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
        // Проверяем на ключевое слово "while" или "if"
        if (strcmp(tok_text, "while") == 0) { tok_type = TOK_WHILE; }
        else if (strcmp(tok_text, "if") == 0) { tok_type = TOK_IF; } // Сахарный IF!
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
void parse_if(); // Прототип перед parse_statements

// Функция разбора блока команд (внутри фигурных скобок или до конца файла)
void parse_statements()
{
    // Крутим цикл, пока не встретим закрывающую скобку или конец текста
    while (tok_type != TOK_EOF && (tok_type != TOK_OP || tok_text[0] != '}'))
    {
        if (tok_type == TOK_WHILE) { parse_while(); } 
        else if (tok_type == TOK_IF) { parse_if(); } // Ловим наш сахарный IF
        else if (tok_type == TOK_ID) { parse_assignment(); } 
        else { next_token(); } 
    }
}

// Разбор "сахарного" условия: if x { ... } -> превращается в одноразовый while со скрытым флагом
void parse_if()
{
    next_token(); // Пропускаем "if"
    if (tok_type != TOK_ID)
    {
        printf("// Ошибка синтаксиса: Ожидалось условие\n");
        return;
    }
    // Запоминаем оригинальное имя переменной условия (её мы портить НЕ будем)
    char cond_var[64];
    strcpy(cond_var, tok_text);
    // Генерируем уникальный номер флага для этого конкретного if
    int current_if_id = if_counter;
    if_counter++; // Сдвигаем счетчик для следующего if
    // 1. Создаем скрытый флаг в Си и присваиваем ему значение нашего условия
    print_indent();
    printf("int __flag_%d = %s;\n", current_if_id, cond_var);
    // 2. Запускаем цикл while по нашему флагу
    print_indent(); 
    printf("while (__flag_%d)\n", current_if_id); 
    print_indent(); 
    printf("{\n");
    next_token(); // Шагаем к '{'
    if (tok_type != TOK_OP || tok_text[0] != '{')
    {
        printf("// Ошибка синтаксиса: Ожидалась скобка '{'\n");
        return;
    }
    next_token(); // Заходим внутрь тела if
    indent_level++; 
    parse_statements(); // Парсим внутренности 
    // 3. МАГИЯ ДЕСУГАРИНГА: Перед выходом гасим скрытый флаг, а не оригинальную переменную!
    print_indent();
    printf("__flag_%d = 0;\n", current_if_id);
    indent_level--; 
    if (tok_type != TOK_OP || tok_text[0] != '}')
    {
        printf("// Ошибка синтаксиса: Ожидалась закрывающая скобка '}'\n");
        return;
    }
    print_indent();
    printf("}\n");
    next_token(); // Пропускаем '}'
}

// Разбор идентификатора: присваивание, декремент, вызов или объявление функции
void parse_assignment()
{
    char var_name[64];
    strcpy(var_name, tok_text);
    next_token(); // Шагаем к следующему токену за именем
    // Сценарий 1: Работа со скобками (Вызов или Объявление функции)
    if (tok_type == TOK_OP && tok_text[0] == '(')
    {
        next_token(); // Пропускаем '('
        // Переменные для проверки аргументов
        int has_arg = 0;
        int arg_num = 0;
        char arg_id[64] = {0};
        if (tok_type == TOK_NUM)
        {
            has_arg = 1;
            arg_num = tok_value;
            next_token();
        }
        else if (tok_type == TOK_ID)
        {
            has_arg = 2;
            strcpy(arg_id, tok_text);
            next_token();
        }
        if (tok_type != TOK_OP || tok_text[0] != ')')
        {
            printf("// Ошибка синтаксиса: Ожидалась закрывающая скобка ')'\n");
            return;
        }
        next_token(); // Шагаем за закрывающую скобку ')'
        // ПРОВЕРКА: Если дальше идет открывающая скобка '{' — это ОБЪЯВЛЕНИЕ функции!
        if (tok_type == TOK_OP && tok_text[0] == '{')
        {
            // Наш уровень отступа сбрасывается в 0, так как функция глобальная
            indent_level = 0; 
            // Если пользователь объявил main, переименовываем для Си в c_main
            if (strcmp(var_name, "main") == 0) { printf("void __main()\n"); }
            else { printf("void %s()\n", var_name); }
            printf("{\n");
            next_token(); // Пропускаем '{' и заходим в тело функции
            indent_level = 1; // Внутри функции отступ равен 1
            // Парсим внутренности функции. Они напечатаются внутри тела
            parse_statements();
            if (tok_type != TOK_OP || tok_text[0] != '}')
            {
                printf("// Ошибка синтаксиса: Ожидалась закрывающая скобка '}' в конце функции\n");
                return;
            }
            printf("}\n\n"); // Закрываем нашу функцию в Си
            next_token(); // Пропускаем '}'
            return;
        }
        // Если дальше НЕ скобка '{' — это обычный ВЫЗОВ функции
        print_indent();
        // Если пользователь вызывает main(), подменяем имя на c_main()
        if (strcmp(var_name, "main") == 0) { printf("__main("); }
        else { printf("%s(", var_name); }
        if (has_arg == 1) printf("%d", arg_num);
        if (has_arg == 2) printf("%s", arg_id);
        printf(");\n");
        return;
    }
    // Сценарий 2: Декремент (x--)
    if (tok_type == TOK_DEC)
    {
        print_indent();
        printf("%s--;\n", var_name);
        next_token();
        return;
    }
    // Сценарий 3: Обычное присваивание числа (x = 42)
    if (tok_type != TOK_OP || tok_text[0] != '=')
    {
        printf("// Ошибка синтаксиса: Ожидался знак '=', '--' или '('\n");
        return;
    }
    next_token();
    if (tok_type != TOK_NUM)
    {
        printf("// Ошибка синтаксиса: Ожидалось число\n");
        return;
    }
    print_indent();
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
    // 7. Выводим глобальную шапку Си-файла в стиле Allman
    printf("#include <stdio.h>\n");
    printf("#include <locale.h>\n\n");
    printf("// Глобальная спартанская память языка C$\n");
    printf("int x = 0;\n");
    printf("int y = 0;\n");
    printf("int z = 0;\n\n");
    next_token(); // Заряжаем первый токен из файла
    // 8. Запускаем парсер. Он разберет функции на глобальном уровне
    parse_statements();
    // 9. Автоматически дописываем точку входа Си, которая вызовет нашу функцию main()
    printf("int main()\n");
    printf("{\n");
    printf("    setlocale(0, \"\");\n");
    printf("    __main(); // Вызов главной функции\n");
    printf("    return 0;\n");
    printf("}\n");
    // Освобождаем память
    free(file_buffer);
    return 0;
}