#include <stdio.h>
#include <locale.h>

/*/ |¹1| /*/ void compile(const char *code);
/*/ |¹2| /*/ void parse(const char *code);
/*/ |¹3| /*/ void analysis();
/*/ |¹4| /*/ void synthesis();

int main()
{
   setlocale(0, "");

   //parse("code...");
   compile("code...");

   return 0;
}

void compile(const char *code)
{
   parse(code);
}
void parse(const char *code)
{
   printf(code);
}
void analysis()
{
}
void synthesis()
{
}