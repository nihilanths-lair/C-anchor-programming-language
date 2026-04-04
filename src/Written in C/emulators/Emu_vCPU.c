// <!-- Encoding: Windows-1251 -->
#include <stdint.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

#ifndef _WIN32
#include <windows.h>
#endif

#define switch_open {
#define switch_close }

//uint8_t str_format[0xFF];

uint8_t vIP; uint16_t vEIP;
uint8_t vSP;
uint8_t vDI, vSI;

// comparison flag / флаг сравнения
bool CF;

uint8_t vMEMORY[0xFF];
//uint8_t vSTACK[0xFF];

char _data_1[0xFF];

enum UppercaseLetters // Заглавные буквы , LowercaseLetters // Строчные буквы: hlt, nop = 0x90,/*144*/
{
    HLT,
    NOP = 0x90,//144
    INC = 2, DEC,
    JMP, // Безусловный переход
    MOV,
    ADD, SUB, MUL, DIV, // Арифметические операции
    CMP, // Операция сравнения
    JE, // Условный переход
    _INT
    //PUSH
};

struct TableOpcode {
    //unsigned char identifier;
    uint8_t opcode;
    char symbolic_name[0x0F+1]; 
} table_opcode[0xFF];

struct CompilationError {
    uint8_t row_position;
    uint8_t column_position;
    uint8_t binary_position;
} compilation_error = {1, 1, 0};

//uint8_t slider = 0;
uint8_t step = 0;

// Syntax AT&T / Intel
uint8_t opcode[] =
{
    MOV, 1,
    'C', '$',
    CMP, 'C', '$',
    JE, 0,
    HLT,
    ' ', 'i', 's', ' ', 'a', 'w', 'e', 's', 'o', 'm', 'e', '!', '!', '!',
    //" C$ is awesome!!!",
    //JMP, 0,
    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
    HLT
};

const char ProcAsciiChr(uint8_t chr)
{
    switch (chr)
    switch_open
    case '\0': return '·'; // ··0
    case 0x01: return '·'; // ··1
    case 0x02: return '·'; // ··2
    case 0x03: return '·'; // ··3
    case 0x04: return '·'; // ··4
    case 0x05: return '·'; // ··5
    case 0x06: return '·'; // ··6
    case 0x07: return '·'; // ··7
    case 0x08: return '·'; // ··8
    case 0x09: return '·'; // ··9
    case '\n': return '·'; // ·10
    case 0x0B: return '·'; // ·11
    case 0x0C: return '·'; // ·12
    case '\r': return '·'; // ·13
    //case 0x0E: return '·'; // ·14
    case 0x0F: return '·'; // ·15
    case 0x10: return '·'; // ·16
    case 0x11: return '·'; // ·17
    case 0x12: return '·'; // ·18
    case 0x13: return '·'; // ·19
    case 0x14: return '·'; // ·20
    case 0x1B: return '·'; // ·27
    // 30-39 или 048-057: 0-9
    // 41-5A или 065-090: A-Z
    // 61-7A или 097-122: a-z
    case 0x90: return '·'; // 144
    case 0x95: return '·'; // 149
    //    A8 или     168: Ё
    //    B8 или     184: ё
    // C0-DF или 192-223: А-Я
    // E0-FF или 224-255: а-я
    default: return chr;
    switch_close
}

/// ... ///
/// метка: оператор операнд-1 операнд-2
// Метка является необязательным параметром, оператор может иметь два, один или вообще не иметь операндов
enum { FREE_STYLE, STRICT_STYLE };
bool syntax_style = STRICT_STYLE; // Стиль синтаксиса
// Свободный стиль - компилятор на разное количество отступов в разных местах не ругается
void FreeStyle(const char *text)
{
    puts("\n ENTRANCE: FreeStyle()");
    //char filter[0xFF]; //filter[0] = '\0';
}
// Строгий стиль - синтаксис жёстко зафиксирован и компилятор будет ругаться если ставить отступы в разных местах не по стандартам
void StrictStyle(const char *text)
{
    puts("\n ENTRANCE: StrictStyle()");
    //char filter[0xFF]; //filter[0] = '\0';
}
/// ... ///

//bool strcmpex(const char * str1, const char * str2) { return false; }
//bool scaner(const char * str1, const char * str2) { return false; }

//Assembly(){} //Disassembly(){}
/*
void _fwrite(size_t len)
{
    // Записываем в файл
    FILE * desc = fopen("_.bin", "wb");
    if (desc == NULL) { printf("Ошибка открытия файла."); return -1; }
    //ptr_data = 0xFF;
    //while (file_size--) fputc(data[++ptr_data], desc);
    fclose(desc);
}
*/
/// КОМПИЛЯТОР ///
char _source_code[0xFF] = "";
//uint8_t _ptr_source_code = 0xFF;

