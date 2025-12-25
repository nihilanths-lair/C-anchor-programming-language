#include <stdio.h>
#include <locale.h>

/*/ |№1| /*/ void compile(const char *code);
/*/ |№2| /*/ void execute(const char *code);
/*/ |№3| /*/ void parse(const char *code);
/*/ |№4| /*/ void analysis(const char *code);
/*/ |№5| /*/ void synthesis();

int main()
{
   setlocale(0, "");

   analysis("code...");
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
void analysis(const char *code)
{
}
void synthesis()
{
}