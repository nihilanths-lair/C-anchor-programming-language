#include <stdio.h>
#include <locale.h>
#include <string.h>

enum
{
    TOKEN__ABSENT,
    TOKEN__NUMBER,
    TOKEN__STAR_SIGN,
    TOKEN__PLUS_SIGN,
    TOKEN__MINUS_SIGN,
    TOKEN__SLASH_SIGN,
    TOKEN__INDENTATION,
    TOKEN__ASSIGNMENT, // = , присваивание
    TOKEN__UNKNOWN,

    TOKEN__NEW_LINE              = 10, // '\n', спец. символ новой строки
    TOKEN__INDENT                = 32, // ' ', пробел (он же отступ)
    TOKEN__OPENING_ROUND_BRACKET = 40, // '('
    TOKEN__CLOSING_ROUND_BRACKET     , // ')'
};
const char token_name_table[][40+1] =
{
    "",                      // 00 ABSENT
    "NUMBER",                // 01
    "STAR_SIGN",             // 02
    "PLUS_SIGN",             // 03
    "MINUS_SIGN",            // 04
    "SLASH_SIGN",            // 05
    //"LEFT_ROUND_BRACKET", "RIGHT_ROUND_BRACKET", // () , приоритетность
    "INDENTATION",
    "ASSIGNMENT", // = , присваивание

    "UNKNOWN",

    "?=09",                  // 09
    "NEW_LINE",              // 10 - '\n', спец. символ новой строки
    "?=11",                  // 11
    "?=12",                  // 12
    "?=13",                  // 13
    "?=14",                  // 14
    "?=15",                  // 15
    "?=16",                  // 16
    "?=17",                  // 17
    "?=18",                  // 18
    "?=19",                  // 19
    "?=20",                  // 20
    "?=21",                  // 21
    "?=22",                  // 22
    "?=23",                  // 23
    "?=24",                  // 24
    "?=25",                  // 25
    "?=26",                  // 26
    "?=27",                  // 27
    "?=28",                  // 28
    "?=29",                  // 29
    "?=30",                  // 30
    "?=31",                  // 31
    "INDENT",                // 32 - ' ', пробел (он же отступ)
    "?",                     // 33
    "?",                     // 34
    "?",                     // 35
    "?",                     // 36
    "?",                     // 37
    "?",                     // 38
    "?",                     // 39
    "OPENING_ROUND_BRACKET", // 40 - '('
    "CLOSING_ROUND_BRACKET"  // 41 - ')'
};

//typedef struct LexicalAnalyzer LexicalAnalyzer;
struct LexicalAnalyzer
{
    char stream[0xFF];
    char *ptr__stream;
    int itr__stream;

    int row_position;
    int column_position;
    int binary_position;
    int remember_position;
}
this__lexical_analyzer;

// -[@]- >>> Markup-1 {{ -[@]- //
static inline char GetNextCharacter(){}
static inline char GetPreviousCharacter(){}
// -[@]- }} Markup-1 <<< -[@]- //

#define MAX_TOKENS 0xFF
//typedef struct LexicalSynthesizer LexicalSynthesizer;
unsigned char number_of_tokens = 0;
struct LexicalSynthesizer
{
    // Начальная и конечная позиция в строке
    int token_start;/*[MAX_TOKENS];*/
    int token_end;/*[MAX_TOKENS];*/

    int token_id;/*[MAX_TOKENS];*/

    int token_row_position;/*[MAX_TOKENS];*/
    int token_column_position;/*[MAX_TOKENS];*/
    int token_binary_position;/*[MAX_TOKENS];*/

    unsigned char token_type;/*[MAX_TOKENS];*/
    //char token_name[MAX_TOKENS][40+1];
    char token_value[40+1];/*[MAX_TOKENS][40+1];*/
}
this__lexical_synthesizer[MAX_TOKENS];

struct Lexer
{
    struct Analyzer
    {
        char *data;
    }
    this__analyzer;
    struct Synthesizer
    {

    }
    this__synthesizer;
}
this__lexer;

void AddNewToken(){}