char _processed_text[0xFF];
//uint8_t _idx_processed_text;
//////////////////////////////////////////
char labels[0xF][0xFF];
uint8_t count_labels = 0xFF;
//////////////////////////////////////////
// Однопроходное развертывание макросов //
//////////////////////////////////////////
void SinglepassMacroDeployment(const char *text, bool taking_into_account_errors)
{
    #if !defined DEBUG
     printf("\n ENTRANCE: SinglepassMacroDeployment(..., %s)\n", (taking_into_account_errors) ? "true" : "false");
    #endif

    switch (taking_into_account_errors) // С учётом ошибок?
    switch_open
    case false:
    {
        uint8_t idx_text;
        uint8_t idx_processed_text;

        idx_processed_text = 0-1;
        idx_text = 0;
        _1_run:
        switch (text[idx_text])
        switch_open
        case '\0': goto _1_end;
        case 'J': // проверить следующие два символа, возможно JMP
        {
            printf("\n\t№%d, символ '%c' обнаружен!", idx_text, text[idx_text]);
            _processed_text[++idx_processed_text] = text[idx_text++];
            //++idx__text;
            switch (text[idx_text])
            switch_open
            case '\0': goto _1_end;
            case 'M': // проверить следующий символ, возможно JMP
            {
                printf("\n\t№%d, символ '%c' обнаружен!", idx_text, text[idx_text]);
                _processed_text[++idx_processed_text] = text[idx_text++];
                //++idx__text;
                switch (text[idx_text])
                switch_open
                case '\0': goto _1_end;
                case 'P': // проверить след. символ, необходим отступ/пробел
                {
                    printf("\n\t№%d, символ '%c' обнаружен!", idx_text, text[idx_text]);
                    _processed_text[++idx_processed_text] = text[idx_text++];
                    //++idx__text;
                    switch (text[idx_text])
                    switch_open
                    case '\0': goto _1_end;
                    case ' ':
                        printf("\n\t№%d, символ '%c' обнаружен!", idx_text, text[idx_text]);
                        _processed_text[++idx_processed_text] = text[idx_text++];
                        printf("\n Обработанные данные: \"%s\"", _processed_text);
                        //++idx__text; // пока мы не знаем метка находится выше или ниже
                        // ... // здесь уже идёт метка перехода, необходимо её просканировать и записать по какому адресу находится
                    default:
                    {
                        _processed_text[++idx_processed_text] = text[idx_text++];
                        goto _1_run; // если не ' ' !
                    }
                    switch_close
                }
                default:
                {
                    _processed_text[++idx_processed_text] = text[idx_text++];
                    goto _1_run; // если не 'P' !
                }
                switch_close
            }
            default: // не макрос, записываем как есть!
            {
                _processed_text[++idx_processed_text] = text[idx_text++];
                goto _1_run; // если не 'M' !
            }
            switch_close
        }
        case ':': // обнаружена метка, необходимо её проанализировать на наличие ошибок, а затем сохранить адрес перехода
        {
            printf("\n\t№%d, МЕТКА ОБНАРУЖЕНА!\n", idx_text);
            uint8_t addr_labels = idx_text; // запомним адрес конца метки
            uint8_t idx__labels = 0xFF;
            --idx_text;
            // идём обратным ходом, ... //
            _2_run:
            switch (text[idx_text])
            switch_open
            case '\0': goto _1_end;
            case ' ': // ... пока не будет обнаружен отступ/пробел означающий начало метки
            {
                labels[0][++idx__labels] = '\0';
                idx__labels = 0xFF;
                printf("\n\tМЕТКА №%d: \"%s\" - ПОЙМАНА!\n", count_labels, labels[count_labels]);
                idx_text += addr_labels;
                goto _1_run;
            }
            default: // идём по метке
            {
                printf("\n\t№%d, СОБИРАЕМ МЕТКУ!\n", idx_text);
                labels[count_labels][++idx__labels] = text[idx_text--];
                goto _2_run;
            }
            switch_close
            printf("\n\t№%d, МЕТКА ОБРАБОТАНА!\n", idx_text);
            goto _1_run;
        }
        default: // не макрос, записываем как есть!
        {
            _processed_text[++idx_processed_text] = text[idx_text++];
            goto _1_run;
        }
        switch_close _1_end:
        _processed_text[++idx_processed_text] = '\0';
    } break;
    case true: {}
    switch_close

    #if !defined DEBUG
     printf("\n EXIT: SinglepassMacroDeployment");
    #endif
}
//////////////////////////////////////////
// Двухпроходное развертывание макросов //
//////////////////////////////////////////
void TwopassMacroDeployment(const char *text, bool taking_into_account_errors)
{
    #if !defined DEBUG
     printf("\n ENTRANCE: TwopassMacroDeployment(..., %s)\n", (taking_into_account_errors) ? "true" : "false");
    #endif

    switch (taking_into_account_errors) // С учётом ошибок?
    switch_open
    case false:
    {
        uint8_t idx_text;
        uint8_t idx_processed_text;
        printf(" < I проход >");
        idx_processed_text = 0-1;
        idx_text = 0;
        /*//*/
        _1_run:
        switch (text[idx_text])
        switch_open
        case '\0': goto _1_end;
        case ':': // обнаружена метка, необходимо её проанализировать на наличие ошибок, а затем сохранить адрес перехода
        {
            printf("\n\t№%d, МЕТКА ОБНАРУЖЕНА!\n", idx_text);
            uint8_t addr_labels = idx_text; // запомним адрес конца метки
            uint8_t idx__labels = 0xFF;
            ++idx_text;
            // идём обратным ходом, ... //
            _2_run:
            switch (text[idx_text])
            switch_open
            case '\0': goto _1_end;
            case ' ': // ... пока не будет обнаружен отступ/пробел означающий начало метки
            {
                //labels[0][++idx__labels] = '\0';
                //idx__labels = 0xFF;
                printf("\n\tМЕТКА №%d: \"%s\" - ПОЙМАНА!\n", count_labels, labels[count_labels]);
                //idx_text += addr_labels;
                goto _1_run;
            }
            default: // идём по метке
            {
                //printf("\n\t№%d, СОБИРАЕМ МЕТКУ!\n", idx_text);
                //labels[count_labels][++idx__labels] = text[idx_text--];
                goto _2_run;
            }
            switch_close
            printf("\n\t№%d, МЕТКА ОБРАБОТАНА!\n", idx_text);
            goto _1_run;
        }
        default: // не макрос, записываем как есть!
        {
            _processed_text[++idx_processed_text] = text[idx_text++];
            goto _1_run;
        }
        switch_close _1_end:
        _processed_text[++idx_processed_text] = '\0';
        /*//*/
        printf(" < II проход >");
        idx_processed_text = 0-1;
        idx_text = 0;
        _2_1_run:
        switch (text[idx_text])
        switch_open
        case '\0': goto _1_end;
        case 'J': // проверить следующие два символа, возможно JMP
        {
            printf("\n\t№%d, символ '%c' обнаружен!", idx_text, text[idx_text]);
            _processed_text[++idx_processed_text] = text[idx_text++];
            //++idx__text;
            switch (text[idx_text])
            switch_open
            case '\0': goto _1_end;
            case 'M': // проверить следующий символ, возможно JMP
            {
                printf("\n\t№%d, символ '%c' обнаружен!", idx_text, text[idx_text]);
                _processed_text[++idx_processed_text] = text[idx_text++];
                //++idx__text;
                switch (text[idx_text])
                switch_open
                case '\0': goto _1_end;
                case 'P': // проверить след. символ, необходим отступ/пробел
                {
                    printf("\n\t№%d, символ '%c' обнаружен!", idx_text, text[idx_text]);
                    _processed_text[++idx_processed_text] = text[idx_text++];
                    //++idx__text;
                    switch (text[idx_text])
                    switch_open
                    case '\0': goto _1_end;
                    case ' ':
                        printf("\n\t№%d, символ '%c' обнаружен!", idx_text, text[idx_text]);
                        _processed_text[++idx_processed_text] = text[idx_text++];
                        printf("\n Обработанные данные: \"%s\"", _processed_text);
                        //++idx__text; // пока мы не знаем метка находится выше или ниже
                        // ... // здесь уже идёт метка перехода, необходимо её просканировать и записать по какому адресу находится
                    default:
                    {
                        _processed_text[++idx_processed_text] = text[idx_text++];
                        goto _2_1_run; // если не ' ' !
                    }
                    switch_close
                }
                default:
                {
                    _processed_text[++idx_processed_text] = text[idx_text++];
                    goto _2_1_run; // если не 'P' !
                }
                switch_close
            }
            default: // не макрос, записываем как есть!
            {
                _processed_text[++idx_processed_text] = text[idx_text++];
                goto _2_1_run; // если не 'M' !
            }
            switch_close
        }
        switch_close
        /*//*/
    }
    break;
    case true: {}
    switch_close

    #if !defined DEBUG
     printf("\n EXIT: TwopassMacroDeployment");
    #endif
}
//////////////////////////////////////////
// Удаление комментариев и развёртка макросов
void FullPreprocessing(const char *text)
{
    #if !defined DEBUG
     printf("\n ENTRANCE: FullPreprocessing()\n");
    #endif

    // ... //

    #if !defined DEBUG
     printf("\n EXIT: FullPreprocessing");
    #endif
}
// Только развёртка макросов
void DeployingMacros(const char *text, bool taking_into_account_errors)
{
    #if !defined DEBUG
     printf("\n ENTRANCE: DeployingMacros(..., %s)", (taking_into_account_errors) ? "true" : "false");
    #endif

    switch (2) // Кол-во проходов
    switch_open
    case 1: SinglepassMacroDeployment(text, taking_into_account_errors); break; // Однопроходная (сбор меток + подстановка адресов
    case 2: TwopassMacroDeployment(text, taking_into_account_errors); // Двухпроходная (сначала сбор меток, затем подстановка адресов)
    switch_close

    #if !defined DEBUG
     printf("\n EXIT: DeployingMacros");
    #endif
}

