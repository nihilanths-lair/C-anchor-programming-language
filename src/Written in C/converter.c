#include <stdio.h>
#include <locale.h>
#include <string.h>

unsigned char input_data[0xFFFFFF] = "\
";
unsigned char output_data[0xFFFFFF] = "\
";

struct Byte {
    char ascii;
    char hex;
    char dec;
    char bin;
} byte[0xFF];

struct Syntax {
    char ascii;
    char hex[2+sizeof(char)];
    char dec[3+sizeof(char)];
    char bin[8+sizeof(char)];
} syntax[0xFF];

// Микроассемблирование (микрокомпиляция)
void _16txt_to_ascii(){}
void _10txt_to_ascii(){}
void _2txt_to_ascii(){}
// Микродизассемблирование
void ascii_to_16txt(){}
void ascii_to_10txt(){}
void ascii_to_2txt(){}

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
    cnv("hex.txt", "ascii.txt"); // syntax -> byte
    cnv("ascii.txt", "hex.txt"); // byte -> syntax

    cnv("dec.txt", "ascii.txt"); // syntax -> byte
    cnv("ascii.txt", "dec.txt"); // byte -> syntax

    cnv("bin.txt", "ascii.txt"); // syntax -> byte
    cnv("ascii.txt", "bin.txt"); // byte -> syntax
    //
    return 0;
}