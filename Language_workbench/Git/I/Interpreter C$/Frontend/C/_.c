#include <stdio.h>
#include <locale.h>
#include <string.h>

enum
{
    TOKEN_UNKNOWN,

    TOKEN_NUMBER,
    TOKEN_OP_MUL, TOKEN_OP_ADD, TOKEN_OP_SUB, TOKEN_OP_DIV,
    TOKEN_INDENTATION,
    TOKEN_ASSIGNMENT,

    TOKEN_ABSENT
};
const char token_name_table[][40+1] =
{
    "UNKNOWN",

    "NUMBER",
    "OP_MUL", "OP_ADD", "OP_SUB", "OP_DIV",
    "INDENTATION",
    "ASSIGNMENT"//Присваивание

    "ABSENT"
};

//typedef struct LexicalAnalyzer LexicalAnalyzer;
struct LexicalAnalyzer
{
    int row_position;
    int column_position;
    int binary_position;
}
this__lexical_analyzer;
void InitLexicalAnalyzer(struct LexicalAnalyzer *lexical_analyzer)
{
    //struct LexicalAnalyzer LexicalAnalyzer;

    // Глобальная переменная структуры
    this__lexical_analyzer.row_position = 1;
    this__lexical_analyzer.column_position = 1;
    this__lexical_analyzer.binary_position = 1;
    
    // Локальная переменная структуры (чище и безопаснее)
    lexical_analyzer->row_position = 1;
    lexical_analyzer->column_position = 1;
    lexical_analyzer->binary_position = 1;
}

#define MAX_TOKENS 0xFF
//typedef struct LexicalSynthesizer LexicalSynthesizer;
struct LexicalSynthesizer
{
    int token_id/*[MAX_TOKENS]*/;

    int token_row_position/*[MAX_TOKENS]*/;
    int token_column_position/*[MAX_TOKENS]*/;
    int token_binary_position/*[MAX_TOKENS]*/;

    unsigned char token_type/*[MAX_TOKENS]*/;
    //char token_name[MAX_TOKENS][40+1];
    char token_value/*[MAX_TOKENS]*/[40+1];
}
this__lexical_synthesizer[MAX_TOKENS];
void InitLexicalSynthesizer(struct LexicalSynthesizer *lexical_synthesizer)
{
    //struct LexicalSynthesizer LexicalSynthesizer;

    // Глобальная переменная структуры
    /*
    this__lexical_synthesizer.? = 1;
    
    // Локальная переменная структуры (чище и безопаснее)
    lexical_synthesizer->? = 1;
    */
}

unsigned char number_of_tokens = 0;
unsigned char token_starting_position = 0;

