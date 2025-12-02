#include <stdio.h>
#include <locale.h>

#include "peg_ast.h"

int main()
{
    setlocale(0, "");

    PNode *expr_rule = pnode_new(P_RULE, "expr");
    PNode *choice = pnode_new(P_CHOICE, NULL);
    pnode_add(choice, pnode_new(P_RULE_REF, "term"));
    pnode_add(choice, pnode_new(P_RULE_REF, "number"));
    pnode_add(expr_rule, choice);

    return 0;
}