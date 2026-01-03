#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

// Используем enum для типов токенов (чище чем массив строк)
typedef enum {
   TOKEN_UNKNOWN = 0,
   TOKEN_NUMBER,
   TOKEN_OP_MUL,
   TOKEN_OP_DIV,
   TOKEN_OP_ADD,
   TOKEN_OP_SUB,
   TOKEN_LPAREN,   // '('
   TOKEN_RPAREN,   // ')'
   TOKEN_EOF       // Конец входных данных
} TokenType;

// Структура для токена (лучше чем два массива)
typedef struct {
   TokenType type;
   char value[32];  // Для хранения значения (числа как строка)
   int line;        // Для отладки (позже)
} Token;

// Таблица для отладочного вывода
const char* token_names[] =
{
   "UNKNOWN",
   "NUMBER",
   "OP_MUL",
   "OP_DIV",
   "OP_ADD", 
   "OP_SUB",
   "LPAREN",
   "RPAREN",
   "EOF"
};

Token tokens[100];  // Массив токенов
int token_count = 0;
int current_pos = 0;  // Текущая позиция в исходной строке

// Прототипы функций
void add_token(TokenType type, const char *value);
void lexical_analyzer(const char *input);
void print_tokens();

// Добавить токен в массив
void add_token(TokenType type, const char *value)
{
   if (token_count >= 100)
   {
      printf("Ошибка: слишком много токенов!\n");
      return;
   }
   tokens[token_count].type = type;
   if (value)
   {
      strncpy(tokens[token_count].value, value, 31);
      tokens[token_count].value[31] = '\0';
   }
   else tokens[token_count].value[0] = '\0';
   tokens[token_count].line = 1;  // Пока всегда 1
   token_count++;
}

// Лексический анализатор (исправленный)
void lexical_analyzer(const char* input) {
   token_count = 0;  // Сбрасываем счетчик
   current_pos = 0;
   
   while (input[current_pos] != '\0') {
      char c = input[current_pos];
      
      // Пропускаем пробелы
      if (c == ' ' || c == '\t') {
         current_pos++;
         continue;
      }
      
      // Число
      if (c >= '0' && c <= '9')
      {
         char number[32];
         int i = 0;
         
         // Читаем все цифры
         while (input[current_pos] >= '0' && input[current_pos] <= '9')
         {
            if (i < 31) number[i++] = input[current_pos];
            current_pos++;
         }
         number[i] = '\0';
         add_token(TOKEN_NUMBER, number);
         continue;
      }
      
      // Операторы и скобки
      switch (c) {
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

      case '(':
         add_token(TOKEN_LPAREN, "(");
         break;

      case ')':
         add_token(TOKEN_RPAREN, ")");
         break;

      default: // Неизвестный символ
         char unknown[2] = {c, '\0'};
         add_token(TOKEN_UNKNOWN, unknown);
         break;
      }
      current_pos++;
   }
   // Добавляем токен конца файла
   add_token(TOKEN_EOF, "");
}

// Вывод всех токенов
void print_tokens()
{
   printf("\n=== ТОКЕНЫ (%d штук) ===\n", token_count);
   for (int i = 0; i < token_count; i++)
   {
      printf("[%02d] %-10s", i, token_names[tokens[i].type]);
      if (tokens[i].value[0] != '\0') printf(" : '%s'", tokens[i].value);
      printf("\n");
   }
   printf("=======================\n");
}

// Простой парсер и интерпретатор (добавим позже)
int parse_expression();

int main()
{
   setlocale(LC_ALL, "");
   
   printf("<DEBUG>: Интерпретатор выражений запущен.\n");
   printf("Примеры: 2+3, (5-2)*4, 10/2\n");
   printf("Введите 'выход' для завершения.\n\n");
   
   char input[100];
   
   while (1)
   {
      printf(">>> ");
      fgets(input, sizeof(input), stdin);
      input[strcspn(input, "\n")] = '\0';
      
      // Проверка на выход
      if (strcmp(input, "выход") == 0 || strcmp(input, "exit") == 0) break;
      
      // Лексический анализ
      lexical_analyzer(input);
      
      // Вывод токенов
      print_tokens();
      
      // TODO: Парсинг и вычисление
      // int result = parse_expression();
      // printf("Результат: %d\n", result);
   }
   printf("Завершение работы.\n");
   return 0;
}