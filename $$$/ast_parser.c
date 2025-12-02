#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_PRINT,
    NODE_ASSIGN,
    NODE_UNKNOWN
} PNodeType;

typedef struct PNode {
    PNodeType type;
    const char *text;
    struct PNode **children;
    int child_count;
} PNode;

// Создание нового узла
PNode* pnode_new(PNodeType type, const char *text) {
    PNode *node = (PNode*)malloc(sizeof(PNode));
    node->type = type;
    node->text = text ? strdup(text) : NULL;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

// Добавление дочернего узла
void pnode_add_child(PNode *parent, PNode *child) {
    parent->child_count++;
    parent->children = (PNode**)realloc(parent->children, sizeof(PNode*) * parent->child_count);
    parent->children[parent->child_count - 1] = child;
}

// Рекурсивный вывод AST
void ast_print(PNode *node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("- Node type: %d, text: %s\n", node->type, node->text ? node->text : "NULL");
    for (int i = 0; i < node->child_count; i++)
        ast_print(node->children[i], level + 1);
}

// Освобождение памяти
void pnode_free(PNode *node) {
    if (!node) return;
    if (node->text) free((void*)node->text);
    for (int i = 0; i < node->child_count; i++)
        pnode_free(node->children[i]);
    free(node->children);
    free(node);
}

// Мини-парсер для нашего DSL
PNode* parse_dsl(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Не удалось открыть файл: %s\n", filename);
        return NULL;
    }
    PNode *root = pnode_new(NODE_UNKNOWN, "root");

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *start = strtok(line, "\n"); // удаляем \n
        if (!start) continue;

        if (strncmp(start, "print ", 6) == 0)
        {
            char *arg = start + 6;
            PNode *node = pnode_new(NODE_PRINT, arg);
            pnode_add_child(root, node);
        }
        // Можно добавлять обработку других команд здесь
    }
    fclose(file);
    return root;
}
// Мини-парсер для нашего DSL (Upgrade-version)
PNode* parse_dsl_line(const char *line)
{
    PNode *node = malloc(sizeof (PNode));
    if (!node) return NULL;

    // Проверяем команду print
    if (strncmp(line, "print ", 6) == 0)
    {
        node->type = NODE_PRINT;
        node->text = strdup(line + 6); // текст после "print "
    }
    // Проверяем команду assign (например: assign x = 10)
    else if (strncmp(line, "assign ", 7) == 0)
    {
        node->type = NODE_ASSIGN;
        node->text = strdup(line + 7); // остальное
    }
    else
    {
        node->type = NODE_UNKNOWN;
        node->text = strdup(line);
    }
    return node;
}
#include <locale.h>
int main()
{
    setlocale(0, "");

    const char *dsl_file = "ast.dsl";
    printf("\nПарсинг DSL: %s\n", dsl_file);

    // Используем парсер, который читает файл
    PNode *ast = parse_dsl(dsl_file);

    printf("AST:\n");
    ast_print(ast, 0);

    pnode_free(ast);

    return 0;
}