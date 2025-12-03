#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*/-------------------/*/
/*/      PEG AST      /*/
/*/-------------------/*/
/*----------------------------------------*/
/*      PEG: QUANTIFIERS & CHARCLASS      */
/*----------------------------------------*/

typedef enum {
    PEG_RULE,
    PEG_SEQ,
    PEG_ALT,
    PEG_LITERAL,
    PEG_IDENT,
    PEG_GROUP,
    PEG_REPEAT,
    PEG_CLASS,
    PEG_CLASS_ITEM,
    PEG_CLASS_RANGE,
    PEG_CLASS_ESC,
    PEG_CLASS_CHAR
} PEGNodeType;

typedef struct PEGNode {
    PEGNodeType type;
    char * text; // имя правила, литерал или квантификатор
    struct PEGNode ** child;
    int child_count;
} PEGNode;

PEGNode * peg_new(PEGNodeType type, const char * text)
{
    PEGNode * n = malloc(sizeof (PEGNode));
    n->type = type;
    n->text = text ? strdup(text) : NULL;
    n->child = NULL;
    n->child_count = 0;
    return n;
}
void peg_add(PEGNode * parent, PEGNode * child)
{
    parent->child_count++;
    parent->child = realloc(parent->child, sizeof (PEGNode *) * parent->child_count);
    parent->child[parent->child_count - 1] = child;
}
/*/----------------------------/*/
/*/      PEG: AST printer      /*/
/*/----------------------------/*/
const char * names[] =
{
    "RULE",        // 0
    "SEQ",         // 1
    "ALT",         // 2
    "LIT",         // 3
    "IDENT",       // 4
    "GROUP",       // 5
    "REPEAT",      // 6
    "CLASS",       // 7
    "CLASS_ITEM",  // 8
    "CLASS_RANGE", // 9
    "CLASS_ESC",   // 10
    "CLASS_CHAR"   // 11
};
void peg_print(PEGNode * n, int lvl)
{
    if (!n) return;
    for (int i = 0; i < lvl; i++) printf("  ");

    int idx = (int) n->type;
    const char * tname = (idx >= 0 && idx < (int) (sizeof (names) / sizeof (names[0]))) ? names[idx] : "UNKNOWN";
    printf("<%s> %s", tname, n->text ? n->text : "");
    for (int i = 0; i < n->child_count; i++)
    {
        putchar('\n');
        peg_print(n->child[i], lvl + 1);
    }
}
/*/-----------------------/*/
/*/    PEG PARSER CORE    /*/
/*/-----------------------/*/
const char * src; // указатель на текст
void skip_ws()
{
    for (;;)
    {
        if (*src == ' ' || *src == '\t' || *src == '\r' || *src == '\n')
        {
            src++;
            continue;
        }
        // Однострочный комментарий
        if (*src == ';')
        {
            while (*src && *src != '\n') src++;
            continue;
        }
        break;
    }
}
int match(const char * s)
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
char * read_ident()
{
    skip_ws();
    const char *start = src;
    if (!isalpha(*src) && *src != '_') return NULL;
    while (isalnum(*src) || *src == '_') src++;
    int len = src - start;
    char *out = malloc(len + 1);
    memcpy(out, start, len);
    out[len] = 0;
    return out;
}
char * read_literal()
{
    skip_ws();
    if (*src != '"') return NULL;
    src++;
    const char *start = src;
    while (*src && *src != '"') src++;
    int len = src - start;
    char *out = malloc(len + 1);
    memcpy(out, start, len);
    out[len] = 0;
    if (*src == '"') src++;
    return out;
}
/*/-----------------------------------------------/*/
/*/      PEG: CLASS = [A..Z a..z 0..9 _ - .]      /*/
/*/-----------------------------------------------/*/
PEGNode * parse_class()
{
    skip_ws();
    if (*src != '[')
        return NULL;

    src++; // consume '['
    skip_ws();

    int neg = 0;
    if (*src == '!')
    {
        neg = 1;
        src++;
        skip_ws();
    }
    PEGNode * class_node = peg_new(PEG_CLASS, neg ? "NEG" : "POS");
    while (*src && *src != ']')
    {
        skip_ws();
        // -------- escape ----------
        if (*src == '\\')
        {
            src++;
            if (!*src)
                break;

            char esc = *src++;
            char str[2] = {esc, 0};

            PEGNode *e = peg_new(PEG_CLASS_ESC, str);
            peg_add(class_node, e);
            continue;
        }
        // -------- range: x..z  or x...z ----------
        if (src[0] && src[1] && src[2] && src[1] == '.' && src[2] == '.')
        {
            char start = *src;
            src += 3; // skip "x.."
            int three = 0;
            if (*src == '.')
            {
                three = 1;
                src++;
            }
            if (!*src)
            {
                printf("Ошибка: незавершённый диапазон в классе.\n");
                break;
            }
            char end = *src++;
            char buf[4];
            buf[0] = start;
            buf[1] = end;
            buf[2] = three ? '3' : '2';
            buf[3] = '\0';

            PEGNode * r = peg_new(PEG_CLASS_RANGE, buf);
            peg_add(class_node, r);
            continue;
        }

        // -------- single char ----------
        char c = *src++;
        char str[2] = {c, 0};

        PEGNode *ch = peg_new(PEG_CLASS_CHAR, str);
        peg_add(class_node, ch);
    }
    if (*src == ']')
        src++; // consume ']'
    else
        printf("Ошибка: нет закрывающей квадратной скобки в классе символов.\n");

    return class_node;
}
/*/-------------------------------------------/*/
/*/      PEG: GROUP = "(" expression ")"      /*/
/*/-------------------------------------------/*/
// forward
PEGNode * parse_expression();
PEGNode * parse_group()
{
    skip_ws();
    if (*src != '(') return NULL;
    src++; // consume "("
    PEGNode *inside = parse_expression();
    skip_ws();
    if (*src != ')')
    {
        printf("Ошибка: нет закрывающей круглой скобки.\n");
        return NULL;
    }
    src++; // consume ")"
    PEGNode *g = peg_new(PEG_GROUP, NULL);
    peg_add(g, inside);
    return g;
}
/*/--------------------------------------------------------------/*/
/*/      PEG: primary = group | charclass | literal | ident      /*/
/*/--------------------------------------------------------------/*/
PEGNode * parse_primary()
{
    skip_ws();

    PEGNode *cls = parse_class();
    if (cls) return cls;

    PEGNode *g = parse_group();
    if (g) return g;

    char *lit = read_literal();
    if (lit) return peg_new(PEG_LITERAL, lit);

    char *id = read_ident();
    if (id) return peg_new(PEG_IDENT, id);

    return NULL;
}
/*/-------------------------------------------------------/*/
/*/      PEG: sequence = primary+ с квантификаторами      /*/
/*/-------------------------------------------------------/*/
PEGNode * parse_sequence()
{
    PEGNode * seq = peg_new(PEG_SEQ, NULL);
    PEGNode * p;
    const char * last_src;
    while (1)
    {
        last_src = src;
        p = parse_primary();
        if (!p) break;

        skip_ws();

        if (*src == '*' || *src == '+' || *src == '?')
        {
            char qtxt[2];
            qtxt[0] = *src;
            qtxt[1] = '\0';

            PEGNode * r = peg_new(PEG_REPEAT, qtxt);
            src++;
            peg_add(r, p);
            p = r;
        }

        peg_add(seq, p);
        if (src == last_src) break;
        skip_ws();
    }
    if (seq->child_count == 1)
    {
        PEGNode * tmp = seq->child[0];
        free(seq->child);
        free(seq);
        return tmp;
    }
    return seq;
}
/*/------------------------------------------------------/*/
/*/      PEG: expression = sequence ("/" sequence)*      /*/
/*/------------------------------------------------------/*/
PEGNode * parse_expression()
{
    PEGNode * left = parse_sequence();
    while (match("/"))
    {
        PEGNode * alt = peg_new(PEG_ALT, NULL);
        peg_add(alt, left);
        peg_add(alt, parse_sequence());
        left = alt;
    }
    return left;
}
/*/--------------------------------------------/*/
/*/      PEG: rule = IDENT "=" expression      /*/
/*/--------------------------------------------/*/
PEGNode * parse_rule()
{
    char * id = read_ident();
    if (!id) return NULL;
    if (!match("="))
    {
        printf("Ошибка: ожидался символ присваивания после %s.\n", id);
        exit(1);
    }
    PEGNode * rule = peg_new(PEG_RULE, id);
    peg_add(rule, parse_expression());
    return rule;
}
/*/---------------------/*/
/*/      READ FILE      /*/
/*/---------------------/*/
char * load_file(const char * fn)
{
    FILE * f = fopen(fn, "rb");
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
/*//*/
int main()
{
    char * text = load_file("grammar.peg");
    if (!text) return 1;

    PEGNode * root = peg_new(PEG_SEQ, "ROOT");

    src = text;
    while (*src)
    {
        skip_ws();
        if (!*src) break;

        PEGNode * r = parse_rule();
        if (r) {
            peg_add(root, r);
        }
        else
        {
            // Если правило не распарсилось — продвигаемся до конца строки,
            // чтобы не остаться в той же позиции и не зациклиться.
            while (*src && *src != '\n') src++;
            if (*src == '\n') src++; // skip newline
        }
    }
    //putchar('\n');
    peg_print(root, 0);
    printf("\n--");
    free(text);
    return 0;
}