// Только удаление комментариев
void DeletingComments(const char *text)
{
    #if !defined DEBUG
     puts("\n ENTRANCE: DeletingComments()");
    #endif

    //int number_of_opening_singleline_comments = 0; // number_of_singleline_comments
    //int number_of_opening_multiline_comments = 0;  // number_of_multiline_comments

    uint8_t idx_text;
    uint8_t idx_processed_text;

    idx_processed_text = 0-1;
    idx_text = 0;

    _1_run:
    switch (text[++idx_text])
    switch_open
    case '\0': goto _1_end;
    case '-': // Выдать ошибку на этапе препроцессинга об отсутствии открывающего многострочного комментария.
    {
        _4_run:
        switch (text[++idx_text])
        switch_open
        case '\0': goto _1_end;
        case ';': // Ошибка: Отсутствует открывающий многострочный комментарий!
        {
            printf("\nError: Missing opening multi-line comment!");
            goto _1_end;
        }
        default: // Это не комментарий
        {
            _processed_text[++idx_processed_text] = text[--idx_text];
            goto _1_run;
        }
        switch_close
    }
    case ';': // Начало однострочного или многострочного комментария?
    {
        switch (text[++idx_text])
        switch_open
        case '\0': goto _1_end;
        case '-': // Начало многострочного комментария
        {
            printf("\n';-' - №%d №%d", idx_text-1, idx_text);
            _2_run:
            switch (text[++idx_text])
            switch_open
            case '\0': // Выдать ошибку на этапе препроцессинга об отсутствии закрывающего многострочного комментария.
            {
                printf("\nError: Missing closing multi-line comment!"); // Ошибка: Отсутствует закрывающий многострочный комментарий!
                goto _1_end;
            }
            case '-':
            { 
                switch (text[++idx_text])
                switch_open
                case '\0': goto _1_end;
                case ';': // Конец многострочного комментария
                {
                    printf("\n'-;' - №%d №%d", idx_text-1, idx_text);
                    switch (text[++idx_text])
                    switch_open
                    case '\0': goto _1_end;
                    case '\r':
                    {
                        printf("\n'\\r' - №%d", idx_text);
                        switch (text[++idx_text])
                        switch_open
                        case '\0': goto _1_end;
                        case '\n':
                        {
                            printf("\n'\\n' - №%d", idx_text);
                            goto _1_run;
                        }
                        default: goto _1_run;
                        switch_close
                    }
                    default: goto _1_run; // Конец многострочного комментария
                    switch_close
                }
                default: goto _2_run; // Пропускаем многострочный комментарий
                switch_close
            }
            default: goto _2_run; // Пропускаем многострочный комментарий
            switch_close
        }
        default: // Начало однострочного комментария
        {
            printf("\n';' - №%d", idx_text);
            _3_run:
            switch (text[++idx_text])
            switch_open
            case '\0': goto _1_end;
            case '\r': // Конец однострочного комментария?
            {
                _processed_text[++idx_processed_text] = text[idx_text];
                switch (text[++idx_text])
                switch_open
                case '\0': goto _1_end;
                case '\n': // Конец однострочного комментария
                {
                    _processed_text[++idx_processed_text] = text[idx_text];
                    goto _1_run;
                }
                default: goto _3_run; // Не конец однострочного комментария
                switch_close
            }
            default: goto _3_run; // Пропускаем однострочный комментарий
            switch_close
        }
        switch_close
        goto _1_run;
    }
    default: // Не комментарий
    {
        _processed_text[++idx_processed_text] = text[idx_text];
        goto _1_run;
    }
    switch_close
    _1_end:
    _processed_text[++idx_processed_text] = '\0';

    #if !defined DEBUG
     printf("\n EXIT: DeletingComments");
    #endif
}

