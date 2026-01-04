#include <stdio.h>
#include <string.h>

#include "lexer.h"

int main()
{
    char input[100];
    while (1)
    {
        printf("\n>>> ");
        fgets(input, sizeof (input), stdin);
        int i = 0;
        do
        {
            switch (input[i])
            {
                case '\n':
                    printf("[%d] = '\\n'\n", i);
                    break;
                
                default:
                    printf("[%d] = '%c'\n", i, input[i]);
            }
            
        }
        while (input[i++] != '\n');
        printf("pos '\\n': %d", strcspn(input, "\n"));
        //input[strcspn(input, "\n")] = '\0';
    }

    return 0;
}