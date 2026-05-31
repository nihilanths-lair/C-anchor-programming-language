// </ Прожектор v.0.0.1 />

#include <stdio.h>
#include <string.h>

char source_code[0xFFFFFF];
char stitched_source_code[0xFFFFFF] = "\
</emit_c:\r\n\
#include <stdio.h>\r\n\
#include <string.h>\r\n\
\r\n\
int main(int argc, char *argv[])\r\n\
{\r\n\
    return 0;\r\n\
}\r\n\
/>\
";

void Lexer(const char *source_code)
{
    putchar('\n');
    while (*source_code != '\0')
    {
        int is_tag_processed = 0; // Флаг, чтобы знать, обрабатывали ли мы тег
        if (*source_code == '<')
        {
            if (!strncmp(source_code, "</emit_c:", strlen("</emit_c:")))
            {
                const char *check_ptr = source_code + strlen("</emit_c:");
                // Проверим перенос строки для Windows стиль синтаксиса (\r\n)
                if (*check_ptr == '\r' && *(check_ptr + 1) == '\n')
                {
                    source_code = check_ptr + 2; // Перешагиваем тег и \r\n
                    printf("\n Оператор многострочной вставки С-кода обнаружен.");
                    is_tag_processed = 1;
                    
                    while (*source_code != '\0') 
                    {
                        if (*source_code == '/' && *(source_code + 1) == '>') 
                        {
                            source_code += 1; // Шагаем на '>', а нижний source_code++ выведет нас за тег
                            break;
                        }
                        putchar(*source_code);
                        source_code++;
                    }
                }
                // Проверим перенос строки для UNIX стиль синтаксиса (\n)
                else if (*check_ptr == '\n')
                {
                    source_code = check_ptr + 1; // Перешагиваем тег и \n
                    printf("\n Оператор многострочной вставки С-кода обнаружен.");
                    is_tag_processed = 1;
                    
                    while (*source_code != '\0') 
                    {
                        if (*source_code == '/' && *(source_code + 1) == '>') 
                        {
                            source_code += 1; // Шагаем на '>', а нижний source_code++ выведет нас за тег
                            break;
                        }
                        putchar(*source_code);
                        source_code++;
                    }
                }
            }
        }
        // Если мы только что успешно обработали и закрыли тег, указатель уже сдвинут.
        // Во всех остальных случаях (обычный символ или ложный <) делаем стандартный шаг вперед.
        if (is_tag_processed == 0)
        {
            if (*source_code != '<') { putchar(*source_code); }
            source_code++;
        }
        else { source_code++; }
    }
}

int main(int argc, char *argv[])
{
    printf("\n\n");
    Lexer(stitched_source_code);
    printf("\n\n");

    if (argc < 2)
    {
        printf("\n compile: %s source_code.cdlr\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        printf("\n <Error>: Не удалось открыть файл ``%s``!", argv[1]);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    fread(source_code, sizeof (char), file_size, file);
    source_code[file_size] = '\0';
    
    Lexer(source_code);
    putchar('\n');
    return 0;
}