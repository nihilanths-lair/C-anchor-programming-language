#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

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

    repeat: switch (rules_table[pc]) {

    case OP_END_OF_FILE: break;

    case OP_READ_CHAR:

    {

        src_char = source_code[src_idx];

        if (src_char != '\0') { src_idx++; }

        pc++;

        goto repeat;

    }

    case OP_MATCH_CHAR:

    {

        if (src_char == rules_table[pc+1]) { pc += 2; }

        else

        { 

            // Если символ не совпал (например, это обычный код C$, а не тег),

            // мы просто сбрасываем PC на начало цикла чтения следующего символа

            pc = 0;

        }

        goto repeat;

    }

    case OP_EMIT_UNTIL_TAG:

    {

        while (source_code[src_idx] != '\0')

        {

            if (source_code[src_idx] == '/')

            {

                if (source_code[src_idx+1] == '>')

                {

                    src_idx += 2;

                    src_char = source_code[src_idx];

                    goto repeat;

                }

            }

            if (source_code[src_idx] != '\r') { putchar(source_code[src_idx]); }

            src_idx++;

        }

        pc++;

        goto repeat;

    }

    default:

    {

        pc++;

        goto repeat;

    }}

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



    int test_rules[] =

    {

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

        // Перешагиваем возможные переводы строк после двоеточия

        OP_READ_CHAR, 

        OP_EMIT_UNTIL_TAG, // Включаем слепую инъекцию Си-кода!

        OP_END_OF_FILE

    };

    execute_meta_core(test_rules);

    return 0;

}