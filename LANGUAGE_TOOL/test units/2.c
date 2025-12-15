#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

void parse_factor();
void parse_term();
void parse_expr();
void parse_stmt();
void parse_block();
void parse_if();
void parse_while();
void parse_for();
void parse_func_def();
void parse_call(const char * func_name);
void parse_return();
void parse_param_list();
void parse_assign();

enum TypeToken {
    T_IDENT,
    T_EQ,
    T_NUM,    //    0..9
    T_PLUS,   //    +
    T_MINUS,  //    -
    T_EOF,
    T_LPAREN, //    (
    T_RPAREN, //    )
    T_STAR,   //    *
    T_SLASH,  //    /

    T_FUNC,
    T_LBRACE,  //    {
    T_RBRACE,  //    }
    T_COMMA,   //    ,
    T_SEMICOLON, //    ;

    T_RETURN,  //(если хотим)
    T_WHILE,
    T_FOR,
    T_IF,
    T_ELSE     //(опционально)
};

typedef struct {
    int type;
    char text[32];
} Token;
Token tokens[100];

int tok_count = 0;
int pos = 0;

void expect(int type)
{
    if (tokens[pos].type != type)
    {
        printf("\nError: expected token type %d", type);
        exit(1);
    }
}

// factor → NUM | IDENT '(' | '(' expr ')'
void parse_factor()
{

    if (tokens[pos].type == T_NUM)
    {
        printf("FACTOR: number %s\n", tokens[pos].text);
        pos++;
        return;
    }

    if (tokens[pos].type == T_IDENT)
    {
        // Сохраняем имя
        char * name = tokens[pos].text;
        pos++;

        // Если после идентификатора идёт '(', то это вызов
        if (tokens[pos].type == T_LPAREN)
        {
            parse_call(name);
            return;
        }

        // Иначе — обычная переменная
        printf("FACTOR: ident %s\n", name);
        return;
    }

    if (tokens[pos].type == T_LPAREN)
    {
        pos++;
        parse_expr();
        expect(T_RPAREN);
        pos++;
        return;
    }

    printf("Error: unexpected token in factor\n");
    exit(1);
}

// term → factor (('*' | '/') factor)*
void parse_term()
{
    // сначала парсим левый фактор
    parse_factor();

    // затем пока видим * или /
    while (tokens[pos].type == T_STAR || tokens[pos].type == T_SLASH)
    {

        // запоминаем оператор
        char op = tokens[pos].text[0];
        pos++;

        // парсим фактор справа
        printf("\nTERM: operator %c", op);
        parse_factor();
    }
}

// expr → term (('+' | '-') term)*
void parse_expr()
{
    // сначала парсим левый term
    parse_term();

    // затем пока видим + или -
    while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
    {
        // запоминаем оператор
        char op = tokens[pos].text[0];
        pos++;

        // выводим оператор (позже здесь будет строительство AST)
        printf("EXPR: operator %c\n", op);

        // парсим следующий term справа
        parse_term();
    }
}

// stmt → IDENT '=' expr
void parse_stmt()
{
    if (tokens[pos].type == T_WHILE)
    {
        parse_while();
        return;
    }

    if (tokens[pos].type == T_FOR)
    {
        parse_for();
        return;
    }

    if (tokens[pos].type == T_IF)
    {
        parse_if();
        return;
    }

    if (tokens[pos].type == T_RETURN)
    {
        parse_return();
        return;
    }

    // блок { ... }
    if (tokens[pos].type == T_LBRACE)
    {
        parse_block();
        return;
    }

    // 1. Проверяем, что первый токен — идентификатор
    // присваивание
    if (tokens[pos].type == T_IDENT && tokens[pos+1].type == T_EQ)
    {
        parse_assign();
        return;
    }

    // 3. Парсим выражение справа
    // иначе — выражение (например вызов функции)
    parse_expr();
    /*
    // выводим имя переменной (позже — создадим узел AST)
    printf("STMT: ident %s\n", tokens[pos].text);
    pos++;

    // 2. Ожидаем '='
    if (tokens[pos].type != T_EQ)
    {
        printf("Error: expected '='\n");
        exit(1);
    }

    printf("STMT: '='\n");
    pos++;

    parse_expr();
    */
}

