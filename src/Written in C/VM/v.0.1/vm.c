// <!-- Virtual Machine C$ v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>

#include "..\..\batch_files\action.c"

int main(int argc, char *argv[])
{
    setlocale(0, "");
    //
	//-/
    //Action();
    #include "..\..\batch_files\action.txt"
    //-/
    //
    return 0;
}