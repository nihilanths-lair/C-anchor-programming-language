#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
//#include <locale.h>
#include <windows.h>

// Константы типов токенов
#define TOK_EOF   0
#define TOK_WHILE 1
#define TOK_ID    2
#define TOK_NUM   3
#define TOK_OP    4
#define TOK_DEC   5
#define TOK_IF    6  // Уникальный номер для if
#define TOK_EQ    7  // Уникальный номер для ==
#define TOK_NEQ   8  // Уникальный номер для !=
#define TOK_STR   9  // Токен для строк в кавычках "текст"
#define TOK_CHAR  10 // Токен для одиночных символов в кавычках 'x'

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
    // НОВАЯ ФИЧА: Разбираем СТРОКИ в кавычках (Например: "Итерация цикла\n")
    if (*src_ptr == '"')
    {
        src_ptr++; // Пропускаем открывающую кавычку
        int len = 0;
        while (*src_ptr != '"' && *src_ptr != '\0')
        {
            if (len < 63) { tok_text[len++] = *src_ptr; }
            src_ptr++;
        }
        tok_text[len] = '\0';
        if (*src_ptr == '"') { src_ptr++; } // Пропускаем закрывающую кавычку
        tok_type = TOK_STR;
        return;
    }
    // НОВАЯ ФИЧА: Разбираем СИМВОЛЬНЫЕ литералы в одинарных кавычках (Например: '\n')
    if (*src_ptr == '\'')
    {
        src_ptr++; // Пропускаем открывающую кавычку
        int len = 0;
        // Считываем, пока не встретим закрывающую кавычку или конец строки
        while (*src_ptr != '\'' && *src_ptr != '\0')
        {
            if (*src_ptr == '\\' && *(src_ptr + 1) != '\0') // Обработка экранирования
            {
                tok_text[len++] = *src_ptr++;
            }
            if (len < 63) { tok_text[len++] = *src_ptr; }
            src_ptr++;
        }
        tok_text[len] = '\0';
        if (*src_ptr == '\'') { src_ptr++; } // Пропускаем закрывающую кавычку
        tok_type = TOK_CHAR;
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
    // ПРОВЕРКА НА ОПЕРАТОР ==
    if (*src_ptr == '=' && *(src_ptr + 1) == '=')
    {
        tok_text[0] = '='; tok_text[1] = '='; tok_text[2] = '\0';
        tok_type = TOK_EQ;
        src_ptr += 2;
        return;
    }
    // ПРОВЕРКА НА ОПЕРАТОР !=
    if (*src_ptr == '!' && *(src_ptr + 1) == '=')
    {
        tok_text[0] = '!'; tok_text[1] = '='; tok_text[2] = '\0';
        tok_type = TOK_NEQ; // Привели к единому имени TOK_NEQ
        src_ptr += 2;
        return;
    }
    // Все остальные одиночные символы (=, +, {, }, (, ))
    tok_text[0] = *src_ptr;
    tok_text[1] = '\0';
    tok_type = TOK_OP;
    src_ptr++;
}

// Прототипы функций, чтобы Си не ругался на порядок их объявления
void parse_memory_store();
void parse_statements();
void parse_assignment();
void parse_while();
void parse_if(); // Прототип перед parse_statements

// Разбор записи в память вида: __[i] = c, __['$'] = 42, или __ = "строка"
void parse_memory_store()
{
    if (tok_type != TOK_OP || tok_text[0] != '[') { print_indent(); printf("// Ошибка: Ожидалась '['\n"); return; }
    next_token(); // Пропускаем открывающую '[' и шагаем к индексу
    int is_num_idx = 0;
    int is_char_idx = 0;
    int num_idx_val = 0;
    char index_var[64];
    char char_idx_val[64];
    if (tok_type == TOK_NUM) { is_num_idx = 1; num_idx_val = tok_value; }
    else if (tok_type == TOK_ID) { strcpy(index_var, tok_text); }
    else if (tok_type == TOK_CHAR) { is_char_idx = 1; strcpy(char_idx_val, tok_text); } // НОВАЯ ФИЧА: Символьный индекс
    else { print_indent(); printf("// Ошибка синтаксиса: Ожидался индекс\n"); return; }
    next_token(); // Шагаем к закрывающей ']'
    if (tok_type != TOK_OP || tok_text[0] != ']') { print_indent(); printf("// Ошибка: Ожидалась ']'\n"); return; }
    next_token(); // Шагаем к '='
    if (tok_type != TOK_OP || tok_text[0] != '=') { print_indent(); printf("// Ошибка: Ожидался знак '='\n"); return; }
    next_token(); // Шагаем к значению
    print_indent();
    if (is_num_idx) { printf("__[%d] = ", num_idx_val); }
    else if (is_char_idx) { printf("__['%s'] = ", char_idx_val); } // Генерируем символьный индекс для Си
    else { printf("__[%s] = ", index_var); }
    // Проверяем, что именно присваиваем (число, переменную, символ или СТРОКУ)
    if (tok_type == TOK_NUM) { printf("%d;\n", tok_value); }
    else if (tok_type == TOK_ID) { printf("%s;\n", tok_text); }
    else if (tok_type == TOK_CHAR) { printf("'%s';\n", tok_text); }
    else if (tok_type == TOK_STR) { printf("(intptr_t)\"%s\";\n", tok_text); } 
    else { printf("0; // Ошибка значения\n"); }
    next_token();
}

