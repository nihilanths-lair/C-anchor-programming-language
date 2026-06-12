#include <stdio.h>
/*
 *    [Пользовательское соглашение]
 * - - - - - - - - - - - - - - - - - -
 *         Адрес      | Описание
 *
 *      0x0 -- 0xFF   | Размещается микропрограмма (ядро) мета-процессора
 *    0x100 -- 0xFFFF | Размещается любая прикладная программа
*/
unsigned char memory[0xFFFF];
int main(int argc, char* argv[])
{
    // 1. Проверяем, передал ли нам пользователь файл с байт-кодом
    if (argc < 2)
    {
        printf("\n Usage: interpreter.exe <bin_file>\n");
        return 1;
    }

    // 2. Открываем бинарный файл
    FILE* file = fopen(argv[1], "rb");
    if (!file)
    {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    // 3. Считываем байты напрямую в наш массив __ 
    // fread возвращает количество прочитанных байт, мы можем использовать это для безопасности
    size_t bytes_read = fread(memory, 1, sizeof (memory), file);
    fclose(file);

    if (bytes_read == 0)
    {
        printf("Error: File is empty or corrupted\n");
        return 1;
    }

    void* dispatch[] = {
        &&do_halt,     // 0
        &&do_inc_dp,   // 1
        &&do_dec_dp,   // 2
        &&do_inc_val,  // 3
        &&do_dec_val,  // 4
        &&do_jmp_zero, // 5
        &&do_sys_call  // 6
    };

    int ip = 0;
    int dp = 0x80;

    #define macro__jmp_do_opcode() goto *dispatch[memory[ip++]]
    macro__jmp_do_opcode();

    do_halt: {
        return 0;
    }

    do_inc_dp: {
        dp++;
        macro__jmp_do_opcode();
    }

    do_dec_dp: {
        dp--;
        macro__jmp_do_opcode();
    }

    do_inc_val: {
        memory[dp]++;
        macro__jmp_do_opcode();
    }

    do_dec_val: {
        memory[dp]--;
        macro__jmp_do_opcode();
    }

    do_jmp_zero: {
        int target = memory[ip++];
        if (memory[dp] == 0) { ip = target; }
        macro__jmp_do_opcode();
    }

    do_sys_call: {
        if (memory[dp] == 1) memory[dp] = fgetc(stdin);
        if (memory[dp] == 2) putchar(memory[dp+1]);
        macro__jmp_do_opcode();
    }
}