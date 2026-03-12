// <!-- Encoding: Windows-1251 -->
#include <stdint.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define _rb_ {
#define _eb_ }

char ProcAsciiChr(unsigned char chr)
{
    switch (chr) {
    case '\0'/*000*/: return '·';
    case 0x07/*007*/: return '·';
    case 0x08/*008*/: return '·';
    case 0x09/*009*/: return '·';
    case '\n'/*010*/: return '·';
    case '\r'/*013*/: return '·';
    case 0x1b/*027*/: return '·';
    case 0x95/*149*/: return '·';
    // 30-39 или 048-057: 0-9
    // 41-5A или 065-090: A-Z
    // 61-7A или 097-122: a-z
    //    A8 или     168: Ё
    //    B8 или     184: ё
    // C0-DF или 192-223: А-Я
    // E0-FF или 224-255: а-я
    default: return chr;
    }
}

uint8_t vIP;
uint8_t vSP;
uint8_t vDI;
uint8_t vSI;

uint8_t vMEMORY[0xFF];
uint8_t vSTACK[0xFF];

char _data_1[0xFF];
char * _ptr_data_1 = _data_1;

char processed_text[0xFF];
unsigned char idx__processed_text;
//char _data_2[0xFF];

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

bool strcmpex(const char * str1, const char * str2)
{
    return false;
}
bool scaner(const char * str1, const char * str2)
{
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

    // cdlr -E 1 file_name.cdlr > file_name.cdlr (будет получен файл без комментариев)
    // cdlr -E 2 file_name.cdlr > file_name.cdlr (будет получен файл с развёрнутыми макросами)
    // cdlr -E 3 file_name.cdlr > file_name.cdlr (будет получен файл без комментариев и с развёрнутыми макросами)

    // Для отладочной информации можно выбрать либо только замена макросов (их развёртка) с сохранением комментариев, либо удаление только комментариев с сохранением макросов
    // Для релизного выходного файла используется вариант с заменой макросов (их развёрткой) и удалением комментариев

    printf("\n----\n<До>\n----\n%s\n\n", text);
    for (int i = 0; text[i] != '\0'; i++) printf("%c", ProcAsciiChr(text[i]));

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
    JMP <?>
    ...
    <?>:
    /--/
    <?>:
    ...
    JMP <?>
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

void Compile(char * text, size_t file_size)
{
    printf("\n>> Compile()");
    Preprocessing(text, 1, file_size);
}

void LoadingProgramIntoMemory(const char * opcode)
{
    puts("\n>> LoadingProgramIntoMemory()");
    for (int i = 0; opcode[i] != -1; i++) vMEMORY[i] = opcode[i];
    //for ( ; opcode[i] != '\0'; i++) vMEMORY[i] = '\0';
    //printf("opcode >> %lld", len);
    //for (int i = 0; i < len; i++) vMEMORY[i] = opcode[i];
}

void ShowDashboard()
{
    printf("\n    HEX | DEC | ASCII\n");
    //for (int i = 0; i < 256; i++)
    printf(" IP: %02X | %03d | %c\n", vIP, vIP, ProcAsciiChr(vIP));
    printf(" SP: %02X | %03d | %c\n", vSP, vSP, ProcAsciiChr(vSP));
    //printf("DI  [%02X]  [%03d]  [%c]\n", vDI, vDI, ProcAsciiChr(vDI));
    //printf("SI  [%02X]  [%03d]  [%c]\n", vSI, vSI, ProcAsciiChr(vSI));
    char op = 0;
    switch (op){
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
    }}
}

// Шаг вперёд
void StepForward(){}
// Шаг назад
//void StepBack(){}

enum UppercaseLetters // Заглавные буквы
{
    MOV = 1,
    INC, DEC,
    ADD, SUB, MUL, DIV,
    JMP,
    PUSH,
    INT,
};
enum LowercaseLetters // Строчные буквы
{
    mov = 1,
    inc, dec,
    add, sub, mul, div,
    jmp,
    push,
    _int,
};
struct TableOpcode {
    unsigned char identifier;
    char symbolic_name[0xF+1];
    unsigned char opcode;
} table_opcode[0xFF] = {
    1, "JMP", JMP,
    2, "jmp", jmp
};
enum IVT {
    PUTCHAR = 1,
    PRINTF
};