void Preprocessing(char *text, uint8_t preprocessing_type, bool taking_into_account_errors) // size_t file_size режим
{
    #if !defined DEBUG
     puts("\n ENTRANCE: Preprocessing()");
    #endif

    // cdlr -E -M file_name.cdlr > file_name.cdlr (препроцессорная обработка с сохранением нераскрытых макросов)
    // cdlr -E -C file_name.cdlr > file_name.cdlr (препроцессорная обработка с сохранением комментариев)
    // cdlr -E file_name.cdlr > file_name.cdlr (препроцессорная обработка без сохранения того и другого)

    //printf(" Preprocessing started...\n"); // Препроцессорная обработка начата...

    printf("\n----\n<До>\n----\n%s\n\n", text);
    for (int i = 0; text[i] != '\0'; i++) printf("%c", ProcAsciiChr(text[i]));
    putchar('\n');

    switch (taking_into_account_errors) // проверять на наличие ошибок (корректность синтаксиса)
    switch_open
    case false:
    {
        switch (preprocessing_type)
        switch_open
        case 1: DeletingComments(text); break; // Только удаление комментариев
        case 2: DeployingMacros(text, false); break; // Только развёртка макросов
        case 3: FullPreprocessing(text); // Удаление комментариев и развёртка макросов
        switch_close
    }
    break;
    case true:
    {
        switch (preprocessing_type)
        switch_open
        case 1: DeletingComments(text); break; // Только удаление комментариев
        case 2: DeployingMacros(text, true); break; // Только развёртка макросов
        case 3: FullPreprocessing(text); // Удаление комментариев и развёртка макросов
        switch_close
    }
    switch_close

    printf("\n\n-------\n<После>\n-------\n%s\n\n", _processed_text);
    for (int i = 0; _processed_text[i] != '\0'; i++) printf("%c", ProcAsciiChr(_processed_text[i]));
    putchar('\n');

    FILE * desc = fopen("preprocessing\\_.asm", "wb");
    fwrite(_processed_text, sizeof (_processed_text), sizeof (char), desc);
    fclose(desc);

    //printf("\n\n Preprocessing completed!\n"); // Препроцессорная обработка закончена!

    #if !defined DEBUG
     printf("\n EXIT: Preprocessing");
    #endif
}

struct Lexer {
    uint64_t row_position;
    uint64_t column_position;
    uint64_t binary_position;
} lexer = {
    .row_position = 1,
    .column_position = 1,
    .binary_position = 0
};
typedef struct Lexer Lexer;

struct Token {
    uint8_t type;
} token[0xFF];
typedef struct Token Token;

const char _table_of_symbolic_names[][3+sizeof(char)] =
{
    "", "", "", "", "JMP"
};
///*-------------------------------------*/
// Только лексический анализ
void LexicalAnalysis(const char *text, Lexer *lexer)
{
    #if !defined DEBUG
     printf("\n ::=> LexicalAnalysis(");
     printf("\n  row_position = %d", lexer->row_position);
     printf("\n  column_position = %d", lexer->column_position);
     printf("\n  binary_position = %d", lexer->binary_position);
     printf("\n )");
    #endif

    uint8_t idx_text = 0;
    _1_run:
    switch (text[idx_text])
    switch_open
    case '\0': goto _1_end;
    // ... //
    switch_close
    _1_end:

    #if !defined DEBUG
     printf("\n <=:: LexicalAnalysis");
    #endif
}
// Только лексический синтез
void LexicalSynthesis(const char *text, Lexer *lexer)
{
    #if !defined DEBUG
     printf("\n ::=> LexicalSynthesis(");
     printf("\n  row_position = %d", lexer->row_position);
     printf("\n  column_position = %d", lexer->column_position);
     printf("\n  binary_position = %d", lexer->binary_position);
     printf("\n )");
    #endif

    uint8_t idx_text = 0;
    _1_run:
    switch (text[idx_text])
    switch_open
    case '\0': goto _1_end;
    // ... //
    switch_close
    _1_end:

    #if !defined DEBUG
     printf("\n <=:: LexicalSynthesis");
    #endif
}
// Лексический анализ и синтез (анализ лексем и выдача токенов [классифицируемых лексем])
void LexicalAnalysisAndSynthesis(const char *text, Lexer *lexer)
{
    #if !defined DEBUG
     printf("\n ::=> LexicalAnalysisAndSynthesis(");
     printf("\n  row_position = %d", lexer->row_position);
     printf("\n  column_position = %d", lexer->column_position);
     printf("\n  binary_position = %d", lexer->binary_position);
     printf("\n )");
    #endif

    uint8_t idx_text = 0;
    _1_run:
    switch (text[idx_text])
    switch_open
    case '\0': goto _1_end;
    case ' ': // не схлопываем, важен для строгого стиля, где пробелы учитываются
    {
        printf("\n [ЛА %d:%d, %d]: Токен ' ' обнаружен.", lexer->row_position, lexer->column_position, lexer->binary_position);
        idx_text++;
        lexer->column_position++;
        lexer->binary_position++;
        goto _1_run;
    }
    case 'J':
    {
        idx_text++;
        lexer->column_position++;
        lexer->binary_position++;
        switch (text[idx_text])
        switch_open
        case 'M':
        {
            idx_text++;
            lexer->column_position++;
            lexer->binary_position++;
            switch (text[idx_text])
            switch_open
            case 'P':
            {
                token->type = JMP;
                printf("\n [ЛА %d:%d, %d]: Токен \"%s\", тип %d обнаружен.", lexer->row_position, lexer->column_position, lexer->binary_position, _table_of_symbolic_names[token->type], token->type);
                idx_text++;
                lexer->column_position++;
                lexer->binary_position++;
                goto _1_run;
            }
            default: printf("\n [ЛА %d:%d, %d]: Ошибка! Токен \"JM\" не обнаружен ...", lexer->row_position, lexer->column_position, lexer->binary_position);
            switch_close
            goto _1_run;
        }
        default: printf("\n [ЛА %d:%d, %d]: Ошибка! Токен 'J' не обнаружен ...", lexer->row_position, lexer->column_position, lexer->binary_position);
        switch_close
        goto _1_run;
    }
    default:
    {
        // обычно крутим дальше, ищя другие токены, либо завершаем цикл
        printf("\n [ЛА %d:%d, %d]: Ошибка! Токен не обнаружен ...", lexer->row_position, lexer->column_position, lexer->binary_position); // Неопознанный/неизвестный тип токена
    }
    switch_close
    _1_end:

    #if !defined DEBUG
     printf("\n <=:: LexicalAnalysisAndSynthesis");
    #endif
}

// Синтаксический анализ и синтез (анализ синтаксиса и построение: CST [для отладки] / AST [для релиза])
void SyntacticAnalysisAndSynthesis(){}

// Семантический анализ и синтез (анализ семантики и построение промежуточного псевдокода [IR], либо без него и переход на след. этап: кодогенерация)
void SemanticAnalysisAndSynthesis(){}

// Генерация кода
void CodeGeneration()
{
    #if !defined DEBUG
     puts("\n ::=> CodeGeneration()");
    #endif

    // ... //

    #if !defined DEBUG
     puts("\n <=:: CodeGeneration");
    #endif
}
/*-------------------------------------*///

