#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

// Константы типов токенов (в Pawn это были бы дефайны)
#define TOK_EOF   0
#define TOK_WHILE 1
#define TOK_ID    2
#define TOK_NUM   3
#define TOK_OP    4

// Глобальное состояние текущего токена (наши "структуры")
int tok_type;
int tok_value;
char tok_text[64];

// Указатель на текущую позицию в строке исходного кода
const char *src_ptr;

// Поточная функция: считывает ровно ОДИН следующий токен
void next_token()
{
    // 1. Пропускаем пробелы
    while (*src_ptr != '\0' && isspace(*src_ptr)) { src_ptr++; }

    // Если дошли до конца текста
    if (*src_ptr == '\0')
    {
        tok_type = TOK_EOF;
        return;
    }

    // 2. Разбираем числа
    if (isdigit(*src_ptr))
    {
        tok_value = 0;
        while (isdigit(*src_ptr))
        {
            tok_value = tok_value * 10 + (*src_ptr - '0');
            src_ptr++;
        }
        tok_type = TOK_NUM;
        return;
    }

    // 3. Разбираем имена переменных и ключевые слова
    if (isalpha(*src_ptr) || *src_ptr == '_')
    {
        int len = 0;
        while (isalnum(*src_ptr) || *src_ptr == '_')
        {
            if (len < 63) { tok_text[len++] = *src_ptr; }
            src_ptr++;
        }
        tok_text[len] = '\0'; // Закрываем строку

        // Проверяем на ключевое слово "while"
        if (strcmp(tok_text, "while") == 0) { tok_type = TOK_WHILE; }
        else { tok_type = TOK_ID; } // Просто переменная
        return;
    }

    // 4. Разбираем операторы (=, +, {, }, (, ))
    tok_text[0] = *src_ptr;
    tok_text[1] = '\0';
    tok_type = TOK_OP;
    src_ptr++;
}

int main()
{
    setlocale(0, "");

    // Наш подопытный исходный код (Мини-Си)
    const char *code = "while x = 42";
    
    // Инициализируем указатель лексера на начало строки
    src_ptr = code;
    
    printf("\n Разбор строки: \"%s\"\n", code);
    
    // Запускаем цикл, пока лексер не встретит конец файла (TOK_EOF)
    next_token();
    while (tok_type != TOK_EOF)
    {
        if (tok_type == TOK_WHILE) { printf("\n Токен: КЛЮЧЕВОЕ СЛОВО [while]"); } 
        else if (tok_type == TOK_ID) { printf("\n Токен: ИДЕНТИФИКАТОР (имя) [%s]", tok_text); } 
        else if (tok_type == TOK_NUM) { printf("\n Токен: ЧИСЛО [%d]\n", tok_value); } 
        else if (tok_type == TOK_OP) { printf("\n Токен: ОПЕРАТОР/ЗНАК [%s]", tok_text); }
        
        // Просим лексер выдать следующий токен
        next_token();
    }
    printf("\n Разбор успешно завершен!\n");
    return 0;
}