int main()
{
    setlocale(0, "");

    char cmd[128+1];
    while (false)
    {
        printf("CMD> ");

        fgets(cmd, sizeof (cmd), stdin); // fgets считывает строку включая пробелы и '\n'
        cmd[strcspn(cmd, "\n")] = '\0';  // Удаляем символ переноса строки '\n', если он есть
    }

    //ShowDashboard();
    //vIP = 0x00;
    vSP = 0xFE;
    //vDI = 0x00;
    //vSI = 0x00;
    //ShowDashboard();

    unsigned char data[0xFF] = "";
    unsigned char ptr_data = 0xFF;
    FILE *desc;
    // Считываем с файла
    desc = fopen("_.asm", "rb");
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
    /*
    Compile(
        "JMP _\n"
        ""
        "_: ; точка входа в программу\n"
        "MOV 0, 'Г'\n"
        ";-\n"
        "MOV 1, 'л'\n"
        "MOV 2, 'е'\n"
        "-;\n"
        "MOV 3, 'б'"
    );
    ///
    /*
    >> Compile()
    JMP _
    _: ; точка входа в программу
    MOV 0, 'Г'
    ;-
    MOV 1, 'л'
    MOV 2, 'е'
    -;
    MOV 3, 'б'
    */
    // Syntax AT&T / Intel
    unsigned char opcode[] =
    {
        /*
        MOV, 0, 'Г', // MOV 0, 'Г' | 01 00 C3 | 001 000 195 | ··Г
        MOV, 1, 'л', // MOV 1, 'л' | 01 01 EB | 001 001 235 | ··л
        MOV, 2, 'е', // MOV 2, 'е' | 01 02 E5 | 001 002 229 | ··е
        MOV, 3, 'б', // MOV 3, 'б' | 01 03 E1 | 001 003 225 | ··б
        MOV, 4, '.', // MOV 4, '.' | 01 04 2E | 001 004 046 | ··.
        INC, 4,      // INC 4      | 02 04    | 002 004     | ··
        DEC, 4,      // DEC 4      | 03 04    | 003 004     | ··
        MOV, 255, 5, // MOV 255, 5 | 01 FF 05 | 001 255 005 | ·я·
        */
        MOV, 254,  2, // MOV 254, 2 | 01 FE 02 | 001 254 002 | ·ю·
        //MOV, 255, 3,// MOV 255, 3 | 01 FF 03 | 001 255 003 | ·я·
        ADD, 254,  3, // ADD 254, 3 | 04 FE 03 | 004 254 003 | ·ю·

        MOV, 255, 11, // MOV 255, 11 | 01 FF 0B | 001 255 011 | ·я·
        SUB, 255,  4, // SUB 255, 4  | 05 FF 04 | 005 255 004 | ·я·
        EOF
    };
    LoadingProgramIntoMemory(opcode);
    vIP = 0; // Инициализация
    //printf("vIP = %d", vIP);
    for (int i = 0; i < 4; i++){
        switch (opcode[vIP]){
        case MOV: {
            // Intel (помещение данных в произвольную ячейку памяти)
            vMEMORY[opcode[--vIP]] = opcode[vIP+=2];
            vIP += 2;
            // AT&T (помещение данных в произвольную ячейку памяти)
            //vMEMORY[opcode[++vIP]] = opcode[++vIP];
            //++vIP;
        } break;
        case INC: {
            vMEMORY[opcode[++vIP]]++;
            vIP++;
        } break;
        case DEC: {
            vMEMORY[opcode[++vIP]]--;
            vIP++;
        } break;
        case ADD: {
            // Intel (сложение данных в произвольной ячейки памяти)
            vMEMORY[opcode[--vIP]] += opcode[vIP+=2];
            vIP += 2;
            // AT&T (сложение данных в произвольной ячейки памяти)
            //vMEMORY[opcode[--vIP]] = opcode[--vIP] + opcode[vIP+=2];
            //vIP += 2;
        } break;
        case SUB: {
            // Intel (вычитание данных в произвольной ячейки памяти)
            vMEMORY[opcode[--vIP]] -= opcode[vIP+=2];
            vIP += 2;
        } break;
        case MUL: {
            // Intel (вычитание данных в произвольной ячейки памяти)
            vMEMORY[opcode[--vIP]] *= opcode[vIP+=2];
            vIP += 2;
        } break;
        case DIV: {
            // Intel (вычитание данных в произвольной ячейки памяти)
            vMEMORY[opcode[--vIP]] /= opcode[vIP+=2];
            vIP += 2;
        } break;
        case JMP: {
            vIP = opcode[++vIP];
        }}
        ShowDashboard();
    }
    char * ptr_op_code = opcode;
    while (false)
    {
        switch (opcode[vIP]){
        case MOV:
        {
            vMEMORY[vIP+1] = opcode[vIP+2];
        } break;
        case PUSH: vSTACK[vSP--] = opcode[++vIP];
        return 0;
        break;
        case INT:
        {
            vIP++; // сдвигаем указатель на след. инструкцию
            switch (*++ptr_op_code){
            case PUTCHAR:
            {
                vIP++; // сдвигаем указатель на след. инструкцию
                putchar(opcode[vIP]);
            } break;
            case PRINTF:
            {
                //printf("", );
            } break; }
        } break; }
    }

    return 0;
}
/// Текущая ячейка / Произвольная ячейка
/// ...-> Выборка -> Декодирование -> Исполнение -> Смещение IP на след. инструкцию (автоматически) ->...

// Инструкции и данные находятся в одной секции или разделены?
// Инструкции кодируются как в x86 первые два байта - одним или каждый по раздельности?