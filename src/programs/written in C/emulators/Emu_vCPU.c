// <!-- Encoding: Windows-1251 -->
#include <stdint.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define _rb_ {
#define _eb_ }

//uint8_t str_format[0xFF];

uint8_t vIP;
uint8_t vSP;
uint8_t vDI, vSI;

uint8_t vMEMORY[0xFF];
//uint8_t vSTACK[0xFF];

char _data_1[0xFF];

char processed_text[0xFF];
uint8_t idx__processed_text;

enum UppercaseLetters // Заглавные буквы
{
    INC = 1, DEC,
    MOV,
    ADD, SUB, MUL, DIV,
    JMP,
    PUSH,
    _INT
};
enum LowercaseLetters // Строчные буквы
{
    inc = 1, dec,
    mov,
    add, sub, mul, _div,
    jmp,
    push,
    _int
};

struct TableOpcode {
    //unsigned char identifier;
    unsigned char opcode;
    char symbolic_name[0x0F+1];
    
} table_opcode[0xFF] = {
    //1,
    INC, "INC",
    //2,
    DEC, "DEC",
    //3,
    MOV, "MOV",
    //4,
    ADD, "ADD",
    //5,
    SUB, "SUB"
};

unsigned char step = 0;

#define HLT 0xFF
// Syntax AT&T / Intel
uint8_t opcode[] =
{
    MOV, 36, 'C', // MOV 16, 'C' | 03 10 43 | 003 036 067 | ··C
    MOV, 37, '$', // MOV 17, '$' | 03 11 24 | 003 037 036 | ··$
    MOV, 38, ' ', // MOV 18, ' ' | 03 12 20 | 003 038 032 | ··
    MOV, 39, 'r', // MOV 19, 'r' | 03 13 72 | 003 039 114 | ··r
    MOV, 40, 'u', // MOV 20, 'u' | 03 14 75 | 003 040 117 | ··u
    MOV, 41, 'l', // MOV 21, 'l' | 03 15 6C | 003 041     | ··l
    MOV, 42, 'i', // MOV 22, 'i' | 03 16 69 | 003 042     | ··i
    MOV, 43, 't', // MOV 23, 't' | 03 17 74 | 003 043     | ··t
    MOV, 44, '!', // MOV 24, '!' | 03 18 21 | 003 044     | ··!
    //HLT
};

char ProcAsciiChr(uint8_t chr)
{
    switch (chr)
    _rb_
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
    case 0x95: return '·'; // 149
    //    A8 или     168: Ё
    //    B8 или     184: ё
    // C0-DF или 192-223: А-Я
    // E0-FF или 224-255: а-я
    default: return chr;
    _eb_
}

/// ... ///
/// метка: оператор операнд-1 операнд-2
// Метка является необязательным параметром, оператор может иметь два, один или вообще не иметь операндов
enum { FREE_STYLE, STRICT_STYLE };
bool syntax_style = STRICT_STYLE; // Стиль синтаксиса
// Свободный стиль - компилятор на разное количество отступов в разных местах не ругается
void FreeStyle(const char * text)
{
    puts("\n>> FreeStyle()");
    char filter[0xFF]; //filter[0] = '\0';
}
// Строгий стиль - синтаксис жёстко зафиксирован и компилятор будет ругаться если ставить отступы в разных местах не по стандартам
void StrictStyle(const char * text)
{
    puts("\n>> StrictStyle()");
    char filter[0xFF]; //filter[0] = '\0';
}
/// ... ///

bool strcmpex(const char * str1, const char * str2){
    return false;
}
bool scaner(const char * str1, const char * str2){
    return false;
}

//Assembly(){}
//Disassembly(){}
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

// Удаление комментариев и развёртка макросов
void DeletingCommentsAndDeployingMacros(const char * text)
{
    printf("\n>> DeletingCommentsAndDeployingMacros()\n");
}

// Только развёртка макросов
void DeployingMacros(const char * text)
{
    printf("\n>> DeployingMacros()\n");
}

