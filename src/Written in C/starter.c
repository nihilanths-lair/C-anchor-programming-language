#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define TOK_EOF   0
#define TOK_WHILE 1
#define TOK_ID    2
#define TOK_NUM   3
#define TOK_OP    4
#define TOK_DEC   5
#define TOK_IF    6
#define TOK_STR   7
#define TOK_CHAR  8
#define TOK_EQ    9
#define TOK_NEQ   10
#define TOK_INC   11

intptr_t tok_type;
intptr_t tok_value;
char tok_text_buffer[1024];
char *tok_text = tok_text_buffer;
char *file_buffer;
char *src_ptr;
int indent_level = 0;
int blank_line = 0;

void next_token();
void parse_statements();
void parse_assignment();
void parse_while();
void parse_if();
void parse_memory_store();

void print_indent()
{
    for (int i = 0; i < indent_level; i++) { printf("    "); }
}

void next_token()
{
    int nl_count = 0;
    while (*src_ptr != '\0' && isspace((unsigned char)*src_ptr)) 
    { 
        if (*src_ptr == '\n') { nl_count++; }
        src_ptr++; 
    }
    if (nl_count > 1) { blank_line = 1; }
    
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
            if (*src_ptr == '\\' && *(src_ptr + 1) == 'n')
            {
                if (len < 999) { *(tok_text + len) = '\\'; *(tok_text + len + 1) = 'n'; len += 2; }
                src_ptr += 2; continue;
            }
            if (*src_ptr == '\\' && *(src_ptr + 1) == '"')
            {
                if (len < 999) { *(tok_text + len) = '\\'; *(tok_text + len + 1) = '"'; len += 2; }
                src_ptr += 2; continue;
            }
            if (len < 1000) { *(tok_text + len) = *src_ptr; len++; }
            src_ptr++;
        }
        *(tok_text + len) = '\0';
        if (*src_ptr == '"') { src_ptr++; }
        tok_type = TOK_STR;
        return;
    }
    if (*src_ptr == '\'')
    {
        src_ptr++; int len = 0;
        while (*src_ptr != '\'' && *src_ptr != '\0')
        {
            if (*src_ptr == '\\' && *(src_ptr + 1) != '\0') { *(tok_text + len) = *src_ptr; len++; src_ptr++; }
            if (len < 1000) { *(tok_text + len) = *src_ptr; len++; }
            src_ptr++;
        }
        *(tok_text + len) = '\0';
        if (*src_ptr == '\'') { src_ptr++; }
        tok_type = TOK_CHAR;
        return;
    }
    if (isalpha((unsigned char)*src_ptr) || *src_ptr == '_')
    {
        int len = 0;
        while (isalnum((unsigned char)*src_ptr) || *src_ptr == '_')
        {
            if (len < 1000) { *(tok_text + len) = *src_ptr; len++; }
            src_ptr++;
        }
        *(tok_text + len) = '\0';
        if (strcmp(tok_text, "while") == 0) { tok_type = TOK_WHILE; }
        else if (strcmp(tok_text, "if") == 0) { tok_type = TOK_IF; }
        else { tok_type = TOK_ID; }
        return;
    }
    if (*src_ptr == '+' && *(src_ptr + 1) == '+')
    {
        *(tok_text + 0) = '+'; *(tok_text + 1) = '+'; *(tok_text + 2) = '\0';
        tok_type = TOK_INC; src_ptr += 2; return;
    }
    if (*src_ptr == '-' && *(src_ptr + 1) == '-')
    {
        *(tok_text + 0) = '-'; *(tok_text + 1) = '-'; *(tok_text + 2) = '\0';
        tok_type = TOK_DEC; src_ptr += 2; return;
    }
    if (*src_ptr == '=' && *(src_ptr + 1) == '=')
    {
        *(tok_text + 0) = '='; *(tok_text + 1) = '='; *(tok_text + 2) = '\0';
        tok_type = TOK_EQ; src_ptr += 2; return;
    }
    if (*src_ptr == '!' && *(src_ptr + 1) == '=')
    {
        *(tok_text + 0) = '!'; *(tok_text + 1) = '='; *(tok_text + 2) = '\0';
        tok_type = TOK_NEQ; src_ptr += 2; return;
    }
    *(tok_text + 0) = *src_ptr; *(tok_text + 1) = '\0'; tok_type = TOK_OP; src_ptr++;
}