unsigned char token_starting_position = 0;
FILE *file_descriptor;
#define DEBUG
#if defined DEBUG
// Отладка
void Debug(const char *interval)
{
    //fprintf(descriptor, "\n<%s> this__lexical_synthesizer[%d].token_start = %d", interval, number_of_tokens, this__lexical_synthesizer[number_of_tokens].token_start);
    fprintf(file_descriptor, "\n<%s> this__lexical_synthesizer[%d].token_type = \"%s\"", interval,
        number_of_tokens, token_name_table[this__lexical_synthesizer[number_of_tokens].token_type]
    );
    fprintf(file_descriptor, "\n<%s> this__lexical_synthesizer[%d].token_value = \"%s\"\n", interval, number_of_tokens, this__lexical_synthesizer[number_of_tokens].token_value);
    //fprintf(descriptor, "\n<%s> this__lexical_synthesizer[%d].token_end = %d\n", interval, number_of_tokens, this__lexical_synthesizer[number_of_tokens].token_end);
}
#endif
static inline void Number()
{
    #if defined DEBUG
     Debug("До");
    #endif
    this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
    this__lexical_synthesizer[number_of_tokens].token_value[0] = *this__lexical_analyzer.ptr__stream;

    this__lexical_analyzer.column_position ++;
    this__lexical_analyzer.binary_position ++;
    this__lexical_analyzer.ptr__stream ++;
    
    static unsigned char i;
    i = 1;
    // Допустима цифра в диапазоне от 0 до 9
    while (*this__lexical_analyzer.ptr__stream >= '0' && *this__lexical_analyzer.ptr__stream <= '9')
    {
        this__lexical_synthesizer[number_of_tokens].token_value[i++] = *this__lexical_analyzer.ptr__stream;

        this__lexical_analyzer.column_position ++;
        this__lexical_analyzer.binary_position ++;
        this__lexical_analyzer.ptr__stream ++;
    }
    this__lexical_synthesizer[number_of_tokens].token_value[i] = '\0';
    this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__NUMBER;
    this__lexical_synthesizer[number_of_tokens].token_end = this__lexical_analyzer.binary_position;
    #if defined DEBUG
     Debug("После");
    #endif
    number_of_tokens ++;
    this__lexical_analyzer.column_position ++;
    this__lexical_analyzer.binary_position ++;
    this__lexical_analyzer.ptr__stream ++;
}
// Лексический анализ с синтезом
void LexicalAnalysisWithSynthesis()
{
    #if defined DEBUG
    file_descriptor = fopen("file_output.txt", "w");
    //if (file_descriptor == NULL) perror("Код возврата: 2.");

    fprintf(file_descriptor, "\nВызов функции: LexicalAnalysisWithSynthesis(\"%s\")\n", this__lexical_analyzer.stream);
    #endif

    this__lexical_analyzer.ptr__stream = this__lexical_analyzer.stream;
    unsigned char loop = 1;
    while (loop)
    {
        switch (*this__lexical_analyzer.ptr__stream)
        {
            case '\0'://0
            {
                loop = 0;
                break;
            }
            case '\n'://10
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__NEW_LINE;
                strcpy(this__lexical_synthesizer[number_of_tokens].token_value, "\n");
                this__lexical_synthesizer[number_of_tokens].token_end = ++this__lexical_analyzer.binary_position;
                #if defined DEBUG
                 Debug("После");
                #endif
                number_of_tokens ++;
                this__lexical_analyzer.row_position ++;
                this__lexical_analyzer.column_position = 0;
                this__lexical_analyzer.ptr__stream ++;
                break;
            }
            case '\r'://13
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.binary_position ++;
                this__lexical_analyzer.ptr__stream ++;
                #if defined DEBUG
                 Debug("После");
                #endif
                break;
            }
            case ' '://32
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__INDENT;
                strcpy(this__lexical_synthesizer[number_of_tokens].token_value, " ");
                this__lexical_synthesizer[number_of_tokens].token_end = ++this__lexical_analyzer.binary_position;
                #if defined DEBUG
                 Debug("После");
                #endif
                number_of_tokens ++;
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.ptr__stream ++;
                break;
            }
            case '('://40
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__OPENING_ROUND_BRACKET;
                strcpy(this__lexical_synthesizer[number_of_tokens].token_value, "(");
                this__lexical_synthesizer[number_of_tokens].token_end = ++this__lexical_analyzer.binary_position;
                #if defined DEBUG
                 Debug("После");
                #endif
                number_of_tokens ++;
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.ptr__stream ++;
                break;
            }
            case ')'://41
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__CLOSING_ROUND_BRACKET;
                strcpy(this__lexical_synthesizer[number_of_tokens].token_value, ")");
                this__lexical_synthesizer[number_of_tokens].token_end = ++this__lexical_analyzer.binary_position;
                #if defined DEBUG
                 Debug("После");
                #endif
                number_of_tokens ++;
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.ptr__stream ++;
                break;
            }
            case '*'://42
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__STAR_SIGN;
                strcpy(this__lexical_synthesizer[number_of_tokens].token_value, "*");
                this__lexical_synthesizer[number_of_tokens].token_end = ++this__lexical_analyzer.binary_position;
                #if defined DEBUG
                 Debug("После");
                #endif
                number_of_tokens ++;
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.ptr__stream ++;
                break;
            }
            case '+'://43
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__PLUS_SIGN;
                strcpy(this__lexical_synthesizer[number_of_tokens].token_value, "+");
                this__lexical_synthesizer[number_of_tokens].token_end = ++this__lexical_analyzer.binary_position;
                #if defined DEBUG
                 Debug("После");
                #endif
                number_of_tokens ++;
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.ptr__stream ++;
                break;
            }
            case '-'://45
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__MINUS_SIGN;
                strcpy(this__lexical_synthesizer[number_of_tokens].token_value, "-");
                this__lexical_synthesizer[number_of_tokens].token_end = ++this__lexical_analyzer.binary_position;
                #if defined DEBUG
                 Debug("После");
                #endif
                number_of_tokens ++;
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.ptr__stream ++;
                break;
            }
            case '/'://47
            {
                #if defined DEBUG
                 Debug("До");
                #endif
                this__lexical_synthesizer[number_of_tokens].token_start = this__lexical_analyzer.binary_position;
                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN__SLASH_SIGN;
                strcpy(this__lexical_synthesizer[number_of_tokens].token_value, "/");
                this__lexical_synthesizer[number_of_tokens].token_end = ++this__lexical_analyzer.binary_position;
                #if defined DEBUG
                 Debug("После");
                #endif
                number_of_tokens ++;
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.ptr__stream ++;
                break;
            }
            case '0': Number(); break;
            case '1': Number(); break;
            case '2': Number(); break;
            case '3': Number(); break;
            case '4': Number(); break;
            case '5': Number(); break;
            case '6': Number(); break;
            case '7': Number(); break;
            case '8': Number(); break;
            case '9': Number(); break;
            /*
            case '='://61
            {
                printf("\nЗнак << '%c'", *input);
                break;
            }
            */
            default:
            {
                //else if (*input >= 'A' && *input <= 'Z' || *input == '_' || *input >= 'a' && *input <= 'z') {} // ^IDENT позже
                // Нераспознанная лексема
                fprintf(file_descriptor, "\
                    \nLexical analysis error/Ошибка лексического анализа: row/строка - %d, column/столбец - %d, binary/бинарная - %d.",
                    this__lexical_analyzer.row_position,
                    this__lexical_analyzer.column_position,
                    this__lexical_analyzer.binary_position
                );
                fprintf(file_descriptor, "\nInvalid symbol/Недопустимый символ: %c.", *this__lexical_analyzer.ptr__stream ++);
                fprintf(file_descriptor, "\n                                    ^");
                this__lexical_analyzer.column_position ++;
                this__lexical_analyzer.binary_position ++;
                this__lexical_analyzer.ptr__stream ++;
            }
        }
    }
    #if defined DEBUG
    fprintf(file_descriptor, "\nВозврат из функции: LexicalAnalysisWithSynthesis");
    fclose(file_descriptor);
    #endif
}

