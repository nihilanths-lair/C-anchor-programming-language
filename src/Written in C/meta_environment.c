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
// ЗОНА №1: ЧИСТОКРОВНЫЙ x86 ИНТЕРПРЕТАТОР (ОДИН УКАЗАТЕЛЬ RIP)
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

    // --- НАСТОЯЩИЙ ВШИТЫЙ BIOS (ПЗУ МАТЕРИНСКОЙ ПЛАТЫ) ---
    // Настоящий BIOS инициализирует систему и передает управление ОС/Программе.
    // Опкод 6 — это jmp, за ним идут 2 байта адреса старта программы (0x01, 0x00).
    unsigned char embedded_bios[] = {
        6, 0x01, 0x00   // 0x00: jmp 0x0100 -> Аппаратный прыжок BIOS на прикладную программу!
    };
    memcpy(memory, embedded_bios, sizeof(embedded_bios));

    // Загружаем прикладную программу (нашу ОС/Игру) с адреса 0x100
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
    uint64_t rip = 0; // Процессор x86 жестко начинает выполнять код с адреса 0 (из BIOS)

    uint64_t rax = 0; 
    uint64_t rbx = 0; 
    uint64_t rcx = 0; 
    uint64_t rdx = 0; 
    uint8_t  flag_zero = 0; 

    void* dispatch[] =
    {
        &&do_hlt,         // 0
        &&do_mov_rax_imm, // 1
        &&do_mov_rbx_imm, // 2
        &&do_add_rax_rbx, // 3
        &&do_cmp_rax_rbx, // 4
        &&do_je,          // 5
        &&do_jmp,         // 6
        &&do_sys_print,   // 7
        &&do_mov_rax_ptr_rcx, // 8
        &&do_mov_ptr_rcx_rax  // 9
    };

    // Запуск тактового генератора процессора
    macro__jmp_do_opcode();

    do_hlt: // 0
    {
        printf("\n [EASM Core]: Процессор остановлен. RAX = %llu, RBX = %llu, RIP = 0x%llX\n", rax, rbx, rip);
        free(memory);
        return 0;
    }

    do_mov_rax_imm: // 1
    {
        rax = *(uint64_t*)(memory + rip);
        rip += 8;
        macro__jmp_do_opcode();
    }

    do_mov_rbx_imm: // 2
    {
        rbx = *(uint64_t*)(memory + rip);
        rip += 8;
        macro__jmp_do_opcode();
    }

    do_add_rax_rbx: // 3
    {
        rax += rbx;
        macro__jmp_do_opcode();
    }

    do_cmp_rax_rbx: // 4
    {
        flag_zero = (rax == rbx) ? 1 : 0;
        macro__jmp_do_opcode();
    }

    do_je: // 5
    {
        uint64_t target = (memory[rip] << 8) | memory[rip + 1];
        rip += 2;
        if (flag_zero == 1)
        {
            rip = target;
        }
        macro__jmp_do_opcode();
    }

    do_jmp: // 6
    {
        uint64_t target = (memory[rip] << 8) | memory[rip + 1];
        rip += 2;
        rip = target;
        macro__jmp_do_opcode();
    }

    do_sys_print: // 7
    {
        putchar((int)rax);
        fflush(stdout);
        macro__jmp_do_opcode();
    }

    do_mov_rax_ptr_rcx: // 8
    {
        rax = *(uint64_t*)(memory + rcx);
        macro__jmp_do_opcode();
    }

    do_mov_ptr_rcx_rax: // 9
    {
        *(uint64_t*)(memory + rcx) = rax;
        macro__jmp_do_opcode();
    }
}

// ============================================================================
// ЗОНА №2: ВРЕМЕННЫЙ СИ-КОМПИЛЯТОР (БУДЕТ ПОЛНОСТЬЮ ВЫРЕЗАН В БУДУЩЕМ)
// ============================================================================
typedef struct
{
    char name[LABEL_NAME_LEN];
    int address;
} Label;

typedef struct
{
    char label_name[LABEL_NAME_LEN];
    int jmp_offset;
} JmpRequest;

Label labels[MAX_LABELS];
int label_count = 0;

JmpRequest jmp_requests[MAX_LABELS];
int jmp_request_count = 0;

int find_label_address(const char* name)
{
    for (int i = 0; i < label_count; i++)
    {
        if (strcmp(labels[i].name, name) == 0)
        {
            return labels[i].address;
        }
    }
    return -1;
}

