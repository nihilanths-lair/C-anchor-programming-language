#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>

#define MEMORY_SIZE 65536
#define MAX_LABELS 512
#define LABEL_NAME_LEN 64
#define BUFFER_SIZE 65536

int run_interpreter(const char* prog_path);
int run_compiler(const char* src_path, const char* out_path);

uint64_t atoull(const char* str)
{
    uint64_t res = 0;
    while (*str >= '0' && *str <= '9')
    {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}

// ============================================================================
// [ГЛАВНАЯ ТОЧКА ВХОДА И ДИСПЕТЧЕРИЗАЦИЯ ПЛАТФОРМЫ]
// ============================================================================
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    if (argc < 3)
    {
        printf("\n Использование:\n");
        printf("   Компиляция: meta_environment.exe -c <исходник.meta> <выход.bin>\n");
        printf("   Запуск VM:  meta_environment.exe -r <программа.bin>\n\n");
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0)
    {
        if (argc < 4)
        {
            printf("\n Ошибка: Укажите выходной файл для компиляции.\n");
            return 1;
        }
        return run_compiler(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-r") == 0)
    {
        return run_interpreter(argv[2]);
    }
    else
    {
        printf("\n Ошибка: Неизвестный флаг '%s'. Используйте -c или -r.\n", argv[1]);
        return 1;
    }
}

// ============================================================================
// ЗОНА №1: РАСШИРЕННЫЙ МЕТА-КРИСТАЛЛ (ПОЛНАЯ x86-ЧЕТВЕРКА РЕГИСТРОВ)
// ============================================================================
#define macro__jmp_do_opcode() goto *dispatch[memory[rip++]]

int run_interpreter(const char* prog_path)
{
    unsigned char* memory = (unsigned char*)calloc(MEMORY_SIZE, sizeof(unsigned char));
    if (memory == NULL)
    {
        printf("\n Ошибка: Не удалось выделить память под ОЗУ.\n");
        return 1;
    }

    // Вшитый BIOS: jmp 0x0100 (Опкод 8, за ним адрес 0x0100 в формате Big-Endian)
    unsigned char embedded_bios[] = {
        8, 0x01, 0x00   
    };
    memcpy(memory, embedded_bios, sizeof(embedded_bios));

    FILE* prog = fopen(prog_path, "rb");
    if (prog == NULL)
    {
        printf("\n Ошибка: Не удалось открыть файл программы %s\n", prog_path);
        free(memory);
        return 1;
    }
    fread(memory + 0x100, sizeof(unsigned char), MEMORY_SIZE - 0x100, prog);
    fclose(prog);

    // Аппаратный сброс процессора (RESET CPU)
    uint64_t rip = 0; 

    // Сверхбыстрые нативные локальные переменные-регистры x86-64
    uint64_t rax = 0; 
    uint64_t rbx = 0; 
    uint64_t rcx = 0; 
    uint64_t rdx = 0; 
    uint8_t  flag_zero = 0; 

    // Слепая адресная матрица полного базиса команд
    void* dispatch[] =
    {
        &&opcode_0,  // 0:  hlt
        &&opcode_1,  // 1:  mov rax, imm64
        &&opcode_2,  // 2:  mov rbx, imm64
        &&opcode_3,  // 3:  mov rcx, imm64
        &&opcode_4,  // 4:  mov rdx, imm64
        &&opcode_5,  // 5:  add rax, rbx
        &&opcode_6,  // 6:  sub rax, rbx
        &&opcode_7,  // 7:  cmp rax, rbx
        &&opcode_8,  // 8:  jmp <label>
        &&opcode_9,  // 9:  je <label>
        &&opcode_10, // 10: jne <label>
        &&opcode_11, // 11: sys_print
        &&opcode_12, // 12: mov rax, [rcx]
        &&opcode_13, // 13: mov [rcx], rax
        &&opcode_14, // 14: mov rbx, [rcx]
        &&opcode_15  // 15: mov [rcx], rbx
    };

    macro__jmp_do_opcode();

    opcode_0: // hlt
    {
        printf("\n [EASM Core]: Процессор остановлен.\n");
        printf(" RAX = %llu, RBX = %llu, RCX = %llu, RDX = %llu, RIP = 0x%llX\n", rax, rbx, rcx, rdx, rip);
        free(memory);
        return 0;
    }

    opcode_1: // mov rax, imm64
    {
        rax = (uint64_t)memory[rip]         |
              ((uint64_t)memory[rip + 1] << 8)  |
              ((uint64_t)memory[rip + 2] << 16) |
              ((uint64_t)memory[rip + 3] << 24) |
              ((uint64_t)memory[rip + 4] << 32) |
              ((uint64_t)memory[rip + 5] << 40) |
              ((uint64_t)memory[rip + 6] << 48) |
              ((uint64_t)memory[rip + 7] << 56);
        rip += 8;
        macro__jmp_do_opcode();
    }

    opcode_2: // mov rbx, imm64
    {
        rbx = (uint64_t)memory[rip]         |
              ((uint64_t)memory[rip + 1] << 8)  |
              ((uint64_t)memory[rip + 2] << 16) |
              ((uint64_t)memory[rip + 3] << 24) |
              ((uint64_t)memory[rip + 4] << 32) |
              ((uint64_t)memory[rip + 5] << 40) |
              ((uint64_t)memory[rip + 6] << 48) |
              ((uint64_t)memory[rip + 7] << 56);
        rip += 8;
        macro__jmp_do_opcode();
    }

    opcode_3: // mov rcx, imm64
    {
        rcx = (uint64_t)memory[rip]         |
              ((uint64_t)memory[rip + 1] << 8)  |
              ((uint64_t)memory[rip + 2] << 16) |
              ((uint64_t)memory[rip + 3] << 24) |
              ((uint64_t)memory[rip + 4] << 32) |
              ((uint64_t)memory[rip + 5] << 40) |
              ((uint64_t)memory[rip + 6] << 48) |
              ((uint64_t)memory[rip + 7] << 56);
        rip += 8;
        macro__jmp_do_opcode();
    }

    opcode_4: // mov rdx, imm64
    {
        rdx = (uint64_t)memory[rip]         |
              ((uint64_t)memory[rip + 1] << 8)  |
              ((uint64_t)memory[rip + 2] << 16) |
              ((uint64_t)memory[rip + 3] << 24) |
              ((uint64_t)memory[rip + 4] << 32) |
              ((uint64_t)memory[rip + 5] << 40) |
              ((uint64_t)memory[rip + 6] << 48) |
              ((uint64_t)memory[rip + 7] << 56);
        rip += 8;
        macro__jmp_do_opcode();
    }

    opcode_5: // add rax, rbx
    {
        rax += rbx;
        macro__jmp_do_opcode();
    }

    opcode_6: // sub rax, rbx
    {
        rax -= rbx;
        macro__jmp_do_opcode();
    }

    opcode_7: // cmp rax, rbx
    {
        flag_zero = (rax == rbx) ? 1 : 0;
        macro__jmp_do_opcode();
    }

    opcode_8: // jmp <label>
    {
        uint64_t target = (memory[rip] << 8) | memory[rip + 1];
        rip += 2;
        rip = target;
        macro__jmp_do_opcode();
    }

    opcode_9: // je <label>
    {
        uint64_t target = (memory[rip] << 8) | memory[rip + 1];
        rip += 2;
        if (flag_zero == 1)
        {
            rip = target;
        }
        macro__jmp_do_opcode();
    }

    opcode_10: // jne <label>
    {
        uint64_t target = (memory[rip] << 8) | memory[rip + 1];
        rip += 2;
        if (flag_zero == 0)
        {
            rip = target;
        }
        macro__jmp_do_opcode();
    }

    opcode_11: // sys_print
    {
        putchar((int)rax);
        fflush(stdout);
        macro__jmp_do_opcode();
    }

    opcode_12: // mov rax, [rcx]
    {
        rax = *(uint64_t*)(memory + rcx);
        macro__jmp_do_opcode();
    }

    opcode_13: // mov [rcx], rax
    {
        *(uint64_t*)(memory + rcx) = rax;
        macro__jmp_do_opcode();
    }

    opcode_14: // mov rbx, [rcx]
    {
        rbx = *(uint64_t*)(memory + rcx);
        macro__jmp_do_opcode();
    }

    opcode_15: // mov [rcx], rbx
    {
        *(uint64_t*)(memory + rcx) = rbx;
        macro__jmp_do_opcode();
    }
}

// ============================================================================
// ЗОНА №2: ВРЕМЕННЫЙ СИ-КОМПИЛЯТОР (ТРАНСЛЯТОР В НАТИВНЫЙ x86-64 ДЛЯ WINDOWS)
// ============================================================================
int run_compiler(const char* src_path, const char* out_path)
{
    FILE* src = fopen(src_path, "r");
    if (src == NULL)
    {
        printf("\n Ошибка: Не удалось открыть исходный файл %s\n", src_path);
        return 1;
    }

    // Создаем промежуточный .asm файл для линкера FASM
    char asm_path[256];
    strncpy(asm_path, out_path, 250);
    char* ext = strrchr(asm_path, '.');
    if (ext != NULL) { *ext = 0; }
    strcat(asm_path, ".asm");

    FILE* out = fopen(asm_path, "w");
    if (out == NULL)
    {
        printf("\n Ошибка: Не удалось создать файл %s\n", asm_path);
        fclose(src);
        return 1;
    }

    // Записываем каноничный заголовок 64-битного PE-файла для Windows Windows
    fprintf(out, "format PE64 console\n");
    fprintf(out, "entry start\n\n");
    fprintf(out, "include 'win64ax.inc'\n\n"); // Стандартные макросы FASM для WinAPI
    fprintf(out, "section '.code' code readable executable\n\n");

    char line[BUFFER_SIZE];

    while (fgets(line, sizeof(line), src))
    {
        line[strcspn(line, "\r\n")] = 0;

        char* comment = strchr(line, '#');
        if (comment != NULL) { *comment = 0; }

        if (strlen(line) == 0) { continue; }

        char* cmd = line;
        while (*cmd == ' ' || *cmd == '\t') { cmd++; }

        size_t len = strlen(cmd);
        while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '\t'))
        {
            cmd[len - 1] = 0;
            len--;
        }

        if (*cmd == 0) { continue; }

        // Если это метка — просто копируем её как есть (синтаксис совпадает!)
        if (cmd[strlen(cmd) - 1] == ':')
        {
            fprintf(out, "%s\n", cmd);
            continue;
        }

        // --- ПРЯМАЯ ХИРУРГИЧЕСКАЯ ТРАНСЛЯЦИЯ В ИНСТРУКЦИИ INTEL ---
        if (strcmp(cmd, "hlt") == 0)
        {
            // Настоящий hlt в Windows вызовет падение ОС, поэтому мы транслируем 
            // его в каноничное штатное завершение процесса через WinAPI ExitProcess
            fprintf(out, "    invoke ExitProcess, 0\n");
        }
        else if (strncmp(cmd, "mov rax,", 8) == 0)
        {
            fprintf(out, "    mov rax, %s\n", cmd + 8);
        }
        else if (strncmp(cmd, "mov rbx,", 8) == 0)
        {
            fprintf(out, "    mov rbx, %s\n", cmd + 8);
        }
        else if (strncmp(cmd, "mov rcx,", 8) == 0)
        {
            fprintf(out, "    mov rcx, %s\n", cmd + 8);
        }
        else if (strncmp(cmd, "mov rdx,", 8) == 0)
        {
            fprintf(out, "    mov rdx, %s\n", cmd + 8);
        }
        else if (strcmp(cmd, "add rax, rbx") == 0)
        {
            fprintf(out, "    add rax, rbx\n");
        }
        else if (strcmp(cmd, "sub rax, rbx") == 0)
        {
            fprintf(out, "    sub rax, rbx\n");
        }
        else if (strcmp(cmd, "cmp rax, rbx") == 0)
        {
            fprintf(out, "    cmp rax, rbx\n");
        }
        else if (strncmp(cmd, "jmp ", 4) == 0)
        {
            fprintf(out, "    jmp %s\n", cmd + 4);
        }
        else if (strncmp(cmd, "je ", 3) == 0)
        {
            fprintf(out, "    je %s\n", cmd + 3);
        }
        else if (strncmp(cmd, "jne ", 4) == 0)
        {
            fprintf(out, "    jne %s\n", cmd + 4);
        }
        else if (strcmp(cmd, "sys_print") == 0)
        {
            // Заменяем наш виртуальный принт на реальный нативный вызов CRT putchar
            // Перед вызовом функций в Win64 нужно выровнять стек, но FASM-макрос invoke делает это сам
            fprintf(out, "    ccall [putchar], rax\n");
        }
        else if (strcmp(cmd, "mov rax, [rcx]") == 0)
        {
            fprintf(out, "    mov rax, [rcx]\n");
        }
        else if (strcmp(cmd, "mov [rcx], rax") == 0)
        {
            fprintf(out, "    mov [rcx], rax\n");
        }
        else if (strcmp(cmd, "mov rbx, [rcx]") == 0)
        {
            fprintf(out, "    mov rbx, [rcx]\n");
        }
        else if (strcmp(cmd, "mov [rcx], rbx") == 0)
        {
            fprintf(out, "    mov [rcx], rbx\n");
        }
        else if (strncmp(cmd, "db ", 3) == 0)
        {
            fprintf(out, "    db %s\n", cmd + 3);
        }
        else
        {
            printf("\n Ошибка синтаксиса EASM: Неизвестная инструкция '%s'\n", cmd);
            fclose(src);
            fclose(out);
            return 1;
        }
    }
    fclose(src);

    // Дописываем секцию импорта системных функций Windows (WinAPI)
    fprintf(out, "\nsection '.idata' import data readable writeable\n");
    fprintf(out, "    library kernel32, 'KERNEL32.DLL', msvcrt, 'MSVCRT.DLL'\n\n"); // ИСПРАВЛЕНО НА MSVCRT
    fprintf(out, "    import kernel32, ExitProcess, 'ExitProcess'\n");
    fprintf(out, "    import msvcrt, putchar, 'putchar'\n"); // ИСПРАВЛЕНО НА MSVCRT

    fclose(out);
    printf("\n Трансляция в нативный EASM успешна: сохранен %s\n", asm_path);
    return 0;
}