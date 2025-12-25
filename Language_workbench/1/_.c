#include <stdio.h>
#include <locale.h>

/*/ |№1| /*/ void compile(const char *code);
/*/ |№2| /*/ void execute();
/*/ |№3| /*/ void parse(const char *code);
/*/ |№4| /*/ void analysis();
/*/ |№5| /*/ void synthesis();

int main()
{
   setlocale(0, "");

   //parse("code...");
   compile("code...");
   //execute();

   return 0;
}

void compile(const char *code)
{
   parse(code);
}
void execute()
{
}
void parse(const char *code)
{
   analysis();
   synthesis();
}
void analysis()
{
}
void synthesis()
{
}