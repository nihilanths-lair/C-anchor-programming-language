// @ Minimum viable product of the compiler is 15% ready / Минимально жизнеспособный продукт компилятора готов на 15%
//
#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
//
enum
{
    // Идентификация токенов для лексера и парсера (лексического/синтаксического анализа и синтеза)
    TOKEN__NUMERIC_LITERAL,       // ЧИСЛОВОЙ_ЛИТЕРАЛ
    TOKEN__LEFT_SIDED_ASSIGNMENT, // ЛЕВОСТОРОННЕЕ_ПРИСВАИВАНИЕ
    TOKEN__IDENTIFIER,            // ИДЕНТИФИКАТОР
    TOKEN__SPACE_SEPARATOR,       // РАЗДЕЛИТЕЛЬ_ПРОСТРАНСТВА
    TOKEN__END_OF_STATEMENT,      // КОНЕЦ_ЗАЯВЛЕНИЯ
    TOKEN__KEYWORD_GOTO,          // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕЙТИ
    TOKEN__LABEL_IDENTIFIER,      // ИДЕНТИФИКАТОР_МЕТКИ

    // ... //

    TOKEN__UNKNOWN, // НЕИЗВЕСТНЫЙ
    TOKEN__EOF,     // КОНЕЦ
    TOKEN__ERROR    // ОШИБКА
};
//
short number_of_tokens = -1;
char token__type_identifier[128];
char token__type_name[][64+1] =
{
    "NUMERIC_LITERAL",       // ЧИСЛОВОЙ_ЛИТЕРАЛ
    "LEFT_SIDED_ASSIGNMENT", // ЛЕВОСТОРОННЕЕ_ПРИСВАИВАНИЕ
    "IDENTIFIER",            // ИДЕНТИФИКАТОР
    "SPACE_SEPARATOR",       // РАЗДЕЛИТЕЛЬ_ПРОСТРАНСТВА
    "END_OF_STATEMENT",      // КОНЕЦ_ЗАЯВЛЕНИЯ
    "KEYWORD__GOTO",         // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕЙТИ
    "LABEL_IDENTIFIER",      // ИДЕНТИФИКАТОР_МЕТКИ

    // ... //

    "UNKNOWN", // НЕИЗВЕСТНЫЙ
    "EOF",     // КОНЕЦ
    "ERROR"    // ОШИБКА
};
char token__lexeme[][64+1] =
{
    "'0'~'9'",
    "'='",
    "'A'~'Z', 'a'~'z'",
    "' '",
    "';'",
    "\"goto\"",
    "':'",

    "'\\?'",
    "'\\0'",
    // ... //
};
/*
char * GetTypeToken(short idx)
{
    char * ptr_type_token = &get_type_token[idx+1];
    return ptr_type_token;
}
*/
//struct Token {} token; // global object's
//
void AddToken(const char * token_type)
{
    static short idx = 0;
    token__type_identifier[idx] = idx;
    strcpy(token__type_name[idx], token_type);
    idx++;
}
//
struct Token { short type_identifier; /*type_name[64+1];*/ char lexeme[64+1]; } token[1500]; // global object's: на первых порах макс. лимит - 1500 токенов
struct Lexer { int s_pos; int e_pos; char * cursor; } lexer = {0, 0, '\0'}; // global object's
struct Parser { char * cursor; } parser; // global object's
//
//void LexicalAnalysisWithoutSynthesis(){} // Лексический анализ без синтеза (сканирует/проверяет на наличие ошибок, ничего не воспроизводит), распознающий компонент/модуль лексера
//void LexicalAnalysisWithSynthesis(){}    // Лексический анализ с синтезом (сканирует/проверяет на наличие ошибок и воспроизводит токены), порождающий компонент/модуль лексера
//
char * ptr_code;
//
void init_lexer(char * code) { ptr_code = code; }
//
short get_token()
{
    while (isspace(*ptr_code)) ptr_code++; // Пропусĸаем пробелы

    switch (*ptr_code){
    case '\0':
        token[++number_of_tokens].type_identifier = TOKEN__EOF;
        return TOKEN__EOF;

    case '=':
        token[++number_of_tokens].type_identifier = TOKEN__LEFT_SIDED_ASSIGNMENT;
        strcpy(token[number_of_tokens].lexeme, "=");
        ptr_code++;
        return TOKEN__LEFT_SIDED_ASSIGNMENT;

    case ';':
        token[++number_of_tokens].type_identifier = TOKEN__END_OF_STATEMENT;
        strcpy(token[number_of_tokens].lexeme, ";");
        ptr_code++;
        return TOKEN__END_OF_STATEMENT;

    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        short i = -1;
        token[++number_of_tokens].lexeme[++i] = *ptr_code;
        ptr_code++;
        while (isdigit(*ptr_code))
        {
            token[number_of_tokens].lexeme[++i] = *ptr_code;
            ptr_code++;
        }
        token[number_of_tokens].lexeme[++i] = '\0';
        token[number_of_tokens].type_identifier = TOKEN__NUMERIC_LITERAL;
        return TOKEN__NUMERIC_LITERAL;

    default:
        //printf("\n def 1");
        if (isalpha(*ptr_code) || *ptr_code == '_') // Первым символом не может быть цифра
        {
            // Пока не знаем что за токен: это может быть либо идентификатор, либо идентификатор метки
            short i = -1;
            token[++number_of_tokens].lexeme[++i] = *ptr_code;
            ptr_code++;
            while (isalnum(*ptr_code) || *ptr_code == '_')
            {
                token[number_of_tokens].lexeme[++i] = *ptr_code;
                ptr_code++;
            }
            token[number_of_tokens].lexeme[++i] = '\0';
            // Идентификатор из букв/цифр и нижнего подчёркивания собран
            if (!strcmp(token[number_of_tokens].lexeme, "goto"))
            {
                token[number_of_tokens].type_identifier = TOKEN__KEYWORD_GOTO;
                return TOKEN__KEYWORD_GOTO;
            }
            /// Если будут другие ключевые слова, добавляем проверки ///
            if (*ptr_code == ':')
            {
                token[number_of_tokens].type_identifier = TOKEN__LABEL_IDENTIFIER;
                token[number_of_tokens].lexeme[++i] = ':';
                token[number_of_tokens].lexeme[++i] = '\0';
                ptr_code++;
                return TOKEN__LABEL_IDENTIFIER;
            }
            token[number_of_tokens].type_identifier = TOKEN__IDENTIFIER;
            return TOKEN__IDENTIFIER;
        }
        token[++number_of_tokens].type_identifier = TOKEN__UNKNOWN;
        token[number_of_tokens].lexeme[0] = *ptr_code;
        ptr_code++;
        return TOKEN__UNKNOWN;
    }
    //printf("\n def 2");
    return TOKEN__ERROR;
}
//
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
    char code[] = "_number = 1530,;\n number_2 `= 628;\n xyz = 71"; // inline-код для быстрого тестирования (временно)
    printf("\n %s", code);
    init_lexer(code);
    short token_type_identifier;
    while ((token_type_identifier = get_token()) != TOKEN__EOF){}
    putchar('\n');
    number_of_tokens = -1;
    while (token[++number_of_tokens].type_identifier != TOKEN__EOF)
    {
        printf("\n %s (%s)", token__type_name[token[number_of_tokens].type_identifier], token[number_of_tokens].lexeme);
    }
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