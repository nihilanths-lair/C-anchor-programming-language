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
    size_t read_bytes = fread(buf, 1, sizeof (buf), file);
    buf[read_bytes] = '\0';
    printf("\n```\n%s\n```", buf);

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