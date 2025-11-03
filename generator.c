/*/
 *    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->
/*/
#include <stdio.h>
//#include <stdlib.h> // Для system()

// Заменить на нужное название
#define FILE_NAME "template.c"

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
    fprintf(f, "unsigned char operation_code[0xFF];\n");
    fprintf(f, "unsigned char conveyor = 0xFF; // Размер конвейера всегда должен соответствовать размеру `operation code`\n");
    fprintf(f, "unsigned char memory[0xFF]; // = {0}; / Если требуется инициализировать память для избавления от мусора\n");
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
    fprintf(f, "    // Обработчик команд (диспетчеризация)\n");
    fprintf(f, "    _%02X: goto *address[operation_code[++conveyor]];\n", 0x100);
    //fprintf(f, "    //-==========-[RUN_BLOCK]-==========-//\n");
    fprintf(f, "    _%02X: // %s\n", 0, "STOP");
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", conveyor, conveyor, operation_code[conveyor], operation_code[conveyor]);\n");
    fprintf(f, "     return 0;\n");
    fprintf(f, "    _%02X: // %s\n", 1, "INC");
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", conveyor, conveyor, operation_code[conveyor], operation_code[conveyor]);\n");
    fprintf(f, "     memory[conveyor]++;\n");
    fprintf(f, "     goto *address[operation_code[++conveyor]]; // goto _100; // \n");
    fprintf(f, "    _%02X: // %s\n", 2, "DEC");
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\", conveyor, conveyor, operation_code[conveyor], operation_code[conveyor]);\n");
    fprintf(f, "     memory[conveyor]--;\n");
    fprintf(f, "     goto *address[operation_code[++conveyor]]; // goto _100; // \n");
    fprintf(f, "    ");
    for (unsigned char j = 2; j < 15; j++)
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
    fprintf(f, "     printf(\"Offset: [%%02X|%%03d], byte: [%%02X|%%03d]\\n\", conveyor, conveyor, operation_code[conveyor], operation_code[conveyor]); // Unused/Reserve Instructions @ Undefined behavior...\n");
    fprintf(f, "     goto *address[operation_code[++conveyor]]; // goto loop; // \n");
    //fprintf(f, "     return 0;\n");
    //fprintf(f, "    //-==========-[END_BLOCK]-==========-//\n");
    fprintf(f, "    return 0;\n");
    fprintf(f, "}");
    fclose(f);
    //f = fopen("0.bat", "w");
    //fprintf(f, "cd C:\\msys64\\ucrt64\\bin\n");
    //fprintf(f, "gcc.exe 0.exe -o "FILE_NAME""); // C:\Users\Глеб\Documents\Coding
    //fclose(f);
    return 0;
}