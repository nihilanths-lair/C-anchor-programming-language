/** [Path: Language_workbench/1/_.c] **/

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include "_.h"

int main(int argc, char *argv[])
{
   setlocale(0, "");
   //putchar('\n');

   syntactic_analyzer("5 + 6");  // Разбирает синтаксис
   //syntactic_synthesizer();      // Воспроизводит  AST
   //code_generator(ABSTRACT_COMPUTING_MACHINE, "_.bin", "^");


   //putchar('\n');
   return 0;
}
// Интерпретатор грамматики
   //char byte_code[] = {'^', '\0'};
   //parse_string("x = 8", "Идентификатор = Число"); // Спецификация
   //code_constructor();
/*
   if (strcmp(argv[0], "meta_compiler.exe")) return -1;

   FILE *file = fopen(argv[1], "rb");
   if (file == NULL) return -1;
   
   fseek(file, 0, SEEK_END);
   long fsize = ftell(file);
   printf("\nfsize = %ld", fsize);
   
   // Выделяем память с запасом для нулевого терминатора
   char *lang = malloc(fsize + 1);
   if (lang == NULL)
   {
      fclose(file);
      return -1;
   }
   
   fseek(file, 0, SEEK_SET);
   size_t count_symbols = fread(lang, 1, fsize, file); // Читаем fsize байт
   fclose(file);
   
   lang[count_symbols] = '\0'; // Корректно завершаем строку
   
   printf("\ncount_symbols = %zu", count_symbols);
   printf("\nlang:\n%s", lang);
   
   free(lang);
*/