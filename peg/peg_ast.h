/*/ Типы узлов PEG /*/
enum PNodeType {
    P_LITERAL,       // "abc"
    P_SEQUENCE,      // A B
    P_CHOICE,        // A / B
    P_ZERO_OR_MORE,  // A*
    P_ONE_OR_MORE,   // A+
    P_OPTIONAL,      // A?
    P_RULE_REF,      // name
    P_RULE           // name = expr
};

/*/ Структура узла /*/
typedef struct PNode {
    enum PNodeType type;
    char *text;               // для literal и rule name
    struct PNode **children;  // массив детей
    int child_count;          // кол-во детей
} PNode;

PNode* pnode_new(enum PNodeType type, const char *text);
void   pnode_add(PNode *parent, PNode *child);
void   pnode_free(PNode *node);