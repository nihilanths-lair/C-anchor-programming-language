/*/
 *    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->
/*/
// Подключаемые библиотеки
#include <stdio.h>
//#include <stdlib.h> // Для system()

// Заменить на нужное название
#define FILE_NAME "template.c"
// Установите 1 если хотите отслеживать работу интерпретатора, в противном случае 0
#define DEBUG 1

FILE *f;

int main()
{
    f = fopen(""FILE_NAME"", "w");
    fprintf(f, "/*/\n");
    fprintf(f, " *    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->\n");
    fprintf(f, "/*/\n");
    fprintf(f, "// Подключаемые библиотеки\n");
    fprintf(f, "#include <stdio.h>\n");
    fprintf(f, "#include <locale.h>\n");
    fprintf(f, "\n");
    fprintf(f, "//unsigned char __pointer_1 = 0x00;\n");
    fprintf(f, "//unsigned char __pointer_2 = 0x00;\n");
    fprintf(f, "\n");
    fprintf(f, "#define OPERATION_CODE __pointer_1\n");
    fprintf(f, "unsigned char OPERATION_CODE = 0x00;\n");
    fprintf(f, "unsigned char operation_code[0xFF];\n");
    fprintf(f, "//unsigned char conveyor = 0xFF; // Размер конвейера всегда должен соответствовать размеру `operation code`\n");
    fprintf(f, "\n");
    fprintf(f, "#define MEMORY __pointer_2\n");
    fprintf(f, "unsigned char MEMORY = 0x00;\n");
    fprintf(f, "unsigned char memory[0xFF]; // = {0}; / Если требуется инициализировать память для избавления от мусора\n");
    fprintf(f, "\n");
    fprintf(f, "#define STACK __pointer_3\n");
    fprintf(f, "unsigned char STACK = 0x00;\n");
    fprintf(f, "unsigned char stack[0xFF]; // = {0}; / Если требуется инициализировать стек для избавления от мусора\n");

    fprintf(f, "\n");

    fprintf(f, "int main()\n");
    fprintf(f, "{\n");
    fprintf(f, "    setlocale(0, \"\"); // для отображения кириллицы\n");
    fprintf(f, "    FILE *f;\n");
    fprintf(f, "    // Открытие файла на чтение\n");
    fprintf(f, "    if ((f = fopen(\"0.bf++\", \"r\")) == NULL) return 1;\n");
    fprintf(f, "    // Чтение всего файла целиком\n");
    fprintf(f, "    fread(operation_code, sizeof (operation_code), 1, f);\n");
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
    fprintf(f, "    _%02X: goto *address[operation_code[++OPERATION_CODE]];\n", 0x100);
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    //unsigned char label_name[7][10+1] = {}
    unsigned char count = 0xFF;
    //fprintf(f, "    //-==========-[RUN_BLOCK]-==========-//\n");
    fprintf(f, "    _%02X: // %s\n", ++count, "STOP");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     return 0;\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `+` | INC @~> (Increment/Инкремент) текущей ячейки памяти");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     memory[MEMORY]++;\n");
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `-` | DEC @~> (Decrement/Декремент) текущей ячейки памяти");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     memory[MEMORY]--;\n");
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `>` | SCRF @~> Scroll forward ~ Прокрутка на шаг вперёд [|] (Move the memory pointer forward one step / Переместить указатель памяти на один шаг вперед) :: MMPFOS");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     MEMORY++;\n");
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `<` | SCRB @~> Scroll back ~ Прокрутка на шаг назад [|] (Move the memory pointer back one step / Переместить указатель памяти на один шаг назад) :: MMPBOS");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     MEMORY--;\n");
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n");  // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `=` | PVICMC @~> (Place a value into the current memory cell / Поместить значение в текущую ячейку памяти)");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     memory[MEMORY] = operation_code[++OPERATION_CODE];\n");
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "BF: `&` | Получить значение с текущей ячейки памяти / Get the value from the current memory cell");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "");
    fprintf(f, "     // <?> = memory[MEMORY];\n");
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n"); // goto _100;
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "?? PUSH ??");
    fprintf(f, "    /*\n");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     stack[STACK]++;\n");
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n"); // goto _100;
    fprintf(f, "    */\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "?? POP ??");
    fprintf(f, "    /*\n");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     stack[STACK]--;\n");
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n"); // goto _100;
    fprintf(f, "    */\n");
    fprintf(f, "    //----------------------------------------------------------------------------------------------------//\n");
    //----------------------------------------------------------------------------------------------------//
    fprintf(f, "    _%02X: // %s\n", ++count, "?? INT ??");
    fprintf(f, "    /*\n");
    #if (DEBUG)
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]);\n");
    #endif
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]];\n"); // goto _100;
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
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\\n\", OPERATION_CODE, OPERATION_CODE, operation_code[OPERATION_CODE], operation_code[OPERATION_CODE]); // Unused/Reserve Instructions @ Undefined behavior...\n");
    #endif
    fprintf(f, "     goto *address[operation_code[++OPERATION_CODE]]; // goto _100;\n");
    //fprintf(f, "     return 0;\n");
    //fprintf(f, "    //-==========-[END_BLOCK]-==========-//\n");
    fprintf(f, "    return 0;\n");
    fprintf(f, "}");
    fclose(f);
    return 0;
}