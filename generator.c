/*/
 *    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->
/*/
// Подключаемые библиотеки
#include <stdio.h>
#include <locale.h>
#include <string.h>
//#include <stdlib.h> // Для system()

//#define __run_namespace__ {
//#define __end_namespace__ }

// Заменить на нужное название
#define FILE_NAME "interpreter.c"
///#define FILE_NAME "compiler.c"
// Установите 1 если хотите отслеживать работу интерпретатора, в противном случае 0
#define DEBUG 1

unsigned char source_code[4096];
//FILE *f;
void Main()
{
    FILE *f = fopen(""FILE_NAME"", "w");
    fprintf(f, "/*/\n");
    fprintf(f, " *    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->\n");
    fprintf(f, "/*/\n");
    fprintf(f, "// Подключаемые библиотеки\n");
    fprintf(f, "#include <stdio.h>\n");
    fprintf(f, "#include <locale.h>\n");
    fprintf(f, "\n");
    fprintf(f, "unsigned char __operation_code__[0xFF];\n");
    fprintf(f, "unsigned char *operation_code = __operation_code__;\n");

    fprintf(f, "//unsigned char conveyor = 0xFF; // Размер конвейера всегда должен соответствовать размеру `operation code`\n");

    fprintf(f, "#define MEMORY __pointer_2\n");
    fprintf(f, "unsigned char MEMORY = 0x00;\n");
    fprintf(f, "unsigned char memory[0xFF]; // = {0}; / Если требуется инициализировать память для избавления от мусора\n");

    fprintf(f, "#define STACK __pointer_3\n");
    fprintf(f, "unsigned char STACK = 0x00;\n");
    fprintf(f, "unsigned char stack[0xFF]; // = {0}; / Если требуется инициализировать стек для избавления от мусора\n");
    fprintf(f, "\n");
    fprintf(f, "#include \"declaration.h\"\n");

    fprintf(f, "unsigned char Main()\n");
    fprintf(f, "{\n");
    //fprintf(f, "    setlocale(0, \"\"); // для отображения кириллицы\n");
    fprintf(f, "    FILE *f;\n");
    fprintf(f, "    // Открытие файла на чтение\n");
    fprintf(f, "    if ((f = fopen(\"0.bf++\", \"r\")) == NULL) return 1;\n");
    fprintf(f, "    // Чтение всего файла целиком\n");
    fprintf(f, "    fread(operation_code, sizeof (operation_code), 1, f);\n");
    fprintf(f, "    fclose(f);\n");
    
    fprintf(f, "    vCPU();\n");

    fprintf(f, "}\n");

    #if (DEBUG)
    fprintf(f, "FILE *f;\n");
    fprintf(f, "void ShowMemoryPanel()\n");
    fprintf(f, "{\n");
    fprintf(f, "    static unsigned char counter = 0;\n");
    fprintf(f, "    if (!counter) fprintf(f, \"        HEX|DEC\\n\");\n");
    fprintf(f, "    else fprintf(f, \"\\n\\n        HEX|DEC\\n\");\n");  
    fprintf(f, "    ++counter;\n");
    fprintf(f, "    fprintf(f, \"Counter: %%02X|%%03d\\n\", counter, counter);\n");
    fprintf(f, "    fprintf(f, \"Bytecode: %%02X|%%03d\\n\", *operation_code, *operation_code);\n");
    fprintf(f, "    fprintf(f, \"+--------+--------------------------------------------------------------------------------+----------------+\\n\");\n");
    fprintf(f, "    fprintf(f, \"| Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |\\n\");\n");
    fprintf(f, "    fprintf(f, \"+--------+--------------------------------------------------------------------------------+----------------+\\n\");\n");
    for (unsigned char iter_0 = 0; iter_0 < 16; iter_0++)
    {
        fprintf(f, "    fprintf(f, \"|    %3d |", iter_0 * 16);
        for (unsigned char iter_1 = 0; iter_1 < 16; iter_1++)
        {
            fprintf(f, " %%03d ");
        }
        fprintf(f, "|                |\\n\",");
        //fprintf(f, " |                |\\n\",\n");
        //fprintf(f, "    ");
        for (unsigned char iter_1 = 0; iter_1 < 15; iter_1++)
        {
            fprintf(f, " memory[%d],", iter_0 * 16 + iter_1);
        }
        //fprintf(f, " memory[%d]\n", 15);
        fprintf(f, " memory[%d]);\n", iter_0 * 16 + 15);
        //fprintf(f, "     );\n");
    }
    fprintf(f, "    fprintf(f, \"+--------+--------------------------------------------------------------------------------+----------------+\");\n");
    //fprintf(f, "    fprintf(f, \"\\n\");\n");
    fprintf(f, "}\n");
    #endif

    fprintf(f, "unsigned char vCPU()\n");
    fprintf(f, "{\n");
    fprintf(f, "    void *address[] =\n");
    fprintf(f, "    {\n");
    for (unsigned char i = 0; i < 15; i++)
    {
        fprintf(f, "        ");
        for (unsigned char j = 0; j < 15; j++)
        {
            fprintf(f, "&&_%02X, ", i * 16 + j);
        }
        fprintf(f, "&&_%02X,\n", i * 16 + 15);
        //fprintf(f, "\n");
    }
    for (unsigned char i = 15; i < 16; i++)
    {
        fprintf(f, "        ");
        for (unsigned char j = 0; j < 15; j++)
        {
            fprintf(f, "&&_%02X, ", i * 16 + j);
        }
        fprintf(f, "&&_%02X\n", i * 16 + 15);
        //fprintf(f, "\n");
    }
    fprintf(f, "    };\n");
    //fprintf(f, "    unsigned char i = 0xFF;\n");
    #if (DEBUG)
    fprintf(f, "    f = fopen(\"logging.txt\", \"w\");\n");
    //fprintf(f, "    fclose(f);\n");
    //fprintf(f, "    f = fopen(\"logging.txt\", \"a\");\n");
    //fprintf(f, "    ShowMemoryPanel();\n");
    #endif

    fprintf(f, "    //  /!\\ Запускаем процессор на исполнение команд / инструкций (даём старт) /!\\  //\n");
    fprintf(f, "    goto *address[*operation_code];\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    //unsigned char label_name[7][10+1] = {}
    unsigned char count = 0xFF;
    fprintf(f, "    _%02X: // %s\n", ++count, "STOP");
    #if (DEBUG)
    fprintf(f, "     ShowMemoryPanel();\n");
    #endif
    fprintf(f, "     return 0;\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `+` | INC @~> (Increment/Инкремент) текущей ячейки памяти");
    fprintf(f, "     memory[MEMORY]++;\n");
    #if (DEBUG)
    fprintf(f, "     ShowMemoryPanel();\n");
    #endif
    fprintf(f, "     goto *address[*(++operation_code)];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `-` | DEC @~> (Decrement/Декремент) текущей ячейки памяти");
    fprintf(f, "     memory[MEMORY]--;\n");
    #if (DEBUG)
    fprintf(f, "     ShowMemoryPanel();\n");
    #endif
    fprintf(f, "     goto *address[*(++operation_code)];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `>` | SCRF @~> Scroll forward ~ Прокрутка на шаг вперёд [|] (Move the memory pointer forward one step / Переместить указатель памяти на один шаг вперед) :: MMPFOS");
    #if (DEBUG)
    
    #endif
    fprintf(f, "     MEMORY++;\n");
    fprintf(f, "     goto *address[*(++operation_code)];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `<` | SCRB @~> Scroll back ~ Прокрутка на шаг назад [|] (Move the memory pointer back one step / Переместить указатель памяти на один шаг назад) :: MMPBOS");
    #if (DEBUG)
    
    #endif
    fprintf(f, "     MEMORY--;\n");
    fprintf(f, "     goto *address[*(++operation_code)];\n");  // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `=` | Поместить значение в текущую ячейку памяти / Place a value into the current memory cell (@~> PVICMC");
    #if (DEBUG)
    
    #endif
    fprintf(f, "     memory[MEMORY] = *(++operation_code);\n");
    fprintf(f, "     goto *address[*(++operation_code)];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `&` | Получить значение с текущей ячейки памяти / Get the value from the current memory cell (@~> GVFCMC");
    #if (DEBUG)
    
    #endif
    fprintf(f, "");
    fprintf(f, "     // <?> = memory[MEMORY];\n");
    fprintf(f, "     goto *address[*(++operation_code)];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "?? PUSH ??");
    fprintf(f, "    /*\n");
    #if (DEBUG)
    
    #endif
    fprintf(f, "     stack[STACK]++;\n");
    fprintf(f, "     goto *address[*(++operation_code)];\n"); // goto _100;
    fprintf(f, "    */\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "?? POP ??");
    fprintf(f, "    /*\n");
    #if (DEBUG)
    
    #endif
    fprintf(f, "     stack[STACK]--;\n");
    fprintf(f, "     goto *address[*(++operation_code)];\n"); // goto _100;
    fprintf(f, "    */\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "?? INT ??");
    fprintf(f, "    /*\n");
    #if (DEBUG)
    
    #endif
    fprintf(f, "     goto *address[*(++operation_code)];\n"); // goto _100;
    fprintf(f, "    */\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    ");
    for (unsigned char j = 10; j < 15; j++)
    {
        fprintf(f, "_%02X:", 0 + j); // 0 * 16 + j
    }
    fprintf(f, "_%02X:\n", 15); // 0 * 16 + 15
    for (unsigned char i = 1; i < 15; i++)
    {
        fprintf(f, "    ");
        for (unsigned char j = 0; j < 15; j++)
        {
            fprintf(f, "_%02X:", i * 16 + j);
        }
        fprintf(f, "_%02X:\n", i * 16 + 15);
        //fprintf(f, "\n");
    }
    for (unsigned char i = 15; i < 16; i++)
    {
        fprintf(f, "    ");
        for (unsigned char j = 0; j < 15; j++)
        {
            fprintf(f, "_%02X:", i * 16 + j);
        }
        fprintf(f, "_%02X:", i * 16 + 15);
        //fprintf(f, "\n");
    }
    fprintf(f, "\n");
    #if (DEBUG)
    // Unused/Reserve Instructions @ Undefined behavior...\n");
    #endif
    fprintf(f, "     goto *address[*(++operation_code)]; // goto _100;\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    fclose(f);\n");
    fprintf(f, "}\n");
    fprintf(f, "#include \"main.c\"");
    fclose(f);
}
#include "main.c"
/*
unsigned char *AcceptTheForm(const unsigned char *file_name)
{
    ...
    long arr_size = strlen(file_name);
    unsigned char str_1[] = ".form";
    unsigned char str_2[5+1] = {'\0'}, j = 0xFF;
    for (int i = arr_size; i; i--)
    {
        str_2[++j] = file_name[i];
        if (file_name[i] == '.') break;
    }
    if (strcmp(str_1, str_2)) return;
    ...
    FILE *f = fopen(file_name, "rb");
    fseek(f, 0, SEEK_END);
    long f_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(source_code, sizeof (source_code), 1, f);
    printf("%s\n", source_code);
    //unsigned char found[2] = {0};
    unsigned char i = 0xFF, j = 0xFF;
    unsigned char count = f_size;
    while (count)
    {
        ++i, ++j;
        //if (source_code[i] == '\0') break;
        printf("%d - '%c' | %d - '%c' | %d\n", j, source_code[j], i, source_code[i], count);
        if (source_code[i] == '\r') ++i;
        //if (source_code[i] == '\n') ++i;
        source_code[j] = source_code[i];
        //
        --count;
    }
    printf("f_size = %ld // размер файла\n", f_size);
    fclose(f);
    return source_code;
}
unsigned char yt[] = "\
/./.
 .    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->.
/./.
";
void format(const unsigned char s[])
{
    return;
}
void fformat(const unsigned char *file_name, const unsigned char *mode, const unsigned char *s)
{
    return;
}
*/