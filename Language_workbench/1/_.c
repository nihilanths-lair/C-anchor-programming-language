/** [Path: Language_workbench/1/_.c] **/

#include <stdio.h>
#include <locale.h>

#include "_.h"

int main()
{
   setlocale(0, "");

   lexical_analyzer("/!\\");
   lexical_synthesizer("/!\\");

   syntactic_analyzer("/!\\");
   syntactic_synthesizer("/!\\");

   semantic_analyzer("/!\\");
   semantic_synthesizer("/!\\");

   return 0;
}