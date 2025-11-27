#include <stdio.h>
#include <locale.h>
/************************************************************/
/*                       DECLARATION                        */
/************************************************************/
/*1*/void print_code_2d_arr(const char *arr[]);
/************************************************************/
/*                         STORAGE                          */
/************************************************************/
// Подаваемый код на вход лексеру
//char *code = NULL;
char code[0xFFF];
// Нотация регулярной грамматики (тип 4) для лексера
// Загрузку производим напрямую с файла
//char *regular_grammar_notation;
char regular_grammar_notation[0xFFF];
// Нотация регулярной грамматики (тип 4) для лексера
typedef enum {
    TOKEN__NUMBER,
    TOKEN__VARIABLE,
    TOKEN__SEMICOLON,
    TOKEN__SPACE,
    TOKEN__ASSIGN,
    TOKEN__LOG,
    TOKEN__PLUS,
    TOKEN__MINUS,
    TOKEN__LPAR,
    TOKEN__RPAR
} Type;
// Класс: Тип токена
typedef struct {
    Type name;         // Тип токена
    const char *regex; // Регулярное выражение типа токена
} TokenType;
// Класс: Токен
typedef struct {
    int number;         // Порядковый номер токена
    TokenType type;     // Тип токена
    const char *begin;  // Начало токена
    const char *end;    // Конец токена
    int pos_line;       // Позиция токена в строке
    int pos_column;     // Позиция токена в столбце
} Token;
/**/
FILE *file = NULL;
size_t symbols;
/************************************************************/
/*                        PROGRAM                           */
/************************************************************/
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

    const char *ptr__code = code;
    while (*ptr__code != '\0')
    {
        //распознаём токен
        //cursor += длина токена
        ptr__code++;
    }
    putchar('\n');
    return 0;
}
/************************************************************/
/*                      SUBPROGRAM                          */
/************************************************************/
/*1*/void print_code_2d_arr(const char *arr[]) { for (int i = 0; arr[i] != NULL; i++) printf("%s", arr[i]); }