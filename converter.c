#include <stdio.h>
#include <locale.h>

unsigned char input_data[0xFFFFFF] = "\
";
unsigned char output_data[0xFFFFFF] = "\
";

int main()
{
    setlocale(0, "");

    // режимы
    printf("\ntxt(16) -> bin\n%s\n", input_data);
    printf("txt(10) -> bin\n%s\n", input_data);
    printf("bin -> txt(16)\n%s\n", input_data);
    printf("bin -> txt(10)\n%s", input_data);
}