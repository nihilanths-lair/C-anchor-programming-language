#include <stdio.h>
#include <locale.h>
#include <string.h>

//#include "lexer.h"

//typedef struct Lexical_analyzer Lexical_analyzer;
struct Lexical_analyzer
{
    int row_position;
    int column_position;
    int binary_position;
}
this__lexical_analyzer;
void InitLexicalAnalyzer(struct Lexical_analyzer *lexical_analyzer)
{
    //struct Lexical_analyzer lexical_analyzer;

    this__lexical_analyzer.row_position = 1;
    this__lexical_analyzer.column_position = 1;
    this__lexical_analyzer.binary_position = 1;
    
    lexical_analyzer->row_position = 1;
    lexical_analyzer->column_position = 1;
    lexical_analyzer->binary_position = 1;
}

#define MAX_TOKENS 0xFF

unsigned char number_of_tokens = 0;
unsigned char token_starting_position = 0;

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

int token_id[MAX_TOKENS];

int token_row_position[MAX_TOKENS];
int token_column_position[MAX_TOKENS];
int token_binary_position[MAX_TOKENS];

unsigned char token_type[MAX_TOKENS];
//char token_name[MAX_TOKENS][40+1];
char token_value[MAX_TOKENS][40+1];

void LexicalAnalyzer(const char *input)
{
    printf("\nВызов функции: lexical_analyzer(\"%s\")\n\n", input);

    unsigned char loop = 1;
    while (loop)
    {
        switch (*input)
        {
            case '\0':
            {
                printf("[%d] = '\\0'\n", token_starting_position);

                token_id[number_of_tokens] = number_of_tokens;
                token_value[number_of_tokens][token_starting_position] = '\0';

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
                token_row_position[number_of_tokens] = this__lexical_analyzer.row_position;
                token_column_position[number_of_tokens] = ++this__lexical_analyzer.column_position;
                token_binary_position[number_of_tokens] = ++this__lexical_analyzer.binary_position;

                token_value[number_of_tokens][token_starting_position++] = *input++;
                token_value[number_of_tokens][token_starting_position] = '\0';
                token_type[number_of_tokens] = TOKEN_INDENTATION;
                token_id[number_of_tokens] = number_of_tokens++;

                token_starting_position = 0;
                break;
            }
            case '*':
            {
                token_row_position[number_of_tokens] = this__lexical_analyzer.row_position;
                token_column_position[number_of_tokens] = ++this__lexical_analyzer.column_position;
                token_binary_position[number_of_tokens] = ++this__lexical_analyzer.binary_position;

                token_value[number_of_tokens][token_starting_position++] = *input++;
                token_value[number_of_tokens][token_starting_position] = '\0';
                token_type[number_of_tokens] = TOKEN_OP_MUL;
                token_id[number_of_tokens] = number_of_tokens++;

                token_starting_position = 0;
                break;
            }
            case '+':
            {
                token_row_position[number_of_tokens] = this__lexical_analyzer.row_position;
                token_column_position[number_of_tokens] = ++this__lexical_analyzer.column_position;
                token_binary_position[number_of_tokens] = ++this__lexical_analyzer.binary_position;

                token_value[number_of_tokens][token_starting_position++] = *input++;
                token_value[number_of_tokens][token_starting_position] = '\0';
                token_type[number_of_tokens] = TOKEN_OP_ADD;
                token_id[number_of_tokens] = number_of_tokens++;

                token_starting_position = 0;
                break;
            }
            case '-':
            {
                token_row_position[number_of_tokens] = this__lexical_analyzer.row_position;
                token_column_position[number_of_tokens] = ++this__lexical_analyzer.column_position;
                token_binary_position[number_of_tokens] = ++this__lexical_analyzer.binary_position;

                token_value[number_of_tokens][token_starting_position++] = *input++;
                token_value[number_of_tokens][token_starting_position] = '\0';
                token_type[number_of_tokens] = TOKEN_OP_SUB;
                token_id[number_of_tokens] = number_of_tokens++;

                token_starting_position = 0;
                break;
            }
            case '/':
            {
                token_row_position[number_of_tokens] = this__lexical_analyzer.row_position;
                token_column_position[number_of_tokens] = ++this__lexical_analyzer.column_position;
                token_binary_position[number_of_tokens] = ++this__lexical_analyzer.binary_position;

                token_value[number_of_tokens][token_starting_position++] = *input++;
                token_value[number_of_tokens][token_starting_position] = '\0';
                token_type[number_of_tokens] = TOKEN_OP_DIV;
                token_id[number_of_tokens] = number_of_tokens++;

                token_starting_position = 0;
                break;
            }
            case '=':
            {
                token_row_position[number_of_tokens] = this__lexical_analyzer.row_position;
                token_column_position[number_of_tokens] = ++this__lexical_analyzer.column_position;
                token_binary_position[number_of_tokens] = ++this__lexical_analyzer.binary_position;

                token_value[number_of_tokens][token_starting_position++] = *input++;
                token_value[number_of_tokens][token_starting_position] = '\0';
                token_type[number_of_tokens] = TOKEN_ASSIGNMENT;
                token_id[number_of_tokens] = number_of_tokens++;

                token_starting_position = 0;
            }
            default:
            {
                if (*input >= '0' && *input <= '9')
                {
                    token_row_position[number_of_tokens] = this__lexical_analyzer.row_position;
                    token_column_position[number_of_tokens] = ++this__lexical_analyzer.column_position;
                    token_binary_position[number_of_tokens] = ++this__lexical_analyzer.binary_position;

                    token_value[number_of_tokens][token_starting_position++] = *input++;
                    while (loop)
                    {
                        switch (*input)
                        {
                            case '\0':
                            {
                                printf("[%d] = '\\0'\n", token_starting_position);

                                token_id[number_of_tokens] = number_of_tokens;
                                token_value[number_of_tokens][token_starting_position] = '\0';

                                loop = 0;
                                break;
                            }
                            default:
                            {
                                if (*input >= '0' && *input <= '9')
                                {
                                    this__lexical_analyzer.column_position++;

                                    token_value[number_of_tokens][token_starting_position++] = *input++;
                                }
                                else
                                {
                                    this__lexical_analyzer.column_position++;

                                    token_value[number_of_tokens][token_starting_position] = '\0';
                                    token_type[number_of_tokens] = TOKEN_NUMBER;
                                    token_id[number_of_tokens] = number_of_tokens++;
                                    
                                    token_starting_position = 0;
                                    loop = 0;
                                }
                            }
                        }
                    }
                    loop = 1;
                }
                else
                {
                    printf("5436546547657575757575765");
                    token_value[number_of_tokens][token_starting_position++] = *input++;
                    token_value[number_of_tokens][token_starting_position] = '\0';
                    token_type[number_of_tokens] = TOKEN_UNKNOWN;
                    token_id[number_of_tokens] = number_of_tokens++;
                }
            }
        }
        printf("[%d] = '\\0'\n", token_starting_position);
        input++;
    }

    for (int i = 0; i < number_of_tokens; i++)
    {
        printf("\nИдентификатор токена: %d", token_id[i]);
        printf("\nПозиция токена: [строка: %d, столбец: %d, бинарно: %d]", token_row_position[i], token_column_position[i], token_binary_position[i]);
        printf("\nТип токена: %s", token_name_table[token_type[i]]);
        printf("\nЗначение токена: %s\n", token_value[i]);
    }

    printf("\nВозврат из функции: lexical_analyzer\n");
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

    struct Lexical_analyzer lexical_analyzer;
    InitLexicalAnalyzer(&lexical_analyzer);
    LexicalAnalyzer(file_input);

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