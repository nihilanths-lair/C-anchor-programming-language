#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================
   PEG AST
   ============================ */

typedef enum {
    PEG_RULE,
    PEG_SEQ,
    PEG_ALT,
    PEG_LITERAL,
    PEG_IDENT
} PEGNodeType;

typedef struct PEGNode {
    PEGNodeType type;
    char *text;
    struct PEGNode **child;
    int child_count;
} PEGNode;

PEGNode* peg_new(PEGNodeType type, const char *text)
{
    PEGNode *n = malloc(sizeof(PEGNode));
    n->type = type;
    n->text = text ? strdup(text) : NULL;
    n->child = NULL;
    n->child_count = 0;
    return n;
}

void peg_add(PEGNode *p, PEGNode *c)
{
    p->child_count++;
    p->child = realloc(p->child, sizeof (PEGNode*) * p->child_count);
    p->child[p->child_count - 1] = c;
}

void peg_print(PEGNode *n, int lvl)
{
    if (!n) return;
    for (int i = 0; i < lvl; i++) printf("  ");

    const char *names[] = { "RULE", "SEQ", "ALT", "LIT", "IDENT" };
    printf("[%s] %s\n", names[n->type], n->text ? n->text : "");

    for (int i = 0; i < n->child_count; i++)
        peg_print(n->child[i], lvl + 1);
}

/* ============================
   PEG PARSER CORE
   ============================ */

const char *src;

void skip_ws()
{
    /* НЕ трогаем '\n' — это конец правила */
    while (*src == ' ' || *src == '\t' || *src == '\r')
        src++;
}

int match(const char *s)
{
    skip_ws();
    int len = strlen(s);
    if (strncmp(src, s, len) == 0)
    {
        src += len;
        return 1;
    }
    return 0;
}

char* read_ident()
{
    skip_ws();

    if (!isalpha(*src) && *src != '_')
        return NULL;

    const char *start = src;
    while (isalnum(*src) || *src == '_')
        src++;

    int len = src - start;
    char *out = malloc(len + 1);
    memcpy(out, start, len);
    out[len] = 0;

    return out;
}

char* read_literal()
{
    skip_ws();
    if (*src != '"') return NULL;

    src++;  // opening quote
    const char *start = src;

    while (*src && *src != '"')
        src++;

    int len = src - start;
    char *out = malloc(len + 1);
    memcpy(out, start, len);
    out[len] = 0;

    if (*src == '"') src++; // closing quote
    return out;
}

/* forward */
struct PEGNode* parse_expression();

/* ============================
   primary = literal / ident / "(" expression ")"
   ============================ */

PEGNode* parse_primary()
{
    skip_ws();

    char *lit = read_literal();
    if (lit) return peg_new(PEG_LITERAL, lit);

    char *id = read_ident();
    if (id) return peg_new(PEG_IDENT, id);

    if (match("("))
    {
        PEGNode *e = parse_expression();
        match(")");
        return e;
    }
    return NULL;
}

/* ============================
   sequence = primary+
   ============================ */

PEGNode* parse_sequence()
{
    PEGNode *seq = peg_new(PEG_SEQ, NULL);

    PEGNode *p;
    while ((p = parse_primary()))
    {
        peg_add(seq, p);
        skip_ws();
    }

    if (seq->child_count == 1)
    {
        PEGNode *only = seq->child[0];
        free(seq->child);
        free(seq);
        return only;
    }
    return seq;
}

/* ============================
   expression = sequence ( "/" sequence )*
   ============================ */

PEGNode* parse_expression()
{
    PEGNode *left = parse_sequence();

    while (match("/"))
    {
        PEGNode *alt = peg_new(PEG_ALT, NULL);
        peg_add(alt, left);
        peg_add(alt, parse_sequence());
        left = alt;
    }
    return left;
}

/* ============================
   rule = IDENT "=" expression "\n"
   ============================ */

PEGNode* parse_rule()
{
    char *id = read_ident();
    if (!id) return NULL;

    if (!match("=")) {
        printf("ERROR: expected '=' after rule %s\n", id);
        exit(1);
    }

    PEGNode *rule = peg_new(PEG_RULE, id);
    peg_add(rule, parse_expression());

    /* ловим конец строки */
    if (*src == '\n')
        src++;

    return rule;
}

/* ============================
   full grammar = rule*
   ============================ */

PEGNode* parse_grammar(const char *text)
{
    src = text;
    PEGNode *root = peg_new(PEG_SEQ, "ROOT");

    while (*src) {
        skip_ws();
        if (*src == 0) break;

        PEGNode *r = parse_rule();
        if (r) peg_add(root, r);
    }
    return root;
}

/* ============================
   load file
   ============================ */

char* load_file(const char *fn)
{
    FILE *f = fopen(fn, "r");  // обычный текстовый режим
    if (!f) { printf("Не открыть %s\n", fn); return NULL; }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);

    char *buf = malloc(sz + 1);
    fread(buf, 1, sz, f);
    buf[sz] = 0;

    fclose(f);
    return buf;
}

/* ============================
   main
   ============================ */

int main()
{
    char *text = load_file("grammar.peg");
    if (!text) return 1;

    PEGNode *ast = parse_grammar(text);
    peg_print(ast, 0);

    return 0;
}