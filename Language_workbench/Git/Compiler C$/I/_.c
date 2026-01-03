#include <stdio.h>
#include <locale.h>
#include <string.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_VALUE_LEN 50

// Типы токенов
typedef enum {
   TOKEN_NUMBER,
   TOKEN_OP_MUL,
   TOKEN_OP_DIV,
   TOKEN_OP_ADD,
   TOKEN_OP_SUB,
   TOKEN_UNKNOWN
} TokenType;

// Структура для хранения токена
typedef struct {
   TokenType type;
   char value[MAX_TOKEN_VALUE_LEN];
} Token;

Token tokens[MAX_TOKENS];
int token_count = 0;

// Функция для добавления токена
void add_token(TokenType type, const char *value)
{
   if (token_count >= MAX_TOKENS)
   {
      printf("\nСлишком много токенов!");
      return;
   }
   tokens[token_count].type = type;
   if (value)
   {
      strncpy(tokens[token_count].value, value, MAX_TOKEN_VALUE_LEN - 1);
      tokens[token_count].value[MAX_TOKEN_VALUE_LEN - 1] = '\0';
   }
   else {
      tokens[token_count].value[0] = '\0';
   }
   token_count++;
}

// Лексический анализатор
void lexical_analyzer(const char *input)
{
   token_count = 0; // Сброс счетчика токенов
   while (*input)
   {
      // Пропускаем пробелы
      if (*input == ' ')
      {
         input++;
         continue;
      }

      // Если цифра, то считываем всё число
      if (*input >= '0' && *input <= '9')
      {
         char number[MAX_TOKEN_VALUE_LEN];
         int i = 0;
         while (*input >= '0' && *input <= '9' && i < MAX_TOKEN_VALUE_LEN - 1) {
            number[i++] = *input++;
         }
         number[i] = '\0';
         add_token(TOKEN_NUMBER, number);
         continue;
      }

      // Операторы
      switch (*input){
      case '*':
         add_token(TOKEN_OP_MUL, "*");
         break;
      case '/':
         add_token(TOKEN_OP_DIV, "/");
         break;
      case '+':
         add_token(TOKEN_OP_ADD, "+");
         break;
      case '-':
         add_token(TOKEN_OP_SUB, "-");
         break;
      default: // Неизвестный символ
         char unknown[2] = {*input, '\0'};
         add_token(TOKEN_UNKNOWN, unknown);
         break;
      }
      input++;
   }
}

// Функция для печати токенов
void print_tokens()
{
   for (int i = 0; i < token_count; i++)
   {
      const char *type_str;
      switch (tokens[i].type)
      {
         case TOKEN_NUMBER: type_str = "NUMBER"; break;
         case TOKEN_OP_MUL: type_str = "OP_MUL"; break;
         case TOKEN_OP_DIV: type_str = "OP_DIV"; break;
         case TOKEN_OP_ADD: type_str = "OP_ADD"; break;
         case TOKEN_OP_SUB: type_str = "OP_SUB"; break;
         default: type_str = "UNKNOWN"; break;
      }
      printf("Token: { type: %s, value: %s }\n", type_str, tokens[i].value);
   }
}

int main()
{
   setlocale(LC_ALL, "");
   
   char input[100];
   printf("Введите выражение: ");
   fgets(input, sizeof (input), stdin);
   input[strcspn(input, "\n")] = '\0'; // Убираем символ новой строки

   lexical_analyzer(input);
   print_tokens();

   return 0;
}