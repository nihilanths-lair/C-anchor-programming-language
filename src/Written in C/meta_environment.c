#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdint.h>

#define MEMORY_SIZE 65536
#define macro__jmp_do_opcode() goto *dispatch[memory[dsl_ip++]]

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    if (argc < 3)
    {
        printf("\n Использование: meta_environment.exe <architecture_bin> <program_bin>\n");
        return 1;
    }

    unsigned char* memory = (unsigned char*)calloc(MEMORY_SIZE, sizeof(unsigned char));
    if (memory == NULL)
    {
        printf("\n Ошибка: Не удалось выделить память под ОЗУ.\n");
        return 1;
    }

    // Загрузка бинарников
    FILE* arch = fopen(argv[1], "rb");
    if (arch == NULL)
    {
        printf("\n Ошибка: Не удалось открыть файл архитектуры %s\n", argv[1]);
        free(memory);
        return 1;
    }
    fread(memory, sizeof(unsigned char), 0x100, arch);
    fclose(arch);

    FILE* prog = fopen(argv[2], "rb");
    if (prog == NULL)
    {
        printf("\n Ошибка: Не удалось открыть файл программы %s\n", argv[2]);
        free(memory);
        return 1;
    }
    fread(memory + 0x100, sizeof(unsigned char), MEMORY_SIZE - 0x100, prog);
    fclose(prog);

    // --- НАСТОЯЩИЕ 64-ВИТНЫЕ РЕГИСТРЫ x86-64 ---
    uint64_t dsl_ip = 0; // Указатель команд прошивки (микрокод процессора)
    uint64_t gpl_ip = 0x100; // Указатель данных прикладной программы

    uint64_t rax = 0; // Регистр А (Аккумулятор)
    uint64_t rbx = 0; // Регистр Б (Базовый)
    uint64_t rcx = 0; // Регистр С (Счетчик)
    uint64_t rdx = 0; // Регистр Д (Данные)
    uint8_t  flag_zero = 0; // Флаг нуля для условных переходов (ZF)

    void* dispatch[] =
    {
        &&do_hlt,         // 0 — Остановить процессор
        &&do_mov_rax_imm, // 1 — EASM: mov rax, imm64 (Прошивка считывает 8 байт)
        &&do_mov_rbx_imm, // 2 — EASM: mov rbx, imm64
        &&do_add_rax_rbx, // 3 — EASM: add rax, rbx
        &&do_cmp_rax_rbx, // 4 — EASM: cmp rax, rbx (выставляет флаг нуля)
        &&do_je,          // 5 — EASM: je <label> (прыжок если flag_zero == 1)
        &&do_jmp,         // 6 — EASM: jmp <label> (безусловный прыжок)
        &&do_sys_print    // 7 — EASM: системный вывод значения RAX на экран
    };

    macro__jmp_do_opcode();

    // --- ИСПОЛНЕНИЕ ИНСТРУКЦИЙ EASM ---

    do_hlt:
    {
        printf("\n [EASM Core]: Процессор остановлен. RAX = %llu, RBX = %llu\n", rax, rbx);
        free(memory);
        return 0;
    }

    do_mov_rax_imm:
    {
        rax = 0;
        for (int i = 0; i < 8; i++)
        {
            rax = (rax << 8) | memory[dsl_ip++];
        }
        macro__jmp_do_opcode();
    }

    do_mov_rbx_imm:
    {
        rbx = 0;
        for (int i = 0; i < 8; i++)
        {
            rbx = (rbx << 8) | memory[dsl_ip++];
        }
        macro__jmp_do_opcode();
    }

    do_add_rax_rbx:
    {
        rax += rbx;
        macro__jmp_do_opcode();
    }

    do_cmp_rax_rbx:
    {
        flag_zero = (rax == rbx) ? 1 : 0;
        macro__jmp_do_opcode();
    }

    do_je:
    {
        uint64_t target = (memory[dsl_ip] << 8) | memory[dsl_ip + 1];
        dsl_ip += 2;
        if (flag_zero == 1)
        {
            dsl_ip = target;
        }
        macro__jmp_do_opcode();
    }

    do_jmp:
    {
        uint64_t target = (memory[dsl_ip] << 8) | memory[dsl_ip + 1];
        dsl_ip += 2;
        dsl_ip = target;
        macro__jmp_do_opcode();
    }

    do_sys_print:
    {
        putchar((int)rax);
        fflush(stdout);
        macro__jmp_do_opcode();
    }
}