char Compile(const char *text, size_t file_size, const char *params)
{
    #if !defined DEBUG
     printf("\n ::=> Compile()");
    #endif

    // Однопроходная или многопроходная?
    switch (2)
    switch_open
    case 1:
    {
        /** Однопроходная компиляция */
        // ... //
        /* Однопроходная компиляция **/
    }
    break;
    case 2: // Разделение ответственности
    {
        uint8_t idx_text = 0;
        uint8_t idx_processed_text = 0-1;
        // type-3 (регулярные), type-2 (контекстно-свободные), type-1 (контекстно-зависимые), type-0 (неограниченные/рекурсивно-перечислимые)
        /** Многопроходная компиляция или же компиляция в несколько этапов */
        //lexer.row_position = 1, lexer.column_position = 1, lexer.binary_position = 0;
        LexicalAnalysis(text, &lexer);  // Только лексический анализ
        LexicalSynthesis(text, &lexer); // Только лексический синтез
        LexicalAnalysisAndSynthesis(text, &lexer); // Лексический анализ и синтез вместе
        //return 0;
        ////////////////////////
        // Синтаксический анализ, пока без синтеза CST (для отладки) / AST (для релиза)
        ///////////////////////////
        // Семантический анализ и синтез кодогенерации (пока без IR)
        // Сбор (вычисление) меток
        for (size_t _file_size = file_size; _file_size; _file_size--)
        {
            switch (text[++idx_text])
            switch_open
            case '\0': goto _1_end;
            // ... //
            switch_close
        }
        _1_end:
        // Макрозамена меток
        for (size_t _file_size = file_size; _file_size; _file_size--)
        {
            switch (text[++idx_text])
            switch_open
            case '\0': goto _2_end;
            // ... //
            switch_close
        }
        _2_end:
        //////////////////////////
        /* Многопроходная компиляция **/
    }
    switch_close

    #if !defined DEBUG
     printf("\n <=:: Compile");
    #endif
}

