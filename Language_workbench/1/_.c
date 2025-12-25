/** [Path: Language_workbench/1/_.c] **/

#include <stdio.h>
#include <locale.h>

#include "_.h"

int main()
{
   setlocale(0, "");

   parse_analysis("code...");
   //synthesis();
   //parse("code...");
   //compile("code...");
   //execute();

   return 0;
}

void compile(const char *code)
{
   //parse(code);
}
void execute(const char *code)
{
   //parse(code);
}
void parse(const char *code)
{
   //analysis();
   //synthesis();
}
void parse_analysis(const char *code)
{
   //synthesis();
}
void parse_synthesis(const char *code)
{
}