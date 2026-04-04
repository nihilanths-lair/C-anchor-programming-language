#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

unsigned char input_data[0xFFFFFF] = "\
";
unsigned char output_data[0xFFFFFF] = "\
";

struct Syntax {
    //char ascii;
    char hex[2+sizeof(char)];
    char dec[3+sizeof(char)];
    char bin[8+sizeof(char)];
} syntax[0xFF+sizeof(char)] = {
    // ... //
};

char syntax_hex[0xFF][2+sizeof(char)];
char syntax_dec[0xFF][3+sizeof(char)];
char syntax_bin[0xFF][3+sizeof(char)];

struct Byte {
    char ascii;
    char hex;
    char dec;
    char bin;
} byte[0xFF];

// Микроассемблирование (микрокомпиляция)
void _16txt_to_ascii()
{
    unsigned char i = 0;
    printf("\n HEX(syntax) -> ASCII(byte)");
    do printf("\n \t  %02X -> %c", i, i);
    while (i++ != 0xFF);
}
void _10txt_to_ascii()
{
    unsigned char i = 0;
    printf("\n DEC(syntax) -> ASCII(byte)");
    do printf("\n \t  %03d -> %c", i, i);
    while (i++ != 0xFF);
}
void _2txt_to_ascii()
{
    // ... //
}
// Микродизассемблирование
void ascii_to_16txt()
{
    unsigned char i = 0;
    printf("\n ASCII(byte) -> HEX(syntax) ");
    do printf("\n \t  %c -> %02X", i, i);
    while (i++ != 0xFF);
}
void ascii_to_10txt()
{
    // ... //
}
void ascii_to_2txt()
{
    // ... //
}

void cnv(const char *input_format, const char *output_format)
{
    /***/if (!strcmp(input_format, "hex.txt") && !strcmp(output_format, "ascii")) _16txt_to_ascii();
    else if (!strcmp(input_format, "dec.txt") && !strcmp(output_format, "ascii")) _10txt_to_ascii();
    else if (!strcmp(input_format, "bin.txt") && !strcmp(output_format, "ascii")) _2txt_to_ascii();
    else if (!strcmp(input_format, "ascii") && !strcmp(output_format, "hex.txt")) ascii_to_16txt();
    else if (!strcmp(input_format, "ascii") && !strcmp(output_format, "dec.txt")) ascii_to_10txt();
    else if (!strcmp(input_format, "ascii") && !strcmp(output_format, "bin.txt")) ascii_to_2txt();
}

void byte_to_binary(unsigned char n, char *buffer)
{
    for (int i = 7; i >= 0; i--)
    {
        // Проверяем i-й бит: если 1, пишем '1', иначе '0'
        buffer[7-i] = (n & (1 << i)) ? '1' : '0';
    }
    buffer[8] = '\0'; // Закрываем строку
}

void byte_to_binary_ultra(unsigned char n, char *b)
{
    b[0] = (n & 0x80) ? '1' : '0';
    b[1] = (n & 0x40) ? '1' : '0';
    b[2] = (n & 0x20) ? '1' : '0';
    b[3] = (n & 0x10) ? '1' : '0';
    b[4] = (n & 0x08) ? '1' : '0';
    b[5] = (n & 0x04) ? '1' : '0';
    b[6] = (n & 0x02) ? '1' : '0';
    b[7] = (n & 0x01) ? '1' : '0';
    b[8] = '\0';
}

unsigned char hex_to_byte_fast(const char *s)
{
    unsigned char res = 0;
    for (int i = 0; i < 2; i++)
    {   // Обрабатываем 2 символа (1 байт)
        char c = s[i];
        res <<= 4; // Сдвигаем на 4 бита влево (освобождаем место под новую цифру)
        
        if (c >= '0' && c <= '9') res |= (c - '0');
        else if (c >= 'A' && c <= 'F') res |= (c - 'A' + 10);
        else if (c >= 'a' && c <= 'f') res |= (c - 'a' + 10);
    }
    return res;
}