void parse_statements()
{
    while (tok_type != TOK_EOF && (tok_type != TOK_OP || tok_text[0] != '}'))
    {
        if (tok_type == TOK_WHILE) { parse_while(); }
        else if (tok_type == TOK_IF) { parse_if(); }
        else if (tok_type == TOK_ID && strcmp(tok_text, "__") == 0) { next_token(); parse_memory_store(); }
        else if (tok_type == TOK_ID)
        {
            // ХАК ДЛЯ ОБЪЯВЛЕНИЙ: заглядываем на один токен вперед через скрытый буфер лексера, либо смотрим на спец-знаки
            // Но чтобы не ломать лексер, мы можем временно адаптировать parse_assignment, либо разобрать структуру прямо здесь
            char var_name[64];
            strcpy(var_name, tok_text);
            next_token(); // Шагаем за имя
            // Сценарий А: Объявление массива вида name[64]
            if (tok_type == TOK_OP && tok_text[0] == '[')
            {
                next_token(); // Пропускаем '['
                int size = tok_value;
                next_token(); // Пропускаем число-размер
                next_token(); // Пропускаем ']'
                if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); } // Пропускаем необязательную ';'
                //print_indent();
                printf("intptr_t %s[%d];\n", var_name, size);
            }
            // Сценарий Б: Простое объявление переменной вида name; или name без всего
            else if (tok_type == TOK_OP && tok_text[0] == ';')
            {
                next_token(); // Пропускаем ';'
                //print_indent();
                printf("intptr_t %s;\n", var_name);
            }
            // Сценарий В: Это функция или обычное присваивание/вызов — откатываем имя обратно в лексер (точнее, подменяем токен)
            else
            {
                // Нам нужно передать управление в parse_assignment, но мы уже сделали next_token(). 
                // Чтобы не усложнять откат, мы передадим сохраненное имя var_name внутрь обновленной parse_assignment.
                // Но проще всего — вернуть лексер на шаг назад, уменьшив src_ptr на длину текущих токенов.
                // Однако, так как мы пишем спартанский компилятор, мы сделаем изящнее: 
                // если за именем идет '(', '=' или '--', мы просто передаем это в parse_assignment, доработав ее.
                // Давайте вернемся в parse_statements и сделаем это красивым inline-откатом.
                src_ptr = src_ptr - strlen(tok_text) - strlen(var_name); // Спартанский откат указателя лексера назад
                next_token(); // Перезаряжаем токен имени
                parse_assignment();
            }
        }
        else if (tok_type == TOK_OP && tok_text[0] == '[') { parse_memory_store(); }
        else if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); } 
        else { next_token(); }
    }
}

