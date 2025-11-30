#include <stdio.h>
#include <locale.h>
#include <string.h>

//----------------------------------------------------------//
//                         STORAGE                          //
//----------------------------------------------------------//
char code[0xFFF];
char regular_grammar_notation[0xFFF];
// Кол-во токенов
int token_count = 0;
//const char token_type[0xF][0xFF] =
static const char *token_names[] =
{
    "NUMBER",
    "IDENT",
    "EOF"
};
/**/
// Lexer //
typedef struct {
    const char *input;
    const char *cursor;
    int row;
    int column;
    //int token_count;
} Lexer;
/**/
// TokenType //
typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENT,
    TOKEN_EOF
} TokenType;
/**/
// Token //
typedef struct {
    int number;
    TokenType type;
    const char *begin;
    const char *end;
    int row;
    int column;
    int offset;
} Token;
//FILE *__file = NULL;
size_t symbols;
//----------------------------------------------------------//
//
//
//
//----------------------------------------------------------//
//                       DECLARATION                        //
//----------------------------------------------------------//
void trim(char *s, const char *chars_to_remove);

void lexer_init(Lexer *lexer, const char *code);
Token lexer_nextToken(Lexer *lexer);
void lexer_printToken(Token *token);

void parse_rule(const char *line);
void parse_pattern(const char *pattern);
//----------------------------------------------------------//
//----------------------------------------------------------//
//                        PROGRAM                           //
//----------------------------------------------------------//
int main()
{
    setlocale(0, "");

    FILE *file;
    file = fopen("$.form", "rb");
    if (file == NULL) { printf("\n[file: $.c | function: main]: An exception was thrown: 1."); return 1; }
    /** Временно убрано, пока без malloc, задано фиксировано
    fseek(file, 0, SEEK_END);
    long flength = ftell(file);
    fseek(file, 0, SEEK_SET); IGNORE: Имеет ли смысл это делать сразу, или лучше потом, когда понадобится/потребуется?
    */
    printf("\nДо: regular_grammar_notation = \"%s\".", regular_grammar_notation); // До загрузки грамматики
    symbols = fread(regular_grammar_notation, 1, sizeof (regular_grammar_notation) - 1, file);
    fclose(file);
    regular_grammar_notation[symbols] = '\0';
    printf("\nsymbols = %lld.", symbols);
    printf("\nПосле: regular_grammar_notation = \"\n%s\n\".", regular_grammar_notation); // После загрузки грамматики

    file = fopen("$.txt", "rb");
    if (file == NULL) { printf("\n[file: $.c | function: main]: An exception was thrown: 2."); return 2; }
    /** Временно убрано, пока без malloc, задано фиксировано
    fseek(file, 0, SEEK_END);
    long flength = ftell(file);
    fseek(file, 0, SEEK_SET); // IGNORE: Имеет ли смысл это делать сразу, или лучше потом, когда понадобится/потребуется?
    */
    printf("\nДо: code = \"%s\".", code); // До загрузки кода
    symbols = fread(code, 1, sizeof (code) - 1, file);
    fclose(file);
    code[symbols] = '\0';
    printf("\nsymbols = %lld.", symbols);
    printf("\nПосле: code = \"\n%s\n\".", code); // После загрузки кода

    putchar('\n');
    Lexer lexer;
    lexer_init(&lexer, code);
    //int token_count = 0;
    Token token;
    for (;;)
    {
        token = lexer_nextToken(&lexer);
        //token.number = token_count++;
        lexer_printToken(&token);
        if (token.type == TOKEN_EOF) break;
    }
    printf("\n\n");

    //parse_rule("= a | b"); putchar('\n');
    //parse_rule("= a|b)c"); putchar('\n');
    //parse_rule("= [0-9]+ | \"xyz\"");
    parse_rule("ТОКЕН-1 = '0'|'1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9'|' '");
    putchar('\n');
    parse_rule("ТОКЕН-2 = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | ' '");

    /*
    FILE *gen = fopen("gen.txt", "wb");
    int ch = 0;
    while (ch < 256)
    {
        fprintf(gen, "case '%c':\n", ch);
        ch++;
    }
    fclose(gen);
    */
    putchar('\n');
    return 0;
}
//----------------------------------------------------------//
//
//
//
//----------------------------------------------------------//
//                      SUBPROGRAM                          //
//----------------------------------------------------------//
// Удаляет символы из начала и конца строки
void trim(char *s, const char *chars_to_remove)
{
    char *start = s;
    char *end;

    // --- Сдвигаем start до первого символа, который не нужно удалять
    while (*start && strchr(chars_to_remove, *start)) start++;

    // --- Если строка не пустая, сдвигаем оставшуюся часть в начало
    if (start != s) memmove(s, start, strlen(start) + 1);

    // --- Находим конец строки
    end = s + strlen(s) - 1;

    // --- Отрезаем символы справа
    while (end >= s && strchr(chars_to_remove, *end))
    {
        *end = '\0';
        end--;
    }
}
void lexer_init(Lexer *lexer, const char *code)
{
    lexer->input = code;
    lexer->cursor = code;
    //lexer->token_count = 0;
    lexer->row = 1;
    lexer->column = 1;
}
Token lexer_nextToken(Lexer *lexer)
{
    // Пропуск пробельных символов, а также спец. символов (по необходимости)
    // Если пробельный символ или какие-то спец. символы имеют значение - как токены, тогда исключаем лишнее
    //char c;
    while (1)
    {
        char c = *lexer->cursor;
        /*
        // При достижении конца файла
        if (*lexer->cursor == '\0') break;
        */
        if (c == ' ')
        {
            lexer->cursor++;
            lexer->column++;
        }
        else if (c == '\t')
        {
            lexer->cursor++;
            lexer->column += 4; // или 1 (по усмотрению)
        }
        else if (c == '\n')
        {
            lexer->cursor++;
            lexer->row++;
            lexer->column = 1;
        }
        else if (c == '\r') lexer->cursor++;
        else break;
    }
    char c = *lexer->cursor;
    // При достижении конца файла
    if (*lexer->cursor == '\0')
    {
        Token token;
        token.type = TOKEN_EOF;
        token.begin = token.end = lexer->cursor;
        token.row = lexer->row;
        token.column = lexer->column;
        token.offset = 0;
        return token;
    }
    // В будущем: тут будет распознавание токенов
    Token token;
    token.type = TOKEN_EOF;
    token.begin = token.end = lexer->cursor;
    token.row = lexer->row;
    token.column = lexer->column;
    token.offset = 0;
    token.number = token_count++;
    return token;
}
// Отладочная информация / Debugging information
void lexer_printToken(Token *token)
{
    /*
    printf("\nТип токена: \"%s\"", token_type[token->type]);
    printf("\nПозиция в ряду: %d", token->row);
    printf("\nПозиция в колонке: %d", token->column);
    */
    printf("\nНомер токена: %d, имя токена: \"%s\", строка: %d, позиция внутри строки: %d, смещение: %d.", token->number, token_names[token->type], token->row, token->column, token->offset);
    FILE *file = fopen("debug_info.txt", "ab");
    if (file == NULL) { printf("\n[file: $.c | function: lexer_printToken]: An exception was thrown: 1."); }
    fprintf(file, "Номер токена: %d, имя токена: \"%s\", строка: %d, позиция внутри строки: %d, смещение: %d.", token->number, token_names[token->type], token->row, token->column, token->offset);
    fclose(file);
}
//----------------------------------------------------------//
#define then {
#define switch_end }

