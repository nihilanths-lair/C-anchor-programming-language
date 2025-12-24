#include <stdio.h>
#define _ int main
int i;
_()
{
    printf("\nswitch-case");
    i = 3;
    switch (i){
    case 1: printf("\n1"); // break;
    case 2: printf("\n2"); // break;
    case 3: printf("\n3"); // break;
    case 4: printf("\n4"); // break;
    default: printf("\n0 || 5 .. max");
    }
    
    printf("\n\nwhile");
    i = 3;
    while (i <= 4){
        printf("\n%d", i);
        i++;
    }
    printf("\n0 || 5 .. max");

    printf("\n\ncomputed goto");
    void * addr[] = {&&case_1, &&case_2, &&case_3, &&case_4};
    i = 3-1;
    if (i > 0 && i < 5) goto * addr[i];
    else goto default_;
    case_1: printf("\n1"); // goto forward_;
    case_2: printf("\n2"); // goto forward_;
    case_3: printf("\n3"); // goto forward_;
    case_4: printf("\n4"); // goto forward_;
    default_: printf("\n0 || 5 .. max"); // goto forward_;
    forward_:

    putchar('\n');
}