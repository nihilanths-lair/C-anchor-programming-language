// </ Прожектор v.0.0.1 />

#include <stdio.h>

char source_code[0xFFFFFF];
char stitched_source_code[0xFFFFFF] = "\
emit_c:\r\n\
#include <stdio.h>\r\n\
\r\n\
int main(int argc, char *argv[])\r\n\
{\r\n\
    return 0;\r\n\
}\
";

void Lexer(const char *source_code)
{
    putchar('\n');
    while (*source_code != '\0')
    {
        //printf("%c", *source_code);
        if (*source_code == 'e')
        {
            printf("%c", *source_code);
            source_code++;
        }
        else source_code++;
    }
}

int main(int argc, char *argv[])
{
    printf("\n argc: %d", argc);
    for (int i = 0; i < argc; i++) printf("\n argv %d: \"%s\"", i+1, argv[i]);
    //printf("\n Текст на кириллице!");
    printf("\n\n");
    printf("\n stitched_source_code\n···\n%s\n···", stitched_source_code);
    Lexer(stitched_source_code);
    printf("\n\n");
    for (int i = 0; stitched_source_code[i] != '\0'; i++) printf("%c", stitched_source_code[i]);
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
    printf("\n\n file_size: %ld\n", file_size);
    fread(source_code, sizeof (char), file_size, file);
    source_code[file_size] = '\0';
    printf("\n source_code\n···\n%s\n···", source_code);
    Lexer(source_code);
    putchar('\n');
    return 0;
}