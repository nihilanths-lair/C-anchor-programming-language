#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*/---------------/*/
/*/    PEG AST    /*/
/*/---------------/*/

typedef enum {
    PEG_RULE,
    PEG_SEQ,
    PEG_ALT,
    PEG_LITERAL,
    PEG_IDENT,
    PEG_GROUP
} PEGNodeType;

typedef struct PEGNode {
    PEGNodeType type;
    char *text;
    struct PEGNode **child;
    int child_count;
} PEGNode;

PEGNode *peg_new(PEGNodeType type, const char *text)
{
    PEGNode *n = malloc(sizeof (PEGNode));
    n->type = type;
    n->text = text ? strdup(text) : NULL;
    n->child = NULL;
    n->child_count = 0;
    return n;
}

void peg_add(PEGNode *parent, PEGNode *child)
{
    parent->child_count++;
    parent->child = realloc(parent->child, sizeof (PEGNode*) * parent->child_count);
    parent->child[parent->child_count - 1] = child;
}

void peg_print(PEGNode *n, int lvl)
{
    if (!n) return;
    for (int i = 0; i < lvl; i++) printf("  ");

    const char *names[] = {"RULE","SEQ","ALT","LIT","IDENT","GROUP"};
    printf("[%s] %s\n", names[n->type], n->text ? n->text : "");

    for (int i = 0; i < n->child_count; i++) peg_print(n->child[i], lvl + 1);
}

/*/-----------------------/*/
/*/    PEG PARSER CORE    /*/
/*/-----------------------/*/

const char *src; // указатель в тексте

/* skip_ws: НЕ удаляем '\n' — оставляем перевод строки как маркер конца правила */
void skip_ws() {
    while (*src == ' ' || *src == '\t' || *src == '\r') src++;
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

char *read_ident()
{
    skip_ws();
    const char *start = src;

    if (!isalpha((unsigned char)*src) && *src != '_') return NULL;
    while (isalnum((unsigned char)*src) || *src == '_') src++;

    int len = src - start;
    char *out = malloc(len + 1);
    memcpy(out, start, len);
    out[len] = 0;
    return out;
}

char *read_literal()
{
    skip_ws();
    if (*src != '"') return NULL;

    src++; // пропустили "

    const char *start = src;
    while (*src && *src != '"') src++;

    int len = src - start;
    char *out = malloc(len + 1);
    memcpy(out, start, len);
    out[len] = 0;

    if (*src == '"') src++;
    return out;
}

/*/-------------------------------------------------/*/
/*/    PEG extension: GROUP = "(" expression ")"    /*/
/*/-------------------------------------------------/*/

PEGNode *parse_expression(); // forward

PEGNode *parse_group()
{
    skip_ws();
    if (*src != '(') return NULL;

    src++;  // consume "("
    skip_ws();

    PEGNode *inside = parse_expression();
    if (!inside)
    {
        /* пустая группа — это синтаксическая ошибка в грамматике */
        fprintf(stderr, "Ошибка: пустая или неверная группа\n");
        return NULL;
    }

    skip_ws();
    if (*src != ')')
    {
        fprintf(stderr, "Ошибка: нет закрывающей ')'\n");
        return NULL;
    }

    src++; // consume ")"

    PEGNode *g = peg_new(PEG_GROUP, NULL);
    peg_add(g, inside);
    return g;
}

/*/--------------------------------/*/
/*/    PEG: primary = group | literal | ident    /*/
/*/--------------------------------/*/

PEGNode *parse_primary()
{
    skip_ws();

    // группа
    PEGNode *g = parse_group();
    if (g) return g;

    // литерал
    char *lit = read_literal();
    if (lit) return peg_new(PEG_LITERAL, lit);

    // идентификатор
    char *id = read_ident();
    if (id) return peg_new(PEG_IDENT, id);

    return NULL;
}

/*/--------------------------------/*/
/*/    PEG: sequence = primary+    /*/
/*/--------------------------------/*/

PEGNode *parse_sequence()
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
        PEGNode *tmp = seq->child[0];
        free(seq->child);
        free(seq);
        return tmp;
    }
    return seq;
}

/*/--------------------------------------------------/*/
/*/    PEG: expression = sequence ("/" sequence)*    /*/
/*/--------------------------------------------------/*/

PEGNode *parse_expression()
{
    PEGNode *left = parse_sequence();
    if (!left) return NULL;

    while (1)
    {
        // мы не используем match("/") напрямую, потому что match() вызывает skip_ws()
        // и skip_ws() не убирает '\n' => если '/' будет на следующей строке, match("/") вернёт 0.
        skip_ws();
        if (*src != '/') break;
        src++; // consume '/'
        PEGNode *right = parse_sequence();
        if (!right)
        {
            fprintf(stderr, "Ошибка: ожидается sequence после '/'\n");
            return NULL;
        }
        PEGNode *alt = peg_new(PEG_ALT, NULL);
        peg_add(alt, left);
        peg_add(alt, right);
        left = alt;
    }
    return left;
}

/*/----------------------------------------/*/
/*/    PEG: rule = IDENT "=" expression    /*/
/*/----------------------------------------/*/

PEGNode *parse_rule()
{
    skip_ws();

    /* если мы на переводе строки — это пустая строка; пропустим */
    if (*src == '\n')
    {
        src++;
        return NULL;
    }

    char *id = read_ident();
    if (!id) return NULL;

    /* после идентификатора обязательно '=' */
    skip_ws();
    if (*src != '=')
    {
        fprintf(stderr, "Ошибка: ожидался '=' после %s\n", id);
        free(id);
        return NULL;
    }
    src++; // consume '='

    PEGNode *expr = parse_expression();
    if (!expr)
    {
        fprintf(stderr, "Ошибка: выражение после '=' отсутствует для %s\n", id);
        free(id);
        return NULL;
    }

    /* после выражения допустим конеч строки — если есть, пропускаем один \n */
    skip_ws();
    if (*src == '\n') src++;

    PEGNode *rule = peg_new(PEG_RULE, id);
    peg_add(rule, expr);
    return rule;
}

/*/-----------------/*/
/*/    READ FILE    /*/
/*/-----------------/*/

char *load_file(const char *fn)
{
    FILE *f = fopen(fn, "r"); /* текстовый режим */
    if (!f)
    {
        printf("Не открыть %s\n", fn);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);

    char *buf = malloc(sz + 1);
    fread(buf, 1, sz, f);
    buf[sz] = 0;

    fclose(f);
    return buf;
}

/*/-------/*/
/*/  MAIN /*/
/*/-------/*/

int main()
{
    char *text = load_file("grammar.peg");
    if (!text) return 1;

    PEGNode *root = peg_new(PEG_SEQ, "ROOT");

    src = text;
    while (*src)
    {
        skip_ws();
        if (!*src) break;

        PEGNode *r = parse_rule();
        if (r)
        {
            peg_add(root, r);
            continue;
        }
        /* parse_rule вернул NULL — сдвинем к началу следующей строки,
           чтобы избежать зацикливания при некорректных/пустых строках */
        while (*src && *src != '\n') src++;
        if (*src == '\n') src++;
    }

    peg_print(root, 0);
    free(text);
    return 0;
}