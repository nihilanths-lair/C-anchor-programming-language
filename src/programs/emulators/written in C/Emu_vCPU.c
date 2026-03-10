// <!-- Encoding: Windows-1251 -->
#include <stdint.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

//#define opening_block {
//#define closing_block }

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
//char _data_2[0xFF];

void Preprocessing(const unsigned char * text)
{
    puts("\n>> Preprocessing()");
    printf("%s", text);
    int i = 0;
    _loop_run:
    switch (text[i]){
    // 0-9 | 48-57
    case (unsigned char)'0': {} break;
    case (unsigned char)'1': {} break;
    case (unsigned char)'2': {} break;
    case (unsigned char)'3': {} break;
    case (unsigned char)'4': {} break;
    case (unsigned char)'5': {} break;
    case (unsigned char)'6': {} break;
    case (unsigned char)'7': {} break;
    case (unsigned char)'8': {} break;
    case (unsigned char)'9': {} break;
    // A-Z | 65-90
    case (unsigned char)'A': {} break;
    case (unsigned char)'B': {} break;
    case (unsigned char)'C': {} break;
    case (unsigned char)'D': {} break;
    case (unsigned char)'E': {} break;
    case (unsigned char)'F': {} break;
    case (unsigned char)'G': {} break;
    case (unsigned char)'H': {} break;
    case (unsigned char)'I': {} break;
    case (unsigned char)'J': {} break;
    case (unsigned char)'K': {} break;
    case (unsigned char)'L': {} break;
    case (unsigned char)'M': {} break;
    case (unsigned char)'N': {} break;
    case (unsigned char)'O': {} break;
    case (unsigned char)'P': {} break;
    case (unsigned char)'Q': {} break;
    case (unsigned char)'R': {} break;
    case (unsigned char)'S': {} break;
    case (unsigned char)'T': {} break;
    case (unsigned char)'U': {} break;
    case (unsigned char)'V': {} break;
    case (unsigned char)'W': {} break;
    case (unsigned char)'X': {} break;
    case (unsigned char)'Y': {} break;
    case (unsigned char)'Z': {} break;
    // a-z | 97-122
    case (unsigned char)'a': {} break;
    case (unsigned char)'b': {} break;
    case (unsigned char)'c': {} break;
    case (unsigned char)'d': {} break;
    case (unsigned char)'e': {} break;
    case (unsigned char)'f': {} break;
    case (unsigned char)'g': {} break;
    case (unsigned char)'h': {} break;
    case (unsigned char)'i': {} break;
    case (unsigned char)'j': {} break;
    case (unsigned char)'k': {} break;
    case (unsigned char)'l': {} break;
    case (unsigned char)'m': {} break;
    case (unsigned char)'n': {} break;
    case (unsigned char)'o': {} break;
    case (unsigned char)'p': {} break;
    case (unsigned char)'q': {} break;
    case (unsigned char)'r': {} break;
    case (unsigned char)'s': {} break;
    case (unsigned char)'t': {} break;
    case (unsigned char)'u': {} break;
    case (unsigned char)'v': {} break;
    case (unsigned char)'w': {} break;
    case (unsigned char)'x': {} break;
    case (unsigned char)'y': {} break;
    case (unsigned char)'z': {} break;
    // А-я | 192-255
    case (unsigned char)'А': {} break;
    case (unsigned char)'Б': {} break;
    case (unsigned char)'В': {} break;
    case (unsigned char)'Г': {} break;
    case (unsigned char)'Д': {} break;
    case (unsigned char)'Е': {} break;
    case (unsigned char)'Ж': {} break;
    case (unsigned char)'З': {} break;
    case (unsigned char)'И': {} break;
    case (unsigned char)'Й': {} break;
    case (unsigned char)'К': {} break;
    case (unsigned char)'Л': {} break;
    case (unsigned char)'М': {} break;
    case (unsigned char)'Н': {} break;
    case (unsigned char)'О': {} break;
    case (unsigned char)'П': {} break;
    case (unsigned char)'Р': {} break;
    case (unsigned char)'С': {} break;
    case (unsigned char)'Т': {} break;
    case (unsigned char)'У': {} break;
    case (unsigned char)'Ф': {} break;
    case (unsigned char)'Х': {} break;
    case (unsigned char)'Ц': {} break;
    case (unsigned char)'Ч': {} break;
    case (unsigned char)'Ш': {} break;
    case (unsigned char)'Щ': {} break;
    case (unsigned char)'Ъ': {} break;
    case (unsigned char)'Ы': {} break;
    case (unsigned char)'Ь': {} break;
    case (unsigned char)'Э': {} break;
    case (unsigned char)'Ю': {} break;
    case (unsigned char)'Я': {} break;
    case (unsigned char)'а': {} break;
    case (unsigned char)'б': {} break;
    case (unsigned char)'в': {} break;
    case (unsigned char)'г': {} break;
    case (unsigned char)'д': {} break;
    case (unsigned char)'е': {} break;
    case (unsigned char)'ж': {} break;
    case (unsigned char)'з': {} break;
    case (unsigned char)'и': {} break;
    case (unsigned char)'й': {} break;
    case (unsigned char)'к': {} break;
    case (unsigned char)'л': {} break;
    case (unsigned char)'м': {} break;
    case (unsigned char)'н': {} break;
    case (unsigned char)'о': {} break;
    case (unsigned char)'п': {} break;
    case (unsigned char)'р': {} break;
    case (unsigned char)'с': {} break;
    case (unsigned char)'т': {} break;
    case (unsigned char)'у': {} break;
    case (unsigned char)'ф': {} break;
    case (unsigned char)'х': {} break;
    case (unsigned char)'ц': {} break;
    case (unsigned char)'ч': {} break;
    case (unsigned char)'ш': {} break;
    case (unsigned char)'щ': {} break;
    case (unsigned char)'ъ': {} break;
    case (unsigned char)'ы': {} break;
    case (unsigned char)'ь': {} break;
    case (unsigned char)'э': {} break;
    case (unsigned char)'ю': {} break;
    case (unsigned char)'я': {} break;
    }
    // Generator 'case'
    putchar('\n');
    //0-9 | 48-57
    //for (int i = 48; i <= 57; i++) printf("case (unsigned char)'%c': {} break;\n", i);
    //A-Z | 65-90
    //for (int i = 65; i <= 90; i++) printf("case (unsigned char)'%c': {} break;\n", i);
    //a-z | 97-122
    //for (int i = 97; i <= 122; i++) printf("case (unsigned char)'%c': {} break;\n", i);
    //А-я | 192-255
    //for (int i = 192; i <= 255; i++) printf("case (unsigned char)'%c': {} break;\n", i);
}

