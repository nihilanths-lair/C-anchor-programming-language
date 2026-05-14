// @ The minimum viable product of the temporary compiler for the permanent meta-compiler is 50,0% done.
// @ Минимально жизнеспособный продукт временного компилятора для постоянного мета-компилятора сделан на 50,0%.
//
#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
//
#include "enum__token.h"
//
short number_of_tokens = -1;
char token__type_identifier[128];
//
#define MAX_VARS 256
char var_names[MAX_VARS][64];
int var_count = 0;
//
int get_varidx(const char * name)
{
    for (int i = 0; i < var_count; i++) if (strcmp(var_names[i], name) == 0) return i;
    strcpy(var_names[var_count], name);
    return var_count++;
}
//
#include "token__type_name.c"
#include "token__lexeme.c"
//
#define MACRO__MAXIMUM_CODE_LIMIT (1 << 16) // 65'536
unsigned char gl__opcodes[MACRO__MAXIMUM_CODE_LIMIT];
unsigned char * gl__ptr__opcodes = gl__opcodes; // Указатель, который будет двигаться по массиву и по мере надобности/необходимости заполнять его
unsigned short gl__idx__opcodes = 0-1;
//
#include "vm.c"
//
/*
char * GetTypeToken(short idx)
{
    char * ptr_type_token = &get_type_token[idx+1];
    return ptr_type_token;
}
void AddToken(const char * token_type)
{
    static short idx = 0;
    token__type_identifier[idx] = idx;
    strcpy(token__type_name[idx], token_type);
    idx++;
}
*/
#define MACRO__MAXIMUM_TOKEN_LIMIT 1500
struct Token
{
    short type_identifier;
    /*type_name[64+1];*/
    char lexeme[64+1];
    /*char pos_lexeme[2]*/
} __token, __tokens[MACRO__MAXIMUM_TOKEN_LIMIT]; // global variable struct and global array struct
//
int gl__row_position; // позиция в строке/линии
int gl__column_position; // позиция в столбце/колонке
int gl__flat_position; // плоская позиция
const char * gl__ptr__code; // смещение относительно файла
void InitLexer(const char * code) { gl__ptr__code = code; }
void Error(const char * message) { printf("\n Parse error [row %d, column %d, score: %d]: %s.", gl__row_position, gl__column_position, gl__flat_position, message); }
/*
struct Cursor {
    int row_position; // позиция строки/линии
    int column_position; // позиция столбца/колонки
    int offset_relative_to_file; // смещение относительно файла
} gl__cursor;
*/
//struct Lexer { int s_pos; int e_pos; char * cursor; } lexer = {0, 0, '\0'}; // global object's
//struct Parser { char * cursor; } parser; // global object's
//
//void LexicalAnalysisWithoutSynthesis(){} // Лексический анализ без синтеза (сканирует/проверяет на наличие ошибок, ничего не воспроизводит), распознающий компонент/модуль лексера
//void LexicalAnalysisWithSynthesis(){}    // Лексический анализ с синтезом (сканирует/проверяет на наличие ошибок и воспроизводит токены), порождающий компонент/модуль лексера
//
/// Для поточного режима лексера ///
short GetNextToken()
{
    switch_run:
    switch (*gl__ptr__code){
    case '\0':
    {
        __token.type_identifier = TOKEN__FINAL_TOKEN;
        return TOKEN__FINAL_TOKEN;
    }
    case '\t': case '\v': case '\f': case '\r': case ' ':
    {
        gl__ptr__code++;
        goto switch_run;
    }
    case '\n':
    {
        __token.lexeme[0] = '\\';
        __token.lexeme[1] = 'n';
        __token.lexeme[2] = '\0';
        __token.type_identifier = TOKEN__NEW_LINE;
        gl__ptr__code++;
        return TOKEN__NEW_LINE;
    }
    case '!':
    {
        __token.lexeme[0] = '!';
        gl__ptr__code++;
        if (*gl__ptr__code == '=')
        {
            __token.lexeme[1] = '=';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__INEQUALITY_OPERATOR;
            gl__ptr__code++;
            return TOKEN__INEQUALITY_OPERATOR;
        }
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__INVERSION_OPERATOR;
        return TOKEN__INVERSION_OPERATOR;
    }
    case '\'': // Если символьный литерал
    {
        short i = -1;
        __token.lexeme[++i] = '\'';
        gl__ptr__code++;
        while (*gl__ptr__code && *gl__ptr__code != '\'')
        {
            __token.lexeme[++i] = *gl__ptr__code;
            gl__ptr__code++;
        }
        if (*gl__ptr__code == '\'')
        {
            __token.lexeme[++i] = '\'';
            __token.lexeme[++i] = '\0';
            __token.type_identifier = TOKEN__CHARACTER_LITERAL;
            gl__ptr__code++;
            return TOKEN__CHARACTER_LITERAL;
        }
        return TOKEN__ERROR;
    }
    case '\"': // Если строковый литерал
    {
        short i = -1;
        __token.lexeme[++i] = '\"';
        gl__ptr__code++;
        while (*gl__ptr__code && *gl__ptr__code != '\"')
        {
            __token.lexeme[++i] = *gl__ptr__code;
            gl__ptr__code++;
        }
        if (*gl__ptr__code == '\"')
        {
            __token.lexeme[++i] = '\"';
            __token.lexeme[++i] = '\0';
            __token.type_identifier = TOKEN__STRING_LITERAL;
            gl__ptr__code++;
            return TOKEN__STRING_LITERAL;
        }
        return TOKEN__ERROR;
    }
    case '(':
    {
        __token.lexeme[0] = '(';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__LEFT_PARENTHESIS;
        gl__ptr__code++;
        return TOKEN__LEFT_PARENTHESIS;
    }
    case ')':
    {
        __token.lexeme[0] = ')';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__RIGHT_PARENTHESIS;
        gl__ptr__code++;
        return TOKEN__RIGHT_PARENTHESIS;
    }
    case '*':
    {
        __token.lexeme[0] = '*';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__MULTIPLICATION_OPERATOR;
        gl__ptr__code++;
        return TOKEN__MULTIPLICATION_OPERATOR;
    }
    case '+':
    {
        __token.lexeme[0] = '+';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__ADDITION_OPERATOR;
        gl__ptr__code++;
        return TOKEN__ADDITION_OPERATOR;
    }
    case '-':
    {
        __token.lexeme[0] = '-';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__SUBTRACT_OPERATOR;
        gl__ptr__code++;
        return TOKEN__SUBTRACT_OPERATOR;
    }
    case '/':
    {
        gl__ptr__code++;
        if (*gl__ptr__code == '/')
        {
            while (*gl__ptr__code && *gl__ptr__code != '\n') gl__ptr__code++;
            if (*gl__ptr__code == '\n') gl__ptr__code++;
            goto switch_run;
        }
        else if (*gl__ptr__code == '*')
        {
            static short comment_nesting = 0;
            comment_nesting++;
            gl__ptr__code++;
            while (*gl__ptr__code && *(gl__ptr__code+1)) // && !(*gl__ptr__code == '*' && *(gl__ptr__code+1) == '/')
            {
                if (*gl__ptr__code == '/' && *(gl__ptr__code+1) == '*')
                {
                    comment_nesting++;
                    gl__ptr__code += 2;
                    continue;
                }
                if (*gl__ptr__code == '*' && *(gl__ptr__code+1) == '/')
                {
                    comment_nesting--;
                    gl__ptr__code += 2;
                    if (comment_nesting == 0) break;
                    continue;
                }
                gl__ptr__code++;
            }
            goto switch_run;
        }
        __token.lexeme[0] = '/';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__DIVISION_OPERATOR;
        return TOKEN__DIVISION_OPERATOR;
    }
    case ':':
    {
        __token.lexeme[0] = ':';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__END_OF_LABEL;
        gl__ptr__code++;
        return TOKEN__END_OF_LABEL;
    }
    case ';':
    {
        __token.lexeme[0] = ';';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__END_OF_STATEMENT;
        gl__ptr__code++;
        return TOKEN__END_OF_STATEMENT;
    }
    case '<':
    {
        __token.lexeme[0] = '<';
        gl__ptr__code++;
        if (*gl__ptr__code == '=')
        {
            __token.lexeme[1] = '=';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__BELOW_EQUAL_OPERATOR;
            gl__ptr__code++;
            return TOKEN__BELOW_EQUAL_OPERATOR;
        }
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__BELOW_OPERATOR;
        return TOKEN__BELOW_OPERATOR;
    }
    case '=':
    {
        __token.lexeme[0] = '=';
        gl__ptr__code++;
        switch (*gl__ptr__code){
        case '<':
        {
            __token.lexeme[1] = '<';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__LEFT_ASSIGNMENT;
            gl__ptr__code++;
            return TOKEN__LEFT_ASSIGNMENT;
        }
        case '=':
        {
            __token.lexeme[1] = '=';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__EQUALITY_OPERATOR;
            gl__ptr__code++;
            return TOKEN__EQUALITY_OPERATOR;
        }
        case '>':
        {
            __token.lexeme[1] = '>';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__RIGHT_ASSIGNMENT;
            gl__ptr__code++;
            return TOKEN__RIGHT_ASSIGNMENT;
        }}
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__ASSIGNMENT;
        return TOKEN__ASSIGNMENT;
    }
    case '>':
    {
        __token.lexeme[0] = '>';
        gl__ptr__code++;
        if (*gl__ptr__code == '=')
        {
            __token.lexeme[1] = '=';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__ABOVE_EQUAL_OPERATOR;
            gl__ptr__code++;
            return TOKEN__ABOVE_EQUAL_OPERATOR;
        }
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__ABOVE_OPERATOR;
        return TOKEN__ABOVE_OPERATOR;
    }
    case '[':
    {
        __token.lexeme[0] = '[';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__LEFT_SQUARE_BRACKET;
        gl__ptr__code++;
        return TOKEN__LEFT_SQUARE_BRACKET;
    }
    case ']':
    {
        __token.lexeme[0] = ']';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__RIGHT_SQUARE_BRACKET;
        gl__ptr__code++;
        return TOKEN__RIGHT_SQUARE_BRACKET;
    }
    case '{':
    {
        __token.lexeme[0] = '{';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__LEFT_BRACE;
        gl__ptr__code++;
        return TOKEN__LEFT_BRACE;
    }
    case '}':
    {
        __token.lexeme[0] = '}';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__RIGHT_BRACE;
        gl__ptr__code++;
        return TOKEN__RIGHT_BRACE;
    }
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
    {
        short i = -1;
        __token.lexeme[++i] = *gl__ptr__code;
        gl__ptr__code++;
        while (isdigit(*gl__ptr__code))
        {
            __token.lexeme[++i] = *gl__ptr__code;
            gl__ptr__code++;
        }
        __token.lexeme[++i] = '\0';
        __token.type_identifier = TOKEN__NUMERIC_LITERAL;
        return TOKEN__NUMERIC_LITERAL;
    }
    default:
    {
        //printf("\n def 1");
        if (isalpha(*gl__ptr__code) || *gl__ptr__code == '_') // Первым символом не может быть цифра
        {
            // Пока не знаем что за токен: это может быть либо идентификатор, либо идентификатор метки
            short i = -1;
            __token.lexeme[++i] = *gl__ptr__code;
            gl__ptr__code++;
            while (isalnum(*gl__ptr__code) || *gl__ptr__code == '_')
            {
                __token.lexeme[++i] = *gl__ptr__code;
                gl__ptr__code++;
            }
            __token.lexeme[++i] = '\0';
            // Идентификатор из букв/цифр и нижнего подчёркивания собран
            if (!strcmp(__token.lexeme, "goto"))
            {
                __token.type_identifier = TOKEN__KEYWORD_GOTO;
                return TOKEN__KEYWORD_GOTO;
            }
            if (!strcmp(__token.lexeme, "if"))
            {
                __token.type_identifier = TOKEN__KEYWORD_IF;
                return TOKEN__KEYWORD_IF;
            }
            if (!strcmp(__token.lexeme, "while"))
            {
                __token.type_identifier = TOKEN__KEYWORD_WHILE;
                return TOKEN__KEYWORD_WHILE;
            }
            if (!strcmp(__token.lexeme, "switch"))
            {
                __token.type_identifier = TOKEN__KEYWORD_SWITCH;
                return TOKEN__KEYWORD_SWITCH;
            }
            if (!strcmp(__token.lexeme, "case"))
            {
                __token.type_identifier = TOKEN__KEYWORD_CASE;
                return TOKEN__KEYWORD_CASE;
            }
            if (!strcmp(__token.lexeme, "default"))
            {
                __token.type_identifier = TOKEN__KEYWORD_DEFAULT;
                return TOKEN__KEYWORD_DEFAULT;
            }
            if (!strcmp(__token.lexeme, "else")) // Опционально
            {
                __token.type_identifier = TOKEN__KEYWORD_ELSE;
                return TOKEN__KEYWORD_ELSE;
            }
            if (!strcmp(__token.lexeme, "return"))
            {
                __token.type_identifier = TOKEN__KEYWORD_RETURN;
                return TOKEN__KEYWORD_RETURN;
            }
            if (!strcmp(__token.lexeme, "print"))
            {
                __token.type_identifier = TOKEN__KEYWORD_PRINT;
                return TOKEN__KEYWORD_PRINT;
            }
            /*
            if (*gl__ptr__code == ':')
            {
                //__token.lexeme[++i] = '\0';
                //__token.lexeme[++i] = ':';
                __token.type_identifier = TOKEN__LABEL_IDENTIFIER;
                gl__ptr__code++;
                return TOKEN__LABEL_IDENTIFIER;
            }
            */
            __token.type_identifier = TOKEN__IDENTIFIER;
            return TOKEN__IDENTIFIER;
        }
        __token.type_identifier = TOKEN__UNKNOWN;
        __token.lexeme[0] = *gl__ptr__code;
        gl__ptr__code++;
        return TOKEN__UNKNOWN;
    }
    //
    }
    Error("Абра-кадабра");
    //printf("\n def 2");
    return TOKEN__ERROR;
}
//
/// Накопить токены (удобен для отладочного режима, занимает память) ///
short AccumulateTokens()
{
    switch_run:
    switch (*gl__ptr__code){
    case '\0':
    {
        __tokens[++number_of_tokens].type_identifier = TOKEN__FINAL_TOKEN;
        return TOKEN__FINAL_TOKEN;
    }
    case '\t': case '\v': case '\f': case '\r': case ' ':
    {
        gl__ptr__code++;
        goto switch_run;
    }
    case '\n':
    {
        __tokens[++number_of_tokens].lexeme[0] = '\\';
        __tokens[number_of_tokens].lexeme[1] = 'n';
        __tokens[number_of_tokens].lexeme[2] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__NEW_LINE;
        gl__ptr__code++;
        return TOKEN__NEW_LINE;
    }
    case '!':
    {
        __tokens[++number_of_tokens].lexeme[0] = '!';
        gl__ptr__code++;
        if (*gl__ptr__code == '=')
        {
            __tokens[number_of_tokens].lexeme[1] = '=';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__INEQUALITY_OPERATOR;
            gl__ptr__code++;
            return TOKEN__INEQUALITY_OPERATOR;
        }
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__INVERSION_OPERATOR;
        return TOKEN__INVERSION_OPERATOR;
    }
    case '\'': // Если символьный литерал
    {
        short i = -1;
        __tokens[++number_of_tokens].lexeme[++i] = '\'';
        gl__ptr__code++;
        while (*gl__ptr__code && *gl__ptr__code != '\'')
        {
            __tokens[number_of_tokens].lexeme[++i] = *gl__ptr__code;
            gl__ptr__code++;
        }
        if (*gl__ptr__code == '\'')
        {
            __tokens[number_of_tokens].lexeme[++i] = '\'';
            __tokens[number_of_tokens].lexeme[++i] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__CHARACTER_LITERAL;
            gl__ptr__code++;
            return TOKEN__CHARACTER_LITERAL;
        }
        return TOKEN__ERROR;
    }
    case '\"': // Если строковый литерал
    {
        short i = -1;
        __tokens[++number_of_tokens].lexeme[++i] = '\"';
        gl__ptr__code++;
        while (*gl__ptr__code && *gl__ptr__code != '\"')
        {
            __tokens[number_of_tokens].lexeme[++i] = *gl__ptr__code;
            gl__ptr__code++;
        }
        if (*gl__ptr__code == '\"')
        {
            __tokens[number_of_tokens].lexeme[++i] = '\"';
            __tokens[number_of_tokens].lexeme[++i] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__STRING_LITERAL;
            gl__ptr__code++;
            return TOKEN__STRING_LITERAL;
        }
        return TOKEN__ERROR;
    }
    case '(':
    {
        __tokens[++number_of_tokens].lexeme[0] = '(';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__LEFT_PARENTHESIS;
        gl__ptr__code++;
        return TOKEN__LEFT_PARENTHESIS;
    }
    case ')':
    {
        __tokens[++number_of_tokens].lexeme[0] = ')';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__RIGHT_PARENTHESIS;
        gl__ptr__code++;
        return TOKEN__RIGHT_PARENTHESIS;
    }
    case '*':
    {
        __tokens[++number_of_tokens].lexeme[0] = '*';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__MULTIPLICATION_OPERATOR;
        gl__ptr__code++;
        return TOKEN__MULTIPLICATION_OPERATOR;
    }
    case '+':
    {
        __tokens[++number_of_tokens].lexeme[0] = '+';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__ADDITION_OPERATOR;
        gl__ptr__code++;
        return TOKEN__ADDITION_OPERATOR;
    }
    case '-':
    {
        __tokens[++number_of_tokens].lexeme[0] = '-';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__SUBTRACT_OPERATOR;
        gl__ptr__code++;
        return TOKEN__SUBTRACT_OPERATOR;
    }
    case '/':
    {
        gl__ptr__code++;
        if (*gl__ptr__code == '/')
        {
            while (*gl__ptr__code && *gl__ptr__code != '\n') gl__ptr__code++;
            if (*gl__ptr__code == '\n') gl__ptr__code++;
            goto switch_run;
        }
        else if (*gl__ptr__code == '*')
        {
            static short comment_nesting = 0;
            comment_nesting++;
            gl__ptr__code++;
            while (*gl__ptr__code && *(gl__ptr__code+1)) // && !(*gl__ptr__code == '*' && *(gl__ptr__code+1) == '/')
            {
                if (*gl__ptr__code == '/' && *(gl__ptr__code+1) == '*')
                {
                    comment_nesting++;
                    gl__ptr__code += 2;
                    continue;
                }
                if (*gl__ptr__code == '*' && *(gl__ptr__code+1) == '/')
                {
                    comment_nesting--;
                    gl__ptr__code += 2;
                    if (comment_nesting == 0) break;
                    continue;
                }
                gl__ptr__code++;
            }
            goto switch_run;
        }
        __tokens[++number_of_tokens].lexeme[0] = '/';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__DIVISION_OPERATOR;
        return TOKEN__DIVISION_OPERATOR;
    }
    case ':':
    {
        __tokens[++number_of_tokens].lexeme[0] = ':';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__END_OF_LABEL;
        gl__ptr__code++;
        return TOKEN__END_OF_LABEL;
    }
    case ';':
    {
        __tokens[++number_of_tokens].lexeme[0] = ';';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__END_OF_STATEMENT;
        gl__ptr__code++;
        return TOKEN__END_OF_STATEMENT;
    }
    case '<':
    {
        __tokens[++number_of_tokens].lexeme[0] = '<';
        gl__ptr__code++;
        if (*gl__ptr__code == '=')
        {
            __tokens[number_of_tokens].lexeme[1] = '=';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__BELOW_EQUAL_OPERATOR;
            gl__ptr__code++;
            return TOKEN__BELOW_EQUAL_OPERATOR;
        }
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__BELOW_OPERATOR;
        return TOKEN__BELOW_OPERATOR;
    }
    case '=':
    {
        __tokens[++number_of_tokens].lexeme[0] = '=';
        gl__ptr__code++;
        switch (*gl__ptr__code){
        case '<':
        {
            __tokens[number_of_tokens].lexeme[1] = '<';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__LEFT_ASSIGNMENT;
            gl__ptr__code++;
            return TOKEN__LEFT_ASSIGNMENT;
        }
        case '=':
        {
            __tokens[number_of_tokens].lexeme[1] = '=';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__EQUALITY_OPERATOR;
            gl__ptr__code++;
            return TOKEN__EQUALITY_OPERATOR;
        }
        case '>':
        {
            __tokens[number_of_tokens].lexeme[1] = '>';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__RIGHT_ASSIGNMENT;
            gl__ptr__code++;
            return TOKEN__RIGHT_ASSIGNMENT;
        }}
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__ASSIGNMENT;
        return TOKEN__ASSIGNMENT;
    }
    case '>':
    {
        __tokens[++number_of_tokens].lexeme[0] = '>';
        gl__ptr__code++;
        if (*gl__ptr__code == '=')
        {
            __tokens[number_of_tokens].lexeme[1] = '=';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__ABOVE_EQUAL_OPERATOR;
            gl__ptr__code++;
            return TOKEN__ABOVE_EQUAL_OPERATOR;
        }
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__ABOVE_OPERATOR;
        return TOKEN__ABOVE_OPERATOR;
    }
    case '[':
    {
        __tokens[++number_of_tokens].lexeme[0] = '[';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__LEFT_SQUARE_BRACKET;
        gl__ptr__code++;
        return TOKEN__LEFT_SQUARE_BRACKET;
    }
    case ']':
    {
        __tokens[++number_of_tokens].lexeme[0] = ']';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__RIGHT_SQUARE_BRACKET;
        gl__ptr__code++;
        return TOKEN__RIGHT_SQUARE_BRACKET;
    }
    case '{':
    {
        __tokens[++number_of_tokens].lexeme[0] = '{';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__LEFT_BRACE;
        gl__ptr__code++;
        return TOKEN__LEFT_BRACE;
    }
    case '}':
    {
        __tokens[++number_of_tokens].lexeme[0] = '}';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__RIGHT_BRACE;
        gl__ptr__code++;
        return TOKEN__RIGHT_BRACE;
    }
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
    {
        short i = -1;
        __tokens[++number_of_tokens].lexeme[++i] = *gl__ptr__code;
        gl__ptr__code++;
        while (isdigit(*gl__ptr__code))
        {
            __tokens[number_of_tokens].lexeme[++i] = *gl__ptr__code;
            gl__ptr__code++;
        }
        __tokens[number_of_tokens].lexeme[++i] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__NUMERIC_LITERAL;
        return TOKEN__NUMERIC_LITERAL;
    }
    default:
    {
        //printf("\n def 1");
        if (isalpha(*gl__ptr__code) || *gl__ptr__code == '_') // Первым символом не может быть цифра
        {
            // Пока не знаем что за токен: это может быть либо идентификатор, либо идентификатор метки
            short i = -1;
            __tokens[++number_of_tokens].lexeme[++i] = *gl__ptr__code;
            gl__ptr__code++;
            while (isalnum(*gl__ptr__code) || *gl__ptr__code == '_')
            {
                __tokens[number_of_tokens].lexeme[++i] = *gl__ptr__code;
                gl__ptr__code++;
            }
            __tokens[number_of_tokens].lexeme[++i] = '\0';
            // Идентификатор из букв/цифр и нижнего подчёркивания собран
            if (!strcmp(__tokens[number_of_tokens].lexeme, "goto"))
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_GOTO;
                return TOKEN__KEYWORD_GOTO;
            }
            if (!strcmp(__tokens[number_of_tokens].lexeme, "if"))
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_IF;
                return TOKEN__KEYWORD_IF;
            }
            if (!strcmp(__tokens[number_of_tokens].lexeme, "while"))
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_WHILE;
                return TOKEN__KEYWORD_WHILE;
            }
            if (!strcmp(__tokens[number_of_tokens].lexeme, "switch"))
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_SWITCH;
                return TOKEN__KEYWORD_SWITCH;
            }
            if (!strcmp(__tokens[number_of_tokens].lexeme, "case"))
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_CASE;
                return TOKEN__KEYWORD_CASE;
            }
            if (!strcmp(__tokens[number_of_tokens].lexeme, "default"))
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_DEFAULT;
                return TOKEN__KEYWORD_DEFAULT;
            }
            if (!strcmp(__tokens[number_of_tokens].lexeme, "else")) // Опционально
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_ELSE;
                return TOKEN__KEYWORD_ELSE;
            }
            if (!strcmp(__tokens[number_of_tokens].lexeme, "return"))
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_RETURN;
                return TOKEN__KEYWORD_RETURN;
            }
            if (!strcmp(__tokens[number_of_tokens].lexeme, "print"))
            {
                __tokens[number_of_tokens].type_identifier = TOKEN__KEYWORD_PRINT;
                return TOKEN__KEYWORD_PRINT;
            }
            /*
            if (*gl__ptr__code == ':')
            {
                //__tokens[number_of_tokens].lexeme[++i] = '\0';
                __tokens[number_of_tokens].type_identifier = TOKEN__LABEL_IDENTIFIER;
                //__tokens[number_of_tokens].lexeme[++i] = ':';
                gl__ptr__code++;
                return TOKEN__LABEL_IDENTIFIER;
            }
            */
            __tokens[number_of_tokens].type_identifier = TOKEN__IDENTIFIER;
            return TOKEN__IDENTIFIER;
        }
        __tokens[++number_of_tokens].type_identifier = TOKEN__UNKNOWN;
        __tokens[number_of_tokens].lexeme[0] = *gl__ptr__code;
        gl__ptr__code++;
        return TOKEN__UNKNOWN;
    }
    //
    }
    Error("Абра-кадабра");
    //printf("\n def 2");
    return TOKEN__ERROR;
}
//
/// Распечатать все токены ///
void PrintAllTokens()
{
    while ((__token.type_identifier = GetNextToken()) != TOKEN__FINAL_TOKEN)
    {
        printf("\n--------------------------+---------------------------------");
        printf("\n %s | %s", token__type_name[__token.type_identifier], __token.lexeme);
    }
    printf("\n--------------------------+---------------------------------");
}
//
char is_binary_operator(const short token__type_identifier)
{
    switch (token__type_identifier){
    //- Бинарные операторы -/
    case TOKEN__ADDITION_OPERATOR:
    case TOKEN__SUBTRACT_OPERATOR:
    case TOKEN__MULTIPLICATION_OPERATOR:
    case TOKEN__DIVISION_OPERATOR:
    case TOKEN__BELOW_OPERATOR:
    case TOKEN__ABOVE_OPERATOR:
    case TOKEN__BELOW_EQUAL_OPERATOR:
    case TOKEN__ABOVE_EQUAL_OPERATOR:
    case TOKEN__EQUALITY_OPERATOR:
    case TOKEN__INEQUALITY_OPERATOR:
        return 1;
    //
    }
    return 0;
}
//
///////////////////////////////////////////
/// *[* Пространство имён: Parse__? *]* ///
///////////////////////////////////////////
short current_token = 0;
//
void Parse__Priority_Level_One();
void Parse__Priority_Level_Two();
void Parse__Priority_Level_Three();
//
/// Самый высокий (для операндов: чисел/идентификаторов и круглых скобочек которые переопределяют/задают порядок) ///
void Parse__Priority_Level_One() // первичный/базовый, минимальный атом
{
    //printf("\n     CALL OPERAND");
    switch (__tokens[current_token].type_identifier){
    case TOKEN__LEFT_PARENTHESIS:
    {
        current_token++;
        Parse__Priority_Level_Three();
        if (__tokens[current_token].type_identifier == TOKEN__RIGHT_PARENTHESIS) current_token++;
        else Error("Expected ')'");
        break;
    }
    case TOKEN__NUMERIC_LITERAL:
    {
        printf("\n OPERAND: %s", __tokens[current_token].lexeme);
        current_token++;
        break;
    }
    case TOKEN__IDENTIFIER:
    {
        // Здесь будет эмит
        current_token++;
    }}
    //printf("\n     RET OPERAND");
}
//
/// Средний (для умножения, деления) ///
void Parse__Priority_Level_Two()
{
    //printf("\n   CALL OPERATOR MUL/DIV");
    Parse__Priority_Level_One();
    while (__tokens[current_token].type_identifier == TOKEN__MULTIPLICATION_OPERATOR || __tokens[current_token].type_identifier == TOKEN__DIVISION_OPERATOR)
    {
        short operator = __tokens[current_token].type_identifier;
        printf("\n OPERATOR: %s", __tokens[current_token].lexeme);
        current_token++;
        Parse__Priority_Level_One();
        // Здесь будет эмит MUL или DIV
    }
    //printf("\n   RET OPERATOR MUL/DIV");
}
//
/// Самый низкий (для сложения, вычитания) ///
void Parse__Priority_Level_Three()
{
    //printf("\n CALL OPERATOR ADD/SUB");
    Parse__Priority_Level_Two();
    while (__tokens[current_token].type_identifier == TOKEN__ADDITION_OPERATOR || __tokens[current_token].type_identifier == TOKEN__SUBTRACT_OPERATOR)
    {
        short operator = __tokens[current_token].type_identifier;
        printf("\n OPERATOR: %s", __tokens[current_token].lexeme);
        current_token++;
        Parse__Priority_Level_Two();
        // Здесь будет эмит ADD или SUB
    }
    //printf("\n RET OPERATOR ADD/SUB");
}
void Parse__Expression()
{
    Parse__Priority_Level_Three();
}
/*
void Parse__Expression()
{
    // Проверяем первый операнд
    switch (__tokens[current_token].type_identifier){
    //case TOKEN__IDENTIFIER:
    //case TOKEN__CHARACTER_LITERAL:
    case TOKEN__NUMERIC_LITERAL:
    {
        printf("\n Parse__Expression 1: NUMERIC_LITERAL");
        int literal = atoi(__tokens[current_token].lexeme);
        current_token++;
        switch_run:
        // Проверяем унарное это выражение или бинарное (что следует за первым операндом?)
        switch (__tokens[current_token].type_identifier){
        case TOKEN__END_OF_STATEMENT:
        case TOKEN__NEW_LINE:
        case TOKEN__FINAL_TOKEN: printf("\n TOKEN: %s", token__type_name[__tokens[current_token].type_identifier]);
        break;
        // Унарное выражение
        case TOKEN__NUMERIC_LITERAL: printf("\n Это унарное выражение.");
        break;
        // Бинарное выражение
        case TOKEN__ADDITION_OPERATOR:
        case TOKEN__SUBTRACT_OPERATOR:
        case TOKEN__MULTIPLICATION_OPERATOR:
        case TOKEN__DIVISION_OPERATOR:
        {
            printf("\n TOKEN:%s", token__type_name[__tokens[current_token].type_identifier]);
            current_token++;
            // Проверяем след. операнд
            switch (__tokens[current_token].type_identifier){
            case TOKEN__NUMERIC_LITERAL:
            case TOKEN__CHARACTER_LITERAL:
            {
                printf("\n    До | literal: %d", literal);
                switch (__tokens[current_token-1].type_identifier){
                case TOKEN__ADDITION_OPERATOR:       literal += atoi(__tokens[current_token].lexeme); break;
                case TOKEN__SUBTRACT_OPERATOR:       literal -= atoi(__tokens[current_token].lexeme); break;
                case TOKEN__MULTIPLICATION_OPERATOR: literal *= atoi(__tokens[current_token].lexeme); break;
                case TOKEN__DIVISION_OPERATOR:       literal /= atoi(__tokens[current_token].lexeme); break;
                default: printf("\n #Error 1: Не оператор."); return;
                }
                printf("\n После | literal: %d\n", literal);
                current_token++;
                goto switch_run;
            }
            default: printf("\n #Error: След. операнд не число.");
            //
            }
        }
        break;
        default: printf("\n << Parse__Expression 2 >> TOKEN:%s", token__type_name[__tokens[current_token].type_identifier]);
        //
        }
        gl__opcodes[gl__idx__opcodes++] = 0x05; // загрузить след. число в регистр
        gl__opcodes[gl__idx__opcodes++] = literal; // само число
        //gl__opcodes[gl__idx__opcodes++] = opcodes; // отобразить число на консоль
        //printf("\n После | <%03d:%03d> = <%02X:%02X>", gl__idx__opcodes, gl__opcodes[gl__idx__opcodes], gl__idx__opcodes, gl__opcodes[gl__idx__opcodes]);
        //gl__idx__opcodes++;
    }
    break;
    default: printf("\n << Parse__Expression 1 >> TOKEN:%s", token__type_name[__tokens[current_token].type_identifier]);
    //
    }
}
*/
void Parse__Assignment()
{
    if (__tokens[current_token].type_identifier != TOKEN__IDENTIFIER)
    {
        printf("\n #Error: Parse__Assignment!");
        return;
    }
    current_token++;
    if (__tokens[current_token].type_identifier != TOKEN__ASSIGNMENT)
    {
        printf("\n #Error: Parse__Assignment-2!");
        return;
    }
    current_token++;
    //Parse__Expression();
    if (__tokens[current_token].type_identifier != TOKEN__END_OF_STATEMENT)
    {
        printf("\n #Error: Parse__Assignment-3!");
        return;
    }
    current_token++;
}
//
void Parse__Statement()
{
    switch (__tokens[current_token].type_identifier){
    case TOKEN__KEYWORD_PRINT:
    {
        current_token++;
        Parse__Expression(); // генерирует код вывода (0x77)
    }
    break;
    case TOKEN__IDENTIFIER:
    {
        printf("\n Parse__Statement  1: IDENTIFIER");
        ///Parse__Assignment();///
        // Проверка, что это присваивание
        if (__tokens[current_token+1].type_identifier != TOKEN__ASSIGNMENT)
        {
            Error("Expected '='");
            return;
        }
        // Здесь нужно будет обработать переменную, но пока просто пропускаем
        /*
        int i = 0;
        int size = sizeof (identifier) / sizeof (identifier[0]);
        while (i < size)
        {
            //printf("\n \"%s\" %s \"%s\"", __tokens[current_token].lexeme, (!strcmp(__tokens[current_token].lexeme, identifier[i])) ? "==" : "!=", identifier[i]);
            if (!strcmp(__tokens[current_token].lexeme, identifier[i])) /*printf("\n Идентификатор обнаружен, обновим его значения."); // {?} // break;
            i++;
        }
        //printf("\n Идентификатор \"%s\" не обнаружен, создадим/добавим его.", __tokens[current_token].lexeme);
        i = 0;
        strcpy(identifier[i], __tokens[current_token].lexeme);
        */
        current_token++; // пропускаем идентификатор
        current_token++; // пропускаем '='
        Parse__Expression(); // генерирует код загрузки константы (0x05)
    }
    break;
    default:
    {
        Error("This is not a statement");
        return;
    }}
    // После разбора оператора — пропускаем разделитель ('\n' или ';') или конец файла
    switch (__tokens[current_token].type_identifier){
    case TOKEN__END_OF_STATEMENT:
    case TOKEN__NEW_LINE: current_token++;
    break;
    case TOKEN__FINAL_TOKEN: // конец файла — ничего не делаем, это допустимо
    break;
    default: Error("Expected newline or ';' after statement");
    }
}
//
char ga__compiler_stack[0xFF] = {0}; // стек компилятора
char * gp__compiler_sp = ga__compiler_stack;
char gi__compiler_sp = 0;
//
char ga__operator_stack[0xFF]; char * gp__operator_stack = ga__operator_stack; char gi__operator_stack = -1;
char ga__operand_stack[0xFF]; char * gp__operand_stack = ga__operand_stack; char gi__operand_stack = -1;
void MarshallingYard(const char * data)
{
    printf("\n MarshallingYard(\"%s\")", data);
    ga__operand_stack[++gi__operand_stack] = data[0]; // PUSH_OPERAND 5
    ga__operator_stack[++gi__operator_stack] = data[2]; // PUSH_OPERATOR +
    ga__operand_stack[++gi__operand_stack] = data[4]; // PUSH_OPERAND 3
    ga__operator_stack[++gi__operator_stack] = data[6]; // PUSH_OPERATOR *
    ga__operand_stack[++gi__operand_stack] = data[8]; // PUSH_OPERAND 2
    printf("\n %c%c%c%c%c | Инфиксная", data[0], data[2], data[4], data[6], data[8]);
    printf("\n %s%s | Префиксная форма", ga__operator_stack, ga__operand_stack);
    printf("\n %s%s | Постфиксная форма", ga__operand_stack, ga__operator_stack);
}
///////////////////////////////////////////
void _$()
{
    setlocale(0, "");
    //
    const char code[] = "5 + 3 * (6 - 2 / 2)";//x = 123";\ny = 12\nz = 1
    MarshallingYard(code);
    /*
    const char code[] =
     " // Однострочный комментарий\n"
     " /*\n"
     " get_res()\n"
     " {\n"
     "    return 10 / 2;\n"
     " }\n"
     " get_res();\n"
     " string[] = \"C$ is awesome!\";\n"
     " /*\n"
     "    Вложенный\n"
     "    Многострочный\n"
     "    Комментарий\n"
     " *//*\n"
     " 2 > 3;\n"
     " goto _0;\n"
     " !(2 == 3);\n"
     " array[45];\n"
     " if (variable == 5) {}\n"
     " while (variable != 2) {}\n"
     " __abc = 2;\n"
     " switch (__abc)\n"
     " case 2: {}\n"
     " default: {}\n"
     " _0:\n"
     " print(\" 3 + 5 - 1 * 2 = %s\", 3 + 5 - 1 * 2);\n"
     " 3 + 5 - 1 * 2;\n"
     " *//*\n"
     " /*print 5 + 3 - 2 * 3 / 6;*//*\n"
     " /*xyz = 5 + 3 - 2 * 3 / 6;*//*\n"
     " ///*print*//*8 - 2;\n"
     " ///*print*//*6 * 3;\n"
     " ///*print*//*18 / 6;\n"
     " ///*print*//*'C' + '$'/*; // 67 + 36 = 103 / 'g'\n"
     ; // inline-код для быстрого тестирования (временно)*/
    printf("\n%s", code);
    gl__ptr__code = code;
    /*
    while ((__token.type_identifier = GetNextToken()) != TOKEN__FINAL_TOKEN) // Поточный режим лексера (удобен тем, что не засоряем лишнюю память)
    {
        printf("\n--------------------------+---------------------------------");
        printf("\n %s | %s", token__type_name[__token.type_identifier], __token.lexeme);
    }
    printf("\n--------------------------+---------------------------------");
    */
    PrintAllTokens();
    printf("\n\n");
    gl__ptr__code = code;
    while (AccumulateTokens() != TOKEN__FINAL_TOKEN)
    {
        printf("\n--------------------------+---------------------------------");
        printf("\n %s | %s", token__type_name[__tokens[number_of_tokens].type_identifier], __tokens[number_of_tokens].lexeme);
    }
    printf("\n--------------------------+---------------------------------");
    /*
    number_of_tokens = -1;
    while (__tokens[++number_of_tokens].type_identifier != TOKEN__FINAL_TOKEN)
    {
        printf("\n--------------------------+---------------------------------");
        printf("\n %s | %s", token__type_name[__tokens[number_of_tokens].type_identifier], __tokens[number_of_tokens].lexeme);
    }
    printf("\n-----------------------+------------------------------------");
    */
    gl__idx__opcodes = 0;
    gl__opcodes[gl__idx__opcodes++] = 0x76; // выведет строку "Hello"
    current_token = 0;
    while (__tokens[current_token].type_identifier != TOKEN__FINAL_TOKEN) // Если обнаружен конечный токен, завершаем цикл
    {
        //Parse__Priority_Level_Three(); // разбираем выражение
        Parse__Expression(); // разбираем выражение
        if (__tokens[current_token].type_identifier == TOKEN__END_OF_STATEMENT || __tokens[current_token].type_identifier == TOKEN__NEW_LINE) current_token++;
    }
    //if (__tokens[current_token].type_identifier == TOKEN__FINAL_TOKEN)
    printf("\n Analysis is over.\n");
    gl__opcodes[gl__idx__opcodes] = 0x79; // Останова
    //Debug_Loader_VM();
    Loader_VM();
    Debug_Loader_VM();
    Executor_VM();
    /*
    putchar('\n');
    unsigned char i = 0;
    int j = 0xFFFFFF80;
    do
    {
        printf("\n %03d|%02X|%c = %03d|%02X|%c", i, i, i, j, j, j);
        j++;
    }
    while (i++ != 255);
    */
    //
    putchar('\n');
}
//
int main() { _$(); }

