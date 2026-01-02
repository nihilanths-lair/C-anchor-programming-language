#include <stdio.h>
#include <locale.h>
#include <string.h>

// таблица имён токенов
char table_token_name[][40+1] =
{
   {"NUMBER"},
   {"OP_MUL"},
   {"OP_DIV"},
   {"OP_ADD"},
   {"OP_SUB"}
};

int __number_of_tokens = 0;
char *token__name[40];  // имя токена
char *token__value[40]; // значение токена

void lexical_synthesizer(const char *strinp)
{
   int pos_cursor = 0;
   while (*strinp != '\0')
   {
      if (*strinp >= '0' && *strinp <= '9')
      {
         pos_cursor++;
         strinp++;
         if (*strinp == '\0') return;
         while (*strinp >= '0' && *strinp <= '9')
         {
            pos_cursor++;
            strinp++;
            if (*strinp == '\0') break;
         }
         //printf("\nTOKEN__NUMBER");
         printf("\n[До]: token: {name: %s, value: %s}", token__name[__number_of_tokens], token__value[__number_of_tokens]);
         token__name[__number_of_tokens] = "NUMBER";
         token__value[__number_of_tokens] = (char *) &strinp[__number_of_tokens];
         token__value[__number_of_tokens][pos_cursor] = '\0';
         printf("\n[После]: token: {name: %s, value: %s}", token__name[__number_of_tokens], token__value[__number_of_tokens]);
         __number_of_tokens++;
      }
      switch (*strinp)
      {
         case '*':
            //printf("\nTOKEN__OP_MUL");
            printf("\n[До]: token: {name: %s, value: %s}", token__name[__number_of_tokens], token__value[__number_of_tokens]);
            token__name[__number_of_tokens] = "OP_MUL";
            token__value[__number_of_tokens] = (char *) &strinp[__number_of_tokens];
            token__value[__number_of_tokens][pos_cursor] = '\0';
            printf("\n[После]: token: {name: %s, value: %s}", token__name[__number_of_tokens], token__value[__number_of_tokens]);
            __number_of_tokens++;
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