#define __ "    "
int main(int argc, char *argv[])
{
    setlocale(0, "");

    printf("\n argc = %d", argc);
    for (int i = 0; i < argc; i++) printf("\n argv[%d] = \"%s\"", i, argv[i]);

    FILE *file = fopen("hex.txt", "rb");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    if (!file_size)
    {
        printf("\n Файл пуст!");
        //return 0;
    }
    char *source_code = malloc(file_size+1);
    if (source_code == NULL)
    {
        printf("\n Не удалось выделить память!");
        return 0;
    }
    size_t bytes_read = fread(source_code, 1, file_size, file);
    fclose(file);
    source_code[bytes_read] = '\0';
    printf("\n %s", source_code);

    file = fopen("ascii.txt", "wb");

    // Правила разбора (парсинга)

    #include <stdint.h>
    uint64_t IP = 0; // 64-bit's
    uint8_t  IP_8  = 0; //  8-bit's
    uint16_t IP_16 = 0; // 16-bit's
    //uint24_t IP_24 = 0; // 24-bit's
    uint32_t IP_32 = 0; // 32-bit's
    uint64_t IP_64 = 0; // 64-bit's

    uint64_t DP = 0; // 64-bit's
    uint8_t  DP_8  = 0; //  8-bit's
    uint16_t DP_16 = 0; // 16-bit's
    uint32_t DP_32 = 0; // 32-bit's
    uint64_t DP_64 = 0; // 64-bit's

    uint64_t SP = 0; // 64-bit's
    uint8_t  SP_8  = 0; //  8-bit's
    uint16_t SP_16 = 0; // 16-bit's
    uint32_t SP_32 = 0; // 32-bit's
    uint64_t SP_64 = 0; // 64-bit's

    //uint32_t TAPE[0xFFFFFF]; // 16 Mb.
    //uint8_t *TAPE = source_code;
    uint8_t TAPE[] = {1, 1};
    //strcpy(TAPE, source_code);
    // Парсинг шестнадцатеричного текстового представления в бинарное
    void *section_data[0xFF];
    void *section_code[] =
    {
        &&identifier_opcode__1,
        &&identifier_opcode__2,
        &&identifier_opcode__3,
        &&identifier_opcode__4, // JMP  8-bit's
        &&identifier_opcode__5, // JMP 64-bit's
        &&identifier_opcode__6,
        &&identifier_opcode__7
    };
    //char opcode = 0; // временно
    _1_run:
    //opcode = *TAPE;
    // В зависимости от вариаций/комбинаций оп-кодов (повторяющие комбинации исключаются, к идентификатору может быть присвоен любой оп-код)
    // Через конфиг. файл считываются идентификаторы и присвоенные им оп-коды, затем определяется табличный адрес ISA и переход на него)
    // Преимущество такого подхода, можно задавать любые оп-коды или даже символьные имена какие вздумается, при этом архитектура никак не меняется (не ломается)

    #define HLT 0x01 // <%1>
    #define JMP 0x02 // <%2>
    #define INC 0x03 // <%3>
    char opcode_configuration_file[] = {INC, JMP, HLT, '\0'};

    void *opcode_table[] = {&&op_1, &&op_2, &&op_3};
    // В процессе загрузки (псевдологика)
    void *current_isa[0x100];
    for (int i = 0; i < sizeof (opcode_configuration_file); i++) current_isa[opcode_configuration_file[i]] = opcode_table[i]; // например, оп-код 0x05 маппим на внутреннюю логику #i
    for (int i = 0; i < sizeof (opcode_configuration_file); i++) printf("\n 0x%p, 0x%p", opcode_table[i], current_isa[i]);

    /*goto *current_isa[0];*/
    // Базовые реализации
    op_1:
     printf("\n <%%1>: ?");
    op_2:
     printf("\n <%%2>: ?");
    op_3:
     printf("\n <%%3>: ?");
    return 0;
    /* Выполнение (Dispatch loop)
    char program[0xFF];
    uint8_t pc;
    exec: goto *current_isa[program[pc++]];
    */

    // void *dispatch_table[0xFF];
    // goto *dispatch_table[?];

    goto *section_code[TAPE[IP]];
    identifier_opcode__1: goto _1_end; // Остановить/прервать выполнение.
    identifier_opcode__2: // Инкрементировать текущую ячейку памяти.
    {
        printf("\n 1: TAPE[IP=%d] = %02X - %03d - %c", IP, TAPE[IP], TAPE[IP], TAPE[IP]);
        TAPE[IP]++;
        printf("\n 2: TAPE[IP=%d] = %02X - %03d - %c", IP, TAPE[IP], TAPE[IP], TAPE[IP]);
        goto *section_code[TAPE[++IP]];
    }
    identifier_opcode__3: // Декрементировать текущую ячейку памяти.
    {
        printf("\n 1: TAPE[IP=%d] = %02X - %03d - %c", IP, TAPE[IP], TAPE[IP], TAPE[IP]);
        TAPE[IP]--;
        printf("\n 2: TAPE[IP=%d] = %02X - %03d - %c", IP, TAPE[IP], TAPE[IP], TAPE[IP]);
        goto *section_code[TAPE[++IP]];
    }
    identifier_opcode__4: // Перейти к пред. ячейки памяти (сдвиг указателя по адресу на шаг назад).
    {
        goto *section_code[TAPE[--IP]];
    }
    identifier_opcode__5: // Перейти к след. ячейки памяти (сдвиг указателя по адресу на шаг вперёд).
    {
        goto *section_code[TAPE[++IP]];
    }
    identifier_opcode__6: // Перейти к произвольной ячейки памяти с 8-ми битной адресацией (сдвиг указателя по произвольному адресу).
    {
        IP = TAPE[++IP];
        goto *section_code[TAPE[IP]];
    }
    identifier_opcode__7: // Перейти к произвольной ячейки памяти с 64-х битной адресацией (сдвиг указателя по произвольному адресу).
    {
        //TAPE++; // Пропускаем сам оп-код инструкции JMP
        //IP = *(uint64_t*) TAPE; // Получаем 8 байт
        //TAPE += 8; // Сдвигаем указатель за пределы адреса
        // Если IP — это адрес внутри source_code, делаем переход:
        // source_code = (unsigned char*)IP;
        //goto *section_code[*TAPE];
        // Syntax: AT&T, порядок байт: big endian.
        // ... //
        // Syntax: Intel, порядок байт: little endian.
        // ... //
    }
    _1_end:

    _2_run:
    switch (*source_code){
    case '\0': goto _2_end;
    case ' ':
    {
        printf("\n %02X - %c", *source_code, *source_code);
        source_code++;
        goto _2_run;
    }
    case '0':
    {
        printf("\n %02X - %c", *source_code, *source_code);
        source_code++;
        goto _2_run;
    }
    case '3':
    {
        printf("\n %02X - %c", *source_code, *source_code);
        source_code++;
        goto _2_run;
    }
    }_2_end:
    fclose(file);
    free(source_code);

    printf(
     "\n/*"
     "\nstruct Syntax {"
     "\n"__"//char ascii;"
     "\n"__"char hex[2+sizeof(char)];"
     "\n"__"char dec[3+sizeof(char)];"
     "\n"__"char bin[8+sizeof(char)];"
     "\n} syntax[0xFF] = {"
    );
    char buf[8+sizeof(char)];
    unsigned char i = 0;
    do
    {
        /*
        sprintf(buf, "%02X", i);
        strcpy(syntax_hex[i], buf);
        sprintf(buf, "%03d", i);
        strcpy(syntax_dec[i], buf);
        byte_to_binary(i, buf);
        strcpy(syntax_bin[i], buf);

        printf("\n"__"\"%s\", \"%s\", \"%s\",", syntax_hex[i], syntax_dec[i], syntax_bin[i]);
        //printf("\n syntax_dec[%d] = \"%s\"", i, syntax_dec[i]);
        //putchar('\n');
        */
    }
    while (i++ != 0xFF);
    printf(
     "\n};"
     "\n*/"
    );
    /*
    cnv("hex.txt", "ascii"); // syntax -> byte
    cnv("ascii", "hex.txt"); // byte -> syntax

    cnv("dec.txt", "ascii"); // syntax -> byte
    cnv("ascii", "dec.txt"); // byte -> syntax

    cnv("bin.txt", "ascii"); // syntax -> byte
    cnv("ascii", "bin.txt"); // byte -> syntax
    */
    while (1){}
    //
    return 0;
}