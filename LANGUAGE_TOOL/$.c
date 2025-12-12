#include <locale.h>
#include <stdio.h>
#include <string.h>

//#include "LexicalAnalyzer.h"   // #1 Лексический анализатор (Токенизатор) / Lexical analyzer (Tokenizer)
//#include "SyntacticAnalyzer.h" // #2 Синтаксический анализатор (Парсер) / Syntactic analyzer (Parser)
//#include "SemanticAnalyzer.h"  // #3 Семантический анализатор / Semantic analyzer
//#include "InterpreterAST.h"    // #4 Интерпретатор АСД / Interpreter AST
/*/
    #5 High/Middle/Low-level intermediate representation
    #6 InterpreterIR
/*/
/* - - */
FILE * handle;
/* - - */
typedef char ch;
typedef unsigned char uch;
/* - - */
// Load source code and grammar form
uch source_code_cdlr[0xFFFF] = "";
uch grammar_form[0xFFF] = "";  // Форма грамматики
/* - - */
typedef struct {
    uch token[0xFF];
    uch productions_rule[0xFF];  // Правила продукции задаваемые/описываемые в DSL
} GrammarForm;
//uch node[0xFF] = "";   // Определение узла
uch separator[] = "::";  // Разделитель шаблона
typedef struct {
    uch token[0xFF];
    uch productions_rule[0xFF];
} LexicalAnalyzer;
uch * CompileForm(const uch * code, const uch * mode)
{
    if (!strcmp(mode, "DSL"))
    {
        int i = 0;
        while ('@')
        {
            if (code[i] == '\0')
            {
                printf("\nForm compilation completed."); // Компиляция формы завершена.
                break;
            }
            // ... //
            // Всё остальное
            // ... //
            i++;
        }
    }
    else if (!strcmp(mode, "GPL"))
    {
        // ... //
        // Всё остальное
        // ... //
    }
}
// Добавить новый токен
void addNewToken(const ch * token, const ch * production_rule)
{
    printf("~ token = %s", token);
}
// Получить след. токен
char * GetNextToken(const ch * source_code)
{

}
//@//
int main()
{
    setlocale(0, "");

    handle = fopen("C$\\$.form", "r");
    if (handle == NULL)
    {
        printf(" Для начала создайте файл `$.form`.");
        return -1;
    }
    fread(grammar_form, sizeof (grammar_form), 1, handle);
    fclose(handle);
    printf("\ngrammar_form =\n%s", grammar_form);
    //putchar('\n');
    //CompileForm(grammar_form, "DSL"); // Пока поверхностно
    //compile_code(grammar_form, "GPL");

    handle = fopen("C$\\$.cdlr", "r");
    if (handle == NULL)
    {
        printf(" Для начала создайте файл `$.cdlr`.");
        return -1;
    }
    fread(source_code_cdlr, sizeof (source_code_cdlr), 1, handle);
    fclose(handle);
    printf(source_code_cdlr);
    /*
    handle = fopen("C$/$.cdlr", "w");
    fwrite(source_code_cdlr, sizeof (source_code_cdlr)-1, 1, handle);
    fclose(handle);
    /*/
    /**/
    handle = fopen("C$\\$.c", "w");
    //fwrite("#include <stdio.h>\n", sizeof ("#include <stdio.h>\n")-1, 1, handle);
    fwrite("int main()\n", sizeof ("int main()\n")-1, 1, handle);
    fwrite("{\n", sizeof ("{\n")-1, 1, handle);
    fputc('\t', handle);
    fwrite(source_code_cdlr, strlen(source_code_cdlr), 1, handle);
    fwrite(";\n", sizeof(";\n")-1, 1, handle);
    fputc('}', handle);
    fclose(handle);
    /**/
    putchar('\n');

    LexicalAnalyzer lexical_analyzer[0xFF];
    while ('\0')
    {
        //lexical_analyzer[0].token = GetNextToken(source_code_cdlr);
    }

    //addNewToken("Числовой_целочисленный_литерал", "563");

    //run_code();
    //printf("\nDSL | %s%s%s", construction, separator, product_rules_dsl);
    //printf("\nGPL | %s\n%s\n%s", construction, separator, product_rules_gpl);

    //RunLexicalAnalysis(source_code_cdlr, " ");
    //putchar('\n');

    ///---------//
    /*
    printf("\nSyntax analysis has begun."); // Начат синтаксический анализ.
    i = 0, j = 0;
    while (product_rules[i] != '\0' && source_code_cdlr[j] != '\0')
    {
        //source_code_cdlr[j]
        i++, j++;
    }
    if (product_rules[i] == '\0' || source_code_cdlr[j] == '\0') printf("\nSyntax parsing completed."); // Синтаксический анализ завершен.
    putchar('\n');
    */
    //---------///
    return 0;
}