// Лексический анализатор
void LexicalAnalyzer(const char *input)
{
    printf("\nВызов функции: LexicalAnalyzer(\"%s\")\n\n", input);

    unsigned char loop = 1;
    while (loop)
    {
        switch (*input)
        {
            case '\0':
            {
                printf("[%d] = '\\0'\n", token_starting_position);

                this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens;
                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                loop = 0;

                break;
            }
            case '\n':
            {
                this__lexical_analyzer.row_position++;
                this__lexical_analyzer.column_position = 0;
                this__lexical_analyzer.binary_position++;

                break;
            }
            case ' ':
            {
                this__lexical_synthesizer[number_of_tokens].token_row_position = this__lexical_analyzer.row_position;
                this__lexical_synthesizer[number_of_tokens].token_column_position = ++this__lexical_analyzer.column_position;
                this__lexical_synthesizer[number_of_tokens].token_binary_position = ++this__lexical_analyzer.binary_position;

                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;
                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN_INDENTATION;
                this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens++;

                token_starting_position = 0;

                break;
            }
            case '*':
            {
                this__lexical_synthesizer[number_of_tokens].token_row_position = this__lexical_analyzer.row_position;
                this__lexical_synthesizer[number_of_tokens].token_column_position = ++this__lexical_analyzer.column_position;
                this__lexical_synthesizer[number_of_tokens].token_binary_position = ++this__lexical_analyzer.binary_position;

                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;
                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN_OP_MUL;
                this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens++;

                token_starting_position = 0;

                break;
            }
            case '+':
            {
                this__lexical_synthesizer[number_of_tokens].token_row_position = this__lexical_analyzer.row_position;
                this__lexical_synthesizer[number_of_tokens].token_column_position = ++this__lexical_analyzer.column_position;
                this__lexical_synthesizer[number_of_tokens].token_binary_position = ++this__lexical_analyzer.binary_position;

                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;
                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN_OP_ADD;
                this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens++;

                token_starting_position = 0;

                break;
            }
            case '-':
            {
                this__lexical_synthesizer[number_of_tokens].token_row_position = this__lexical_analyzer.row_position;
                this__lexical_synthesizer[number_of_tokens].token_column_position = ++this__lexical_analyzer.column_position;
                this__lexical_synthesizer[number_of_tokens].token_binary_position = ++this__lexical_analyzer.binary_position;

                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;
                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN_OP_SUB;
                this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens++;

                token_starting_position = 0;

                break;
            }
            case '/':
            {
                this__lexical_synthesizer[number_of_tokens].token_row_position = this__lexical_analyzer.row_position;
                this__lexical_synthesizer[number_of_tokens].token_column_position = ++this__lexical_analyzer.column_position;
                this__lexical_synthesizer[number_of_tokens].token_binary_position = ++this__lexical_analyzer.binary_position;

                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;
                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN_OP_DIV;
                this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens++;

                token_starting_position = 0;

                break;
            }
            case '=':
            {
                this__lexical_synthesizer[number_of_tokens].token_row_position = this__lexical_analyzer.row_position;
                this__lexical_synthesizer[number_of_tokens].token_column_position = ++this__lexical_analyzer.column_position;
                this__lexical_synthesizer[number_of_tokens].token_binary_position = ++this__lexical_analyzer.binary_position;

                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;
                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                this__lexical_synthesizer[number_of_tokens].token_type = TOKEN_ASSIGNMENT;
                this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens++;

                token_starting_position = 0;

                break;
            }
            default:
            {
                if (*input >= '0' && *input <= '9')
                {
                    this__lexical_synthesizer[number_of_tokens].token_row_position = this__lexical_analyzer.row_position;
                    this__lexical_synthesizer[number_of_tokens].token_column_position = ++this__lexical_analyzer.column_position;
                    this__lexical_synthesizer[number_of_tokens].token_binary_position = ++this__lexical_analyzer.binary_position;

                    this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;

                    while (loop)
                    {
                        switch (*input)
                        {
                            case '\0':
                            {
                                printf("[%d] = '\\0'\n", token_starting_position);

                                this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens;
                                this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                                loop = 0;
                                break;
                            }
                            default:
                            {
                                if (*input >= '0' && *input <= '9')
                                {
                                    this__lexical_analyzer.column_position++;

                                    this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;
                                }
                                else
                                {
                                    this__lexical_analyzer.column_position++;

                                    this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';
                                    this__lexical_synthesizer[number_of_tokens].token_type = TOKEN_NUMBER;
                                    this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens++;
                                    
                                    token_starting_position = 0;
                                    loop = 0;
                                }
                            }
                        }
                    }
                    // добавить токен тут
                    loop = 1;
                    // добавить токен тут
                }
                else
                {
                    printf("Pik..");
                    this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position++] = *input++;
                    this__lexical_synthesizer[number_of_tokens].token_value[token_starting_position] = '\0';

                    this__lexical_synthesizer[number_of_tokens].token_type = TOKEN_UNKNOWN;
                    this__lexical_synthesizer[number_of_tokens].token_id = number_of_tokens++;
                }
            }
        }
        printf("[%d] = '\\0'\n", token_starting_position);
        input++;
    }

    printf("\nВозврат из функции: LexicalAnalyzer\n");
}
// Лексический синтезатор
void LexicalSynthesizer()
{
    printf("\nВызов функции: LexicalSynthesizer()\n\n");

    printf("\nВозврат из функции: LexicalSynthesizer\n");
}

//typedef struct SyntaxAnalyzer SyntaxAnalyzer;
struct SyntaxAnalyzer
{

};
// Синтаксический анализатор
void SyntaxAnalyzer()
{
    printf("\nВызов функции: SyntaxAnalyzer()\n\n");

    printf("\nВозврат из функции: SyntaxAnalyzer\n");
}
//typedef struct SyntaxSynthesizer SyntaxSynthesizer;
struct SyntaxSynthesizer
{

};
// Синтаксический синтезатор
void SyntaxSynthesizer()
{
    printf("\nВызов функции: SyntaxSynthesizer()\n\n");

    printf("\nВозврат из функции: SyntaxSynthesizer\n");
}

int main()
{
    setlocale(0, "");

    FILE *file_descriptor = fopen("_.txt", "r");
    if (file_descriptor == NULL) perror("Код возврата: 1.");

    char file_input[0xFF];
    size_t len = fread(file_input, 1, sizeof (file_input)-1, file_descriptor);

    fclose(file_descriptor);
    file_input[len] = '\0';

    struct LexicalAnalyzer lexical_analyzer;
    InitLexicalAnalyzer(&lexical_analyzer);
    LexicalAnalyzer(file_input);

    struct LexicalSynthesizer lexical_synthesizer;
    InitLexicalSynthesizer(&lexical_synthesizer);
    LexicalSynthesizer(file_input);

    struct SyntaxAnalyzer syntax_analyzer;
    //InitSyntaxAnalizator(&syntax_analyzer);
    SyntaxAnalyzer(file_input);

    struct SyntaxSynthesizer syntax_synthesizer;
    //InitSyntaxSynthesizer(&syntax_synthesizer);
    SyntaxSynthesizer(file_input);

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
        switch (file_input[i])
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
                printf("[%d] = '%c'\n", i, file_input[i]);
        }
    }
    while (file_input[i++] != '\0');

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