// func_def → 'func' IDENT '(' param_list ')' block
// param_list → /* пусто */ | IDENT (',' IDENT)*
// block → '{' stmt_list '}'
void parse_func_def()
{
    // 1. Ожидаем ключевое слово func
    if (tokens[pos].type != T_FUNC)
    {
        printf("Error: expected 'func'\n");
        exit(1);
    }
    pos++;

    // 2. Ожидаем имя функции
    if (tokens[pos].type != T_IDENT)
    {
        printf("Error: expected function name\n");
        exit(1);
    }

    printf("FUNC_DEF: name = %s\n", tokens[pos].text);
    pos++;

    // 3. Ожидаем (
    if (tokens[pos].type != T_LPAREN)
    {
        printf("Error: expected '('\n");
        exit(1);
    }
    pos++;

    // 4. Параметры
    parse_param_list();

    // 5. Ожидаем )
    if (tokens[pos].type != T_RPAREN)
    {
        printf("Error: expected ')'\n");
        exit(1);
    }
    pos++;

    // 6. Парсим блок функции
    parse_block();
}

// call → IDENT '(' arg_list ')'
// arg_list → /* пусто */ | expr (',' expr)*
void parse_call(const char * func_name)
{

    // мы уже знаем, что tokens[pos] == '('
    pos++;
    printf("CALL: %s\n", func_name);

    // пустые аргументы
    if (tokens[pos].type == T_RPAREN)
    {
        pos++;
        return;
    }

    // первый аргумент — это expr
    parse_expr();

    // оставшиеся аргументы через запятую
    while (tokens[pos].type == T_COMMA)
    {
        pos++;
        parse_expr();
    }

    // ожидаем ')'
    if (tokens[pos].type != T_RPAREN)
    {
        printf("Error: expected ')'\n");
        exit(1);
    }
    pos++;
}

void parse_param_list()
{
    // пустой список параметров: func f()
    if (tokens[pos].type == T_RPAREN) return;

    // иначе должен быть IDENT
    if (tokens[pos].type != T_IDENT)
    {
        printf("Error: expected parameter name\n");
        exit(1);
    }

    printf("PARAM: %s\n", tokens[pos].text);
    pos++;

    // обработка , имя , имя ...
    while (tokens[pos].type == T_COMMA)
    {
        pos++;
        if (tokens[pos].type != T_IDENT)
        {
            printf("Error: expected parameter after ','\n");
            exit(1);
        }
        printf("PARAM: %s\n", tokens[pos].text);
        pos++;
    }
}

void parse_block()
{
    if (tokens[pos].type != T_LBRACE)
    {
        printf("Error: expected '{'\n");
        exit(1);
    }
    pos++;

    // парсим список операторов до }
    while (tokens[pos].type != T_RBRACE) parse_stmt();
    pos++; // съедаем }
}

// if_stmt → 'if' '(' expr ')' stmt ('else' stmt)?
void parse_if()
{
    // 1. ключевое слово if
    if (tokens[pos].type != T_IF)
    {
        printf("Error: expected 'if'\n");
        exit(1);
    }
    pos++;

    // 2. открывающая скобка (
    if (tokens[pos].type != T_LPAREN)
    {
        printf("Error: expected '('\n");
        exit(1);
    }
    pos++;
    printf("IF: condition\n");

    // 3. парсим условие как выражение
    parse_expr();

    // 4. закрывающая скобка )
    if (tokens[pos].type != T_RPAREN)
    {
        printf("Error: expected ')'\n");
        exit(1);
    }
    pos++;

    // 5. парсим тело if
    printf("IF: then-branch\n");
    parse_stmt();

    // 6. проверяем есть ли else
    if (tokens[pos].type == T_ELSE)
    {
        pos++;
        printf("IF: else-branch\n");
        parse_stmt();
    }
}

