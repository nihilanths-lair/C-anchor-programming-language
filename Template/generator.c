#include <stdio.h>
#include <locale.h>
#include <string.h>

#define MAX_SIZE 0xFFFFFF
unsigned char buffer[MAX_SIZE];

#define EMPTY_LINE ""

#define SP1 " "
#define SP4 "    "

int main()
{
    setlocale(0, "");
    FILE *file;
    file = fopen("sample.c", "rb");
    if (!file)
    {
        printf("File: sample.с | Возникла проблема открытия файла...\n");
        return 0;
    }
    size_t byte = fread(buffer, 1, sizeof (buffer) - 1, file);
    //printf("byte = %d\n", byte);
    //printf("buffer = \"\n%s\n\"\n", buffer);
    fclose(file);
    file = fopen("interpreter.c", "wb");
    if (!file)
    {
        printf("File: generator.c | Возникла проблема открытия файла...\n");
        return 0;
    }
    // --- место для автогенерации ---
    fprintf(file, "#include <stdio.h>\n");
    fprintf(file, "#include <locale.h>\n\n");
    //fprintf(file, "##include <string.h>\n\n");

    fprintf(file, "#define then {\n");
    fprintf(file, "#define end }\n\n");

    fprintf(file, "#define register def_register\n\n");

    fprintf(file, "unsigned char __instructions__[0xFF]; // = \" \\nOhota byd' soboi!\\n\"; // Байт-код/оп-код\n");
    fprintf(file, "unsigned char *instructions = __instructions__;\n\n");

    fprintf(file, "unsigned char memory[0xFF]; // Вся доступная память (на текущий момент)\n");
    fprintf(file, "typedef struct then\n");
    fprintf(file, SP1"unsigned char ip; // Указатель на инструкцию\n");
    fprintf(file, SP1"unsigned char sp; // Указатель на стек (опционально)\n");
    fprintf(file, SP1"unsigned char dp; // Указатель на данные\n");
    fprintf(file, "end Register;\n\n");

    fprintf(file, "int main(int argc, char **argv/*/ argv[] /*/) then\n");
    fprintf(file, ""SP1"setlocale(0, \"\");\n\n");
    /*..../ Start of C-code generation block /....*/
    fprintf(file, SP1"printf(\"<Debug> argc = %%d\\n\", argc);\n");
    fprintf(file, SP1"while (argc-- != 0) printf(\"<Debug> argv[%%d] = \\\"%%s\\\"\", argc, argv[argc]);\n\n");

    fprintf(file, "\
"EMPTY_LINE""SP1"FILE *file = NULL;\n\
"EMPTY_LINE""SP1"file = fopen(\"meta_info.cfg\", \"rb\");\n\
"EMPTY_LINE""SP1"if (!file)\n\
"EMPTY_LINE""SP1"{\n\
"EMPTY_LINE""SP4"file = fopen(\"meta_info.cfg\", \"wb\");\n\
"EMPTY_LINE""SP4"if (!file) return -1;\n\
"EMPTY_LINE""SP4"// Если отсутствует файл, записать в него базовые настройки для интерпретатора (VM)\n\
"EMPTY_LINE""SP4"fputc(0x02, file);\n\
"EMPTY_LINE""SP4"fclose(file);\n\
"EMPTY_LINE""SP4"file = fopen(\"meta_info.cfg\", \"rb\");\n\
"EMPTY_LINE""SP4"if (!file) return -1;\n\
"EMPTY_LINE""SP1"}\n\
"   );

    fprintf(file, SP1"/*/ Режимы vCPU: /*/\n");
    fprintf(file, SP1"// 1 - Управление указателем на инструкцию ложится на плечи программиста (как в brainfuck)\n");
    fprintf(file, SP1"// 2 - Управление указателем на инструкцию происходит автоматически, но при желании можно переключиться на ручное управление (как в brainfuck)\n");
    fprintf(file, SP1"// Считывание базовых настроек с файла для интерпретатора (VM)\n");
    fprintf(file, SP1"unsigned char mode = fgetc(file); // По умолчанию - 2\n");
    fprintf(file, SP1"printf(\"\\n<Debug> mode = %%d\", mode);\n");
    fprintf(file, SP1"fclose(file);\n\n");

    fprintf(file, SP1"Register register;\n");
    fprintf(file, SP1"register.ip = 0x0;\n");
    fprintf(file, SP1"register.sp = 0x0;\n");
    fprintf(file, SP1"register.dp = 0x0;\n");

    fprintf(file, SP1"/*/ GNU extension: range-designator /*/\n");
    fprintf(file, SP1"static void *execute[] = then\n");
    fprintf(file, SP1"[0x0] = &&_operation_output,\n");
    fprintf(file, SP1"[0x1 ... 0xFF] = &&_operation_invalid,\n");
    /*
    for (unsigned char i = 0; i < 16; i++)
    {
        fprintf(file, SP1);
        for (unsigned char j = 0; j < 8; j++)
        {
            if (i == 0 && j == 0) { fprintf(file, "&&_operation_output ,"); continue; } // Операция вывода)
            fprintf(file, "&&_operation_invalid,", i*16+j); // Недопустимая операция
        }
        fprintf(file, "\n"SP1);
        for (unsigned char j = 8; j < 16; j++) fprintf(file, "&&_operation_invalid,", i*16+j); // Недопустимая операция
        fprintf(file, "\n");
    }
    */
    fprintf(file, SP1"end;\n");
    fprintf(file, SP1"*instructions = 0x0; // Код операции для вывода текста в терминал/консоль\n");
    fprintf(file, SP1"goto *execute[*instructions];\n");

    fprintf(file, "\n_operation_invalid: // Недопустимая операция\n");
    //fprintf(file, SP1"printf(\"Недопустимая операция!\\n\");\n");
    fprintf(file, SP1"// Обработка: пропустить, остановить или др.\n");
    fprintf(file, SP1"// goto dispatch; // или exit/return\n");
    fprintf(file, SP1"return 0;\n");

    fprintf(file, "\n_operation_output: // Операция вывода текста в терминал/консоль\n");
    fprintf(file, SP1"++instructions; // Сдвинем позицию курсора (указателя) на след. инструкцию\n");
    fprintf(file, SP1"printf(\"%%s\", (const char *) instructions);\n");
    fprintf(file, SP1"goto *execute[*++instructions]; // Перейди к след. действию\n");

    //fprintf(file, buffer);
    /*..../ End of C code generation block /....*/
    fprintf(file, SP1"return 0;\n");
    fprintf(file, "end");
    // ---------------------------------
    fclose(file);
    return 0;
}