void parse_statements()
{
    while (tok_type != TOK_EOF && (tok_type != TOK_OP || *(tok_text + 0) != '}'))
    {
        if (blank_line == 1)
        {
            printf("\n");
            blank_line = 0;
        }

        if (tok_type == TOK_WHILE) { parse_while(); }
        else if (tok_type == TOK_IF) { parse_if(); }
        else if (tok_type == TOK_ID && strcmp(tok_text, "native_c") == 0)
        {
            next_token();
            if (tok_type == TOK_STR)
            {
                print_indent();
                int i = 0;
                while (*(tok_text + i) != '\0')
                {
                    if (*(tok_text + i) == '\\' && *(tok_text + i + 1) == '"') { putchar('"'); i += 2; }
                    else { putchar(*(tok_text + i)); i++; }
                }
                printf("\n");
                next_token();
            }
            else { printf("// Ошибка: Оператор native_c ожидает строковый литерал\n"); next_token(); }
            if (tok_type == TOK_OP && *(tok_text + 0) == ';') { next_token(); }
        }
        else if (tok_type == TOK_ID) 
        { 
            parse_assignment(); 
            if (tok_type == TOK_OP && *(tok_text + 0) == ';') { next_token(); } 
        }
        else if (tok_type == TOK_OP && *(tok_text + 0) == ';') { next_token(); }
        else { next_token(); }
    }
}

