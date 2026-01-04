#include <stdio.h>
#include <locale.h>
#include <string.h>

#include "lexer.h"

int main()
{
    setlocale(0, "");

    FILE *file_descriptor = fopen("_.txt", "r");
    if (file_descriptor == NULL) perror("Код возврата: 1.");

    char file_input[0xFF];
    size_t len = fread(file_input, 1, sizeof (file_input)-1, file_descriptor);
    file_input[len] = '\n';

    fclose(file_descriptor);

    //printf("Файловой ввод: %s.", file_input);
    putchar('\n');
    int i = 0;
    do
    {
        switch (file_input[i])
        {
            case '\n':
                printf("[%d] = '\\n'\n", i);
                break;
            
            default:
                printf("[%d] = '%c'\n", i, file_input[i]);
        }
        
    }
    while (file_input[i++] != '\n');

    char console_input[0xFF];
    while (1)
    {
        printf("\n>>> ");
        fgets(console_input, sizeof (console_input), stdin);
        putchar('\n');
        int i = 0;
        do
        {
            switch (console_input[i])
            {
                case '\n':
                    printf("[%d] = '\\n'\n", i);
                    break;
                
                default:
                    printf("[%d] = '%c'\n", i, console_input[i]);
            }
            
        }
        while (console_input[i++] != '\n');
        //console_input[strcspn(console_input, "\n")] = '\0';
    }

    return 0;
}