//typedef struct SyntaxAnalyzer SyntaxAnalyzer;
struct SyntaxAnalyzer {};
// Синтаксический анализатор
void SyntaxAnalyzer()
{
    printf("\nВызов функции: SyntaxAnalyzer()");
    printf("\nВозврат из функции: SyntaxAnalyzer");
}
//typedef struct SyntaxSynthesizer SyntaxSynthesizer;
struct SyntaxSynthesizer {};
// Синтаксический синтезатор
void SyntaxSynthesizer()
{
    printf("\nВызов функции: SyntaxSynthesizer()");
    printf("\nВозврат из функции: SyntaxSynthesizer");
}

int main()
{
    setlocale(0, "");

    file_descriptor = fopen("_.txt", "rb");
    if (file_descriptor == NULL) perror("Код возврата: 1.");

    size_t len = fread(this__lexical_analyzer.stream, 1, sizeof (this__lexical_analyzer.stream)-1, file_descriptor);
    fclose(file_descriptor);

    this__lexical_analyzer.stream[len] = '\0';

    this__lexical_analyzer.row_position = 1;
    this__lexical_analyzer.column_position = 1;
    this__lexical_analyzer.binary_position = 0;

    LexicalAnalysisWithSynthesis(this__lexical_analyzer.stream);
    
    putchar('\n');

    struct SyntaxAnalyzer syntax_analyzer;
    //InitSyntaxAnalizator(&syntax_analyzer);
    SyntaxAnalyzer(this__lexical_analyzer.stream);

    putchar('\n');

    struct SyntaxSynthesizer syntax_synthesizer;
    //InitSyntaxSynthesizer(&syntax_synthesizer);
    SyntaxSynthesizer(this__lexical_analyzer.stream);

    putchar('\n');

    // Печать структуры токенов
    for (int i = 0; i < number_of_tokens; i++)
    {
        printf("\nИдентификатор токена: %d", this__lexical_synthesizer[i].token_id);
        printf("\nПозиция токена: [строка: %d, столбец: %d, бинарно: %d]",
            this__lexical_synthesizer[i].token_row_position,
            this__lexical_synthesizer[i].token_column_position,
            this__lexical_synthesizer[i].token_binary_position
        );
        printf("\nТип токена: %s", token_name_table[this__lexical_synthesizer[i].token_type]);
        printf("\nЗначение токена: %s\n", this__lexical_synthesizer[i].token_value);
    }

    //printf("Файловой ввод: %s.", file_input);
    putchar('\n');
    int i = 0;
    do
    {
        switch (this__lexical_analyzer.stream[i])
        {
            case '\0':
                printf("[%d] = '\\0'\n", i);
                break;
            
            case '\t':
                printf("[%d] = '\\t'\n", i);
                break;
            
            case '\n':
                printf("[%d] = '\\n'\n", i);
                break;
            
            case '\r':
                printf("[%d] = '\\r'\n", i);
                break;

            default:
                printf("[%d] = '%c'\n", i, this__lexical_analyzer.stream[i]);
        }
    }
    while (this__lexical_analyzer.stream[i++] != '\0');

    char console_input[0xFF];
    while (1)
    {
        printf("\n>>> ");
        fgets(console_input, sizeof (console_input), stdin);
        console_input[strcspn(console_input, "\n")] = '\0';
        putchar('\n');
        int i = 0;
        do
        {
            switch (console_input[i])
            {
                case '\0':
                    printf("[%d] = '\\0'\n", i);
                    break;
                
                case '\t':
                    printf("[%d] = '\\t'\n", i);
                    break;
                
                case '\n':
                    printf("[%d] = '\\n'\n", i);
                    break;

                case '\r':
                    printf("[%d] = '\\r'\n", i);
                    break;
                
                default:
                    printf("[%d] = '%c'\n", i, console_input[i]);
            }
        }
        while (console_input[i++] != '\0');
    }

    return 0;
}