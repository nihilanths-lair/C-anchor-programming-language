/*/
    <!-- Encoding Windows-1251 / Кодировка Windows-1251 -->
/*/

#include <stdio.h>
#include <locale.h>

#define collection switch
#define then {
#define target case
#define end }
#define jmp goto

FILE *desc[2] = {NULL, NULL};
unsigned char container[0xFFFF] = {'\0'};
/**
unsigned char container_1[0xFFFF];
unsigned char container_2[0xFFFF];
/**/

long long iter_0 = 0;

unsigned long long row = 1; // строка
unsigned long long column = 1; // столбец / позиция
unsigned long long position = 1; // позиция / индекс/элемент

int main(int argc, char *argv[]) then
 setlocale(0, "");

 desc[0] = fopen("source_code.ca", "rb");
 if (desc[0] == NULL) { printf("\nDebug: code = -1"); return -1; }

 fseek(desc[0], 0, SEEK_END);
 long file_size = ftell(desc[0]);
 fseek(desc[0], 0, SEEK_SET);
 fread(container, file_size, 1, desc[0]);

 fclose(desc[0]);

/**#define DEBUG_CONSOLE/**/
/**/#define DEBUG_FILE/**/

#ifdef DEBUG_CONSOLE
 int i = -1;
 int j = -1;
printf("\nИсходный код языка C$:\n-\n%s\n-\nРазбив кода на составляющие:\n-\n", container);
_0: switch (container[++i]) then
 case '\0': putchar('\n'); goto _1;
 case '\n': if (i >= 100 && i < 1000) printf(" ·%d\n", i); else if (i >= 10 && i < 100) printf(" ··%d\n", i); else if (i >= 0 && i < 10) printf(" ···%d\n", i); goto _1;
 case '\r': if (i >= 100 && i < 1000) printf(" ·%d ", i); else if (i >= 10 && i < 100) printf(" ··%d ", i); else if (i >= 0 && i < 10) printf(" ···%d ", i); goto _0;
 end
 if (i >= 100 && i < 1000) printf(" ·%d ", i); else if (i >= 10 && i < 100) printf(" ··%d ", i); else if (i >= 0 && i < 10) printf(" ···%d ", i);
 goto _0;
_1: switch (container[++j]) then
 case '\0': goto _2;
 case '\n': printf("  %s\n\n", "\\n"); goto _0;
 case '\r': printf("  %s  ", "\\r"); goto _1;
 end
 printf("   %c  ", container[j]); goto _1;
_2: printf("\n-");
#endif
#ifdef DEBUG_FILE
 FILE *df = fopen("file_io.txt", "wb");
 if (df == NULL) { printf("\nDebug: code = -2"); return -2; }

 int i = -1;
 int j = -1;
fprintf(df, "Исходный код языка C$:\n-\n%s\n-\nРазбив кода на составляющие:\n-\n", container);
_0: switch (container[++i]) then
 case '\0': fputc('\n', df); goto _1;
 case '\n': if (i >= 100 && i < 1000) fprintf(df, " ·%d\n", i); else if (i >= 10 && i < 100) fprintf(df, " ··%d\n", i); else if (i >= 0 && i < 10) fprintf(df, " ···%d\n", i); goto _1;
 case '\r': if (i >= 100 && i < 1000) fprintf(df, " ·%d ", i); else if (i >= 10 && i < 100) fprintf(df, " ··%d ", i); else if (i >= 0 && i < 10) fprintf(df, " ···%d ", i); goto _0;
 end
 if (i >= 100 && i < 1000) fprintf(df, " ·%d ", i); else if (i >= 10 && i < 100) fprintf(df, " ··%d ", i); else if (i >= 0 && i < 10) fprintf(df, " ···%d ", i);
 goto _0;
_1: switch (container[++j]) then
 case '\0': goto _2;
 case '\n': fprintf(df, "  %s\n\n", "\\n"); goto _0;
 case '\r': fprintf(df, "  %s  ", "\\r"); goto _1;
 end
 fprintf(df, "   %c  ", container[j]); goto _1;
_2: fprintf(df, "\n-");
 fclose(df);
#endif
 
 desc[1] = fopen("preprocessor/source_code.ca", "wb");
 if (desc[1] == NULL) { printf("\nDebug: code = -3"); return -3; }

 putchar('\n');
 iter_0 = -1;
_2_1:
 collection (container[++iter_0]) then
 target '\0':
  jmp _2_2;
 target '\n':
  ++position;
  ++row;
  column = 1; 
  jmp _2_1;
 target '/':
  printf("[preprocessor - строка: %d, столбец: %d, позиция: %d]: Возможно начало комментария?\n", row, column, position);
  ++position;
  ++column;
  jmp _2_1;
 end
 ++position;
 ++column;
 jmp _2_1;
_2_2:
 fclose(desc[1]);
 return 0;
end