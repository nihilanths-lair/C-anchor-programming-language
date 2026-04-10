// <!-- Virtual Machine C$ v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>

#include "..\..\batch_files\ProcAsciiChr.c"
#include "..\..\batch_files\ShowDashboard.c"

#include "..\..\batch_files\action.c"

void _()
{
    setlocale(0, "");
    Action();
    //#include "..\..\batch_files\action.txt"
}
int main(int argc, char *argv[]) { _(argc, argv); }