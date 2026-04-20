// <!-- Virtual Machine C$ v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "..\..\batch_files\ProcAsciiChr.c"
#include "..\..\batch_files\ShowDashboard.c"
#include "..\..\batch_files\GetDebuggingInformation.c"

#include "..\..\batch_files\Permutation.c"
#include "..\..\batch_files\CheckTableStatus.c"

#include "..\..\batch_files\Action.c"

// Мета-описание для привязки опкодов к своим уникальным адресам/ключам/индексам)
// <%?> - является идентификатором опкода (кода операции)
// ? - является самим опкодом (может задаваться в ascii/hex/dec/bin формах)
char meta_description[] =
    " <%1> = 1\n"
    " <%2> = 2\n"
    " <%3> = 3\n"
    " <%4> = 4\n"
    " <%5> = 5\n"
;

void _(int argc, char *argv[])
{
    setlocale(0, "");
    //
    Action();
    //#include "..\..\batch_files\action.txt"
}
int main(int argc, char *argv[]) { _(argc, argv); }