#include <stdio.h>
#include <locale.h>
#include <ctype.h>

typedef enum {
    TOKEN__IDENTIFIER,
    TOKEN__NUMBER,
    TOKEN__SYMBOL,
    TOKEN__EOF,
    TOKEN__ERROR
} TypeToken;

typedef struct {
    TypeToken type_token;
    const char * grammar;
    int cursor;
    int row_pos;
    int col_pos;
} Lexer;

typedef struct {
    TypeToken type_token;
    const char * grammar;
    int length;
    int row_pos;
    int col_pos;
} Token;

static inline char peek(Lexer * lexer) { return lexer->grammar[lexer->cursor]; }
static inline void advance(Lexer * lexer)
{
    char c = lexer->grammar[lexer->cursor++];
    if (c == '\n')
    {
        lexer->row_pos++;
        lexer->col_pos = 0;
    }
    else lexer->col_pos++;
    //return c;
}
static inline void skip_whitespace(Lexer * lexer)
{
    for (;;)
    {
        char c = peek(lexer);
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        {
            advance(lexer);
            continue;
        }
        break;
    }
}
static inline Token read_identifier(Lexer * lexer)
{
    Token token;
    token.type_token = TOKEN__IDENTIFIER;
    token.grammar = lexer->grammar + lexer->cursor;
    token.row_pos = lexer->row_pos;
    token.col_pos = lexer->col_pos;
    advance(lexer); // первый символ уже гарантированно буква или '_'
    for (;;)
    {
        char c = peek(lexer);
        if (isalpha(c) || isdigit(c) || c == '_') advance(lexer);
        else break;
    }
    token.length = (lexer->grammar + lexer->cursor) - token.grammar;
    return token;
}
static inline Token read_number(Lexer * lexer)
{
    Token token;
    token.type_token = TOKEN__NUMBER;
    token.grammar = lexer->grammar + lexer->cursor;
    token.row_pos = lexer->row_pos;
    token.col_pos = lexer->col_pos;
    while (isdigit(peek(lexer))) advance(lexer);
    token.length = (lexer->grammar + lexer->cursor) - token.grammar;
    return token;
}
static inline Token read_symbol(Lexer * lexer)
{
    Token token;
    token.type_token = TOKEN__SYMBOL;
    token.grammar    = lexer->grammar + lexer->cursor;
    token.row_pos    = lexer->row_pos;
    token.col_pos    = lexer->col_pos;
    token.length     = 1;
    advance(lexer);
    return token;
}

FILE * file = NULL;
char grammar[1<<24] = ""; // временно, потом заменю на malloc при необходимости

int main()
{
    setlocale(0, "");

    file = fopen("syntax_test_unit.txt", "rb");
    if (file == NULL) { printf("\nВозникла ошибка №%d.\n", 1); return 1; } // Не удалось открыть файл, одна из возможных причин: отсутствие файла.

    size_t count = fread(grammar, 1, sizeof (grammar)-1, file);
    grammar[count] = '\0';
    printf("grammar =\n<#\n%s\n#>", grammar);

    Lexer lexer = {
        .grammar = grammar,
        .cursor = 0,
        .row_pos = 1,
        .col_pos = 0
    };
    Token token;
    for (;;)
    {
        skip_whitespace(&lexer);
        char c = peek(&lexer);
        if (c == '\0')
        {
            token.type_token = TOKEN__EOF;
            break;
        }
        if (isalpha(c) || c == '_')
        {
            token = read_identifier(&lexer);//return read_identifier();
            printf("\nТокен: %.*s , тип токена: Идентификатор.", token.length, token.grammar);
            continue;
        }
        if (isdigit(c))
        {
            token = read_number(&lexer);//return read_number();
            printf("\nТокен: %.*s , тип токена: Число.", token.length, token.grammar);
            continue;
        }
        token = read_symbol(&lexer);//return read_symbol();
        printf("\nТокен: %.*s , тип токена: Символ.", token.length, token.grammar);
    }
    putchar('\n');
    return 0;
}