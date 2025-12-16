// ================================================================
// bootstrap.c — минимальный PEG-интерпретатор + парсер грамматики
// Производственный стиль, но с учебными комментариями
// ================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ================================================================
// 1. БАЗОВАЯ УТИЛИТА ДЛЯ ПАМЯТИ
// ================================================================

static void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p) { fprintf(stderr, "Out of memory\n"); exit(1); }
    return p;
}

// ================================================================
// 2. ЛЕКСЕР
// Разбивает вход на токены: идентификаторы, строки, операторы.
// ================================================================

typedef enum {
    T_EOF,
    T_IDENT,
    T_STRING,
    T_ARROW,    // =~
    T_EQ,       // =
    T_SLASH,    // /
    T_STAR,     // *
    T_PLUS,     // +
    T_QMARK,    // ?
    T_LPAREN,   // (
    T_RPAREN,   // )
    T_SEMI      // ;
} TokenKind;

typedef struct {
    TokenKind kind;
    char text[128];
} Token;

static const char *src;
static Token tok;

// Переходим к следующему токену
static void next_token() {
    while (isspace(*src)) src++;

    if (*src == '\0') {
        tok.kind = T_EOF;
        return;
    }

    // Идентификатор
    if (isalpha(*src) || *src == '_') {
        char *p = tok.text;
        while (isalnum(*src) || *src == '_')
            *p++ = *src++;
        *p = 0;
        tok.kind = T_IDENT;
        return;
    }

    // Строковый литерал "..."
    if (*src == '"') {
        src++;
        char *p = tok.text;
        while (*src && *src != '"')
            *p++ = *src++;
        *p = 0;
        if (*src == '"') src++;
        tok.kind = T_STRING;
        return;
    }

    // Операторы
    if (src[0] == '=' && src[1] == '~') {
        tok.kind = T_ARROW;
        strcpy(tok.text, "=~");
        src += 2;
        return;
    }

    char c = *src++;
    tok.text[0] = c;
    tok.text[1] = 0;

    switch (c) {
        case '=': tok.kind = T_EQ; break;
        case '/': tok.kind = T_SLASH; break;
        case '*': tok.kind = T_STAR; break;
        case '+': tok.kind = T_PLUS; break;
        case '?': tok.kind = T_QMARK; break;
        case '(': tok.kind = T_LPAREN; break;
        case ')': tok.kind = T_RPAREN; break;
        case ';': tok.kind = T_SEMI; break;
        default:
            fprintf(stderr, "Unknown char: %c\n", c);
            exit(1);
    }
}

// ================================================================
// 3. AST ДЛЯ PEG-ГРАММАТИКИ
// ================================================================

typedef enum {
    N_RULE,
    N_ALT,
    N_SEQ,
    N_REPEAT,
    N_LITERAL,
    N_IDENT_REF
} NodeKind;

typedef struct Node {
    NodeKind kind;
    char name[64];     // для RULE / IDENT / LITERAL
    struct Node **list; // для ALT/SEQ
    int list_len;
    struct Node *child; // для REPEAT
    char rep;           // ?, *, +
} Node;

// Создание узлов
static Node *new_node(NodeKind k) {
    Node *n = xmalloc(sizeof(Node));
    memset(n, 0, sizeof(Node));
    n->kind = k;
    return n;
}

static Node *new_list(NodeKind k, Node **els, int n) {
    Node *x = new_node(k);
    x->list = xmalloc(sizeof(Node*) * n);
    memcpy(x->list, els, sizeof(Node*) * n);
    x->list_len = n;
    return x;
}

// ================================================================
// 4. ПАРСЕР ГРАММАТИКИ (PEG)
// Grammar =~ Rule*
// Rule    =~ IDENT "=~" Expr ";"
// Expr    =~ Term ( "/" Term )*
// Term    =~ Factor+
// Factor  =~ IDENT | STRING | "(" Expr ")" | Factor repetition
// ================================================================

static Node *parse_expr(); // forward

