#include <stdio.h>
#include <locale.h>

//----------------------------------------------------------//
//                         STORAGE                          //
//----------------------------------------------------------//
char code[0xFFF];
// Нотация регулярной грамматики (тип 4) для лексера
char regular_grammar_notation[0xFFF];
typedef struct {
    const char *input; // входной поток
    const char *cursor;
    int line; // позиция в строке
    int column; // позиция в столбце (колонке)
} Lexer;
typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENT,
    TOKEN_EOF
} TokenType;
typedef struct {
    TokenType type;
    const char *begin;
    const char *end;
    int line;
    int column;
} Token;
FILE *file = NULL;
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
//----------------------------------------------------------//
//----------------------------------------------------------//
//                        PROGRAM                           //
//----------------------------------------------------------//
int main()
{
    setlocale(0, "");

    file = fopen("$.bnf", "rb");
    if (file == NULL) { printf("\nAn exception was thrown: 1."); return 1; }
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
    if (file == NULL) { printf("\nAn exception was thrown: 2."); return 2; }
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
    for (;;)
    {
        Token token = lexer_nextToken(&lexer);
        //print_token(token);
        printf("token.type = %d", token.type);
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
    lexer->line = 0;
    lexer->column = 0;
}
// Заглушка
Token lexer_nextToken(Lexer *lexer)
{
    Token token;
    token.type = TOKEN_EOF;
    token.begin = token.end = lexer->cursor;
    token.line = lexer->line;
    token.column = lexer->column;
    return token;
}
//----------------------------------------------------------//