int run_compiler(const char* src_path, const char* out_path)
{
    FILE* src = fopen(src_path, "r");
    if (src == NULL)
    {
        printf("\n Ошибка: Не удалось открыть исходный файл %s\n", src_path);
        return 1;
    }

    unsigned char* output_buffer = (unsigned char*)calloc(BUFFER_SIZE, sizeof(unsigned char));
    int current_address = 0;

    label_count = 0;
    jmp_request_count = 0;

    char line[512];

    while (fgets(line, sizeof(line), src))
    {
        line[strcspn(line, "\r\n")] = 0;

        char* comment = strchr(line, '#');
        if (comment != NULL)
        {
            *comment = 0;
        }

        if (strlen(line) == 0)
        {
            continue;
        }

        char* cmd = line;
        while (*cmd == ' ' || *cmd == '\t')
        {
            cmd++;
        }

        size_t len = strlen(cmd);
        while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '\t'))
        {
            cmd[len - 1] = 0;
            len--;
        }

        if (*cmd == 0)
        {
            continue;
        }

        if (cmd[strlen(cmd) - 1] == ':')
        {
            cmd[strlen(cmd) - 1] = 0;
            if (label_count < MAX_LABELS)
            {
                strncpy(labels[label_count].name, cmd, LABEL_NAME_LEN);
                // Настоящий x86-расчет: адрес метки равен смещению 0x100 + позиция в коде
                labels[label_count].address = 0x100 + current_address;
                label_count++;
            }
            continue;
        }

        if (strcmp(cmd, "hlt") == 0)
        {
            output_buffer[current_address++] = 0;
        }
        else if (strncmp(cmd, "mov rax,", 8) == 0)
        {
            output_buffer[current_address++] = 1;
            uint64_t val = atoull(cmd + 8);
            *(uint64_t*)(output_buffer + current_address) = val;
            current_address += 8;
        }
        else if (strncmp(cmd, "mov rbx,", 8) == 0)
        {
            output_buffer[current_address++] = 2;
            uint64_t val = atoull(cmd + 8);
            *(uint64_t*)(output_buffer + current_address) = val;
            current_address += 8;
        }
        else if (strcmp(cmd, "add rax, rbx") == 0)
        {
            output_buffer[current_address++] = 3;
        }
        else if (strcmp(cmd, "cmp rax, rbx") == 0)
        {
            output_buffer[current_address++] = 4;
        }
        else if (strncmp(cmd, "je ", 3) == 0)
        {
            output_buffer[current_address++] = 5;
            char* target_label = cmd + 3;
            int addr = find_label_address(target_label);
            if (addr != -1)
            {
                output_buffer[current_address++] = (addr >> 8) & 0xFF;
                output_buffer[current_address++] = addr & 0xFF;
            }
            else
            {
                strncpy(jmp_requests[jmp_request_count].label_name, target_label, LABEL_NAME_LEN);
                jmp_requests[jmp_request_count].jmp_offset = current_address;
                jmp_request_count++;
                output_buffer[current_address++] = 0;
                output_buffer[current_address++] = 0;
            }
        }
        else if (strncmp(cmd, "jmp ", 4) == 0)
        {
            output_buffer[current_address++] = 6;
            char* target_label = cmd + 4;
            int addr = find_label_address(target_label);
            if (addr != -1)
            {
                output_buffer[current_address++] = (addr >> 8) & 0xFF;
                output_buffer[current_address++] = addr & 0xFF;
            }
            else
            {
                strncpy(jmp_requests[jmp_request_count].label_name, target_label, LABEL_NAME_LEN);
                jmp_requests[jmp_request_count].jmp_offset = current_address;
                jmp_request_count++;
                output_buffer[current_address++] = 0;
                output_buffer[current_address++] = 0;
            }
        }
        else if (strcmp(cmd, "sys_print") == 0)
        {
            output_buffer[current_address++] = 7;
        }
        else if (strcmp(cmd, "mov rax, [rcx]") == 0)
        {
            output_buffer[current_address++] = 8;
        }
        else if (strcmp(cmd, "mov [rcx], rax") == 0)
        {
            output_buffer[current_address++] = 9;
        }
        else if (strncmp(cmd, "db ", 3) == 0)
        {
            output_buffer[current_address++] = (unsigned char)atoi(cmd + 3);
        }
        else
        {
            printf("\n Ошибка синтаксиса EASM: Неизвестная инструкция '%s'\n", cmd);
            fclose(src);
            free(output_buffer);
            return 1;
        }
    }
    fclose(src);

    // Бэкпатчинг абсолютных 16-битных адресов меток x86
    for (int i = 0; i < jmp_request_count; i++)
    {
        int addr = find_label_address(jmp_requests[i].label_name);
        if (addr != -1)
        {
            int offset = jmp_requests[i].jmp_offset;
            output_buffer[offset] = (addr >> 8) & 0xFF;
            output_buffer[offset + 1] = addr & 0xFF;
        }
        else
        {
            printf("\n Ошибка линковки EASM: Метка '%s' не найдена!\n", jmp_requests[i].label_name);
            free(output_buffer);
            return 1;
        }
    }

    FILE* out = fopen(out_path, "wb");
    if (out == NULL)
    {
        printf("\n Ошибка: Не удалось создать файл %s\n", out_path);
        free(output_buffer);
        return 1;
    }
    fwrite(output_buffer, sizeof(unsigned char), current_address, out);
    fclose(out);

    printf("\n Компиляция EASM успешна: %d байт записано в %s\n", current_address, out_path);
    free(output_buffer);
    return 0;
}