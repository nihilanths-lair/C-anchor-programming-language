/*/
 *    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->
/*/
#include <stdio.h>

// Заменить на нужное название
#define FILE_NAME "template.c"

int main()
{
    FILE *f = fopen(""FILE_NAME"", "w");
    fprintf(f, "/*/\n");
    fprintf(f, " *    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->\n");
    fprintf(f, "/*/\n");
    fprintf(f, "// Подключаемые библиотеки\n");
    fprintf(f, "#include <stdio.h>\n");
    fprintf(f, "\n");
    fprintf(f, "unsigned char operation_code[0xFF];\n");
    fprintf(f, "\n");
    fprintf(f, "int main()\n");
    fprintf(f, "{\n");
    fprintf(f, "    FILE *f;\n");
    fprintf(f, "    // Открытие файла на чтение\n");
    fprintf(f, "    if ((f = fopen(\"2.bf++\", \"r\")) == NULL) return 1;\n");
    fprintf(f, "    // Чтение всего файла целиком\n");
    fprintf(f, "    fread(operation_code, sizeof (operation_code), 1, f);\n");
    fprintf(f, "    void *address[] =\n");
    fprintf(f, "    {\n");
    for (unsigned char i = 0; i < 15; i++)
    {
        fprintf(f, "        ");
        for (unsigned char j = 0; j < 15; j++)
        {
            fprintf(f, "&&_0x%02X, ", i * 16 + j);
        }
        fprintf(f, "&&_0x%02X,\n", i * 16 + 15);
        //fprintf(f, "\n");
    }
    for (unsigned char i = 15; i < 16; i++)
    {
        fprintf(f, "        ");
        for (unsigned char j = 0; j < 15; j++)
        {
            fprintf(f, "&&_0x%02X, ", i * 16 + j);
        }
        fprintf(f, "&&_0x%02X\n", i * 16 + 15);
        //fprintf(f, "\n");
    }
    fprintf(f, "    };\n");
    fprintf(f, "    unsigned char i = 0xFF;\n");
    fprintf(f, "    // Обработчик команд (диспетчеризация)\n");
    fprintf(f, "    _0x__:\n");
    fprintf(f, "    goto *address[operation_code[i]];\n");
    fprintf(f, "    //-==========-[RUN_BLOCK]-==========-//\n");
    fprintf(f, "    _0x%02X: // %s\n", 0, "STOP");
    fprintf(f, "     printf(\"_0x00: STOP\");\n");
    //fprintf(f, "     return 0;\n");
    fprintf(f, "    ");
    for (unsigned char j = 1; j < 15; j++)
    {
        fprintf(f, "_0x%02X:", 0 + j); // 0 * 16 + j
    }
    fprintf(f, "_0x%02X:\n", 15); // 0 * 16 + 15
    for (unsigned char i = 1; i < 15; i++)
    {
        fprintf(f, "    ");
        for (unsigned char j = 0; j < 15; j++)
        {
            fprintf(f, "_0x%02X:", i * 16 + j);
        }
        fprintf(f, "_0x%02X:\n", i * 16 + 15);
        //fprintf(f, "\n");
    }
    for (unsigned char i = 15; i < 16; i++)
    {
        fprintf(f, "    ");
        for (unsigned char j = 0; j < 15; j++)
        {
            fprintf(f, "_0x%02X:", i * 16 + j);
        }
        fprintf(f, "_0x%02X:", i * 16 + 15);
        //fprintf(f, "\n");
    }
    fprintf(f, "\n");
    //fprintf(f, "     return 0;\n");
    fprintf(f, "    //-==========-[END_BLOCK]-==========-//\n");
    fprintf(f, "    return 0;\n");
    fprintf(f, "}");
    fclose(f);
    return 0;
}