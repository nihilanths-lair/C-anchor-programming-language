// <!-- Kernel builder v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>

#define switch_open {
#define switch_close }

// Сопоставление с образцом
void PatternMatching(const char *text, const char *sample)
{
    // ... //
}

void _(int argc, char *argv[])
{
    setlocale(0, "");
    //
    char buf[0xFF];
    char *ptr_buf = buf;

    FILE *file = fopen("_.asm", "rb");
    if (file == NULL) { printf("\n #1\n"); return; }
    size_t bytes_read = fread(buf, 1, sizeof (buf), file);
    buf[bytes_read] = '\0';
    printf("\n```\n%s\n```", buf);

    //PatternMatching(buf, "JMP");

    _1_run: switch (*ptr_buf)
    switch_open

    case '\0':
     printf("\n -> '\\0'");
     goto _1_end;

    case 'J':
     printf("\n -> 'J'");
     ptr_buf++;
     goto _1_run;

    case 'M':
     printf("\n -> 'M'");
     ptr_buf++;
     goto _1_run;

    case 'P':
     printf("\n -> 'P'");
     ptr_buf++;
     goto _1_run;

    switch_close
    _1_end:

    putchar('\n');
}
int main(int argc, char *argv[]) { _(argc, argv); }
/*
//C:\Users\Глеб\Documents\github.com\nihilanths-lair\C-anchor\src\Written in C>gcc codegen.c -o _
#include <stdio.h>
#include <locale.h>
//#include <string.h>
#include <stdlib.h> // Для malloc и free

#define MAX_FILE_SIZE (10*1024*1024) // Лимит 10 Мегабайт

#include "action.c"

int main(int argc, char *argv[])
{
    setlocale(0, "");

    printf("\n argc = %d", argc);
    for (int i = 0; i < argc; i++) printf("\n argv[%d] = \"%s\"", i, argv[i]);
    putchar('\n');

    FILE *file = fopen("_.asm", "rb"); // "rb" — чтение в бинарном режиме (надежнее для размера)
    if (!file)
    {
        printf("\n Не удалось открыть файл.");
        return 1;
    }
    // 1. Узнаем размер файла
    fseek(file, 0, SEEK_END);     // Переходим в самый конец
    long file_size = ftell(file); // Получаем позицию (это и есть размер в байтах)
    rewind(file);                 // Возвращаемся в начало файла для чтения

    if (file_size > MAX_FILE_SIZE)
    {
        printf("\n Файл слишком большой! Лимит %d байт.\n", MAX_FILE_SIZE);
        fclose(file);
        return 1;
    }

    // 2. Выделяем память (+1 байт для нулевого терминатора '\0') и проверяем, выделилась ли память
    char *source_code = malloc(file_size+1);
    if (source_code == NULL)
    {
        printf("\n Не удалось выделить память под файл.");
        fclose(file);
        return 1;
    }

    // 3. Читаем данные
    // Теперь fread вернет реальное количество считанных БАЙТ (символов)
    size_t bytes_read = fread(source_code, 1, file_size, file);

    // 4. Обязательно закрываем строку, чтобы strlen и printf не "улетели" в память
    source_code[bytes_read] = '\0';

    printf("\n Файл открыт. Размер: %ld байт. Считано: %zu\n", file_size, bytes_read);
    printf("\n Содержимое:\n···\n%s\n···", source_code);

    // Чистим за собой
    free(source_code);
    fclose(file);

    //-/
    Action();
    #include "action.txt"
    //-/

    main_end: putchar('\n');
    return 0;
}
*/