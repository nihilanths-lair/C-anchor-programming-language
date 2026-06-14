#include <stdio.h>
/*
 *    [Пользовательское соглашение]
 * - - - - - - - - - - - - - - - - - -
 *         Адрес      | Описание
 *
 *      0x0 -- 0xFF   | Размещается микропрограмма (ядро) мета-процессора
 *    0x100 -- 0xFFFF | Размещается любая прикладная программа
*/
unsigned char memory[0x10000];
unsigned char dsl_ip = 0;       // Читает опкоды прошивки (0x00 -- 0xFF)
unsigned short gpl_ip = 0x100;  // Бегает по прикладной программе (0x100 -- 0xFFFF)

int main(int argc, char* argv[])
{
    // 1. Проверяем, что передано ОБА файла: и прошивка, и программа
    if (argc < 3)
    {
        printf("\n Usage: interpreter.exe <architecture_bin> <program_bin>\n");
        return 1;
    }
    // 2. ЗАГРУЗКА АРХИТЕКТУРЫ (DSL) -> строго с адреса 0x00
    FILE* arch_file = fopen(argv[1], "rb");
    if (!arch_file)
    {
        printf("\n Error: Could not open architecture file %s\n", argv[1]);
        return 1;
    }
    // Читаем прошивку в начало памяти (максимум 256 байт, чтобы не залезть на территорию программы)
    size_t arch_bytes = fread(&memory[0x00], 1, 0x100, arch_file);
    fclose(arch_file);
    if (arch_bytes == 0)
    {
        printf("\n Error: Architecture file is empty or corrupted\n");
        return 1;
    }
    // 3. ЗАГРУЗКА ПРОГРАММЫ (GPL) -> строго со смещением 0x100
    FILE* prog_file = fopen(argv[2], "rb");
    if (!prog_file)
    {
        printf("\n Error: Could not open program file %s\n", argv[2]);
        return 1;
    }
    // Читаем программу в память, начиная с адреса memory[0x100]
    // Оставшийся размер памяти: sizeof (memory) - 0x100
    size_t prog_bytes = fread(&memory[0x100], 1, sizeof (memory) - 0x100, prog_file);
    fclose(prog_file);
    if (prog_bytes == 0)
    {
        printf("\n Error: Program file is empty or corrupted\n");
        return 1;
    }
    void* dispatch[] =
    {
        &&do_halt,      // 0
        &&do_inc_ip,    // 1
        &&do_dec_ip,    // 2
        &&do_inc_val,   // 3
        &&do_dec_val,   // 4
        &&do_je,        // 5
        &&do_sys_call,  // 6
        &&do_jmp        // 7
    };
    #define macro__jmp_do_opcode() goto *dispatch[memory[dsl_ip++]]
    macro__jmp_do_opcode();
    do_halt: return 0;
    do_inc_ip:
    {
        gpl_ip++;
        macro__jmp_do_opcode();
    }
    do_dec_ip:
    {
        gpl_ip--;
        macro__jmp_do_opcode();
    }
    do_inc_val:
    {
        memory[gpl_ip]++;
        macro__jmp_do_opcode();
    }
    do_dec_val:
    {
        memory[gpl_ip]--;
        macro__jmp_do_opcode();
    }
    do_je:
    {
        // Считываем 16-битный адрес перехода (2 байта: старший и младший)
        // Сначала берем первый байт, сдвигаем его на 8 бит влево, и склеиваем со вторым
        unsigned short target = (memory[dsl_ip++] << 8) | memory[dsl_ip++];
        if (memory[gpl_ip] == 0) { dsl_ip = target; }
        macro__jmp_do_opcode();
    }
    do_sys_call:
    {
        if (memory[gpl_ip] == 1) { memory[gpl_ip] = fgetc(stdin); }
        if (memory[gpl_ip] == 2)
        {
            gpl_ip++;
            putchar(memory[gpl_ip]);
            fflush(stdout); // выталкиваем из буфера символ на экран
        }
        macro__jmp_do_opcode();
    }
    do_jmp:
    {
        // Безусловный 16-битный переход для dsl_ip
        unsigned short target = (memory[dsl_ip++] << 8) | memory[dsl_ip++];
        dsl_ip = target; // Нам не важны флаги, мы просто прыгаем!
        macro__jmp_do_opcode();
    }
}