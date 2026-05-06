// @ Minimum viable product of the compiler is 32% ready / Минимально жизнеспособный продукт компилятора готов на 32%
//
#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
//
enum
{
    // Идентификация токенов для лексера и парсера (лексического/синтаксического анализа и синтеза)
    TOKEN__NUMERIC_LITERAL,       // ЧИСЛОВОЙ ЛИТЕРАЛ
    TOKEN__STRING_LITERAL,        // СТРОКОВЫЙ ЛИТЕРАЛ
    TOKEN__LEFT_SIDED_ASSIGNMENT, // ЛЕВОСТОРОННЕЕ ПРИСВАИВАНИЕ
    TOKEN__IDENTIFIER,            // ИДЕНТИФИКАТОР
    TOKEN__LABEL_IDENTIFIER,      // ИДЕНТИФИКАТОР МЕТКИ
    TOKEN__END_OF_LABEL,          // КОНЕЦ МЕТКИ
    TOKEN__SPACE_SEPARATOR,       // РАЗДЕЛИТЕЛЬ ПРОСТРАНСТВА
    TOKEN__END_OF_STATEMENT,      // КОНЕЦ ЗАЯВЛЕНИЯ

    TOKEN__LEFT_BRACE,            // ЛЕВАЯ ФИГУРНАЯ СКОБКА    ···· {
    TOKEN__RIGHT_BRACE,           // ПРАВАЯ ФИГУРНАЯ СКОБКА   ···· }

    TOKEN__LEFT_SQUARE_BRACKET,   // ЛЕВАЯ КВАДРАТНАЯ СКОБКА  ···· [
    TOKEN__RIGHT_SQUARE_BRACKET,  // ПРАВАЯ КВАДРАТНАЯ СКОБКА ···· ]

    TOKEN__LEFT_PARENTHESIS,      // ЛЕВАЯ КРУГЛАЯ СКОБКА     ···· (
    TOKEN__RIGHT_PARENTHESIS,     // ПРАВАЯ КРУГЛАЯ СКОБКА    ···· )

    // Ключевые слова
    TOKEN__KEYWORD_IF,      // КЛЮЧЕВОЕ_СЛОВО__ЕСЛИ
    TOKEN__KEYWORD_WHILE,   // КЛЮЧЕВОЕ_СЛОВО__ПОКА
    TOKEN__KEYWORD_GOTO,    // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕЙТИ
    TOKEN__KEYWORD_SWITCH,  // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕКЛЮЧАТЕЛЬ
    TOKEN__KEYWORD_CASE,    // КЛЮЧЕВОЕ_СЛОВО__КЕЙС
    TOKEN__KEYWORD_DEFAULT, // КЛЮЧЕВОЕ_СЛОВО__ПО_УМОЛЧАНИЮ
    TOKEN__KEYWORD_RETURN,  // КЛЮЧЕВОЕ_СЛОВО__ВЕРНУТЬ
    //
    TOKEN__KEYWORD_ELSE,    // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ (Опционально)
    //TOKEN__KEYWORD_ELSE_IF, // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ_ЕСЛИ (Опционально)

    // Операторы
    TOKEN__ADDITION_OPERATOR,       // СЛОЖЕНИЕ             ·     +
    TOKEN__SUBTRACT_OPERATOR,       // ВЫЧИТАНИЕ            ·     -
    TOKEN__MULTIPLICATION_OPERATOR, // УМНОЖЕНИЕ            ·     *
    TOKEN__DIVISION_OPERATOR,       // ДЕЛЕНИЕ              ·     /
    TOKEN__INVERSION_OPERATOR,      // ИНВЕРСИЯ             ·     !
    TOKEN__EQUALITY_OPERATOR,       // РАВЕНСТВО            ·     ==
    TOKEN__INEQUALITY_OPERATOR,     // НЕРАВЕНСТВО          ·     !=
    TOKEN__BELOW_OPERATOR,          // МЕНЬШЕ               ·     <
    TOKEN__ABOVE_OPERATOR,          // БОЛЬШЕ               ·     >
    TOKEN__BELOW_EQUAL_OPERATOR,    // МЕНЬШЕ ИЛИ РАВНО     ·     <=
    TOKEN__ABOVE_EQUAL_OPERATOR,    // БОЛЬШЕ ИЛИ РАВНО     ·     >=

