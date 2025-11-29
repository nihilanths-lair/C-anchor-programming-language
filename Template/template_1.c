#include <stdio.h>

/************************************************************
 *                     CONSTANTS & ENUMS
 ************************************************************/

// --- Token types ---
typedef enum {
    TOK_EOF,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_OPERATOR,
    TOK_KEYWORD,
    TOK_PUNCTUATION,
    // ...
} TokenType;

// --- Grammar rule types ---
typedef enum {
    RULE_SEQUENCE,
    RULE_ALTERNATIVE,
    RULE_NONTERM,
    RULE_TERM,
} RuleType;


/************************************************************
 *                       DATA STRUCTURES
 ************************************************************/

// ------------ Token ------------
typedef struct {
    TokenType type;
    const char *lexeme;
    int length;
    int line;
    int column;
} Token;


// ------------ Lexer State ------------
typedef struct {
    const char *src;
    int pos;
    int line;
    int column;
} Lexer;


// ------------ Grammar Rule ------------
typedef struct GrammarRule {
    RuleType type;
    const char *name;        // имя нетерминала
    struct GrammarRule **children;
    int child_count;
} GrammarRule;


// ------------ AST Node ------------
// (пока не важно, просто структура)
typedef struct ASTNode {
    const char *type;
    struct ASTNode **children;
    int child_count;
    Token token;
} ASTNode;


/************************************************************
 *                    FUNCTION PROTOTYPES
 ************************************************************/

/**************** LEXER ****************/
// Инициализация лексера
void lexer_init(const char *source);

// Получить следующий токен
Token lexer_next();

// Посмотреть текущий токен без продвижения
Token lexer_peek();


/**************** GRAMMAR ****************/
// Загрузить базовую грамматику языка
void grammar_load_default();

// Вывести грамматику (для отладки)
void grammar_dump();


/**************** PARSER ****************/
// Основной вход в парсер: parse(NonTerminal)
ASTNode *parse_program();

// Примеры для выражений (в будущем)
ASTNode *parse_expression();
ASTNode *parse_term();
ASTNode *parse_factor();


/**************** AST ****************/
// Создать AST узел
ASTNode *ast_new_node(const char *type);

// Добавить ребёнка
void ast_add_child(ASTNode *parent, ASTNode *child);

// Вывести AST (debug)
void ast_print(ASTNode *node, int indent);


/**************** UTILS ****************/
// Ошибка компиляции
void compile_error(const char *msg);


/************************************************************
 *                          MAIN
 ************************************************************/

int main() {

    grammar_load_default();

    lexer_init("1 + 2 * 3");
    ASTNode *root = parse_program();

    ast_print(root, 0);

    return 0;
}


/************************************************************
 *                FUNCTION IMPLEMENTATIONS
 ************************************************************/

/**************** LEXER IMPLEMENTATION ****************/

void lexer_init(const char *source) {
    // TODO: Реализуешь сам
}

Token lexer_next() {
    Token t = {0};
    // TODO
    return t;
}

Token lexer_peek() {
    Token t = {0};
    // TODO
    return t;
}


/**************** GRAMMAR IMPLEMENTATION ****************/

void grammar_load_default() {
    // TODO: Здесь будешь добавлять базовые грамматические правила
}

void grammar_dump() {
    // TODO
}


/**************** PARSER IMPLEMENTATION ****************/

ASTNode *parse_program() {
    // TODO (здесь будет разбор выражений/операторов/блоков)
    return NULL;
}

ASTNode *parse_expression() { return NULL; }
ASTNode *parse_term() { return NULL; }
ASTNode *parse_factor() { return NULL; }


/**************** AST IMPLEMENTATION ****************/

ASTNode *ast_new_node(const char *type) {
    // TODO
    return NULL;
}

void ast_add_child(ASTNode *parent, ASTNode *child) {
    // TODO
}

void ast_print(ASTNode *node, int indent) {
    // TODO
}


/**************** UTILS IMPLEMENTATION ****************/

void compile_error(const char *msg) {
    // TODO
}