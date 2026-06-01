// </ compiler_stage_0.c />

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define OP_END_OF_FILE     0
#define OP_READ_CHAR       1
#define OP_MATCH_CHAR      2
#define OP_EMIT_UNTIL_TAG  3

char source_code[0xFFFFFF];
int src_idx = 0;
char src_char = '\0';
int pc = 0;

void execute_meta_core(const int *rules_table)
{
    while (rules_table[pc] != OP_END_OF_FILE)
    {
        switch (rules_table[pc])
        {
            case OP_READ_CHAR:
            {
                src_char = source_code[src_idx];
                if (src_char != '\0') { src_idx++; }
                pc++;
                break;
            }
            case OP_MATCH_CHAR:
            {
                int expected_char = rules_table[pc + 1];
                if (src_char == expected_char) 
                { 
                    pc += 2;
                }
                else 
                { 
                    // Если символ не совпал (например, это обычный код C$, а не тег),
                    // мы просто сбрасываем PC на начало цикла чтения следующего символа
                    pc = 0; 
                }
                break;
            }
            case OP_EMIT_UNTIL_TAG:
            {
                while (source_code[src_idx] != '\0')
                {
                    if (source_code[src_idx] == '/')
                    {
                        if (source_code[src_idx + 1] == '>')
                        {
                            src_idx += 2;
                            src_char = source_code[src_idx];
                            break;
                        }
                    }
                    if (source_code[src_idx] != '\r') { putchar(source_code[src_idx]); }
                    src_idx++;
                }
                pc++;
                break;
            }
            default:
            {
                pc++;
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("\n compile: %s source_code.meta\n", argv[0]);
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
    fclose(file);

    // УПРАВЛЯЮЩИЙ БАЙТ-КОД ХОСТА: ищет строго тег и вырезает Си-код
    int host_rules[] = {
        OP_READ_CHAR,
        OP_MATCH_CHAR, '<',
        OP_READ_CHAR,
        OP_MATCH_CHAR, '/',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'c',
        OP_READ_CHAR,
        OP_MATCH_CHAR, '-',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'i',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'n',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'j',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'e',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'c',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 't',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'i',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'o',
        OP_READ_CHAR,
        OP_MATCH_CHAR, 'n',
        OP_READ_CHAR,
        OP_MATCH_CHAR, ':',
        OP_READ_CHAR, 
        OP_EMIT_UNTIL_TAG,
        OP_END_OF_FILE
    };

    // Главный цикл хоста: бежим по файлу, пока не кончится исходник
    while (source_code[src_idx] != '\0')
    {
        pc = 0; // Сбрасываем Program Counter на старт мета-цепочки
        execute_meta_core(host_rules);
        
        // Если мета-цепочка сбросилась из-за несовпадения букв тега,
        // мы просто продвигаем глобальный индекс хоста на 1 символ вперед
        if (pc == 0 && source_code[src_idx] != '\0') { src_idx++; }
    }

    return 0;
}