void Compile(const unsigned char * text) // size_t len
{
    puts("\n>> Compile()");
    Preprocessing(text);
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
    printf("\n    HEX   DEC    ASCII\n");
    //for (int i = 0; i < 256; i++)
    printf("IP  [%02X]  [%03d]  [%c]\n", vIP, vIP, ProcAsciiChr(vIP));
    printf("SP  [%02X]  [%03d]  [%c]\n", vSP, vSP, ProcAsciiChr(vSP));
    //printf("DI  [%02X]  [%03d]  [%c]\n", vDI, vDI, ProcAsciiChr(vDI));
    //printf("SI  [%02X]  [%03d]  [%c]\n", vSI, vSI, ProcAsciiChr(vSI));
    char op = 0;
    switch (op){
    case 0:
    {
        printf("\nMEMORY (HEX | DEC | ASCII)\n    ");
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
            for (int i = 0; i < 0x0F + 1; i++)
            {
                printf(" %03d ", vMEMORY[i]);
            }
        }
    }
    break;
    case 16:
    {
        printf("MEMORY (HEX)\n    ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%02X]", i);
        for (int j = 0; j < 0x0F + 1; j++)
        {
            printf("\n[%02X]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F + 1; i++)
            {
                printf(" %02X ", vMEMORY[i]);
            }
        }
    }
    break;
    }
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
    long file_size = ftell(desc);
    printf("\nРазмер файла: %ld.\n", file_size);
    long copy_file_size = file_size;
    fseek(desc, 0, SEEK_SET);
    while (file_size--) data[++ptr_data] = fgetc(desc);
    fclose(desc);
    data[++ptr_data] = '\0';

    printf("\n[file: _.asm]\n'''\n%s\n'''\n", data);
    /*
    file_size = copy_file_size;

    // Записываем в файл
    desc = fopen("_.bin", "wb");
    if (desc == NULL) { printf("Ошибка открытия файла."); return -1; }
    ptr_data = 0xFF;
    while (file_size--) fputc(data[++ptr_data], desc);
    fclose(desc);
    */
    Compile(data);
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