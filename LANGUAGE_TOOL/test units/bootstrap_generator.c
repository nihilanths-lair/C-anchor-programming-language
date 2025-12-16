/*** SECTION 1: Includes, tokenizer ***/
/*** SECTION 2: AST structures ***/
/*** SECTION 3: Grammar parser ***/
/*** SECTION 4: Code generator ***/
/*** SECTION 5: Main ***/

/****************************************************
 * SECTION 1: Includes, tokenizer
 ****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOK_EOF,
    TOK_NAME,
    TOK_STRING,
    TOK_RANGE,
    TOK_EQUALS,
    TOK_OR,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_STAR,
    TOK_PLUS,
    TOK_QMARK,
    TOK_SEMI
} Tok;

char *src;
int pos;
Tok tok;
char token_text[256];

void die(const char *msg) {
    printf("Error: %s\n", msg);
    exit(1);
}

void read_name() {
    int i=0;
    while (isalnum(src[pos]) || src[pos]=='_')
        token_text[i++] = src[pos++];
    token_text[i] = 0;
}

void read_string() {
    int q = src[pos++];
    int i=0;
    while (src[pos] && src[pos]!=q)
        token_text[i++] = src[pos++];
    if (!src[pos]) die("unterminated string");
    pos++; 
    token_text[i]=0;
}

void read_range() {
    pos++; // skip '['
    int i=0;
    while (src[pos] && src[pos]!=']')
        token_text[i++] = src[pos++];
    if (!src[pos]) die("unterminated range");
    pos++; 
    token_text[i]=0;
}

void next_tok() {
    while (isspace(src[pos])) pos++;

    int c = src[pos];
    if (!c) { tok = TOK_EOF; return; }

    if (isalpha(c) || c=='_') {
        read_name();
        if (!strcmp(token_text,"or")) tok = TOK_OR;
        else tok = TOK_NAME;
        return;
    }
    if (c=='"' || c=='\'') { read_string(); tok = TOK_STRING; return; }
    if (c=='[') { read_range(); tok = TOK_RANGE; return; }

    switch (c) {
        case '=': pos++; tok=TOK_EQUALS; return;
        case ';': pos++; tok=TOK_SEMI; return;
        case '(': pos++; tok=TOK_LPAREN; return;
        case ')': pos++; tok=TOK_RPAREN; return;
        case '*': pos++; tok=TOK_STAR; return;
        case '+': pos++; tok=TOK_PLUS; return;
        case '?': pos++; tok=TOK_QMARK; return;
    }

    die("unexpected char");
}

/****************************************************
 * SECTION 2: AST Structures
 ****************************************************/

typedef enum {
    NODE_NAME,
    NODE_STRING,
    NODE_RANGE,
    NODE_SEQ,
    NODE_OR,
    NODE_REPEAT0,
    NODE_REPEAT1,
    NODE_OPTIONAL,
    NODE_RULE
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node *a, *b;
    char *value;
} Node;

Node* make(NodeType t, Node *a, Node *b, const char *v) {
    Node *n = calloc(1,sizeof(Node));
    n->type = t;
    n->a = a;
    n->b = b;
    if (v) { n->value = strdup(v); }
    return n;
}

/****************************************************
 * SECTION 3: Grammar parser (bootstrap parser)
 ****************************************************/

Node* parse_expr();

Node* parse_primary() {
    if (tok == TOK_NAME) {
        Node* n = make(NODE_NAME,NULL,NULL,token_text);
        next_tok();
        return n;
    }
    if (tok == TOK_STRING) {
        Node* n = make(NODE_STRING,NULL,NULL,token_text);
        next_tok();
        return n;
    }
    if (tok == TOK_RANGE) {
        Node* n = make(NODE_RANGE,NULL,NULL,token_text);
        next_tok();
        return n;
    }
    if (tok == TOK_LPAREN) {
        next_tok();
        Node* n = parse_expr();
        if (tok != TOK_RPAREN) die("missing ')'");
        next_tok();
        return n;
    }
    die("primary expected");
    return NULL;
}