void parse_rule(const char *rule)
{
    char token[0xFF] = "";
    char separator = '\0';
    char pattern[0xFF] = "";

    char *ptr_token = token;
    char *ptr_pattern = pattern;

_1: // Читаем имя токена
    switch (*rule) then
    case '\0': case '\n': case '\r': goto _0;
    case ' ': // игнорируем пробельные символы
    {
        rule++;
        goto _1;
    }
    case '=': // разделитель
    {
        *ptr_token = '\0';
        printf("\ntoken       = %s", token);
        separator = *rule;
        printf("\nseparator   = %c", separator);
        rule++;
        if (*rule == ' ') rule++;
        goto _2;
    }
    switch_end
    *ptr_token = *rule;
    ptr_token++;
    rule++;
    goto _1;

_2: // Читаем шаблон (правую часть)
    switch (*rule) then
    case '\0': case '\n': case '\r':
    {
        *ptr_pattern = '\0';
        goto _0;
    }
    switch_end
    *ptr_pattern = *rule;
    ptr_pattern++;
    rule++;
    goto _2;

_0:
    printf("\npattern     = %s", pattern);
    parse_pattern(pattern);
}

void parse_pattern(const char *pattern)
{   
    char chr[0xF] = "";     // 4) CHAR (символ: '')
    char string[0xFF] = ""; // 3) STRING (кавычки: "")
    char class[0xFF] = "";  // 1) CLASS (квадратные скобки: [])
    char quantifier = '\0'; // 2) QUANTIFIER (квантификаторы: *+?)

    //char alternation[0xFF]; // 5) ALTERNATION (чередование: |) //[0xFF];

    char *ptr_chr = chr;
    char *ptr_string = string;
    char *ptr_class = class;

    // --- добавляем буфер для последовательности/альтернатив
    char element[0xFF] = "";     // текущий элемент
    char alternation[0xFF] = ""; // накопление альтернатив
    char *ptr_element = element;

_1: // Читаем шаблон
    // --- пропускаем пробелы и табы вне кавычек и классов
    if (*pattern == ' ' || *pattern == '\t')
    {
        pattern++;
        goto _1;
    }
    switch (*pattern) then
    case '\0':case '\n':case '\r': goto _0;
    case '"': // Разбор строки
    {
        *ptr_string = *pattern;
        ptr_string++;
        pattern++;
        goto _string;
    }
    case '\'': // Разбор символа
    {
        *ptr_chr = *pattern;
        ptr_chr++;
        pattern++;
        goto _chr;
    }
    case '[': // Обнаружение класса
    {
        *ptr_class = *pattern;
        ptr_class++;
        pattern++;
        goto _class;
    }
    case ']': // Не был обнаружен класс (его начало)
    {
        printf("\nСинтаксическая ошибка: Класс должен начинаться с открывающейся квадратной скобки.");
        printf("\nSyntax error: Class must begin with an opening square bracket.");
        goto _0;
    }
    case '*': case '+': case '?':
    {
        printf("\nСинтаксическая ошибка: Квантификатор не может стоять перед классом.");
        printf("\nSyntax error: A quantifier cannot appear before a class.");
        goto _0;
    }
    case '|': // альтернатива
    {
        if (*element != '\0')
        {
            trim(element, " \t\n\r");  // удаляем пробелы
            if (*alternation != '\0') strcat(alternation, " | ");
            strcat(alternation, element);
            *element = '\0';
            ptr_element = element;
        }
        pattern++;
        goto _1;
    }
    switch_end
    // добавляем текущий символ в элемент, если это одиночный символ
    *ptr_element = *pattern;
    ptr_element++;
    *ptr_element = '\0';
    pattern++;
    goto _1;

_chr: // Начало символа
    switch (*pattern) then
    case '\0': case '\n': case '\r': goto _0;
    case '\'': // Конец символа
    {
        *ptr_chr = *pattern;
        ptr_chr++;
        *ptr_chr = '\0';

        // --- добавляем в элемент
        strcat(element, chr);
        *chr = '\0';
        ptr_chr = chr;

        printf("\nchar        = %s", element);
        pattern++;
        goto _1;
    }
    switch_end
    *ptr_chr = *pattern;
    ptr_chr++;
    pattern++;
    goto _chr;

_string: // Начало строки
    switch (*pattern) then
    case '\0': case '\n': case '\r': goto _0;
    case '"': // Конец строки
    {
        *ptr_string = *pattern;
        ptr_string++;
        *ptr_string = '\0';

        // --- добавляем в элемент
        strcat(element, string);
        *string = '\0';
        ptr_string = string;

        printf("\nstring     = %s", element);
        pattern++;
        goto _1;
    }
    switch_end
    *ptr_string = *pattern;
    ptr_string++;
    pattern++;
    goto _string;

_class: // Перебираем элементы класса
    switch (*pattern) then
    case '\0': case '\n': case '\r': goto _0;
    case ']': // Если конец класса
    {
        *ptr_class = *pattern;
        ptr_class++;
        *ptr_class = '\0';

        // --- добавляем в элемент
        strcat(element, class);
        *class = '\0';
        ptr_class = class;

        printf("\nclass      = %s", element);
        pattern++;
        goto _quantifier;
    }
    switch_end
    *ptr_class = *pattern;
    ptr_class++;
    pattern++;
    goto _class;

_quantifier:
    switch (*pattern) then
    case '\0': case '\n': case '\r': goto _0;
    case '*': case '+': case '?':
    {
        quantifier = *pattern;
        printf("\nquantifier = %c", quantifier);
        pattern++;
    }
    switch_end
    goto _1;

_0:
    if (*element != '\0')
    {
        trim(element, " \t\n\r");  // удаляем пробелы
        if (*alternation != '\0') strcat(alternation, " | ");
        strcat(alternation, element);
    }
    if (*alternation != '\0') printf("\nalternation = %s", alternation);
}