#include <stdio.h>
#include <locale.h>
#include <ctype.h>

enum TypeToken {
    TOKEN__EOF,
    TOKEN__CHAR,
    TOKEN__STRING,
    TOKEN__NUMBER,
    TOKEN__IDENTIFIER
};

typedef struct {
    const char * grammar;    // данные
    int cursor;              // позиция в данных

    char TypeToken;          // тип токена
    int vertical_position;   // позиция в строке
    int horizontal_position; // позиция в колонке
    int position_in_file;    // позиция в файле
} Lexer;

static inline char peek(Lexer * lexer) { return lexer->grammar[lexer->cursor]; }
static inline char advance(Lexer * lexer)
{
    char c = lexer->grammar[lexer->cursor++];
    if (c == '\n')
    {
        lexer->vertical_position++;
        lexer->horizontal_position = 0;
    }
    else lexer->horizontal_position++;
    return c;
}
static inline void skip_whitespace() {}

FILE * file = NULL;
char grammar[1<<24] = ""; // временно, потом заменю на malloc при необходимости

int main()
{
    setlocale(0, "");

    file = fopen("syntax_test_unit.txt", "rb"); // или r?
    if (file == NULL) { printf("\nВозникла ошибка №%d.\n", 1); return 1; } // Не удалось открыть файл, одна из возможных причин: отсутствие файла.

    fread(grammar, sizeof (grammar), 1, file);
    printf("grammar =\n<#\n%s\n#>", grammar);

    Lexer lexer;
    while ('@')
    {
        //skip_whitespace();
        char c = peek(&lexer);
        if (isalpha(c) || c == '_') {} //return read_identifier();
        if (isdigit(c)) {} //return read_number();
        if (c == '"') {} //return read_string();
        //return read_symbol();
    }
    putchar('\n');
    return 0;
}