// Только удаление комментариев
void DeletingComments(const char * text)
{
    printf("\n>> DeletingComments()\n");

    idx__processed_text = 0-1;
    unsigned char idx__text = 0-1;

    //int number_of_opening_singleline_comments = 0; // number_of_singleline_comments
    //int number_of_opening_multiline_comments = 0;  // number_of_multiline_comments

    _1_run: switch (text[++idx__text]){
    case '\0': goto _1_end;
    case '-': // Выдать ошибку на этапе препроцессинга об отсутствии открывающего многострочного комментария.
        _4_run: switch (text[++idx__text]){
        case '\0': goto _1_end;
        case ';': // Ошибка: Отсутствует открывающий многострочный комментарий!
            printf("\nError: Missing opening multi-line comment!");
            goto _1_end;
        default: // Это не комментарий
            processed_text[++idx__processed_text] = text[--idx__text];
            goto _1_run;
        }
    case ';': // Начало однострочного или многострочного комментария?
        switch (text[++idx__text]){
        case '\0': goto _1_end;
        case '-': // Начало многострочного комментария
            printf("\n';-' - №%d №%d", idx__text-1, idx__text);
            _2_run: switch (text[++idx__text]){
            case '\0': // Выдать ошибку на этапе препроцессинга об отсутствии закрывающего многострочного комментария.
                printf("\nError: Missing closing multi-line comment!"); // Ошибка: Отсутствует закрывающий многострочный комментарий!
                goto _1_end;
            case '-': 
                switch (text[++idx__text]){
                case '\0':
                    //printf("\nError: Missing closing multi-line comment!"); // Ошибка: Отсутствует закрывающий многострочный комментарий!
                    goto _1_end;
                case ';': // Конец многострочного комментария
                    printf("\n'-;' - №%d №%d", idx__text-1, idx__text);
                    switch (text[++idx__text]){
                    case '\0': goto _1_end;
                    case '\r':
                        printf("\n'\\r' - №%d", idx__text);
                        switch (text[++idx__text]){
                        case '\0': goto _1_end;
                        case '\n':
                            printf("\n'\\n' - №%d", idx__text);
                            goto _1_run;
                        default: goto _1_run;
                        }
                    default: goto _1_run; // Конец многострочного комментария
                    }
                default: goto _2_run; // Пропускаем многострочный комментарий
                }
            default: goto _2_run; // Пропускаем многострочный комментарий
            }
        default: // Начало однострочного комментария
            printf("\n';' - №%d", idx__text);
            _3_run: switch (text[++idx__text]){
            case '\0': goto _1_end;
            case '\r': // Конец однострочного комментария?
                processed_text[++idx__processed_text] = text[idx__text];
                switch (text[++idx__text]){
                case '\0': goto _1_end;
                case '\n': // Конец однострочного комментария
                    processed_text[++idx__processed_text] = text[idx__text];
                    goto _1_run;
                default: goto _3_run; // Не конец однострочного комментария
                }
            default: goto _3_run; // Пропускаем однострочный комментарий
            }
        }
        goto _1_run;
    default: // Не комментарий
        processed_text[++idx__processed_text] = text[idx__text];
        goto _1_run;
    }_1_end:
    processed_text[++idx__processed_text] = '\0';
}

