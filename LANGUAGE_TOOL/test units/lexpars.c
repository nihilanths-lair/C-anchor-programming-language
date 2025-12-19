#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <stdlib.h> // exit;

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
    int offset_pos;
} Lexer;
Lexer lexer;

typedef struct {
    TypeToken type_token;
    const char * grammar;
    int length;
    int row_pos;
    int col_pos;
    int offset_pos;
} Token;
// Текущий токен
Token current_token;

/*
typedef struct Node {
    int dummy;
} Node;
Node * NODE_DUMMY;
*/

static inline char peek(Lexer * lexer) { return lexer->grammar[lexer->cursor]; }
static inline void advance(Lexer * lexer)
{
    char c = lexer->grammar[lexer->cursor++];
    if (c == '\n')
    {
        lexer->row_pos++;
        lexer->col_pos = 1;
        lexer->offset_pos++;
    }
    else
    {
        lexer->col_pos++;
        lexer->offset_pos++;
    }
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
    token.grammar    = lexer->grammar + lexer->cursor;
    token.row_pos    = lexer->row_pos;
    token.col_pos    = lexer->col_pos;
    token.offset_pos = lexer->offset_pos;
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
    token.grammar    = lexer->grammar + lexer->cursor;
    token.row_pos    = lexer->row_pos;
    token.col_pos    = lexer->col_pos;
    token.offset_pos = lexer->offset_pos;
    while (isdigit(peek(lexer))) advance(lexer);
    token.length     = (lexer->grammar + lexer->cursor) - token.grammar;
    return token;
}
static inline Token read_symbol(Lexer * lexer)
{
    Token token;
    token.type_token = TOKEN__SYMBOL;
    token.grammar    = lexer->grammar + lexer->cursor;
    token.row_pos    = lexer->row_pos;
    token.col_pos    = lexer->col_pos;
    token.offset_pos = lexer->offset_pos;
    token.length     = 1;
    advance(lexer);
    return token;
}
//static inline Token lexer_next(Lexer * lexer){}

static inline Token next_token(Lexer * lexer)
{
    Token token;
    skip_whitespace(lexer);
    char c = peek(lexer);
    if (c == '\0')
    {
        token.type_token = TOKEN__EOF;
        token.grammar = lexer->grammar + lexer->cursor;
        token.length = 0;
        token.row_pos = lexer->row_pos;
        token.col_pos = lexer->col_pos;
        token.offset_pos = lexer->offset_pos;
        return token;
    }
    if (isalpha(c) || c == '_') return read_identifier(lexer); //printf("\nТокен: %.*s , тип токена: Идентификатор.", token.length, token.grammar);
    if (isdigit(c)) return read_number(lexer); //printf("\nТокен: %.*s , тип токена: Число.", token.length, token.grammar);
    return read_symbol(lexer); //printf("\nТокен: %.*s , тип токена: Символ.", token.length, token.grammar);
}

static inline void expect(TypeToken type_token)
{
    if (current_token.type_token != type_token)
    {
        printf("\nParse error at %d:%d,%d", current_token.row_pos, current_token.col_pos, current_token.offset_pos);
        exit(1);
    }
    current_token = next_token(&lexer);
}
static inline void parse_expr();
static inline void parse_atom();

static inline void parse_expr()
{
    parse_atom();
    while (current_token.type_token == TOKEN__SYMBOL && current_token.grammar[0] == '/')
    {
        current_token = next_token(&lexer); // '/'
        parse_atom();
    }
}
static inline void parse_atom()
{
    if (current_token.type_token == TOKEN__IDENTIFIER)
    {
        current_token = next_token(&lexer);
        return;
    }
    if (current_token.type_token == TOKEN__SYMBOL && current_token.grammar[0] == '(')
    {
        current_token = next_token(&lexer); // '('
        parse_expr();
        if (!(current_token.type_token == TOKEN__SYMBOL && current_token.grammar[0] == ')'))
        {
            printf("Expected ')'\n");
            exit(1);
        }
        current_token = next_token(&lexer); // ')'
        return;
    }
    printf("\nExpected atom at %d:%d,%d.", current_token.row_pos, current_token.col_pos, current_token.offset_pos);
    exit(1);
}
/*
static inline void parse_term(){}
static inline void parse_factor(){}
*/
static inline void parse_rule()
{
    expect(TOKEN__IDENTIFIER); // имя правила
    if (!(current_token.type_token == TOKEN__SYMBOL && current_token.grammar[0] == '~'))
    {
        printf("\nExpected '~' at %d:%d,%d.", current_token.row_pos, current_token.col_pos, current_token.offset_pos);
        printf("\n          ^");
        exit(1);
    }
    current_token = next_token(&lexer);
    parse_expr();
    if (!(current_token.type_token == TOKEN__SYMBOL && current_token.grammar[0] == ';'))
    {
        printf("\nExpected ';' at %d:%d,%d.", current_token.row_pos, current_token.col_pos, current_token.offset_pos);
        printf("\n          ^");
        exit(1);
    }
    current_token = next_token(&lexer);
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

    lexer.grammar    = grammar;
    lexer.cursor     = 0;
    lexer.row_pos    = 1;
    lexer.col_pos    = 1;
    lexer.offset_pos = 1;

    //Token token = next_token(&lexer);
    current_token = next_token(&lexer);
    while (current_token.type_token != TOKEN__EOF) parse_rule();

    putchar('\n');
    return 0;
}