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

FILE * handle;
unsigned char buf[0xFFFF];
int main()
{
    setlocale(0, "");

    handle = fopen("$.c", "rb");
    if (handle == NULL) printf(" Для начала создайте файл `$.c`.");

    fread(buf, sizeof (buf), 1, handle);
    fclose(handle);

    printf(buf);

    //  Загрузка в память исходного кода C$ (целиком)
    unsigned char source_code_cdlr[] = "563;"; // Тесты делаются с программы напрямую
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
    //  Загрузка в память шаблона сопоставления (целиком)
    unsigned char token[] = ""; // Тесты делаются с программы напрямую

    unsigned char ch[256] = {'5', '6', '0', '\0', '2', '6', '\0', '3', '\0'};
    //ch[0] = '5'; ch[1] = '6'; ch[2] = '0'; ch[3] = '\0'; ch[4] = '2'; ch[5] = '6'; ch[6] = '\0'; ch[7] = '3', ch[8] = '\0';
    //unsigned char * ptr_ch = ch;
    //for (int i = 0; i < 8; i++) printf(" '%c'", ch[i]);
    //printf(&ch[4]);

    return 0;
}