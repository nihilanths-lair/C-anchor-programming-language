// <!-- Kernel builder v.0.1 (Encoding: Windows-1251) -->

#include <locale.h>
#include <stdio.h>

#define switch_open {
#define switch_close }

void _(int argc, char *argv[])
{
    setlocale(0, "");
    //
    char buf[0xFF];
    FILE *file = fopen("_.asm", "rb");
    if (file == NULL) { printf("\n #1\n"); return; }
    size_t read_bytes = fread(buf, 1, sizeof (buf), file);
    buf[read_bytes] = '\0';
    printf("\n```\n%s\n```", buf);

    _1_run: switch (*buf)
    switch_open
    case 'J':
     printf("\n -> 'J'\n");
     goto _1_end;
    switch_close
    _1_end:
}
int main(int argc, char *argv[]) { _(argc, argv); }