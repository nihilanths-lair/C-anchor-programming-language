#include <stdio.h>
#include <locale.h>
#include <string.h>

unsigned char input_data[0xFFFFFF] = "\
";
unsigned char output_data[0xFFFFFF] = "\
";

/*
struct Syntax {
    char ascii;
    char hex[2+sizeof(char)];
    char dec[3+sizeof(char)];
    char bin[8+sizeof(char)];
} syntax[0xFF];
*/
char syntax_hex[0xFF][2+sizeof(char)];
char syntax_dec[0xFF][3+sizeof(char)];

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

}
void ascii_to_2txt()
{

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

int main()
{
    setlocale(0, "");
    //
    char buf[8+sizeof(char)];
    unsigned char i = 0;
    do
    {
        sprintf(buf, "%02X", i);
        strcpy(syntax_hex[i], buf);
        sprintf(buf, "%03d", i);
        strcpy(syntax_dec[i], buf);

        printf("\n syntax_hex[%d] = \"%s\"", i, syntax_hex[i]);
        printf("\n syntax_dec[%d] = \"%s\"", i, syntax_dec[i]);
        putchar('\n');
    }
    while (i++ != 0xFF);

    /*
    cnv("hex.txt", "ascii"); // syntax -> byte
    cnv("ascii", "hex.txt"); // byte -> syntax

    cnv("dec.txt", "ascii"); // syntax -> byte
    cnv("ascii", "dec.txt"); // byte -> syntax

    cnv("bin.txt", "ascii"); // syntax -> byte
    cnv("ascii", "bin.txt"); // byte -> syntax
    */
    //
    return 0;
}