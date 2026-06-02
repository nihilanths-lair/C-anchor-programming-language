#include <stdio.h>
#include <locale.h>

char source_code[0xFFFFFF];

int main(int argc, char *argv[])
{
    setlocale(0, "");
    if (argc < 2) { return 1; }
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) { return 2; }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    fread(source_code, sizeof (char), file_size, file);
    source_code[file_size] = '\0';
    fclose(file);
    for (int i = 0; i < file_size; i++) { fputc(source_code[i], stderr); }
    for (int i = 0; i < file_size; i++) { if (source_code[i] != '\r') { putchar(source_code[i]); } }
    return 0;
}

/*
 @echo off
 tcc\tcc.exe compiler_stage_0.c -o compiler_stage_0.exe
 ::compiler_stage_0.exe main.meta > nul
 compiler_stage_0.exe main.meta > compiler_stage_1.c
*/