// Утилита: ожидание токена
static void expect(TokenKind k) {
    if (tok.kind != k) {
        fprintf(stderr, "Expected token %d but got %d\n", k, tok.kind);
        exit(1);
    }
    next_token();
}

static Node *parse_factor() {
    Node *n;

    if (tok.kind == T_IDENT) {
        n = new_node(N_IDENT_REF);
        strcpy(n->name, tok.text);
        next_token();
    }
    else if (tok.kind == T_STRING) {
        n = new_node(N_LITERAL);
        strcpy(n->name, tok.text);
        next_token();
    }
    else if (tok.kind == T_LPAREN) {
        next_token();
        n = parse_expr();
        expect(T_RPAREN);
    }
    else {
        fprintf(stderr, "Unexpected token in factor\n");
        exit(1);
    }

    // repetition: ?, *, +
    if (tok.kind == T_QMARK || tok.kind == T_STAR || tok.kind == T_PLUS) {
        Node *r = new_node(N_REPEAT);
        r->child = n;
        r->rep = tok.text[0];
        next_token();
        return r;
    }

    return n;
}

static Node *parse_term() {
    Node *list[64];
    int count = 0;

    // Фактор должен быть хотя бы один
    do {
        list[count++] = parse_factor();
    } while (
        tok.kind == T_IDENT    ||
        tok.kind == T_STRING   ||
        tok.kind == T_LPAREN
    );

    return new_list(N_SEQ, list, count);
}

static Node *parse_expr() {
    Node *list[64];
    int count = 0;

    list[count++] = parse_term();

    while (tok.kind == T_SLASH) {
        next_token();
        list[count++] = parse_term();
    }

    if (count == 1) return list[0];
    return new_list(N_ALT, list, count);
}

static Node *parse_rule() {
    Node *r = new_node(N_RULE);

    if (tok.kind != T_IDENT) {
        fprintf(stderr, "Rule must start with identifier\n");
        exit(1);
    }

    strcpy(r->name, tok.text);
    next_token();
    expect(T_ARROW);

    r->child = parse_expr();
    expect(T_SEMI);

    return r;
}

// ================================================================
// 5. ВЕРХНЕЕ УПРАВЛЕНИЕ: список правил
// ================================================================

static Node *rules[256];
static int rules_len = 0;

static void parse_grammar(const char *text) {
    src = text;
    next_token();

    while (tok.kind != T_EOF) {
        rules[rules_len++] = parse_rule();
    }
}

// ================================================================
// 6. ПРИМЕР: вывод AST (для обучения)
// ================================================================

static void print_node(Node *n, int indent) {
    for (int i = 0; i < indent; i++) printf("  ");

    switch (n->kind) {
        case N_RULE:
            printf("RULE %s\n", n->name);
            print_node(n->child, indent+1);
            break;
        case N_ALT:
            printf("ALT\n");
            for (int i = 0; i < n->list_len; i++)
                print_node(n->list[i], indent+1);
            break;
        case N_SEQ:
            printf("SEQ\n");
            for (int i = 0; i < n->list_len; i++)
                print_node(n->list[i], indent+1);
            break;
        case N_REPEAT:
            printf("REPEAT %c\n", n->rep);
            print_node(n->child, indent+1);
            break;
        case N_LITERAL:
            printf("LIT \"%s\"\n", n->name);
            break;
        case N_IDENT_REF:
            printf("REF %s\n", n->name);
            break;
    }
}

// ================================================================
// 7. main() — запуск интерпретатора грамматики
// ================================================================

int main() {
    // Пример грамматики PEG
    const char *grammar =
        "Expr =~ Term / Term '+' Expr;\n"
        "Term =~ Factor+;\n"
        "Factor =~ IDENT / '(' Expr ')';\n";

    printf("Parsing grammar...\n");
    parse_grammar(grammar);

    for (int i = 0; i < rules_len; i++) {
        print_node(rules[i], 0);
    }

    return 0;
}