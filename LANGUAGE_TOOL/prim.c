#include <stdio.h>
#define _ int main
_()
{
    printf("\nswitch-case");
    switch (3){
    case 1: printf("\n1");
    case 2: printf("\n2");
    case 3: printf("\n3");
    case 4: printf("\n4");
    case 5: printf("\n5");
    }
    
    printf("\n\nwhile");
    int i = 3;
    while (i <= 5){
        printf("\n%d", i);
        i++;
    }

    printf("\n\ncomputed goto");
    void * addr[] = {&&case_1, &&case_2, &&case_3, &&case_4, &&case_5};
    goto * addr[3-1];
    case_1: printf("\n1");
    case_2: printf("\n2");
    case_3: printf("\n3");
    case_4: printf("\n4");
    case_5: printf("\n5");

    putchar('\n');
}