// Разбор "сахарного" условия: if x == 5 { ... } -> превращается в одноразовый while со скрытым флагом
void parse_if()
{
    next_token(); // Пропускаем "if"
    if (tok_type != TOK_ID)
    {
        printf("// Ошибка синтаксиса: Ожидалась переменная слева в условии\n");
        return;
    }
    char left_var[64]; // Восстановлен правильный размер массива
    strcpy(left_var, tok_text);
    next_token(); // Шагаем к оператору сравнения (== или !=)
    char op_str[4]; // Восстановлен правильный размер массива
    if (tok_type == TOK_EQ) { strcpy(op_str, "=="); }
    else if (tok_type == TOK_NEQ) { strcpy(op_str, "!="); }
    else
    {
        printf("// Ошибка синтаксиса: Ожидался оператор == или !=\n");
        return;
    }
    next_token(); // Шагаем к значение справа (число или переменная)
    char right_val[64]; // Восстановлен правильный размер массива
    if (tok_type == TOK_NUM) { sprintf(right_val, "%d", tok_value); }
    else if (tok_type == TOK_ID) { strcpy(right_val, tok_text); }
    else
    {
        printf("// Ошибка синтаксиса: Справа в условии должно быть число или переменная\n");
        return;
    }
    int current_if_id = if_counter;
    if_counter++; 
    print_indent();
    printf("intptr_t __flag_%d = (%s %s %s);\n", current_if_id, left_var, op_str, right_val); // Тип intptr_t для 64-битных систем
    print_indent(); 
    printf("while (__flag_%d)\n", current_if_id); 
    print_indent(); 
    printf("{\n");
    next_token(); // Шагаем к '{'
    if (tok_type != TOK_OP || tok_text[0] != '{') // Исправлено сравнение первого символа
    {
        printf("// Ошибка синтаксиса: Ожидалась скобка '{'\n");
        return;
    }
    next_token(); // Заходим внутрь тела if
    indent_level++; 
    parse_statements(); // Парсим внутренности 
    print_indent();
    printf("__flag_%d = 0;\n", current_if_id);
    indent_level--; 
    if (tok_type != TOK_OP || tok_text[0] != '}') // Исправлено сравнение первого символа
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
        int has_arg = 0;
        int arg_num = 0;
        char arg_id[64] = {0};
        char arg_str[64] = {0};
        char arg_char[64] = {0};
        if (tok_type == TOK_NUM) { has_arg = 1; arg_num = tok_value; next_token(); }
        else if (tok_type == TOK_ID) { has_arg = 2; strcpy(arg_id, tok_text); next_token(); }
        else if (tok_type == TOK_STR) { has_arg = 3; strcpy(arg_str, tok_text); next_token(); }
        else if (tok_type == TOK_CHAR) { has_arg = 4; strcpy(arg_char, tok_text); next_token(); } // НОВАЯ ФИЧА: Символьный аргумент
        if (tok_type != TOK_OP || tok_text[0] != ')')
        {
            print_indent();
            printf("// Ошибка синтаксиса\n");
            return;
        }
        next_token();
        if (tok_type == TOK_OP && tok_text[0] == '{')
        {
            indent_level = 0; 
            if (strcmp(var_name, "main") == 0) { printf("void __main__()\n"); }
            else { printf("\nvoid %s()\n", var_name); }
            printf("{\n");
            next_token(); 
            indent_level = 1; 
            parse_statements();
            if (tok_type != TOK_OP || tok_text[0] != '}')
            {
                print_indent();
                printf("// Ошибка\n");
                return;
            }
            printf("}\n\n"); 
            next_token(); 
            return;
        }
        print_indent();
        if (strcmp(var_name, "main") == 0) { printf("__main__("); }
        else { printf("%s(", var_name); }
        if (has_arg == 1) { printf("%d", arg_num); }
        if (has_arg == 2) { printf("(char*) %s", arg_id); } // Насильно приводим число-адрес к указателю на строку
        if (has_arg == 3) { printf("\"%s\"", arg_str); }
        if (has_arg == 4) { printf("'%s'", arg_char); } // Выводим одиночный символ в одинарных кавычках для Си
        printf(");\n");
        return;
    }
    // Сценарий 2: Декремент (x--) - для обратной совместимости
    if (tok_type == TOK_DEC)
    {
        print_indent();
        printf("%s--;\n", var_name);
        next_token();
        return;
    }
    // Сценарий 3: Присваивание переменной (x = 42, x = y, x = __[0], x = 'A')
    if (tok_type != TOK_OP || tok_text[0] != '=')
    {
        print_indent();
        printf("// Ошибка\n");
        return;
    }
    next_token(); // Шагаем за '='
    // Чтение из памяти вида: x = __[0] или x = __['$']
    if (tok_type == TOK_ID && strcmp(tok_text, "__") == 0)
    {
        next_token(); // Пропускаем "__"
        if (tok_type != TOK_OP || tok_text[0] != '[') { print_indent(); printf("// Ошибка: Ожидалась '['\n"); return; }
        next_token(); // Пропускаем '['
        print_indent();
        if (tok_type == TOK_NUM) { printf("%s = __[%d];\n", var_name, tok_value); }
        else if (tok_type == TOK_ID) { printf("%s = __[%s];\n", var_name, tok_text); }
        else if (tok_type == TOK_CHAR) { printf("%s = __['%s'];\n", var_name, tok_text); } // НОВАЯ ФИЧА: Символьный индекс при чтении
        next_token(); // Шагаем к ']'
        if (tok_type != TOK_OP || tok_text[0] != ']') { print_indent(); printf("// Ошибка: Ожидалась ']'\n"); return; }
        next_token(); // Пропускаем ']'
        return;
    }
    // Присваивание константного числа (x = 42)
    if (tok_type == TOK_NUM)
    {
        print_indent();
        printf("%s = %d;\n", var_name, tok_value);
        next_token();
        return;
    }
    // Присваивание символа (x = 'A')
    if (tok_type == TOK_CHAR)
    {
        print_indent();
        printf("%s = '%s';\n", var_name, tok_text);
        next_token();
        return;
    }
    // Продвинутое присваивание идентификатора (x = y или x = y + 1)
    if (tok_type == TOK_ID)
    {
        char first_id[64];
        strcpy(first_id, tok_text);
        next_token(); 
        if (tok_type == TOK_OP && (tok_text[0] == '+' || tok_text[0] == '-'))
        {
            char op = tok_text[0];
            next_token(); 
            print_indent();
            if (tok_type == TOK_NUM) { printf("%s = %s %c %d;\n", var_name, first_id, op, tok_value); }
            else if (tok_type == TOK_ID) { printf("%s = %s %c %s;\n", var_name, first_id, op, tok_text); }
            next_token();
            return;
        }
        print_indent();
        printf("%s = %s;\n", var_name, first_id);
        return;
    }
    print_indent();
    printf("// Ошибка синтаксиса: Ожидалось число, переменная, символ или массив\n");
}

