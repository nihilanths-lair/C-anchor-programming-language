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
    while (*src_ptr != '\0' && isspace((unsigned char)*src_ptr)) { src_ptr++; }
    if (*src_ptr == '\0')
    {
        tok_type = TOK_EOF;
        return;
    }
    if (isdigit((unsigned char)*src_ptr))
    {
        tok_value = 0;
        while (isdigit((unsigned char)*src_ptr))
        {
            tok_value = tok_value * 10 + (*src_ptr - '0');
            src_ptr++;
        }
        tok_type = TOK_NUM;
        return;
    }
    if (*src_ptr == '"')
    {
        src_ptr++; int len = 0;
        while (*src_ptr != '"' && *src_ptr != '\0')
        {
            if (len < 63) { tok_text[len++] = *src_ptr; }
            src_ptr++;
        }
        tok_text[len] = '\0';
        if (*src_ptr == '"') { src_ptr++; }
        tok_type = TOK_STR;
        return;
    }
    if (*src_ptr == '\'')
    {
        src_ptr++; int len = 0;
        while (*src_ptr != '\'' && *src_ptr != '\0')
        {
            if (*src_ptr == '\\' && *(src_ptr + 1) != '\0') { tok_text[len++] = *src_ptr++; }
            if (len < 63) { tok_text[len++] = *src_ptr; }
            src_ptr++;
        }
        tok_text[len] = '\0';
        if (*src_ptr == '\'') { src_ptr++; }
        tok_type = TOK_CHAR;
        return;
    }
    if (isalpha((unsigned char)*src_ptr) || *src_ptr == '_')
    {
        int len = 0;
        while (isalnum((unsigned char)*src_ptr) || *src_ptr == '_')
        {
            if (len < 63) { tok_text[len++] = *src_ptr; }
            src_ptr++;
        }
        tok_text[len] = '\0';
        if (strcmp(tok_text, "while") == 0) { tok_type = TOK_WHILE; }
        else if (strcmp(tok_text, "if") == 0) { tok_type = TOK_IF; }
        else { tok_type = TOK_ID; }
        return;
    }
    if (*src_ptr == '-' && *(src_ptr + 1) == '-')
    {
        tok_text[0] = '-'; tok_text[1] = '-'; tok_text[2] = '\0';
        tok_type = TOK_DEC; src_ptr += 2; return;
    }
    if (*src_ptr == '=' && *(src_ptr + 1) == '=')
    {
        tok_text[0] = '='; tok_text[1] = '='; tok_text[2] = '\0';
        tok_type = TOK_EQ; src_ptr += 2; return;
    }
    if (*src_ptr == '!' && *(src_ptr + 1) == '=')
    {
        tok_text[0] = '!'; tok_text[1] = '='; tok_text[2] = '\0';
        tok_type = TOK_NEQ; src_ptr += 2; return;
    }
    tok_text[0] = *src_ptr; tok_text[1] = '\0'; tok_type = TOK_OP; src_ptr++;
}

// Прототипы функций, чтобы Си не ругался на порядок их объявления
void parse_memory_store();
void parse_assignment();
void parse_statements();
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

