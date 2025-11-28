#include <stdio.h>
#include <locale.h>

//----------------------------------------------------------//
//                         STORAGE                          //
//----------------------------------------------------------//
char code[0xFFF];
char regular_grammar_notation[0xFFF];
// Класс токенов
const char token_type[0xF][0xFF] =
{
    "NUMBER",
    "IDENT",
    "EOF"
};
// Lexer //
typedef struct {
    const char *input;
    const char *cursor;
    int row;
    int column;
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
//----------------------------------------------------------//
//----------------------------------------------------------//
//                        PROGRAM                           //
//----------------------------------------------------------//
int main()
{
    setlocale(0, "");

    FILE *file;
    file = fopen("$.bnf", "rb");
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
    Token token;
    for (;;)
    {
        token = lexer_nextToken(&lexer);
        lexer_printToken(&token);
        if (token.type == TOKEN_EOF) break;
    }
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
    lexer->row = 0;
    lexer->column = 0;
}
// Заглушка
Token lexer_nextToken(Lexer *lexer)
{
    Token token;
    token.number = 0;
    token.type = TOKEN_EOF;
    token.begin = token.end = lexer->cursor;
    token.row = lexer->row;
    token.column = lexer->column;
    token.offset = 0;
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
    printf("\nНомер токена: %d, тип токена: \"%s\", строка: %d, позиция: %d, смещение: %d.", token->number, token_type[token->type], token->row, token->column, token->offset);
    FILE *file = fopen("debug_info.txt", "wb");
    if (file == NULL) { printf("\n[file: $.c | function: lexer_printToken]: An exception was thrown: 1."); }
    fprintf(file, "Номер токена: %d, тип токена: \"%s\", строка: %d, позиция: %d, смещение: %d.", token->number, token_type[token->type], token->row, token->column, token->offset);
    fclose(file);
}
//----------------------------------------------------------//