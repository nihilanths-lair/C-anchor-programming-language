#include <stdio.h>
#include <locale.h>

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

    parse_rule("ЧИСЛО = [0-9]+");
    parse_pattern("[0-9]+");
    putchar('\n');
    parse_rule("ИДЕНТИФИКАТОР = [a-zA-Z_][a-zA-Z0-9_]*");
    parse_pattern("[a-zA-Z_][a-zA-Z0-9_]*");


    FILE *gen = fopen("gen.txt", "wb");
    int ch = 0;
    while (ch < 256)
    {
        fprintf(gen, "case '%c':\n", ch);
        ch++;
    }
    fclose(gen);
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
void parse_rule(const char *line)
{
    char token[0xFF];
    char separator = '\0';
    char pattern[0xFF];

    char *ptr_token = token;
    char *ptr_pattern = pattern;

_1: // Читаем имя токена
    switch (*line) {
    case '\0':
    case '\n':
    case '\r':
        goto _0;

    case ' ': // игнорируем пробельные символы
        line++;
        goto _1;

    case '=': // разделитель
    {
        *ptr_token = '\0';
        separator = *line;
        line++;
        if (*line == ' ') line++;
        goto _2;
    }
    default:
        *ptr_token = *line;
        ptr_token++;
        line++;
        goto _1;
    }
_2: // Читаем шаблон (правую часть)
    switch (*line) {
    case '\0':
    case '\n':
    case '\r':
        *ptr_pattern = '\0';
        goto _0;

    default:
        *ptr_pattern = *line;
        ptr_pattern++;
        line++;
        goto _2;
    }
_0:
    printf("\ntoken     = \"%s\"", token);
    printf("\nseparator = '%c'", separator);
    printf("\npattern   = \"%s\"\n", pattern);
}
void parse_pattern(const char *pattern)
{
    char class[0xFF] = {'\0'}; // 1) CLASS (квадратные скобки: [])
    char quantifier = '\0'; // 2) QUANTIFIER (квантификаторы: *+?)

    char *ptr_class = class;

_1: // Читаем шаблон
    switch (*pattern) {
    case '\0':
    case '\n':
    case '\r':
        goto _0;
    case '[': // Если класс
        *ptr_class = *pattern;
        ptr_class++;
        pattern++;
        goto _2;
    }
    goto _1;
_2: // Перебираем элементы класса
    switch (*pattern) {
    case '\0':
    case '\n':
    case '\r':
        goto _0;
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '_':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
        *ptr_class = *pattern;
        ptr_class++;
        pattern++;
        goto _2;
    case ']': // Если конец класса
        *ptr_class = *pattern;
        ptr_class++;
        *ptr_class = '\0';
        pattern++;
        goto _3;
    }
    //default:
    *ptr_class = *pattern;
    ptr_class++;
    pattern++;
    goto _2;
_3: // Смотрим, есть ли квантификатор
    switch (*pattern) {
    case '\0':
    case '\n':
    case '\r':
        goto _0;
    case '*':
    case '+':
    case '?':
        quantifier = *pattern;
        pattern++;
        //goto _1;
    }
    // IGNORE: Квантификатор не обнаружен
    goto _1;
_0:
    printf("\nclass = \"%s\"", class);
    printf("\nquantifier = '%c'", quantifier);
}