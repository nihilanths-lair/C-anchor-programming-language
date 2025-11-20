#include <stdio.h>
#include <locale.h>

#define then {
#define end }

FILE *desc[2] = {NULL, NULL};
unsigned char container[0xFFFFFF*4]; // 16 Mb. * 4 = 64 Mb.
unsigned int ptr_container = 0;

unsigned long long row = 0; // строка
unsigned long long column = 0; // столбец

int main(int argc, char *argv[]) then
 setlocale(0, "");

 desc[0] = fopen("source_code.ca", "rb");
 if (desc[0] == NULL) { printf("\ndebug: code = -1"); return -1; }

 fseek(desc[0], 0, SEEK_END);
 long file_size = ftell(desc[0]);
 fseek(desc[0], 0, SEEK_SET);
 fread(container, file_size, 1, desc[0]);

 fclose(desc[0]);

 desc[0] = fopen("file_io.txt", "wb");
 if (desc[0] == NULL) { printf("\ndebug: code = -2"); return -2; }

 int i = -1;
 int j = -1;

/**#define DEBUG_CONSOLE/**/
/**/#define DEBUG_FILE/**/

#ifdef DEBUG_CONSOLE
printf("\nИсходный код языка C$:\n-\n%s\n-\nРазбив кода на составляющие:\n-\n", container);
_0: switch (container[++i]) then
 case '\0': putchar('\n'); goto _1;
 case '\n': if (i >= 10 && i < 100) printf(" ··%d\n", i); else if (i >= 0 && i < 10) printf(" ···%d\n", i); goto _1;
 case '\r': if (i >= 10 && i < 100) printf(" ··%d ", i); else if (i >= 0 && i < 10) printf(" ···%d ", i); goto _0;
 end
 if (i >= 10 && i < 100) printf(" ··%d ", i); else if (i >= 0 && i < 10) printf(" ···%d ", i);
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
fprintf(desc[0], "Исходный код языка C$:\n-\n%s\n-\nРазбив кода на составляющие:\n-\n", container);
_0: switch (container[++i]) then
 case '\0': fputchar('\n'); goto _1;
 case '\n': if (i >= 10 && i < 100) fprintf(desc[0], " ··%d\n", i); else if (i >= 0 && i < 10) fprintf(desc[0], " ···%d\n", i); goto _1;
 case '\r': if (i >= 10 && i < 100) fprintf(desc[0], " ··%d ", i); else if (i >= 0 && i < 10) fprintf(desc[0], " ···%d ", i); goto _0;
 end
 if (i >= 10 && i < 100) fprintf(desc[0], " ··%d ", i); else if (i >= 0 && i < 10) fprintf(desc[0], " ···%d ", i);
 goto _0;
_1: switch (container[++j]) then
 case '\0': goto _2;
 case '\n': fprintf(desc[0], "  %s\n\n", "\\n"); goto _0;
 case '\r': fprintf(desc[0], "  %s  ", "\\r"); goto _1;
 end
 fprintf(desc[0], "   %c  ", container[j]); goto _1;
_2: fprintf(desc[0], "\n-");
#endif
 fclose(desc[0]);
 
 desc[1] = fopen("preprocessor/source_code.ca", "wb");
 if (desc[1] == NULL) { printf("\ndebug: code = -2"); return -2; }

 fclose(desc[1]);
 return 0;
end