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
#define NOP ""
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
    printf("code = %s", code);
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
unsigned char storage[0xFF] = {0};
unsigned char bytecode[0xFF] = {0};
//-/
int main()
{
    setlocale(0, "");

    handle = fopen("$.c", "rb");
    if (handle == NULL) printf(" Для начала создайте файл `$.c`.");

    fread(buf, sizeof (buf), 1, handle);
    fclose(handle);

    //printf(buf);

    //  Загрузка в память исходного кода C$ (целиком)
    unsigned char source_code_cdlr[] = "563;\n\t2450;"; // Тесты делаются с программы напрямую
    /*/
    handle = fopen("C$/$.cdlr", "r");
    if (handle == NULL) printf(" Для начала создайте файл `$.cdlr`.");
    fread(source_code_cdlr, sizeof (source_code_cdlr), 1, handle);
    fclose(handle);

    handle = fopen("C$/$.cdlr", "w");
    fwrite(source_code_cdlr, sizeof (source_code_cdlr)-1, 1, handle);
    fclose(handle);
    /*/
    /**/
    handle = fopen("C$/$.c", "w");
    if (handle == NULL) printf(" Не удалось записать данные в файл `C$/$.c`.");
    //fwrite("#include <stdio.h>\n", sizeof ("#include <stdio.h>\n")-1, 1, handle);
    fwrite("int main()\n", sizeof ("int main()\n")-1, 1, handle);
    fwrite("{\n", sizeof ("{\n")-1, 1, handle);
    fputc('\t', handle);
    fwrite(source_code_cdlr, sizeof (source_code_cdlr)-1, 1, handle);
    fputc('\n', handle);
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
    unsigned char construction[0xFF];      // Определение конструкции
    unsigned char separator[0xF];          // Разделитель шаблона
    unsigned char product_rules_dsl[0xFF]; // Правила продукции задаваемые/описываемые в DSL

    unsigned char form[0xFFF] = {0};
    handle = fopen("C$/$.form", "r");
    if (handle == NULL) printf(" Для начала создайте файл `$.form`.");
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
    printf("\nDSL | %s%s%s", construction, separator, product_rules_dsl);
    printf("\nGPL | %s\n%s\n%s", construction, separator, product_rules_gpl);

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

    unsigned char ch[256] = {'5', '6', '0', '\0', '2', '6', '\0', '3', '\0'};
    //ch[0] = '5'; ch[1] = '6'; ch[2] = '0'; ch[3] = '\0'; ch[4] = '2'; ch[5] = '6'; ch[6] = '\0'; ch[7] = '3', ch[8] = '\0';
    //unsigned char * ptr_ch = ch;
    //for (int i = 0; i < 8; i++) printf(" '%c'", ch[i]);
    //printf(&ch[4]);

    return 0;
}