Node* parse_postfix() {
    Node* n = parse_primary();
    while (1) {
        if (tok == TOK_STAR) { next_tok(); n = make(NODE_REPEAT0,n,NULL,NULL); continue; }
        if (tok == TOK_PLUS) { next_tok(); n = make(NODE_REPEAT1,n,NULL,NULL); continue; }
        if (tok == TOK_QMARK) { next_tok(); n = make(NODE_OPTIONAL,n,NULL,NULL); continue; }
        break;
    }
    return n;
}

Node* parse_seq() {
    Node* left = parse_postfix();
    while (tok==TOK_NAME || tok==TOK_STRING || tok==TOK_RANGE || tok==TOK_LPAREN) {
        Node* right = parse_postfix();
        left = make(NODE_SEQ,left,right,NULL);
    }
    return left;
}

Node* parse_expr() {
    Node* left = parse_seq();
    while (tok == TOK_OR) {
        next_tok();
        Node* right = parse_seq();
        left = make(NODE_OR,left,right,NULL);
    }
    return left;
}

Node* parse_rule() {
    if (tok != TOK_NAME) die("rule name expected");
    char name[256];
    strcpy(name, token_text);
    next_tok();

    if (tok != TOK_EQUALS) die("expected '='");
    next_tok();

    Node* body = parse_expr();

    if (tok != TOK_SEMI) die("expected ';'");
    next_tok();

    return make(NODE_RULE, body, NULL, name);
}

/****************************************************
 * SECTION 4: Code generator (AST ? parser.c)
 ****************************************************/

// Каждому правилу создадим C-функцию: int parse_<name>().

void emit_node(Node* n, FILE *out, int depth);

void indent(FILE *out, int d) {
    for (int i=0;i<d;i++) fprintf(out,"    ");
}

void emit_string_literal(FILE *out, const char *s) {
    fprintf(out,"\"");
    for (int i=0;i<strlen(s); i++) {
        if (s[i]=='"') fprintf(out,"\\\"");
        else fprintf(out,"%c", s[i]);
    }
    fprintf(out,"\"");
}

// Генерация кода для одного узла (рекурсивно)
void emit_node(Node* n, FILE *out, int depth) {
    switch (n->type) {

    case NODE_STRING:
        indent(out,depth);
        fprintf(out,"if (!match_string(");
        emit_string_literal(out, n->value);
        fprintf(out,")) return 0;\n");
        return;

    case NODE_RANGE:
        indent(out,depth);
        fprintf(out,"if (!match_range(\"%s\")) return 0;\n", n->value);
        return;

    case NODE_NAME:
        indent(out,depth);
        fprintf(out,"if (!parse_%s()) return 0;\n", n->value);
        return;

    case NODE_SEQ:
        emit_node(n->a,out,depth);
        emit_node(n->b,out,depth);
        return;

    case NODE_OR:
        indent(out,depth);
        fprintf(out,"{\n");
        indent(out,depth+1);
        fprintf(out,"int saved = pos;\n");
        emit_node(n->a,out,depth+1);
        indent(out,depth+1);
        fprintf(out,"goto _ok_%p;\n", n);
        indent(out,depth);
        fprintf(out,"}\n");
        indent(out,depth);
        fprintf(out,"pos = saved;\n");
        emit_node(n->b,out,depth);
        indent(out,depth);
        fprintf(out,"_ok_%p: ;\n", n);
        return;

    case NODE_REPEAT0:
        indent(out,depth);
        fprintf(out,"while (1) {\n");
        indent(out,depth+1);
        fprintf(out,"int saved = pos;\n");
        emit_node(n->a,out,depth+1);
        indent(out,depth+1);
        fprintf(out,"if (pos == saved) break;\n");
        indent(out,depth);
        fprintf(out,"}\n");
        return;

    case NODE_REPEAT1:
        // At least one
        emit_node(n->a,out,depth);
        indent(out,depth);
        fprintf(out,"while (1) {\n");
        indent(out,depth+1);
        fprintf(out,"int saved = pos;\n");
        emit_node(n->a,out,depth+1);
        indent(out,depth+1);
        fprintf(out,"if (pos == saved) break;\n");
        indent(out,depth);
        fprintf(out,"}\n");
        return;

    case NODE_OPTIONAL:
        indent(out,depth);
        fprintf(out,"{\n");
        indent(out,depth+1);
        fprintf(out,"int saved = pos;\n");
        emit_node(n->a,out,depth+1);
        indent(out,depth+1);
        fprintf(out,"pos = pos; /* ok anyway */\n");
        indent(out,depth);
        fprintf(out,"}\n");
        return;

    default:
        die("unknown node");
    }
}

