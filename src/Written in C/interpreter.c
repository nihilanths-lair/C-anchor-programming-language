#include <stdio.h>

unsigned char __[0xFF];

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
    size_t bytes_read = fread(__, 1, sizeof(__), file);
    fclose(file);

    if (bytes_read == 0)
    {
        printf("Error: File is empty or corrupted\n");
        return 1;
    }

    void* dispatch_table[] = {
        &&do_halt,     // 0
        &&do_inc_dp,   // 1
        &&do_dec_dp,   // 2
        &&do_inc_val,  // 3
        &&do_dec_val,  // 4
        &&do_jmp_zero, // 5
        &&do_sys_call  // 6
    };

    int ip = 0;        // Физический IP скорлупы
    int dp = 0x80;     // Data Pointer

    #define macro__jmp_do_opcode() goto *dispatch_table[__[ip++]]
    macro__jmp_do_opcode();

    // --- ЗОНА ВЫПОЛНЕНИЯ КОМАНД ---
    do_halt: {
        return 0; // Полный выход из программы
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
        __[dp]++;
        macro__jmp_do_opcode();
    }

    do_dec_val: {
        __[dp]--;
        macro__jmp_do_opcode();
    }

    do_jmp_zero: {
        int target = __[ip++];
        if (__[dp] == 0) { ip = target; }
        macro__jmp_do_opcode();
    }

    do_sys_call: {
        if (__[dp] == 1) __[dp] = fgetc(stdin);
        if (__[dp] == 2) putchar(__[dp+1]);
        macro__jmp_do_opcode();
    }
}