// while_stmt → 'while' '(' expr ')' stmt
void parse_while()
{

    if (tokens[pos].type != T_WHILE)
    {
        printf("Error: expected 'while'\n");
        exit(1);
    }
    pos++;

    if (tokens[pos].type != T_LPAREN)
    {
        printf("Error: expected '('\n");
        exit(1);
    }
    pos++;

    printf("WHILE: condition\n");
    parse_expr();

    if (tokens[pos].type != T_RPAREN)
    {
        printf("Error: expected ')'\n");
        exit(1);
    }
    pos++;

    printf("WHILE: body\n");
    parse_stmt();
}

// assign → IDENT '=' expr
// stmt   → assign | expr | block | if | ...
void parse_assign()
{
    if (tokens[pos].type != T_IDENT)
    {
        printf("Error: expected identifier for assignment\n");
        exit(1);
    }

    char * name = tokens[pos].text;
    pos++;

    if (tokens[pos].type != T_EQ)
    {
        printf("Error: expected '='\n");
        exit(1);
    }
    pos++;
    printf("ASSIGN: %s =\n", name);

    // справа полноценное выражение
    parse_expr();
}

// return_stmt → 'return' expr
// stmt        → return_stmt | assign | expr | block | if | while ...
void parse_return()
{

    if (tokens[pos].type != T_RETURN)
    {
        printf("Error: expected 'return'\n");
        exit(1);
    }
    pos++;
    printf("RETURN: \n");

    // если хотите разрешить пустой return (как в Python), можно поставить проверку:
    // if (tokens[pos].type == T_SEMICOLON || tokens[pos].type == T_RBRACE) return;

    // а пока — обязательное выражение
    parse_expr();
}

// for_stmt → 'for' '(' init ';' cond ';' step ')' stmt
// init → stmt или expr или пусто
// cond → expr или пусто
// step → expr или пусто
void parse_for()
{

    if (tokens[pos].type != T_FOR)
    {
        printf("Error: expected 'for'\n");
        exit(1);
    }
    pos++;

    if (tokens[pos].type != T_LPAREN)
    {
        printf("Error: expected '('\n");
        exit(1);
    }
    pos++;

    printf("FOR: init\n");

    // ---- init ----

    // пустой init? for (; ...
    if (tokens[pos].type != T_SEMICOLON)
    {
        // init может быть присваиванием или выражением
        parse_stmt();  // можно и parse_expr(), но stmt лучше
    }

    if (tokens[pos].type != T_SEMICOLON)
    {
        printf("Error: expected ';' after for-init\n");
        exit(1);
    }
    pos++;

    // ---- condition ----
    printf("FOR: condition\n");

    if (tokens[pos].type != T_SEMICOLON)
    {
        parse_expr();
    }

    if (tokens[pos].type != T_SEMICOLON)
    {
        printf("Error: expected ';' after for-condition\n");
        exit(1);
    }
    pos++;

    // ---- step ----
    printf("FOR: step\n");

    if (tokens[pos].type != T_RPAREN) parse_expr();
    if (tokens[pos].type != T_RPAREN)
    {
        printf("Error: expected ')'\n");
        exit(1);
    }
    pos++;

    // ---- body ----
    printf("FOR: body\n");
    parse_stmt();
}

int main()
{
    setlocale(0, "");

    tok_count = 0;

    tokens[tok_count++] = (Token){T_NUM, "3"};
    tokens[tok_count++] = (Token){T_STAR, "*"};
    tokens[tok_count++] = (Token){T_NUM, "5"};
    tokens[tok_count++] = (Token){T_SLASH, "/"};
    tokens[tok_count++] = (Token){T_NUM, "2"};
    tokens[tok_count++] = (Token){T_EOF, ""};

    // =?
    putchar('\n');

    return 0;
}