// Генерация функций parse_<rule>()
void emit_rule(Node* rule, FILE *out) {
    fprintf(out,"\nint parse_%s() {\n", rule->value);
    fprintf(out,"    int saved = pos;\n");
    emit_node(rule->a, out, 1);
    fprintf(out,"    return 1;\n");
    fprintf(out,"}\n");
}

// Заголовок parser.c
void emit_header(FILE *out) {
    fprintf(out,
        "/* Generated parser */\n"
        "#include <stdio.h>\n"
        "#include <string.h>\n"
        "\n"
        "const char *input;\n"
        "int pos = 0;\n\n"
        "int match_string(const char *s) {\n"
        "    int len = strlen(s);\n"
        "    if (strncmp(input+pos, s, len)==0) { pos += len; return 1; }\n"
        "    return 0;\n"
        "}\n\n"
        "int match_range(const char *spec) {\n"
        "    // format: x..y\n"
        "    char a,b;\n"
        "    if (sscanf(spec, \"%c..%c\", &a, &b) != 2) return 0;\n"
        "    char c = input[pos];\n"
        "    if (!c) return 0;\n"
        "    if (c>=a && c<=b) { pos++; return 1; }\n"
        "    return 0;\n"
        "}\n"
    );
}

// Точка входа в парсер
void emit_main(FILE *out, const char *start_rule) {
    fprintf(out,
        "\nint main(int argc, char **argv) {\n"
        "    if (argc<2) { printf(\"usage: parser <input>\\n\"); return 1; }\n"
        "    input = argv[1];\n"
        "    pos = 0;\n"
        "    if (parse_%s() && input[pos]=='\\0') {\n"
        "        printf(\"OK\\n\"); return 0;\n"
        "    } else {\n"
        "        printf(\"FAIL at pos %%d\\n\", pos);\n"
        "        return 1;\n"
        "    }\n"
        "}\n",
        start_rule
    );
}

/****************************************************
 * SECTION 5: Main (reading grammar, generating code)
 ****************************************************/

// Простая структура для хранения массива правил
Node* rules[1024];
int rule_count = 0;

int main(int argc, char **argv) {
    if (argc < 2) die("usage: bootstrap <grammar.txt>");

    // Load grammar file
    FILE* f = fopen(argv[1], "rb");
    if (!f) die("cannot open grammar file");

    fseek(f,0,SEEK_END);
    long sz = ftell(f);
    fseek(f,0,SEEK_SET);

    src = malloc(sz+1);
    fread(src,1,sz,f);
    fclose(f);
    src[sz]=0;

    pos=0;
    next_tok();

    // Parse all rules
    while (tok != TOK_EOF) {
        Node* r = parse_rule();
        rules[rule_count++] = r;
    }

    // Generate parser.c to stdout
    FILE *out = stdout;

    emit_header(out);

    // Emit all rule functions
    for (int i=0;i<rule_count;i++) {
        emit_rule(rules[i], out);
    }

    // Use the first rule as the start rule
    emit_main(out, rules[0]->value);

    return 0;
}