// Разбор цикла вида: while x == 5 { ... } или while x { ... }
void parse_while()
{
    next_token(); // Пропускаем "while"
    if (tok_type != TOK_ID)
    {
        printf("// Ошибка синтаксиса: Ожидалась переменная в условии цикла\n");
        return;
    }
    char left_var[64];
    strcpy(left_var, tok_text);
    next_token(); // Шагаем дальше, чтобы проверить, есть ли сравнение
    char op_str[8];
    op_str[0] = '\0'; // Исправлена инициализация массива строк в Си
    char right_val[64] = "";
    int is_comparison = 0;
    // Если обнаружили оператор сравнения
    if (tok_type == TOK_EQ || tok_type == TOK_NEQ) // Исправлено: TOK_NEQ изменен на TOK_NE
    {
        is_comparison = 1;
        if (tok_type == TOK_EQ) { strcpy(op_str, "=="); }
        else { strcpy(op_str, "!="); }
        next_token(); // Шагаем к правому значению условия
        if (tok_type == TOK_NUM) { sprintf(right_val, "%d", tok_value); }
        else if (tok_type == TOK_ID) { strcpy(right_val, tok_text); }
        else
        {
            printf("// Ошибка синтаксиса: Справа в условии цикла должно быть число или переменная\n");
            return;
        }
        next_token(); // Шагаем к следующему токену (ожидаем '{')
    }
    // ГЕНЕРАЦИЯ Си-кода для цикла
    print_indent();
    if (is_comparison) { printf("while (%s %s %s)\n", left_var, op_str, right_val); }
    else { printf("while (%s)\n", left_var); }
    print_indent();
    printf("{\n");
    // Проверяем открывающую скобку цикла
    if (tok_type != TOK_OP || tok_text[0] != '{')
    {
        printf("// Ошибка синтаксиса: Ожидалась скобка '{'\n");
        return;
    }
    next_token(); // Заходим внутрь тела цикла
    indent_level++;
    parse_statements(); // Рекурсивно разбираем команды
    indent_level--;
    // Проверяем закрывающую скобку цикла
    if (tok_type != TOK_OP || tok_text[0] != '}')
    {
        printf("// Ошибка синтаксиса: Ожидалась закрывающая скобка '}'\n");
        return;
    }
    print_indent();
    printf("}\n");
    next_token(); // Пропускаем '}'
}

int main(int argc, char *argv[])
{
    //setlocale(0, "");
    // Нативное и безопасное переключение кодировки консоли Windows без system()
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Насильно включаем кодировку Windows-1251 для ввода и вывода консоли
    //system("chcp 1251 > nul");
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
    printf("#include <windows.h>\n");
    printf("#include <stdint.h>\n"); // Нужен для типа intptr_t
    putchar('\n');
    next_token(); // Заряжаем первый токен из файла
    // 8. Запускаем парсер. Он разберет функции на глобальном уровне
    parse_statements();
    // 9. Автоматически дописываем точку входа Си, которая вызовет нашу функцию main()
    printf("int main()\n");
    printf("{\n");
    printf("    // Нативное и безопасное переключение кодировки консоли Windows без system()\n");
    printf("    SetConsoleCP(1251);\n");
    printf("    SetConsoleOutputCP(1251);\n");
    printf("    __main__(); // Вызов главной функции\n");
    printf("    return 0;\n");
    printf("}");
    // Освобождаем память
    free(file_buffer);
    return 0;
}