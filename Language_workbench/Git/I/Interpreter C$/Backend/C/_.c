#include <stdio.h>
#include <locale.h>
#include <string.h>
/*unsigned*/char console_input[0xFF];
int main(int argc, char *argv[])
{
    setlocale(0, "");

    puts("\n Интерпретатор C$ запущен!");

    printf("<#> argc: %d", argc);
    for (unsigned char i = 0; i < argc; i++) printf("\n<#> argv[%d]: \"%s\"", i, argv[i]);

    switch (argc){
        case 1:
        {
            break;
        }
        case 2:
        {
            enum file_extension { file_extension__cdlr, file_extension__asm, file_extension__bin } en__file_extension;
            // Проверить аргумент на расширение файла
            unsigned char option = 0;

            short i = -1;
            _loop: switch (argv[1][++i]){
                case '\0': printf("\n'\\0'"); break;
                case '.': 
                {
                    //printf("\n'.'");
                    char file_extension[4+1];
                    file_extension[0] = '.';
                    file_extension[1] = argv[1][++i];
                    file_extension[2] = argv[1][++i];
                    file_extension[3] = argv[1][++i];
                    file_extension[4] = '\0';
                    argv[1][i] = '\0';
                    //printf("\n file_extension: \"%s\"", file_extension);
                    if (!strcmp(file_extension, ".asm")) { printf("\n Расширение файла: .asm"); option = file_extension__asm; }
                    else if (!strcmp(file_extension, ".bin")) { printf("\n Расширение файла: .bin"); option = file_extension__bin; }
                    else
                    {
                        file_extension[4] = argv[1][++i];
                        file_extension[5] = '\0';
                        argv[1][i] = '\0';
                        //printf("\n file_extension: \"%s\"", file_extension);
                        if (!strcmp(file_extension, ".cdlr")) { printf("\n Расширение файла: .cdlr"); option = file_extension__cdlr; }
                    }
                    break;
                }
                default: goto _loop;
            }
            /*
            short len = strlen(argv[1]);// - 5;
            int *pos_argv = (int *) argv[1]; // адрес начала
            char *itr_argv = (char *) argv[1];
            switch (*itr_argv){
                case '\0': printf("'%c'", *itr_argv); break;
                case '.': printf("'%c'", *itr_argv); break;
            }
            printf("\n argv[1]: \"%s\"", argv[1]);
            printf("\n len: %d, argv[1][%d]: '%c'", len, len, argv[1][len]);
            */
        }
    }

    unsigned char loop = 1;
    while (loop)
    {
        printf("\n>>> ");
        fgets(console_input, sizeof (console_input), stdin);
        console_input[strcspn(console_input, "\n")] = '\0';

        printf("<#> %s", console_input);
        //loop = 0;
    }
    //printf("<#> %s", console_input);
    return 0;
}