    TOKEN__UNKNOWN, // НЕИЗВЕСТНЫЙ
    TOKEN__EOF,     // КОНЕЦ
    TOKEN__ERROR    // ОШИБКА
};
//
short number_of_tokens = -1;
char token__type_identifier[128];
char token__type_name[][64+1] =
{
    "         NUMERIC_LITERAL", // ЧИСЛОВОЙ ЛИТЕРАЛ
    "          STRING_LITERAL", // СТРОКОВЫЙ ЛИТЕРАЛ
    "   LEFT_SIDED_ASSIGNMENT", // ЛЕВОСТОРОННЕЕ ПРИСВАИВАНИЕ
    "              IDENTIFIER", // ИДЕНТИФИКАТОР
    "        LABEL_IDENTIFIER", // ИДЕНТИФИКАТОР МЕТКИ
    "            END_OF_LABEL", // КОНЕЦ МЕТКИ
    "         SPACE_SEPARATOR", // РАЗДЕЛИТЕЛЬ ПРОСТРАНСТВА
    "        END_OF_STATEMENT", // КОНЕЦ ЗАЯВЛЕНИЯ

    "              LEFT_BRACE", // ЛЕВАЯ ФИГУРНАЯ СКОБКА    ···· {
    "             RIGHT_BRACE", // ПРАВАЯ ФИГУРНАЯ СКОБКА   ···· }

    "     LEFT_SQUARE_BRACKET", // ЛЕВАЯ КВАДРАТНАЯ СКОБКА  ···· [
    "    RIGHT_SQUARE_BRACKET", // ПРАВАЯ КВАДРАТНАЯ СКОБКА ···· ]

    "        LEFT_PARENTHESIS", // ЛЕВАЯ КРУГЛАЯ СКОБКА     ···· (
    "       RIGHT_PARENTHESIS", // ПРАВАЯ КРУГЛАЯ СКОБКА    ···· )

    // Ключевые слова
    "             KEYWORD__IF", // КЛЮЧЕВОЕ_СЛОВО__ЕСЛИ
    "          KEYWORD__WHILE", // КЛЮЧЕВОЕ_СЛОВО__ПОКА
    "           KEYWORD__GOTO", // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕЙТИ
    "         KEYWORD__SWITCH", // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕКЛЮЧАТЕЛЬ
    "           KEYWORD__CASE", // КЛЮЧЕВОЕ_СЛОВО__КЕЙС
    "        KEYWORD__DEFAULT", // КЛЮЧЕВОЕ_СЛОВО__ПО_УМОЛЧАНИЮ
    "         KEYWORD__RETURN", // КЛЮЧЕВОЕ_СЛОВО__ВЕРНУТЬ
    //
    "           KEYWORD__ELSE", // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ (Опционально)
    //"     KEYWORD__ELSE_IF", // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ_ЕСЛИ (Опционально)

    // Операторы
    "      OPERATOR__ADDITION", // СЛОЖЕНИЕ             ·     +
    "      OPERATOR__SUBTRACT", // ВЫЧИТАНИЕ            ·     -
    "OPERATOR__MULTIPLICATION", // УМНОЖЕНИЕ            ·     *
    "      OPERATOR__DIVISION", // ДЕЛЕНИЕ              ·     /
    "     OPERATOR__INVERSION", // ИНВЕРСИЯ             ·     !
    "      OPERATOR__EQUALITY", // РАВЕНСТВО            ·     ==
    "    OPERATOR__INEQUALITY", // НЕРАВЕНСТВО          ·     !=
    "         OPERATOR__BELOW", // МЕНЬШЕ               ·     <
    "         OPERATOR__ABOVE", // БОЛЬШЕ               ·     >
    "   OPERATOR__BELOW_EQUAL", // МЕНЬШЕ ИЛИ РАВНО     ·     <=
    "   OPERATOR__ABOVE_EQUAL", // БОЛЬШЕ ИЛИ РАВНО     ·     >=

    "                 UNKNOWN", // НЕИЗВЕСТНЫЙ
    "                     EOF", // КОНЕЦ
    "                   ERROR"  // ОШИБКА
};
char token__lexeme[][64+1] =
{
    "'0'~'9'",                            // ЧИСЛОВОЙ ЛИТЕРАЛ
    "\"\"",                               // СТРОКОВЫЙ ЛИТЕРАЛ
    "'='",                                // ЛЕВОСТОРОННЕЕ ПРИСВАИВАНИЕ
    "'A'~'Z', 'a'~'z', '_', '0'~'9'",     // ИДЕНТИФИКАТОР
    "'A'~'Z', 'a'~'z', '_', '0'~'9' ':'", // ИДЕНТИФИКАТОР МЕТКИ
    "':'",                                // КОНЕЦ МЕТКИ

    "' '", // РАЗДЕЛИТЕЛЬ ПРОСТРАНСТВА
    "';'", // КОНЕЦ ЗАЯВЛЕНИЯ

    "'{'", "'}'",
    "'['", "']'",
    "'('", "')'",

    "\"if\"",      // КЛЮЧЕВОЕ_СЛОВО__ЕСЛИ
    "\"while\"",   // КЛЮЧЕВОЕ_СЛОВО__ПОКА
    "\"goto\"",    // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕЙТИ
    "\"switch\"",  // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕКЛЮЧАТЕЛЬ
    "\"case\"",    // КЛЮЧЕВОЕ_СЛОВО__КЕЙС
    "\"default\"", // КЛЮЧЕВОЕ_СЛОВО__ПО_УМОЛЧАНИЮ
    "\"return\"",  // КЛЮЧЕВОЕ_СЛОВО__ВЕРНУТЬ

    "\"else\"",    // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ (Опционально)

    // Операторы
    "'+'",    // СЛОЖЕНИЕ             ·     +
    "'-'",    // ВЫЧИТАНИЕ            ·     -
    "'*'",    // УМНОЖЕНИЕ            ·     *
    "'/'",    // ДЕЛЕНИЕ              ·     /
    "'!'",    // ИНВЕРСИЯ             ·     !
    "\"==\"", // РАВЕНСТВО            ·     ==
    "\"!=\"", // НЕРАВЕНСТВО          ·     !=
    "'<'",    // МЕНЬШЕ               ·     <
    "'>'",    // БОЛЬШЕ               ·     >
    "\"<=\"", // МЕНЬШЕ ИЛИ РАВНО     ·     <=
    "\">=\"", // БОЛЬШЕ ИЛИ РАВНО     ·     >=

    "'\\?'",   // НЕИЗВЕСТНЫЙ
    "'\\0'",   // КОНЕЦ
    "'\\/!\\'" // ОШИБКА
};
// Массив ниже заполняется оп-кодами, которые затем могут быть выполнены интерпретатором
// Если unsigned, то макс. размер кода 0x100=256 байт? Если же signed, то ???
// Пока 8-bit's архитектурная модель (возможно с расширениями через сегментацию, но это в дальнейшем, сейчас же для теста достаточно и этого)
#define MACRO__MAXIMUM_CODE_LIMIT (1 << 16) // 65'536
char opcodes[MACRO__MAXIMUM_CODE_LIMIT] = {[0 ... MACRO__MAXIMUM_CODE_LIMIT-1] = 0x79}; // В таком массиве могут хранится как знаковые, так и беззнаковые оп-коды?
char * ptr__opcodes = opcodes; // Указатель, который будет двигаться по массиву и по мере надобности (необходимости) заполнять его
//
//#define MACRO__VIRTUAL_ADDRESS (cs8 << 8) + ip8 // максимально допустимая при двух 8-ми битных регистрах
//#define MACRO__VIRTUAL_ADDRESS (cs16 << 8) + ip16 // максимально допустимая при двух 16-ти битных регистрах
unsigned char cs8 = 0;   //  8-bit's сегментный-регистр
unsigned char ip8 = 0;   //  8-bit's регистр-указатель на инструкцию
char r8 = 0;
unsigned char cs16 = 0;  // 16-bit's сегментный-регистр
unsigned short ip16 = 0; // 16-bit's регистр-указатель на инструкцию
short r16 = 0;
//
/// Для экспериментов ///
char rcv8 = 0;
short rcv16 = 0;
char src8 = 0;
short src16 = 0;
//
void Performer_VM() // Spin / Executor
{
    switch_run:
    switch (*ptr__opcodes){
    //
    case 0x01: // 16-bit's addressation, rcv = i8 + i8; | add rcv, i8 i8 · add i8 i8, rcv ; сложение / AT&T-specification (Right-associativity), результат в 16-bit's приёмник
    {
        rcv16 = *(++ptr__opcodes) + *(++ptr__opcodes);
        ++ptr__opcodes;
        goto switch_run;
    }
    case 0x79: printf("\n Stopped.."); return;
    default:
    {
        // ... //
    }
    //
    }
}
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
struct Token { short type_identifier; /*type_name[64+1];*/ char lexeme[64+1]; /*char pos_lexeme[2]*/ } __token, __tokens[MACRO__MAXIMUM_TOKEN_LIMIT]; // global variable struct and global array struct
//struct Lexer { int s_pos; int e_pos; char * cursor; } lexer = {0, 0, '\0'}; // global object's
//struct Parser { char * cursor; } parser; // global object's
//
//void LexicalAnalysisWithoutSynthesis(){} // Лексический анализ без синтеза (сканирует/проверяет на наличие ошибок, ничего не воспроизводит), распознающий компонент/модуль лексера
//void LexicalAnalysisWithSynthesis(){}    // Лексический анализ с синтезом (сканирует/проверяет на наличие ошибок и воспроизводит токены), порождающий компонент/модуль лексера
//
const char * ptr_code;
void init_lexer(const char * code) { ptr_code = code; }
//
void error(const char * msg) { printf(msg); }
//
/// Для поточного режима лексера ///
short GetNextToken()
{
    switch_run:
    switch (*ptr_code){
    case '\0':
    {
        __token.type_identifier = TOKEN__EOF;
        return TOKEN__EOF;
    }
    case ' ': case '\t': case '\v': case '\n': case '\r': case '\f':
    {
        ptr_code++;
        goto switch_run;
    }
    case '!':
    {
        __token.lexeme[0] = '!';
        ptr_code++;
        if (*ptr_code == '=')
        {
            __token.lexeme[1] = '=';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__INEQUALITY_OPERATOR;
            ptr_code++;
            return TOKEN__INEQUALITY_OPERATOR;
        }
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__INVERSION_OPERATOR;
        return TOKEN__INVERSION_OPERATOR;
    }
    case '\"':
    {
        short i = -1;
        __token.lexeme[++i] = '\"';
        ptr_code++;
        while (*ptr_code && *ptr_code != '\"')
        {
            __token.lexeme[++i] = *ptr_code;
            ptr_code++;
        }
        if (*ptr_code == '\"')
        {
            __token.lexeme[++i] = '\"';
            ptr_code++;
            __token.lexeme[++i] = '\0';
            __token.type_identifier = TOKEN__STRING_LITERAL;
            return TOKEN__STRING_LITERAL;
        }
        return TOKEN__UNKNOWN; // или TOKEN__ERROR?
    }
    case '(':
    {
        __token.lexeme[0] = '(';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__LEFT_PARENTHESIS;
        ptr_code++;
        return TOKEN__LEFT_PARENTHESIS;
    }
    case ')':
    {
        __token.lexeme[0] = ')';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__RIGHT_PARENTHESIS;
        ptr_code++;
        return TOKEN__RIGHT_PARENTHESIS;
    }
    case '*':
    {
        __token.lexeme[0] = '*';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__MULTIPLICATION_OPERATOR;
        ptr_code++;
        return TOKEN__MULTIPLICATION_OPERATOR;
    }
    case '+':
    {
        __token.lexeme[0] = '+';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__ADDITION_OPERATOR;
        ptr_code++;
        return TOKEN__ADDITION_OPERATOR;
    }
    case '-':
    {
        __token.lexeme[0] = '-';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__SUBTRACT_OPERATOR;
        ptr_code++;
        return TOKEN__SUBTRACT_OPERATOR;
    }
    case '/':
    {
        ptr_code++;
        if (*ptr_code == '/')
        {
            while (*ptr_code && *ptr_code != '\n') ptr_code++;
            if (*ptr_code == '\n') ptr_code++;
            goto switch_run;
        }
        else if (*ptr_code == '*')
        {
            static short comment_nesting = 0;
            comment_nesting++;
            ptr_code++;
            while (*ptr_code && *(ptr_code+1)) // && !(*ptr_code == '*' && *(ptr_code+1) == '/')
            {
                if (*ptr_code == '/' && *(ptr_code+1) == '*')
                {
                    comment_nesting++;
                    ptr_code += 2;
                    continue;
                }
                if (*ptr_code == '*' && *(ptr_code+1) == '/')
                {
                    comment_nesting--;
                    ptr_code += 2;
                    if (comment_nesting == 0) break;
                    continue;
                }
                ptr_code++;
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
        ptr_code++;
        return TOKEN__END_OF_LABEL;
    }
    case ';':
    {
        __token.lexeme[0] = ';';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__END_OF_STATEMENT;
        ptr_code++;
        return TOKEN__END_OF_STATEMENT;
    }
    case '<':
    {
        __token.lexeme[0] = '<';
        ptr_code++;
        if (*ptr_code == '=')
        {
            __token.lexeme[1] = '=';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__BELOW_EQUAL_OPERATOR;
            ptr_code++;
            return TOKEN__BELOW_EQUAL_OPERATOR;
        }
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__BELOW_OPERATOR;
        return TOKEN__BELOW_OPERATOR;
    }
    case '=':
    {
        __token.lexeme[0] = '=';
        ptr_code++;
        if (*ptr_code == '=')
        {
            __token.lexeme[1] = '=';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__EQUALITY_OPERATOR;
            ptr_code++;
            return TOKEN__EQUALITY_OPERATOR;
        }
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__LEFT_SIDED_ASSIGNMENT;
        return TOKEN__LEFT_SIDED_ASSIGNMENT;
    }
    case '>':
    {
        __token.lexeme[0] = '>';
        ptr_code++;
        if (*ptr_code == '=')
        {
            __token.lexeme[1] = '=';
            __token.lexeme[2] = '\0';
            __token.type_identifier = TOKEN__ABOVE_EQUAL_OPERATOR;
            ptr_code++;
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
        ptr_code++;
        return TOKEN__LEFT_SQUARE_BRACKET;
    }
    case ']':
    {
        __token.lexeme[0] = ']';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__RIGHT_SQUARE_BRACKET;
        ptr_code++;
        return TOKEN__RIGHT_SQUARE_BRACKET;
    }
    case '{':
    {
        __token.lexeme[0] = '{';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__LEFT_BRACE;
        ptr_code++;
        return TOKEN__LEFT_BRACE;
    }
    case '}':
    {
        __token.lexeme[0] = '}';
        __token.lexeme[1] = '\0';
        __token.type_identifier = TOKEN__RIGHT_BRACE;
        ptr_code++;
        return TOKEN__RIGHT_BRACE;
    }
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
    {
        short i = -1;
        __token.lexeme[++i] = *ptr_code;
        ptr_code++;
        while (isdigit(*ptr_code))
        {
            __token.lexeme[++i] = *ptr_code;
            ptr_code++;
        }
        __token.lexeme[++i] = '\0';
        __token.type_identifier = TOKEN__NUMERIC_LITERAL;
        return TOKEN__NUMERIC_LITERAL;
    }
    default:
    {
        //printf("\n def 1");
        if (isalpha(*ptr_code) || *ptr_code == '_') // Первым символом не может быть цифра
        {
            // Пока не знаем что за токен: это может быть либо идентификатор, либо идентификатор метки
            short i = -1;
            __token.lexeme[++i] = *ptr_code;
            ptr_code++;
            while (isalnum(*ptr_code) || *ptr_code == '_')
            {
                __token.lexeme[++i] = *ptr_code;
                ptr_code++;
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
            /*
            if (*ptr_code == ':')
            {
                //__token.lexeme[++i] = '\0';
                //__token.lexeme[++i] = ':';
                __token.type_identifier = TOKEN__LABEL_IDENTIFIER;
                ptr_code++;
                return TOKEN__LABEL_IDENTIFIER;
            }
            */
            __token.type_identifier = TOKEN__IDENTIFIER;
            return TOKEN__IDENTIFIER;
        }
        __token.type_identifier = TOKEN__UNKNOWN;
        __token.lexeme[0] = *ptr_code;
        ptr_code++;
        return TOKEN__UNKNOWN;
    }}
    //printf("\n def 2");
    return TOKEN__ERROR;
}
//
/// Накопить токены (удобен для отладочного режима, занимает память) ///
short AccumulateTokens()
{
    switch_run:
    switch (*ptr_code){
    case '\0':
    {
        __tokens[++number_of_tokens].type_identifier = TOKEN__EOF;
        return TOKEN__EOF;
    }
    case ' ': case '\t': case '\v': case '\n': case '\r': case '\f':
    {
        ptr_code++;
        goto switch_run;
    }
    case '!':
    {
        __tokens[++number_of_tokens].lexeme[0] = '!';
        ptr_code++;
        if (*ptr_code == '=')
        {
            __tokens[number_of_tokens].lexeme[1] = '=';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__INEQUALITY_OPERATOR;
            ptr_code++;
            return TOKEN__INEQUALITY_OPERATOR;
        }
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__INVERSION_OPERATOR;
        return TOKEN__INVERSION_OPERATOR;
    }
    case '\"':
    {
        short i = -1;
        __tokens[++number_of_tokens].lexeme[++i] = '\"';
        ptr_code++;
        while (*ptr_code && *ptr_code != '\"')
        {
            __tokens[number_of_tokens].lexeme[++i] = *ptr_code;
            ptr_code++;
        }
        if (*ptr_code == '\"')
        {
            __tokens[number_of_tokens].lexeme[++i] = '\"';
            __tokens[number_of_tokens].lexeme[++i] = '\0';
            ptr_code++;
            __tokens[number_of_tokens].type_identifier = TOKEN__STRING_LITERAL;
            return TOKEN__STRING_LITERAL;
        }
        return TOKEN__UNKNOWN; // или TOKEN__ERROR?
    }
    case '(':
    {
        __tokens[++number_of_tokens].lexeme[0] = '(';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__LEFT_PARENTHESIS;
        ptr_code++;
        return TOKEN__LEFT_PARENTHESIS;
    }
    case ')':
    {
        __tokens[++number_of_tokens].lexeme[0] = ')';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__RIGHT_PARENTHESIS;
        ptr_code++;
        return TOKEN__RIGHT_PARENTHESIS;
    }
    case '*':
    {
        __tokens[++number_of_tokens].lexeme[0] = '*';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__MULTIPLICATION_OPERATOR;
        ptr_code++;
        return TOKEN__MULTIPLICATION_OPERATOR;
    }
    case '+':
    {
        __tokens[++number_of_tokens].lexeme[0] = '+';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__ADDITION_OPERATOR;
        ptr_code++;
        return TOKEN__ADDITION_OPERATOR;
    }
    case '-':
    {
        __tokens[++number_of_tokens].lexeme[0] = '-';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__SUBTRACT_OPERATOR;
        ptr_code++;
        return TOKEN__SUBTRACT_OPERATOR;
    }
    case '/':
    {
        ptr_code++;
        if (*ptr_code == '/')
        {
            while (*ptr_code && *ptr_code != '\n') ptr_code++;
            if (*ptr_code == '\n') ptr_code++;
            goto switch_run;
        }
        else if (*ptr_code == '*')
        {
            static short comment_nesting = 0;
            comment_nesting++;
            ptr_code++;
            while (*ptr_code && *(ptr_code+1)) // && !(*ptr_code == '*' && *(ptr_code+1) == '/')
            {
                if (*ptr_code == '/' && *(ptr_code+1) == '*')
                {
                    comment_nesting++;
                    ptr_code += 2;
                    continue;
                }
                if (*ptr_code == '*' && *(ptr_code+1) == '/')
                {
                    comment_nesting--;
                    ptr_code += 2;
                    if (comment_nesting == 0) break;
                    continue;
                }
                ptr_code++;
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
        ptr_code++;
        return TOKEN__END_OF_LABEL;
    }
    case ';':
    {
        __tokens[++number_of_tokens].lexeme[0] = ';';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__END_OF_STATEMENT;
        ptr_code++;
        return TOKEN__END_OF_STATEMENT;
    }
    case '<':
    {
        __tokens[++number_of_tokens].lexeme[0] = '<';
        ptr_code++;
        if (*ptr_code == '=')
        {
            __tokens[number_of_tokens].lexeme[1] = '=';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__BELOW_EQUAL_OPERATOR;
            ptr_code++;
            return TOKEN__BELOW_EQUAL_OPERATOR;
        }
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__BELOW_OPERATOR;
        return TOKEN__BELOW_OPERATOR;
    }
    case '=':
    {
        __tokens[++number_of_tokens].lexeme[0] = '=';
        ptr_code++;
        if (*ptr_code == '=')
        {
            __tokens[number_of_tokens].lexeme[1] = '=';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__EQUALITY_OPERATOR;
            ptr_code++;
            return TOKEN__EQUALITY_OPERATOR;
        }
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__LEFT_SIDED_ASSIGNMENT;
        return TOKEN__LEFT_SIDED_ASSIGNMENT;
    }
    case '>':
    {
        __tokens[++number_of_tokens].lexeme[0] = '>';
        ptr_code++;
        if (*ptr_code == '=')
        {
            __tokens[number_of_tokens].lexeme[1] = '=';
            __tokens[number_of_tokens].lexeme[2] = '\0';
            __tokens[number_of_tokens].type_identifier = TOKEN__ABOVE_EQUAL_OPERATOR;
            ptr_code++;
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
        ptr_code++;
        return TOKEN__LEFT_SQUARE_BRACKET;
    }
    case ']':
    {
        __tokens[++number_of_tokens].lexeme[0] = ']';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__RIGHT_SQUARE_BRACKET;
        ptr_code++;
        return TOKEN__RIGHT_SQUARE_BRACKET;
    }
    case '{':
    {
        __tokens[++number_of_tokens].lexeme[0] = '{';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__LEFT_BRACE;
        ptr_code++;
        return TOKEN__LEFT_BRACE;
    }
    case '}':
    {
        __tokens[++number_of_tokens].lexeme[0] = '}';
        __tokens[number_of_tokens].lexeme[1] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__RIGHT_BRACE;
        ptr_code++;
        return TOKEN__RIGHT_BRACE;
    }
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
    {
        short i = -1;
        __tokens[++number_of_tokens].lexeme[++i] = *ptr_code;
        ptr_code++;
        while (isdigit(*ptr_code))
        {
            __tokens[number_of_tokens].lexeme[++i] = *ptr_code;
            ptr_code++;
        }
        __tokens[number_of_tokens].lexeme[++i] = '\0';
        __tokens[number_of_tokens].type_identifier = TOKEN__NUMERIC_LITERAL;
        return TOKEN__NUMERIC_LITERAL;
    }
    default:
    {
        //printf("\n def 1");
        if (isalpha(*ptr_code) || *ptr_code == '_') // Первым символом не может быть цифра
        {
            // Пока не знаем что за токен: это может быть либо идентификатор, либо идентификатор метки
            short i = -1;
            __tokens[++number_of_tokens].lexeme[++i] = *ptr_code;
            ptr_code++;
            while (isalnum(*ptr_code) || *ptr_code == '_')
            {
                __tokens[number_of_tokens].lexeme[++i] = *ptr_code;
                ptr_code++;
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
            /*
            if (*ptr_code == ':')
            {
                //__tokens[number_of_tokens].lexeme[++i] = '\0';
                __tokens[number_of_tokens].type_identifier = TOKEN__LABEL_IDENTIFIER;
                //__tokens[number_of_tokens].lexeme[++i] = ':';
                ptr_code++;
                return TOKEN__LABEL_IDENTIFIER;
            }
            */
            __tokens[number_of_tokens].type_identifier = TOKEN__IDENTIFIER;
            return TOKEN__IDENTIFIER;
        }
        __tokens[++number_of_tokens].type_identifier = TOKEN__UNKNOWN;
        __tokens[number_of_tokens].lexeme[0] = *ptr_code;
        ptr_code++;
        return TOKEN__UNKNOWN;
    }}
    //printf("\n def 2");
    return TOKEN__ERROR;
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
void Parse__Expression()
{
    switch (__tokens[current_token].type_identifier){
    //- Первый операнд -/
    case TOKEN__NUMERIC_LITERAL:
    case TOKEN__IDENTIFIER:
    case TOKEN__STRING_LITERAL:
    {
        current_token++;
        while (is_binary_operator(__tokens[current_token].type_identifier))
        {
            current_token++;
            switch (__tokens[current_token].type_identifier){
            //- Второй операнд -/
            case TOKEN__NUMERIC_LITERAL:
            case TOKEN__IDENTIFIER:
            case TOKEN__STRING_LITERAL:
            {
                current_token++;
                break;
            }
            default:
            {
                printf("\n #Error-2!");
                return;
            }}
        }
        break;
    }
    default:
    {
        printf("\n #Error-1!");
        return;
    }}
}
//
void Parse__Assignment()
{
    if (__tokens[current_token].type_identifier != TOKEN__IDENTIFIER)
    {
        printf("\n #Error: Parse__Assignment!");
        return;
    }
    current_token++;
    if (__tokens[current_token].type_identifier != TOKEN__LEFT_SIDED_ASSIGNMENT)
    {
        printf("\n #Error: Parse__Assignment-2!");
        return;
    }
    current_token++;
    Parse__Expression();
    if (__tokens[current_token].type_identifier != TOKEN__END_OF_STATEMENT)
    {
        printf("\n #Error: Parse__Assignment-3!");
        return;
    }
    current_token++;
}
///////////////////////////////////////////
void _$()
{
    setlocale(0, "");
    //
    /*/
    /// Для экспериментов ///
    AddToken("TOKEN__NUMERIC_LITERAL");
    AddToken("TOKEN__LEFT_SIDED_ASSIGNMENT");
    AddToken("TOKEN__IDENTIFIER");
    AddToken("TOKEN__SPACE_SEPARATOR");
    AddToken("TOKEN__END_OF_STATEMENT");
    AddToken("TOKEN__KEYWORD_GOTO");
    AddToken("TOKEN__LABEL_IDENTIFIER");

    AddToken("TOKEN__UNKNOWN");
    AddToken("TOKEN__EOF");
    /*/
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
     " */\n"
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
     " */\n"
     " rq = 3 + 5 - 1 * 2;\n" // Пока парсим только эту строку (часть) кода!
     ; // inline-код для быстрого тестирования (временно)
    printf("\n%s", code);
    init_lexer(code);
    /*
    while ((__token.type_identifier = GetNextToken()) != TOKEN__EOF) // Поточный режим лексера (удобен тем, что не засоряем лишнюю память)
    {
        printf("\n--------------------------+---------------------------------");
        printf("\n %s | %s", token__type_name[__token.type_identifier], __token.lexeme);
    }
    printf("\n--------------------------+---------------------------------");
    */
    while (AccumulateTokens() != TOKEN__EOF)
    {
        printf("\n--------------------------+---------------------------------");
        printf("\n %s | %s", token__type_name[__tokens[number_of_tokens].type_identifier], __tokens[number_of_tokens].lexeme);
    }
    printf("\n-----------------------+------------------------------------");
    /*
    number_of_tokens = -1;
    while (__tokens[++number_of_tokens].type_identifier != TOKEN__EOF)
    {
        printf("\n--------------------------+---------------------------------");
        printf("\n %s | %s", token__type_name[__tokens[number_of_tokens].type_identifier], __tokens[number_of_tokens].lexeme);
    }
    printf("\n-----------------------+------------------------------------");
    */
    Parse__Assignment();
    if (__tokens[current_token].type_identifier == TOKEN__EOF) printf("\n Присваивание разобрано успешно, конец файла.");
    else printf("\n После присваивания остались лишние токены.");

    Performer_VM();
    //
    putchar('\n');
}
//
int main() { _$(); }

/*/-/// DECLARATION ///-/

Бессмысленные конструкции (игнорируются компилятором, предупреждения выдаются)
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