void parse_assignment()
{
    char var_name_buffer[64];
    char *var_name = var_name_buffer;
    strcpy(var_name, tok_text);
    next_token();
    if (indent_level == 0 && tok_type == TOK_OP && *(tok_text + 0) == '[')
    {
        next_token(); int size = tok_value; next_token(); next_token();
        if (tok_type == TOK_OP && *(tok_text + 0) == ';') { next_token(); }
        printf("intptr_t %s[%d];\n", var_name, size); return;
    }
    if (indent_level == 0 && tok_type == TOK_OP && *(tok_text + 0) == ';')
    {
        next_token(); printf("intptr_t %s;\n", var_name); return;
    }
    if (tok_type == TOK_OP && *(tok_text + 0) == '(')
    {
        next_token(); int has_arg = 0; int arg_num = 0; 
        char arg_id_buf[64]; char arg_str_buf[1024]; char arg_char_buf[64];
        char *arg_id = arg_id_buf; char *arg_str = arg_str_buf; char *arg_char = arg_char_buf;
        if (tok_type == TOK_NUM) { has_arg = 1; arg_num = tok_value; next_token(); }
        else if (tok_type == TOK_ID) { has_arg = 2; strcpy(arg_id, tok_text); next_token(); }
        else if (tok_type == TOK_STR) { has_arg = 3; strcpy(arg_str, tok_text); next_token(); }
        else if (tok_type == TOK_CHAR) { has_arg = 4; strcpy(arg_char, tok_text); next_token(); }
        if (tok_type != TOK_OP || *(tok_text + 0) != ')') { print_indent(); printf("// Ошибка синтаксиса\n"); return; }
        next_token();
        if (tok_type == TOK_OP && *(tok_text + 0) == '{')
        {
            indent_level = 0;
            if (strcmp(var_name, "main") == 0) { printf("void cdlr__main()\n"); }
            else { printf("\nvoid %s()\n", var_name); }
            printf("{\n"); next_token(); indent_level = 1; parse_statements();
            if (tok_type != TOK_OP || *(tok_text + 0) != '}') { print_indent(); printf("// Ошибка\n"); return; }
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
    if (tok_type == TOK_INC)
    {
        print_indent();
        printf("%s++;\n", var_name);
        next_token();
        return;
    }
    if (tok_type == TOK_DEC) { print_indent(); printf("%s--;\n", var_name); next_token(); return; }
    if (tok_type != TOK_OP || *(tok_text + 0) != '=') { print_indent(); printf("// Ошибка: Ожидался знак '=' \n"); return; }
    next_token();
    if (indent_level == 0) { printf("intptr_t "); } else { print_indent(); }
    if (tok_type == TOK_NUM) 
    { 
        printf("%s = %d;\n", var_name, tok_value); next_token(); 
        if (tok_type == TOK_OP && *(tok_text + 0) == ';') { next_token(); }
        return; 
    }
    if (tok_type == TOK_CHAR) 
    { 
        printf("%s = '%s';\n", var_name, tok_text); next_token(); 
        if (tok_type == TOK_OP && *(tok_text + 0) == ';') { next_token(); }
        return; 
    }
    if (tok_type == TOK_ID)
    {
        char first_id_buffer[64];
        char *first_id = first_id_buffer;
        strcpy(first_id, tok_text); next_token();
        if (tok_type == TOK_OP && (*(tok_text + 0) == '+' || *(tok_text + 0) == '-'))
        {
            char op = *(tok_text + 0); next_token();
            if (tok_type == TOK_NUM) { printf("%s = %s %c %d;\n", var_name, first_id, op, tok_value); }
            else if (tok_type == TOK_ID) { printf("%s = %s %c %s;\n", var_name, first_id, op, tok_text); }
            next_token(); 
            if (tok_type == TOK_OP && *(tok_text + 0) == ';') { next_token(); }
            return;
        }
        printf("%s = %s;\n", var_name, first_id); 
        if (tok_type == TOK_OP && *(tok_text + 0) == ';') { next_token(); }
        return;
    }
    printf("// Ошибка синтаксиса\n");
}

void parse_while()
{
    print_indent();
    printf("while ");
    next_token();
    while (tok_type != TOK_EOF && (tok_type != TOK_OP || *(tok_text + 0) != '{'))
    {
        if (tok_type == TOK_NUM) { printf("%d", tok_value); }
        else if (tok_type == TOK_ID) { printf("%s", tok_text); }
        else if (tok_type == TOK_EQ || tok_type == TOK_NEQ) { printf(" %s ", tok_text); }
        else if (tok_type == TOK_OP && (*(tok_text + 0) == '<' || *(tok_text + 0) == '>')) { printf(" %s ", tok_text); }
        else { printf("%s", tok_text); }
        next_token();
    }
    printf("\n");
    print_indent();
    printf("{\n");
    next_token();
    indent_level++;
    parse_statements();
    indent_level--;
    print_indent();
    printf("}\n");
    next_token();
}

void parse_if()
{
    print_indent();
    printf("if ");
    next_token();
    while (tok_type != TOK_EOF && (tok_type != TOK_OP || *(tok_text + 0) != '{'))
    {
        if (tok_type == TOK_NUM) { printf("%d", tok_value); }
        else if (tok_type == TOK_ID) { printf("%s", tok_text); }
        else if (tok_type == TOK_EQ || tok_type == TOK_NEQ) { printf(" %s ", tok_text); }
        else if (tok_type == TOK_OP && (*(tok_text + 0) == '<' || *(tok_text + 0) == '>')) { printf(" %s ", tok_text); }
        else { printf("%s", tok_text); }
        next_token();
    }
    printf("\n");
    print_indent();
    printf("{\n");
    next_token();
    indent_level++;
    parse_statements();
    indent_level--;
    print_indent();
    printf("}\n");
    next_token();
}

void parse_memory_store() { next_token(); }

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
    
    indent_level = 0;
    next_token();
    parse_statements();

    printf("\nint main()\n{\n    cdlr__main();\n    return 0;\n}");
    free(file_buffer);
    return 0;
}