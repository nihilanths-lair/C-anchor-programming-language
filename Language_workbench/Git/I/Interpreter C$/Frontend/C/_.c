#include <stdio.h>

#include "lexer.h"

int main()
{
    char input[100];
    while (1)
    {
        printf(">>> ");
        fgets(input, sizeof (input), stdin);
    }

    return 0;
}