void parse_assignment()
{
    char var_name[64];
    strcpy(var_name, tok_text);
    next_token();
    if (indent_level == 0 && tok_type == TOK_OP && tok_text[0] == '[')
    {
        next_token(); int size = tok_value; next_token(); next_token();
        if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); }
        printf("intptr_t %s[%d];\n", var_name, size); return;
    }
    if (indent_level == 0 && tok_type == TOK_OP && tok_text[0] == ';')
    {
        next_token(); printf("intptr_t %s;\n", var_name); return;
    }
    if (tok_type == TOK_OP && tok_text[0] == '(')
    {
        next_token(); int has_arg = 0; int arg_num = 0; char arg_id[64] = {0}; char arg_str[64] = {0}; char arg_char[64] = {0};
        if (tok_type == TOK_NUM) { has_arg = 1; arg_num = tok_value; next_token(); }
        else if (tok_type == TOK_ID) { has_arg = 2; strcpy(arg_id, tok_text); next_token(); }
        else if (tok_type == TOK_STR) { has_arg = 3; strcpy(arg_str, tok_text); next_token(); }
        else if (tok_type == TOK_CHAR) { has_arg = 4; strcpy(arg_char, tok_text); next_token(); }
        if (tok_type != TOK_OP || tok_text[0] != ')') { print_indent(); printf("// Ошибка синтаксиса\n"); return; }
        next_token();
        if (tok_type == TOK_OP && tok_text[0] == '{')
        {
            indent_level = 0;
            if (strcmp(var_name, "main") == 0) { printf("void cdlr__main()\n"); }
            else { printf("\nvoid %s()\n", var_name); }
            printf("{\n"); next_token(); indent_level = 1; parse_statements();
            if (tok_type != TOK_OP || tok_text[0] != '}') { print_indent(); printf("// Ошибка\n"); return; }
            printf("}\n"); next_token(); indent_level = 0; return;
        }
        print_indent();
        if (strcmp(var_name, "main") == 0) { printf("cdlr__main("); } else { printf("%s(", var_name); }
        if (has_arg == 1) { printf("%d", arg_num); }
        if (has_arg == 2) { printf("(char*) %s", arg_id); }
        if (has_arg == 3) { printf("\"%s\"", arg_str); }
        if (has_arg == 4) { printf("'%s'", arg_char); }
        printf(");\n"); return;
    }
    if (tok_type == TOK_DEC) { print_indent(); printf("%s--;\n", var_name); next_token(); return; }
    if (tok_type != TOK_OP || tok_text[0] != '=') { print_indent(); printf("// Ошибка: Ожидался знак '=' \n"); return; }
    next_token();
    if (indent_level == 0) { printf("intptr_t "); } else { print_indent(); }
    if (tok_type == TOK_NUM) 
    { 
        printf("%s = %d;\n", var_name, tok_value); next_token(); 
        if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); }
        return; 
    }
    if (tok_type == TOK_CHAR) 
    { 
        printf("%s = '%s';\n", var_name, tok_text); next_token(); 
        if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); }
        return; 
    }
    if (tok_type == TOK_ID)
    {
        char first_id[64]; strcpy(first_id, tok_text); next_token();
        if (tok_type == TOK_OP && (tok_text[0] == '+' || tok_text[0] == '-'))
        {
            char op = tok_text[0]; next_token();
            if (tok_type == TOK_NUM) { printf("%s = %s %c %d;\n", var_name, first_id, op, tok_value); }
            else if (tok_type == TOK_ID) { printf("%s = %s %c %s;\n", var_name, first_id, op, tok_text); }
            next_token(); 
            if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); }
            return;
        }
        printf("%s = %s;\n", var_name, first_id); 
        if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); }
        return;
    }
    printf("// Ошибка синтаксиса\n");
}

void parse_statements()
{
    while (tok_type != TOK_EOF && (tok_type != TOK_OP || tok_text[0] != '}'))
    {
        if (tok_type == TOK_WHILE) { parse_while(); }
        else if (tok_type == TOK_IF) { parse_if(); }
        else if (tok_type == TOK_ID) 
        { 
            parse_assignment(); 
            // МАГИЯ НЕОБЯЗАТЕЛЬНОЙ ';': если после выполнения команды лексер видит ';', мы её молча съедаем!
            if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); } 
        }
        else if (tok_type == TOK_OP && tok_text[0] == '[') { parse_memory_store(); }
        else if (tok_type == TOK_OP && tok_text[0] == ';') { next_token(); } // Пропускаем одиночные точки с запятой
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
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    if (argc < 2) { printf("\n Использование: %s <имя_файла.cdlr>\n", argv[0]); return 1; }
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) { printf("Ошибка: Не удалось открыть файл %s\n", argv[1]); return 1; }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    file_buffer = (char *) malloc(file_size + 1);
    if (file_buffer == NULL) { printf("Ошибка: Не удалось выделить память\n"); fclose(file); return 1; }
    fread(file_buffer, 1, file_size, file);
    file_buffer[file_size] = '\0';
    fclose(file);
    src_ptr = file_buffer;
    
    // Чистая шапка Си-файла
    printf("#include <stdio.h>\n#include <windows.h>\n#include <stdint.h>\n\n");
    
    next_token(); // Заряжаем первый токен
    parse_statements(); // Передаем ВСЁ управление единому парсеру

    printf("\nint main()\n{\n    SetConsoleCP(1251);\n    SetConsoleOutputCP(1251);\n    cdlr__main();\n    return 0;\n}");
    free(file_buffer);
    return 0;
}