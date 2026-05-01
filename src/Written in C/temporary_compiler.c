// <! Minimum viable product of the compiler is 5% ready / Минимально жизнеспособный продукт компилятора готов на 5% >
//
#include <stdio.h>
#include <locale.h>
//
enum
{
    // Идентификация токенов для лексера и парсера (лексического/синтаксического анализа и синтеза)
    E__NUMERIC_LITERAL = 1,   // ЧИСЛОВОЙ_ЛИТЕРАЛ
    E__LEFT_SIDED_ASSIGNMENT, // ЛЕВОСТОРОННЕЕ_ПРИСВАИВАНИЕ
    E__IDENTIFIER,            // ИДЕНТИФИКАТОР
    E__SPACE_SEPARATOR,       // РАЗДЕЛИТЕЛЬ_ПРОСТРАНСТВА
    E__END_OF_STATEMENT,      // КОНЕЦ_ЗАЯВЛЕНИЯ
    E__KEYWORD__GOTO,         // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕЙТИ
    // ... //
};
//
struct Token { char type; } token[1500]; // global object's: на первых порах макс. лимит - 1500 токенов
struct Lexer { int s_pos; int e_pos; char * cursor; } lexer = {0, 0, '\0'}; // global object's
struct Parser { char * cursor; } parser; // global object's
//
/*/
\\\
 НКА/NFA
 ДКА/DFA
\\\
/*/
//
void LexicalAnalysisWithoutSynthesis(){} // Лексический анализ без синтеза (сканирует/проверяет на наличие ошибок, ничего не воспроизводит), распознающий компонент/модуль лексера
void LexicalAnalysisWithSynthesis(){}    // Лексический анализ с синтезом (сканирует/проверяет на наличие ошибок и воспроизводит токены), порождающий компонент/модуль лексера
//
void _$()
{
    setlocale(0, "");
    //
    char * code = "x = 15,;"; // inline-код для быстрого тестирования (временно)
    lexer.cursor = code;
    char state_vector = 0; // state vector / вектор состояний
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
}
//
int main() { _$(); } // выделяем/нарезаем токены/группируем единицы (от мелких к крупным)

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