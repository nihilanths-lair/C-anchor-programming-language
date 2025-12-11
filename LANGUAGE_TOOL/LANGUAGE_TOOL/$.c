#include <locale.h>
#include <stdio.h>
#include <string.h>

#include "LexicalAnalyzer.h"   // #1 Лексический анализатор (Токенизатор) / Lexical analyzer (Tokenizer)
#include "SyntacticAnalyzer.h" // #2 Синтаксический анализатор (Парсер) / Syntactic analyzer (Parser)
#include "SemanticAnalyzer.h"  // #3 Семантический анализатор / Semantic analyzer
#include "InterpreterAST.h"    // #4 Интерпретатор АСД / Interpreter AST

/*/
    #5 High-level intermediate representation
    #6 Middle-level intermediate representation
    #7 Low-level intermediate representation

    #8 InterpreterHIR
    #9 InterpreterMIR
    #10 InterpreterLIR
/*/
int i;
int j;
FILE * handle;
unsigned char buf[0xFFFF];

void compile_code(unsigned char * code, const char * mode)
{
    if (!strcmp(mode, "DSL")) 
    {
        printf("\nmode = 1");
    }
    else if (!strcmp(mode, "GPL"))
    {
        printf("\nmode = 2");
    }
}
void run_code(unsigned char * code)
{
    printf("\ncode = %s", code);
    static void *dispatch_table[] = {[0 ... 255] = &&op_invalid};
    goto *dispatch_table[0];

op_invalid:
    printf("\nInvalid opcode.\n");
    //goto end_program;

end_program:
    return;
}
//-/
unsigned char dp = 0;
unsigned char storage[0xFF] = "";
unsigned char bytecode[0xFF] = "";
//-/
int main()
{
    setlocale(0, "");

    //  Загрузка в память исходного кода C$ (целиком)
    unsigned char source_code_cdlr[0xFFF] = "";

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
    i = 0;
    do
    {
        if (i % 16 == 0) putchar('\n');
        printf(" [%03d] = %03d,", i, storage[i]);
    }
    while (++i != 0xFF);
    printf(" [%03d] = %03d.", i, storage[i]);
    putchar('\n');
    //  Загрузка в память правой части шаблона сопоставления (целиком)
    unsigned char construction[0xFF] = "";      // Определение конструкции
    unsigned char separator[0xF] = "";          // Разделитель шаблона
    unsigned char product_rules_dsl[0xFF] = ""; // Правила продукции задаваемые/описываемые в DSL

    unsigned char form[0xFFF] = "";
    handle = fopen("C$\\$.form", "r");
    if (handle == NULL)
    {
        printf(" Для начала создайте файл `$.form`.");
        return -1;
    }
    fread(form, sizeof (form), 1, handle);
    fclose(handle);
    printf("\nform =\n%s", form);
    putchar('\n');

    compile_code(product_rules_dsl, "DSL");
    //run_code();
    // Правила продукции задаваемые/описываемые в GPL (Применяемая модель: BF++)
    unsigned char product_rules_gpl[0xFF];
    compile_code(product_rules_gpl, "GPL");
    //run_code();
    //printf("\nDSL | %s%s%s", construction, separator, product_rules_dsl);
    //printf("\nGPL | %s\n%s\n%s", construction, separator, product_rules_gpl);

    unsigned char * code = "Hello world!";
    run_code(code);
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