/*/-/// DECLARATION ///-/
Бессмысленные конструкции (предупреждения по умолчанию выдаются, но можно выключить в настройках компилятора, либо через препроцессорную директиву)
x      /!\ `x` нигде не используется
x;     /!\ `x` нигде не используется
5      /!\ `5` нигде не используется
5;     /!\ `5` нигде не используется
x = 5  /!\ `x` нигде не используется
x = 5; /!\ `x` нигде не используется
goto end
goto end;
go to end
go to end;
end:

1 | NUMERIC_LITERAL       | ЧИСЛОВОЙ_ЛИТЕРАЛ
2 | LEFT_SIDED_ASSIGNMENT | ЛЕВОСТОРОННЕЕ_ПРИСВАИВАНИЕ
3 | IDENTIFIER            | ИДЕНТИФИКАТОР
4 | SPACE_SEPARATOR       | РАЗДЕЛИТЕЛЬ_ПРОСТРАНСТВА
5 | END_OF_STATEMENT      | КОНЕЦ_ЗАЯВЛЕНИЯ
6 | KEYWORD__GOTO         | КЛЮЧЕВОЕ_СЛОВО_ПЕРЕЙТИ

///-/*/
/*
// Пропустить специальные символы (одним отдельным проходом или по мере встречи/попадания?)
void SkipSpecialCharacters(char * pos){}
//
// Удалить специальные символы (одним отдельным проходом или по мере встречи/попадания?)
// Пока реализация проход за один раз полностью
void RemoveSpecialCharacters(char * pos)
{
    char * read = pos, * write = pos;
    while (*read != '\0')
    {
        if (*read != ' ')
        {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}
*/
/*
char state_vector = 0; // state vector / вектор состояний
// NextState = Table[CurrentState][InputChar];

// state = Table[state][*code_ptr];
// action = ActionTable[state][*code_ptr]; // Что сделать: записать байт, пропустить и т.д.
//code_ptr++;
char state_table[1][0xFF];
char action_table[1][0xFF];

char action[] = {'\0', '.', '>'};
char state = 1;

while (1)
{
    switch (action[state]){
    case '\0': goto while_end;
    case '.': { printf("\n Символ: '\\d%03d', '%c', '\\h%02X'.", *code, *code, *code); state = 2; } break; // Распечатать символ
    case '>': { ++code; state = 1; } break; // Сдвинуть указатель на один шаг/позицию вперёд
    default: printf("\n Неизвестное действие: '\\d%03d', '%c', '\\h%02X'.", action_table[state][*code], action_table[state][*code], action_table[state][*code]);
    }
}

// transaction_codes / коды транзакций
while (1)
{
    switch (*code){
    case '\0': goto while_end;
    case ' ': { printf("\n Символ: '\\d%03d', '%c', '\\h%02X'.", *code, *code, *code); ++code; } break; // игнорируем пробельные символы (в данной реализации опустим их)
    break; // заглушка (на всякий случай)

    case '+': {} break; // '\d043'
    case '-': {} break; // '\d045'

    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        { printf("\n Символ (цифра): '\\d%03d', '%c', '\\h%02X'.", *code, *code, *code); ++code; } break;

    case ';': { printf("\n Символ: '\\d%03d', '%c', '\\h%02X'.", *code, *code, *code); ++code; } break; // '\d044', ',', '\h2C'

    case '<': {} break; // '\d060'
    case '=': { printf("\n Символ: '\\d%03d', '%c', '\\h%02X'.", *code, *code, *code); ++code; } break; // '\d061'
    case '>': {} break; // '\d062'

    case '_':
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': // 13
    case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z': // 26

    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': // 13
    case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        { printf("\n Символ (буква): '\\d%03d', '%c', '\\h%02X'.", *code, *code, *code); ++code; } break; // '\d097'~'\d122', 'a'~'z', '\h78'~'\h7A' / 26

    default: printf("\n Неизвестный символ: '\\d%03d', '%c', '\\h%02X'.", *code, *code, *code); ++code;
    }
}
while_end:
*/