void Disassembly() //DebuggingInformation
{
    #if defined DEBUG
     puts("\n ENTRANCE: Disassembly");
    #endif

    printf("-------------------------------------------<+>-------------------------------------------------");
    printf("\n                                            ¦            Disassembly: vCPU (8-bit's)");
    printf("\n Address: Opcode (HEX<=>DEC)                ¦       Low-level assembler ¦ High-level assembler");
    printf("\n-------------------------------------------<+>-------------------------<+>---------------------");
    for (uint8_t i = 0; i < sizeof (opcode); )
    {
        switch (opcode[i])
        switch_open

        case HLT: // Specifics: Intel / AT&T
         printf("\n      %02X: %02X\t    |…|\t %03d: %03d\t    ¦  %s              |…| %c   ¦", i, opcode[i], i, opcode[i], table_opcode[HLT].symbolic_name, ProcAsciiChr(opcode[i]));
         // Для дизассемблирования
         ++i;
         // Для интерпретации
         //return 0;
         //break;

        case NOP: // Specifics: Intel / AT&T
         printf("\n      %02X: %02X\t    |…|\t %03d: %03d\t    ¦  %s              |…| %c   ¦", i, opcode[i], i, opcode[i], table_opcode[NOP].symbolic_name, ProcAsciiChr(opcode[i]));
         // Для дизассемблирования
         ++i;
        break;

        case INC: // Specifics: Intel
         printf("\n      %02X: %02X %02X    ¦    %s %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], table_opcode[INC].symbolic_name, vMEMORY[vIP+1]);
         // Для дизассемблирования
         ++i;
         //vMEMORY[vMEMORY[++vIP]]++;
         //vIP++;
        break;

        case DEC: // Specifics: Intel
         printf("\n      %02X: %02X %02X    ¦    %s %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], table_opcode[DEC].symbolic_name, vMEMORY[vIP+1]);
         // Для дизассемблирования
         ++i;
         //vMEMORY[vMEMORY[++vIP]]--;
         //vIP++;
        break;

        case JMP: // Specifics: Intel
         printf("\n      %02X: %02X %02X\t    |…|\t %03d: %03d %03d\t    ¦  %s %d            |…| %c%c  ¦",
          i, opcode[i], opcode[i+1], i, opcode[i], opcode[i+1], table_opcode[JMP].symbolic_name, opcode[i+1], ProcAsciiChr(opcode[i]), ProcAsciiChr(opcode[i+1])
         );
         // Для дизассемблирования
         i += 2;
         // Для интерпретации
         //vIP = vMEMORY[++vIP];
        break;

        case MOV:
         printf("\n      %02X: %02X %02X %02X  |…|  %03d: %03d %03d %03d   ¦  %s %d, %d ; %c\t|…| %c%c%c ¦",
          i, opcode[i], opcode[i+1], opcode[i+2],
          i, opcode[i], opcode[i+1], opcode[i+2],
          table_opcode[MOV].symbolic_name, opcode[i+1], opcode[i+2], ProcAsciiChr(opcode[i+2]),
          ProcAsciiChr(opcode[i]), ProcAsciiChr(opcode[i+1]), ProcAsciiChr(opcode[i+2])
         );
         // Для дизассемблирования
         i += 3;
         // Для интерпретации
         // Specifics: Intel
         //vMEMORY[vMEMORY[--vIP]] = vMEMORY[vIP+=2];
         //vIP += 2;
         //Specifics: AT&T
         //vMEMORY[vMEMORY[++vIP]] = vMEMORY[++vIP];
         //++vIP;
        break;

        case ADD:
         printf("\n      %02X: %02X %02X %02X    ¦    %s %d, %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2], table_opcode[ADD].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2]);
         // Specifics: Intel
         vMEMORY[vMEMORY[--vIP]] += vMEMORY[vIP+=2];
         vIP += 2;
         //Specifics: AT&T
         //vMEMORY[vMEMORY[--vIP]] = vMEMORY[--vIP] + vMEMORY[vIP+=2];
         //vIP += 2;
        break;

        case SUB:
         printf("\n      %02X: %02X %02X %02X    ¦    %s %d, %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2], table_opcode[SUB].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2]);
         // Specifics: Intel
         vMEMORY[vMEMORY[--vIP]] -= vMEMORY[vIP+=2];
         vIP += 2;
        break;

        case MUL:
         printf("\n      %02X: %02X %02X %02X    ¦    %s %d, %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2], table_opcode[MUL].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2]);
         // Specifics: Intel
         vMEMORY[vMEMORY[--vIP]] *= vMEMORY[vIP+=2];
         vIP += 2;
        break;

        case DIV:
         printf("\n      %02X: %02X %02X %02X    ¦    %s %d, %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2], table_opcode[DIV].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2]);
         // Specifics: Intel
         vMEMORY[vMEMORY[--vIP]] /= vMEMORY[vIP+=2];
         vIP += 2;
        break;

        case CMP:
         // Для дизассемблирования
         printf("\n      %02X: %02X %02X %02X  |…|  %03d: %03d %03d %03d   ¦  %s %d %d\t|…| %c%c%c ¦",
          i, opcode[i], opcode[i+1], opcode[i+2],
          i, opcode[i], opcode[i+1], opcode[i+2],
          table_opcode[CMP].symbolic_name, opcode[i+1], opcode[i+2],
          ProcAsciiChr(opcode[i]), ProcAsciiChr(opcode[i+1]), ProcAsciiChr(opcode[i+2])
         );
         i += 3;
         // Для интерпретации
         /*
         CF = (vMEMORY[--vIP] == vMEMORY[vIP+=2]);
         vIP += 2;
         */
        break;

        case JE:
         // Для дизассемблирования
         printf("\n      %02X: %02X %02X\t    |…|\t %03d: %03d %03d\t    ¦  %s %d\t\t|…| %c%c  ¦",
          i, opcode[i], opcode[i+1], i, opcode[i], opcode[i+1], table_opcode[JE].symbolic_name, opcode[i+1], ProcAsciiChr(opcode[i]), ProcAsciiChr(opcode[i+1])
         );
         i += 2;
         // Для интерпретации
         /*
         switch (CF){
          case 0: vIP = vMEMORY[vIP+=2];
          break;
          case 1: vIP = vMEMORY[++vIP];
         }
         */
        break;

        default:
         printf("\n      %02X: %02X\t    |…|\t %03d: %03d\t    ¦  %s                 |…| %c   ¦", i, opcode[i], i, opcode[i], table_opcode[opcode[i]].symbolic_name, ProcAsciiChr(opcode[i]));
         ++i;
        
        switch_close
    }
    printf("\n-------------------------------------------<+>-------------------------<+>---------------------");

    #if defined DEBUG
     puts("\n EXIT: Disassembly");
    #endif
}

// Отладка: шаг назад
void StepBack()
{
    #if defined DEBUG
     puts("\n ENTRANCE: StepBack");
    #endif

    // ... //

    #if defined DEBUG
     puts("\n EXIT: StepBack");
    #endif
}

// Релиз: полный цикл
void FullCycle()
{
    #if defined DEBUG
     puts("\n ENTRANCE: FullCycle");
    #endif

    // ... //

    #if defined DEBUG
     puts("\n EXIT: FullCycle");
    #endif
}

// Отладка: шаг вперёд
void StepNext()
{
    #if defined DEBUG
     puts("\n ENTRANCE: StepNext");
    #endif

    switch (vMEMORY[vIP])
    switch_open

    case INC:
     vMEMORY[vMEMORY[++vIP]]++;
     vIP++;
    break;

    case DEC:
     vMEMORY[vMEMORY[++vIP]]--;
     vIP++;
    break;

    case JMP:
     vIP = vMEMORY[++vIP];
    break;

    case MOV:
     vMEMORY[vMEMORY[--vIP]] = vMEMORY[vIP+=2];
     vIP += 2;
    break;

    case ADD:
     vMEMORY[vMEMORY[--vIP]] += vMEMORY[vIP+=2];
     vIP += 2;
    break;

    case SUB:
     vMEMORY[vMEMORY[--vIP]] -= vMEMORY[vIP+=2];
     vIP += 2;
    break;

    case MUL:
     vMEMORY[vMEMORY[--vIP]] *= vMEMORY[vIP+=2];
     vIP += 2;
    break;

    case DIV:
     vMEMORY[vMEMORY[--vIP]] /= vMEMORY[vIP+=2];
     vIP += 2;
    break;

    case CMP:
     CF = (vMEMORY[--vIP] == vMEMORY[vIP+=2]);
     vIP += 2;
    break;

    case JE:
     switch (CF){
     case 0: vIP = vMEMORY[vIP+=2]; break;
     case 1: vIP = vMEMORY[++vIP];
     }
    //break;

    switch_close
    step++;

    #if defined DEBUG
     puts("\n EXIT: StepNext");
    #endif
}

void Execute() // Launch
{
    #if defined DEBUG
     puts("\n ENTRANCE: Execute");
    #endif

    // ... //

    #if defined DEBUG
     puts("\n EXIT: Execute");
    #endif
}

void ShowDashboard()
{
    #if defined DEBUG
     puts("\n ENTRANCE: ShowDashboard");
    #endif

    switch (0)
    switch_open
    case 0:
    {
        printf("\n MEMORY (HEX | DEC | ASCII)\n    ");
        for (int i = 0; i < 0x0F+1; i++) printf("[%02X]", i);
        printf("        ");
        for (int i = 0; i < 0x0F+1; i++) printf("[%03d]", i);
        //printf("        ");
        //for (int i = 0; i < 0x0F + 1; i++) printf("[%c]", i);
        for (int j = 0; j < 0x0F+1; j++)
        {
            printf("\n[%02X]", j*0x10);
            for (int i = 0; i < 0x0F+1; i++) printf(" %02X ", vMEMORY[j*0x10+i]);
            printf("   ");
            printf("[%03d]", j*0x10);
            for (int i = 0; i < 0x0F+1; i++) printf(" %03d ", vMEMORY[j*0x10+i]);
            printf("   ");
            for (int i = 0; i < 0x0F+1; i++) printf("%c", ProcAsciiChr(vMEMORY[j*0x10+i]));
        }
        break;
    }
    case 10:
    {
        printf("MEMORY (DEC)\n     ");
        for (int i = 0; i < 0x0F+1; i++) printf("[%03d]", i);
        for (int j = 0; j < 0x0F+1; j++)
        {
            printf("\n[%03d]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F+1; i++) printf(" %03d ", vMEMORY[i]);
        }
        break;
    }
    case 16:
    {
        printf("MEMORY (HEX)\n    ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%02X]", i);
        for (int j = 0; j < 0x0F + 1; j++)
        {
            printf("\n[%02X]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F+1; i++) printf(" %02X ", vMEMORY[i]);
        }
    }
    switch_close
    putchar('\n');

    #if defined DEBUG
     puts("\n EXIT: ShowDashboard");
    #endif
}

enum IVT {
    PUTCHAR = 1,
    PRINTF
};

void LoadingProgramIntoMemory()
{
    #if defined DEBUG
     puts("\n ENTRANCE: LoadingProgramIntoMemory");
    #endif

    //printf("%s", opcode);
    //printf(":: %d :: ", sizeof (opcode));
    //for (int i = 0; i < sizeof (opcode); i++) printf("%c", ProcAsciiChr(opcode[i]));

    for (uint8_t i = 0; i < sizeof (opcode); i+=3){
        printf("%d - %02X %02X %02X\n", i, opcode[i], opcode[i+1], opcode[i+2]);
        vMEMORY[i] = opcode[i];
        vMEMORY[i+1] = opcode[i+1];
        vMEMORY[i+2] = opcode[i+2];
    }
    puts("\n Программа загружена в память.");

    #if defined DEBUG
     puts("\n EXIT: LoadingProgramIntoMemory");
    #endif
}

// 1. Возвращаем указатель на char (char*), а не один символ (char)
// 2. Вместо ссылки (&) используем указатель (*index)
char *Strtok(const char *string, int *index)
{
    int length = strlen(string);
    while ((*index < length) && (string[*index] <= ' ')) (*index)++;

    int offset = *index;
    static char result[128];
    result[0] = '\0';

    while ((*index < length) && (string[*index] > ' ') && ((*index - offset) < (sizeof (result) - 1)))
    {
        result[*index - offset] = string[*index];
        (*index)++;
    }
    result[*index - offset] = '\0';

    if (offset == *index) return NULL;
    return result;
}
void _(int argc, char *argv[])
{
    setlocale(0, "");
    /*
    // 1. Устанавливаем кодировку Windows-1251 для кириллицы
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    */
    printf("\n argc = %d", argc);
    for (int i = 0; i < argc; i++) printf("\n argv[%d] = %s", i, argv[i]);
    putchar('\n');

    // Статический (неизменный/фиксированный) компилятор - compiler.exe (в саму программу вшита логика, и поэтому не может быть изменена)
    // Динамический (изменяемый/подвижной) компилятор - compiler.exe (логика программы хранится в отдельном файле и может подвергаться модификациям)
    /// При каждом вызове компилятора (компиляции файла) происходит загрузка ядра компилятора, что при больших объёмах затрачивает огромное количество времени,
    //   в таких случаях рекомендуется перейти на резидентный компилятор, который позволяет многократно использовать ядро, без необходимости повторной загрузки

    //Generator(); // не проверяет на наличие ошибок, просто делает обычную замену макросов или удаляет комментарии, либо делает и то, и другое - одновременно
    //Converter(); // преобразователь
    //Transpilation();
    //Translator(); // переводчик
    //const char params[] = "-E";
    /** Варианты компиляции (сама по себе компиляция предполагает наличие проверок на ошибки[*, но можно отключить*^тогда скорее это будет уже не компиляция, а генерация]) */
    // 1a. Из dec/hex или bin представления в оп-код, без проверок ошибок
    // 1b. Из dec/hex или bin представления в оп-код, с проверкой ошибок
    // 2. Из языка ассемблера в оп-код | _.asm -> _.bin
    // 3a. Из C$ в язык ассемблера | _.cdlr -> _.asm
    // 3b. Из C$ в оп-код | _.cdlr -> _.bin

    // препроцессорная обработка без этапа компиляции
    // препроцессорная обработка с этапом компиляции
    // без процессорной обработки, сразу этап компиляции
    
    const char *file_name = "_.asm"; // временно
    FILE *desc = fopen(file_name, "rb"); // Считываем с файла
    if (desc == NULL)
    {
        printf("\n Ошибка открытия файла.");
        return;
    }
    fseek(desc, 0, SEEK_END);
    size_t file_size = ftell(desc);
    printf("\nРазмер файла: %ld.\n", file_size);
    fseek(desc, 0, SEEK_SET);
    fread(_source_code, file_size, sizeof (char), desc);
    fclose(desc);
    printf("\n[file: _.asm]\n'''\n%s\n'''\n", _source_code);

    uint8_t action = 2; // (временно) 1] Только препроцессорная обработка, обходя этап компиляции, 2] Компиляция
    switch (action)
    switch_open
    case 1: Preprocessing(_source_code, 2, false); break;
    case 2: Compile(_source_code, file_size, ""); break;
    switch_close
    putchar('\n');
    return; // временно, для быстрого тестирования

    char input[128];
    char cmd[128];

    //printf("\nДля отображения списка команд введите: !cmdlist");
    while (true)
    {
        printf("\nДля отображения списка команд введите: !cmdlist");
        printf("\n$: ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\r\n")] = '\0'; // Удаляем перевод строки

        int idx = 0; // Инициализируем индекс нуля для каждого ввода
        char *cmd = Strtok(input, &idx); // Получаем первое слово (команду)
        if (cmd == NULL) goto unknown_command; // Если ничего не ввели

        // Копируем команду в отдельный буфер, если нужно сохранить
        //strcpy(first_word, cmd);
        /** background information :: справочная информация */
        if (!strcmp(cmd, "!cmdlist"))
        {
            puts("\n Список команд:");
            printf("\n1] !compile input_file output_file");
            //printf("\n2] !execute");
        }
        else if (!strcmp(cmd, "!compile"))
        {
            char *input_file = Strtok(input, &idx); // 2-й аргумент: входной файл
            if (input_file == NULL)
            {
                printf(" Ошибка: Введите имя входного файла!\n");
                continue;
            }
            char *output_file = Strtok(input, &idx); // 3-й аргумент: выходной файл
            if (output_file == NULL)
            {
                printf(" Ошибка: Введите имя выходного файла!\n");
                Compile(input_file, 0, "");
                continue;
            }
            printf("output_file = \"%s\"", output_file);
            Compile(input_file, 0, output_file);
        }
        //else if (!strcmp(cmd, "/execute")){}
        else
        {
            unknown_command:
            printf("\nНеизвестная команда: \"%s\"", cmd);
        }
    }

    // Инициализация vCPU
    vIP = 0;

    // Загрузка программы в память
    //LoadingProgramIntoMemory();
    for (uint8_t i = 0; i < sizeof (opcode); i++) vMEMORY[i] = opcode[i];

    table_opcode[HLT].opcode = HLT;
    strcpy(table_opcode[HLT].symbolic_name, "HLT");

    table_opcode[INC].opcode = INC;
    strcpy(table_opcode[INC].symbolic_name, "INC");

    table_opcode[DEC].opcode = DEC;
    strcpy(table_opcode[DEC].symbolic_name, "DEC");

    // Безусловный переход
    table_opcode[JMP].opcode = JMP;
    strcpy(table_opcode[JMP].symbolic_name, "JMP");

    table_opcode[MOV].opcode = MOV;
    strcpy(table_opcode[MOV].symbolic_name, "MOV");

    // Арифметические операции
    table_opcode[ADD].opcode = ADD;
    strcpy(table_opcode[ADD].symbolic_name, "ADD");
    //
    table_opcode[SUB].opcode = SUB;
    strcpy(table_opcode[SUB].symbolic_name, "SUB");
    //
    table_opcode[MUL].opcode = MUL;
    strcpy(table_opcode[MUL].symbolic_name, "MUL");
    //
    table_opcode[DIV].opcode = DIV;
    strcpy(table_opcode[DIV].symbolic_name, "DIV");

    // Операция сравнения
    table_opcode[CMP].opcode = CMP;
    strcpy(table_opcode[CMP].symbolic_name, "CMP");

    // Условный переход
    table_opcode[JE].opcode = JE;
    strcpy(table_opcode[JE].symbolic_name, "JE"); // JZ

    table_opcode[NOP].opcode = 0x90; // .. = 144
    strcpy(table_opcode[NOP].symbolic_name, "NOP");

    while (true)
    {
        printf("\n Шаг: %d\n", step);
        puts("     HEX(16) |  DEC(10)  | ASCII");
        //for (int i = 0; i < 256; i++)
        printf(" IP: [%02X]:%02X | [%03d]:%03d | ['%c']:'%c'\n", vIP, vMEMORY[vIP], vIP, vMEMORY[vIP], ProcAsciiChr(vIP), ProcAsciiChr(vMEMORY[vIP]));
        Disassembly();
        //printf("\n      0F: %02X %02X %02X [DEBUG] 015: %03d %03d %03d\t\t\t  %c%c%c", vMEMORY[15], vMEMORY[16], vMEMORY[17], vMEMORY[15], vMEMORY[16], vMEMORY[17], vMEMORY[15], vMEMORY[16], vMEMORY[17]);
        putchar('\n');
        //printf(" SP: %02X | %03d | %c\n", vSP, vSP, ProcAsciiChr(vSP));
        //printf("DI  [%02X]  [%03d]  [%c]\n", vDI, vDI, ProcAsciiChr(vDI));
        //printf("SI  [%02X]  [%03d]  [%c]\n", vSI, vSI, ProcAsciiChr(vSI));
        //char op = 0;
        ShowDashboard();
        printf(
         "\n---------------------------------------------------------------------"
         "\n Клавиша\tОписание"
         "\n"
         "\n ESC\t\tЗавершить эмуляцию"
         "\n F2\t\tПродвинуться на 1 шаг назад"
         "\n F3\t\tВыполнить все шаги (скорость каждого шага - 2,5 сек.)"
         "\n F4\t\tПродвинуться на 1 шаг вперёд"
         "\n----------------------------------------------------------------------"
         "\n Нажмите соответствующую клавишу..."
        );
        uint8_t ch = _getch();
        switch (ch)
        switch_open
        case 0:
        {
            //printf("\n Вы нажали клавишу №1: %02X | %03d | %c", ch, ch, ProcAsciiChr(ch));
            ch = _getch();
            //printf("\n Вы нажали клавишу №2: %02X | %03d | %c\n", ch, ch, ProcAsciiChr(ch));
            switch (ch)
            switch_open

            case '<': StepBack(); // Шаг назад
            break;

            case '=': for (int i = 0; i < sizeof (opcode); i++) FullCycle(); // Полный цикл / StepNext(); // Шаг вперёд /././ //Sleep(2500); // Задержка на 2500 миллисекунд (2,5 секунд)
            break;

            case '>': StepNext(); // Шаг вперёд
            break;

            default: printf("\n Вы нажали клавишу №2: %02X | %03d | %c\n", ch, ch, ProcAsciiChr(ch));
            switch_close
            break;
        }
        case 27: // 1B | 027 | ·  <ESC>
         //printf("\n Вы нажали клавишу №1: %02X | %03d | %c", ch, ch, ProcAsciiChr(ch));
         return;

        case '<': puts("\n Такая клавиша не определена.");
         //printf("\n Вы нажали клавишу №1: %02X | %03d | %c", ch, ch, ProcAsciiChr(ch));
        break;

        case '=': puts("\n Такая клавиша не определена.");
         //printf("\n Вы нажали клавишу №1: %02X | %03d | %c", ch, ch, ProcAsciiChr(ch));
        break;

        case '>':
        {
            puts("\n Такая клавиша не определена.");
            //printf("\n Вы нажали клавишу №1: %02X | %03d | %c\n", ch, ch, ProcAsciiChr(ch));
            Execute(ch); // Launch
            ShowDashboard();
            break;
        }

        default: puts("\n Такая клавиша не определена.");
        //break;
        // 0D | 013 | ·  <ENTER>
        // E0 | 224 | а  <F11-F12>
        switch_close
    }
}
///
int main(int argc, char *argv[]) { _(argc, argv); }
//?// По мимо глобальных меток, необходимо ввести локальные метки
/*
//char sample[2+sizeof(char)];
//if (!strcmp(sample, ";-")){}
//else if (!strcmp(sample, ";")){}
//else if (!strcmp(sample, "-;")){}
*/
/*
    char filter[0xFF]; //filter[0] = '\0';
    //printf("\n%s\n", _data_1);
    //for (int i = 0; text[i] != '\0'; i++) printf("%c", ProcAsciiChr(_data_1[i]));
    // Generator 'case'
    //putchar('\n');
    /*
    //puts("0-9 | 48-57");
    //for (int i = 48; i <= 57; i++) printf("case (unsigned char)'%c': {} break;\n", i);
    //puts("A-Z | 65-90");
    //for (int i = 65; i <= 90; i++) printf("case (unsigned char)'%c': {} break;\n", i);
    //puts("a-z | 97-122");
    //for (int i = 97; i <= 122; i++) printf("case (unsigned char)'%c': {} break;\n", i);
    //puts("А-я | 192-255");
    //for (int i = 192; i <= 255; i++) printf("case (unsigned char)'%c': {} break;\n", i);
*/
/*
case INT:
 vIP++;
 switch (*++ptr_op_code)
 case PUTCHAR:
  vIP++;
  putchar(opcode[vIP]);
  break;
 case PRINTF: break;
*/
/// Текущая ячейка / Произвольная ячейка
/// ...-> Выборка -> Декодирование -> Исполнение -> Смещение IP на след. инструкцию (автоматически) ->...

// Инструкции и данные находятся в одной секции или разделены?
// Инструкции кодируются как в x86 первые два байта - одним или каждый по раздельности?