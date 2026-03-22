// <!-- Software generator C$ v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>
#include <string.h>

const char attic[] =
"\
// <!-- %s v.0.1 (Encoding: Windows-1251) -->\n\
\n\
#include <locale.h>\n\
#include <stdio.h>\n\
\n\
int main(int argc, char *argv[])\n\
{\n\
    setlocale(0, \"\");\n\
";
//
const char basement[] =
"\
    return 0;\n\
}\
";

FILE *file;
//
int main()
{
    setlocale(0, "");

    file = fopen("src\\programs\\written in C\\Interpreter\\v.0.1\\interpreter.c", "w");
    fprintf(file, attic, "Interpreter C$");
    // ... //
    fprintf(file, basement);
    fclose(file);

    file = fopen("src\\programs\\written in C\\Compiler\\v.0.1\\compiler.c", "w");
    fprintf(file, attic, "Compiler C$");
    // ... //
    fprintf(file, basement);
    fclose(file);

    file = fopen("src\\programs\\written in C\\VM\\v.0.1\\vm.c", "w");
    fprintf(file, attic, "Virtual Machine C$");
    // ... //
    fprintf(file, basement);
    fclose(file);
    return 0;
}