//
// <!-- Software generator C$ v.0.1 (Encoding: Windows-1251) -->
//
#include <locale.h>
#include <stdio.h>
#include <string.h>
//
const char attic[] =
"\
// <!-- %s (Encoding: Windows-1251) -->\n\
\n\
#include <locale.h>\n\
#include <stdio.h>\n\
\n\
int main(int argc, char *argv[])\n\
{\n\
    setlocale(0, \"\");\n\
    //\n\
";
//
const char basement[] =
"\n\
    //\n\
    return 0;\n\
}\
";
//
FILE *file;
//
int main()
{
    setlocale(0, "");
    //
    file = fopen("src\\programs\\written in C\\Interpreter\\v.0.1\\interpreter.c", "w");
    fprintf(file, attic, "Interpreter C$ v.0.1");
    fprintf(file,
     "\tprintf(\"%%d\", argc);"
    );
    fprintf(file, basement);
    fclose(file);
    file = fopen("src\\programs\\written in C\\Interpreter\\v.0.1\\_.c", "w"); // C-style
    fprintf(file, attic, "C-style");
    fprintf(file, basement);
    fclose(file);
    file = fopen("src\\programs\\written in C\\Interpreter\\v.0.1\\_.py", "w"); // Python style
    fprintf(file, "print(\"2 + 2 * 2 = \", 2 + 2 * 2)");
    fclose(file);
    file = fopen("src\\programs\\written in C\\Interpreter\\v.0.1\\_.lua", "w"); // Lua style
    fprintf(file, "print(\"2 + 2 * 2 = \", 2 + 2 * 2)");
    fclose(file);

    file = fopen("src\\programs\\written in C\\Compiler\\v.0.1\\compiler.c", "w");
    fprintf(file, attic, "Compiler C$ v.0.1");
    fprintf(file,
     "\tprintf(\"%%d\", argc);"
    );
    fprintf(file, basement);
    fclose(file);

    file = fopen("src\\programs\\written in C\\VM\\v.0.1\\vm.c", "w");
    fprintf(file, attic, "Virtual Machine C$ v.0.1");
    fprintf(file,
     "\tprintf(\"%%d\", argc);"
    );
    fprintf(file, basement);
    fclose(file);
    //
    return 0;
}