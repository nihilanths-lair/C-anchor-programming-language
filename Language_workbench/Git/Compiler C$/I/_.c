#include <stdio.h>
#include <locale.h>
#include <string.h>

void lexical_synthesizer(const char *strinp)
{
   int pos_cursor = 0;
   while (*strinp != '\0')
   {
      while (*strinp >= '0' && *strinp <= '9')
      {
         pos_cursor++;
         strinp++;
      }
      printf("\nTOKEN__NUMBER");
      switch (*strinp)
      {
         case '*':
            printf("\nTOKEN__OP_MUL");
            pos_cursor++;
            strinp++;
            break;

         case '/':
            printf("\nTOKEN__OP_DIV");
            pos_cursor++;
            strinp++;
            break;

         case '+':
            printf("\nTOKEN__OP_ADD");
            pos_cursor++;
            strinp++;
            break;

         case '-':
            printf("\nTOKEN__OP_SUB");
            pos_cursor++;
            strinp++;
            break;
         
         default:
            //printf("\nTOKEN__UNKNOWN));
            pos_cursor++;
            strinp++;
            break;
      }
      //strinp++;
   }
   return;
}

int main()
{
   setlocale(0, "");
   printf("\n<DEBUG>: Интерпретатор выражений запущен.\n");
   char strinp[100];
   while ('!')
   {
      printf("\n/ Ввод /> ");
      fgets(strinp, sizeof (strinp), stdin); // Читает строку с пробелами
      strinp[strcspn(strinp, "\n")] = '\0';  // Без этого, print может не выводить лишнюю новую строку

      // здесь начинается самое интересное
      // включаем в работу токенизатор
      //lexical_analyzer(text);    // проводит анализ (сканирует) и выдаёт ошибки лексем
      lexical_synthesizer(strinp); // воспроизводит цепочку (последовательность) токенов

      printf("\n/ Вывод /> %s\n", strinp);
      //printf("TOKEN_%s", getTokenStr(1));
   }
   return 0;
}