#include <stdio.h>
#include <locale.h>
#include <string.h>

unsigned char input_data[0xFFFFFF] = "\
";
unsigned char output_data[0xFFFFFF] = "\
";

void hextxt_to_ascii(){}//16
void dectxt_to_ascii(){}//10
void bintxt_to_ascii(){}//2
void ascii_to_hextxt(){}
void ascii_to_dectxt(){}
void ascii_to_bintxt(){}

void cnv(const char *input_format, const char *output_format)
{
    /***/if (!strcmp(input_format, "hex.txt") && !strcmp(output_format, "ascii.txt")) hextxt_to_ascii();
    else if (!strcmp(input_format, "dec.txt") && !strcmp(output_format, "ascii.txt")) dectxt_to_ascii();
    else if (!strcmp(input_format, "bin.txt") && !strcmp(output_format, "ascii.txt")) bintxt_to_ascii();
    else if (!strcmp(input_format, "ascii") && !strcmp(output_format, "hex.txt")) ascii_to_hextxt();
    else if (!strcmp(input_format, "ascii") && !strcmp(output_format, "dec.txt")) ascii_to_dectxt();
    else if (!strcmp(input_format, "ascii") && !strcmp(output_format, "bin.txt")) ascii_to_bintxt();
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