void Preprocessing(char * text, unsigned char preprocessing_type, size_t file_size) // режим
{
    printf("\n>> Preprocessing(<?>, %d)\n", preprocessing_type);

    // cdlr -E -M file_name.cdlr > file_name.cdlr (препроцессорная обработка с сохранением нераскрытых макросов)
    // cdlr -E -C file_name.cdlr > file_name.cdlr (препроцессорная обработка с сохранением комментариев)
    // cdlr -E file_name.cdlr > file_name.cdlr (препроцессорная обработка без сохранения того и другого)

    printf("\n----\n<До>\n----\n%s\n\n", text);
    for (int i = 0; text[i] != '\0'; i++) printf("%c", ProcAsciiChr(text[i]));
    putchar('\n');

    switch (preprocessing_type)
    _rb_
    case 1: // Только удаление комментариев
        DeletingComments(text);
        break;
    case 2: // Только развёртка макросов
        DeployingMacros(text);
        break;
    case 3: // Удаление комментариев и развёртка макросов
        DeletingCommentsAndDeployingMacros(text);
        break;
    _eb_

    printf("\n\n-------\n<После>\n-------\n%s\n\n", processed_text);
    for (int i = 0; processed_text[i] != '\0'; i++) printf("%c", ProcAsciiChr(processed_text[i]));
    putchar('\n');

    FILE * desc = fopen("preprocessing\\_.asm", "wb");
    fwrite(processed_text, idx__processed_text, sizeof (char), desc);
    fclose(desc);

/*
//char sample[2+sizeof(char)];
//if (!strcmp(sample, ";-")){}
//else if (!strcmp(sample, ";")){}
//else if (!strcmp(sample, "-;")){}
*/
/*
    switch (syntax_style){
    case FREE_STYLE: {
        FreeStyle(text);
    } break;
    case STRICT_STYLE: {
        StrictStyle(text);
    }}
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
}

void DebuggingInformation(uint8_t vIP) //Disassembly
{
    #if !defined DEBUG
     puts("\n ENTRANCE: DebuggingInformation");
    #endif

    ///

    #if !defined DEBUG
     puts("\n EXIT: DebuggingInformation");
    #endif
}

// Релиз: полный цикл
void FullCycle()
{
    #if defined DEBUG
     puts("\n ENTRANCE: FullCycle");
    #endif

    switch (opcode[vIP])
    _rb_

    case INC: vMEMORY[opcode[++vIP]]++; vIP++;
    break;

    case DEC: vMEMORY[opcode[++vIP]]--; vIP++;
    break;

    case JMP: vIP = opcode[++vIP];
    break;

    case MOV: vMEMORY[opcode[--vIP]] = opcode[vIP+=2]; vIP += 2;
    break;

    case ADD: vMEMORY[opcode[--vIP]] += opcode[vIP+=2]; vIP += 2;
    break;

    case SUB: vMEMORY[opcode[--vIP]] -= opcode[vIP+=2]; vIP += 2;
    break;

    case MUL: vMEMORY[opcode[--vIP]] *= opcode[vIP+=2]; vIP += 2;
    break;

    case DIV: vMEMORY[opcode[--vIP]] /= opcode[vIP+=2]; vIP += 2;
    //break;

    _eb_

    #if defined DEBUG
     puts("\n EXIT: FullCycle");
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

// Отладка: шаг вперёд
void StepNext()
{
    #if defined DEBUG
     puts("\n ENTRANCE: StepNext");
    #endif

    switch (opcode[vIP])
    _rb_

    case INC: vMEMORY[opcode[++vIP]]++; vIP++;
    break;

    case DEC: vMEMORY[opcode[++vIP]]--; vIP++;
    break;

    case JMP: vIP = opcode[++vIP];
    break;

    case MOV: vMEMORY[opcode[--vIP]] = opcode[vIP+=2]; vIP += 2;
    break;

    case ADD: vMEMORY[opcode[--vIP]] += opcode[vIP+=2]; vIP += 2;
    break;

    case SUB: vMEMORY[opcode[--vIP]] -= opcode[vIP+=2]; vIP += 2;
    break;

    case MUL: vMEMORY[opcode[--vIP]] *= opcode[vIP+=2]; vIP += 2;
    break;

    case DIV: vMEMORY[opcode[--vIP]] /= opcode[vIP+=2]; vIP += 2;
    //break;

    _eb_
    step++;

    #if defined DEBUG
     puts("\n EXIT: StepNext");
    #endif
}

void Execute(unsigned char ch) // Launch
{
    #if !defined DEBUG
     puts("\n ENTRANCE: Execute");
    #endif

    switch (ch)
    _rb_
    case '<':
        StepBack();
        break;

    case '=':
        FullCycle();
        break;

    case '>':
        StepNext();
        break;
    _eb_

    #if !defined DEBUG
     puts("\n EXIT: Execute");
    #endif
}

void Compile(char * text, size_t file_size)
{
    #if defined DEBUG
     puts("\n ENTRANCE: Compile");
    #endif

    Preprocessing(text, 2, file_size);

    #if defined DEBUG
     puts("\n EXIT: Compile");
    #endif
}

void ShowDashboard()
{
    #if defined DEBUG
     puts("\n ENTRANCE: ShowDashboard");
    #endif

    switch (0)_rb_
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
    } break;
    case 10:
    {
        printf("MEMORY (DEC)\n     ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%03d]", i);
        for (int j = 0; j < 0x0F + 1; j++)
        {
            printf("\n[%03d]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F + 1; i++) printf(" %03d ", vMEMORY[i]);
        }
    } break;
    case 16:
    {
        printf("MEMORY (HEX)\n    ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%02X]", i);
        for (int j = 0; j < 0x0F + 1; j++)
        {
            printf("\n[%02X]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F + 1; i++) printf(" %02X ", vMEMORY[i]);
        }
    }_eb_
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
// 1 parent b20f142 commit 3b5d72d
int main()
{
    setlocale(0, "");
    /*
    // 1. Устанавливаем кодировку Windows-1251 для кириллицы
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // 2. Включаем поддержку ANSI-последовательностей (цветов)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    */
    // Инициализация vCPU
    vIP = 0;

    // Загрузка программы в память
    LoadingProgramIntoMemory();

    char cmd[128+sizeof(char)];
    //puts("\nДля отображения списка команд введите: /cmdlist");
    _0: while (true)
    {
        printf("\n Шаг: %d\n", step);
        puts("     HEX(16) |  DEC(10)  | ASCII");
        //for (int i = 0; i < 256; i++)
        printf(" IP: [%02X]:%02X | [%03d]:%03d | ['%c']:'%c'\n", vIP, vMEMORY[vIP], vIP, vMEMORY[vIP], ProcAsciiChr(vIP), ProcAsciiChr(vMEMORY[vIP]));

        printf("----------------------------------------------<•>---------------------------");
        printf("\n Address: Opcode (HEX<=>DEC)                   ¦    Assembler vCPU (8-bit's)");
        printf("\n                                               ¦");
        for (uint8_t i = 0; i < 10; i++)
        {
            switch (vMEMORY[vIP])
            _rb_

            case INC: // Syntax: Intel
                printf("\n      %02X: %02X %02X    ¦    %s %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], table_opcode[INC-1].symbolic_name, vMEMORY[vIP+1]);
                vMEMORY[vMEMORY[++vIP]]++; vIP++;
            break;

            case DEC: // Syntax: Intel
                printf("\n      %02X: %02X %02X    ¦    %s %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], table_opcode[DEC-1].symbolic_name, vMEMORY[vIP+1]);
                vMEMORY[vMEMORY[++vIP]]--; vIP++;
            break;

            case JMP: // Syntax: Intel
                printf("\n      %02X: %02X %02X    ¦    %s %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], table_opcode[JMP-1].symbolic_name, vMEMORY[vIP+1]);
                vIP = vMEMORY[++vIP];
            break;

            case MOV:
                printf("\n      %02X: %02X %02X %02X   <=>   %03d: %03d %03d %03d    ¦    %s %d, %d\t¦ ··%c",
                    vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2],
                    vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2],
                    table_opcode[MOV-1].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2], vMEMORY[vIP+2]
                );
                // Syntax: Intel
                vMEMORY[vMEMORY[--vIP]] = vMEMORY[vIP+=2];
                vIP += 2;
                //Syntax: AT&T
                //vMEMORY[vMEMORY[++vIP]] = vMEMORY[++vIP];
                //++vIP;
            break;

            case ADD:
                printf("\n      %02X: %02X %02X %02X    ¦    %s %d, %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2], table_opcode[ADD-1].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2]);
                // Syntax: Intel
                vMEMORY[vMEMORY[--vIP]] += vMEMORY[vIP+=2];
                vIP += 2;
                //Syntax: AT&T
                //vMEMORY[vMEMORY[--vIP]] = vMEMORY[--vIP] + vMEMORY[vIP+=2];
                //vIP += 2;
            break;

            case SUB:
                printf("\n      %02X: %02X %02X %02X    ¦    %s %d, %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2], table_opcode[SUB-1].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2]);
                // Syntax: Intel
                vMEMORY[vMEMORY[--vIP]] -= vMEMORY[vIP+=2]; vIP += 2;
            break;

            case MUL:
                printf("\n      %02X: %02X %02X %02X    ¦    %s %d, %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2], table_opcode[MUL-1].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2]);
                // Syntax: Intel
                vMEMORY[vMEMORY[--vIP]] *= vMEMORY[vIP+=2]; vIP += 2;
            break;

            case DIV:
                printf("\n      %02X: %02X %02X %02X    ¦    %s %d, %d", vIP, vMEMORY[vIP], vMEMORY[vIP+1], vMEMORY[vIP+2], table_opcode[DIV-1].symbolic_name, vMEMORY[vIP+1], vMEMORY[vIP+2]);
                // Syntax: Intel
                vMEMORY[vMEMORY[--vIP]] /= vMEMORY[vIP+=2]; vIP += 2;
            //break;

            _eb_
        }
        //printf("\n      0F: %02X %02X %02X [DEBUG] 015: %03d %03d %03d\t\t\t  %c%c%c", vMEMORY[15], vMEMORY[16], vMEMORY[17], vMEMORY[15], vMEMORY[16], vMEMORY[17], vMEMORY[15], vMEMORY[16], vMEMORY[17]);
        printf("\n----------------------------------------------<•>---------------------------");

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
        unsigned char ch = _getch();
        switch (ch)
        _rb_
        case 0:
            //printf("\n Вы нажали клавишу №1: %02X | %03d | %c", ch, ch, ProcAsciiChr(ch));
            ch = _getch();
            //printf("\n Вы нажали клавишу №2: %02X | %03d | %c\n", ch, ch, ProcAsciiChr(ch));
            switch (ch)
            _rb_
            case '<': // Шаг назад
                Execute('<'); // Launch
                ShowDashboard();
                break;

            case '=': // Полный цикл
                for (int i = 0; i < sizeof (opcode); i++)
                {
                    Execute('=');
                    ShowDashboard();
                    Sleep(2500); // Задержка на 2500 миллисекунд (2,5 секунд)
                }
                break;

            case '>': // Шаг вперёд
                Execute('>'); // Launch
                //ShowDashboard();
                break;

            default:
                printf("\n Вы нажали клавишу №2: %02X | %03d | %c\n", ch, ch, ProcAsciiChr(ch));
            _eb_
            break;

        case 27: // 1B | 027 | ·  <ESC>
            //printf("\n Вы нажали клавишу №1: %02X | %03d | %c", ch, ch, ProcAsciiChr(ch));
            return 0;

        case '<': puts("\n Такая клавиша не определена.");
            //printf("\n Вы нажали клавишу №1: %02X | %03d | %c", ch, ch, ProcAsciiChr(ch));
            break;

        case '=': puts("\n Такая клавиша не определена.");
            //printf("\n Вы нажали клавишу №1: %02X | %03d | %c", ch, ch, ProcAsciiChr(ch));
            break;

        case '>': puts("\n Такая клавиша не определена.");
            //printf("\n Вы нажали клавишу №1: %02X | %03d | %c\n", ch, ch, ProcAsciiChr(ch));
            Execute(ch); // Launch
            ShowDashboard();
            break;

        default:
            puts("\n Такая клавиша не определена.");
            break;
        // 0D | 013 | ·  <ENTER>
        // E0 | 224 | а  <F11-F12>
        _eb_
        goto _0;

        fgets(cmd, sizeof (cmd), stdin); // fgets считывает строку включая пробелы и '\n'
        cmd[strcspn(cmd, "\n")] = '\0';  // Удаляем символ переноса строки '\n', если он есть

        if (!strcmp(cmd, "/cmdlist"))
        {
            puts("\nСписок команд:");
            printf("\n2] /compile");
            printf("\n3] /execute");
            putchar('\n');
        }
        else if (!strcmp(cmd, "/compile"))
        {
            unsigned char data[0xFF] = "";
            unsigned char ptr_data = 0xFF;
            // Считываем с файла
            FILE * desc = fopen("_.asm", "rb");
            if (desc == NULL) { printf("Ошибка открытия файла."); return -1; }
            fseek(desc, 0, SEEK_END);
            size_t file_size = ftell(desc);
            printf("\nРазмер файла: %ld.\n", file_size);
            size_t copy_file_size = file_size;
            fseek(desc, 0, SEEK_SET);
            while (file_size--) data[++ptr_data] = fgetc(desc);
            fclose(desc);
            data[++ptr_data] = '\0';

            printf("\n[file: _.asm]\n'''\n%s\n'''\n", data);
            //file_size = copy_file_size;
            Compile(data, copy_file_size);
        }
        else if (!strcmp(cmd, "/execute")) Execute(0); // Launch
        else printf("Неизвестная/неопознанная команда...");
    }
    return 0;
}
/*
// Формат: printf("\033[<цвет>m<символ>\033[0m");
// \033[0m - сброс цвета обратно на стандартный

printf("\033[31m@\033[0m - Красный символ\n");  // Красный
printf("\033[32m#\033[0m - Зеленый символ\n");  // Зеленый
printf("\033[33m$\033[0m - Желтый символ\n");  // Желтый
printf("\033[34m&\033[0m - Синий символ\n");    // Синий
printf("\033[1;35m*\033[0m - Жирный пурпурный\n"); // Жирный + цвет
*/
    /*char * ptr_op_code = opcode;
    switch (opcode[vIP])
    _rb_
    case PUSH:
        vSTACK[vSP--] = opcode[++vIP];
        break;

    case INT:
        vIP++; // сдвигаем указатель на след. инструкцию
        switch (*++ptr_op_code)
        _rb_
        case PUTCHAR:
            vIP++; // сдвигаем указатель на след. инструкцию
            putchar(opcode[vIP]);
            break;
            
        case PRINTF:
            break;
        _eb_
    _eb_*/
/// Текущая ячейка / Произвольная ячейка
/// ...-> Выборка -> Декодирование -> Исполнение -> Смещение IP на след. инструкцию (автоматически) ->...

// Инструкции и данные находятся в одной секции или разделены?
// Инструкции кодируются как в x86 первые два байта - одним или каждый по раздельности?