#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>

#define MEMORY_SIZE 65536
#define MAX_LABELS 512
#define LABEL_NAME_LEN 64
#define BUFFER_SIZE 65536

// Опережающие объявления, чтобы main оставался чистым и структурированным
int run_interpreter(const char* arch_path, const char* prog_path);
int run_compiler(const char* src_path, const char* out_path);

// ============================================================================
// [ГЛАВНАЯ ТОЧКА ВХОДА И ДИСПЕТЧЕРИЗАЦИЯ ПЛАТФОРМЫ]
// ============================================================================
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    if (argc < 4)
    {
        printf("\n Использование:\n");
        printf("   Компиляция: meta_environment.exe -c <исходник.meta> <выход.bin>\n");
        printf("   Запуск VM:  meta_environment.exe -r <архитектура.bin> <программа.bin>\n\n");
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0)
    {
        return run_compiler(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-r") == 0)
    {
        return run_interpreter(argv[2], argv[3]);
    }
    else
    {
        printf("\n Ошибка: Неизвестный флаг '%s'. Используйте -c или -r.\n", argv[1]);
        return 1;
    }
}


// ============================================================================
// ЗОНА №1: СКОРОСТНОЙ ИНТЕРПРЕТАТОР (ВЕЧНЫЙ СИ-КРЕМНИЙ)
// ============================================================================
#define macro__jmp_do_opcode() goto *dispatch[memory[dsl_ip++]]

int run_interpreter(const char* arch_path, const char* prog_path)
{
    unsigned char* memory = (unsigned char*)calloc(MEMORY_SIZE, sizeof (unsigned char));
    if (memory == NULL)
    {
        printf("\n Ошибка: Не удалось выделить память под ОЗУ.\n");
        return 1;
    }

    FILE* arch = fopen(arch_path, "rb");
    if (arch == NULL)
    {
        printf("\n Ошибка: Не удалось открыть файл архитектуры %s\n", arch_path);
        free(memory);
        return 1;
    }
    fread(memory, sizeof (unsigned char), 0x100, arch);
    fclose(arch);

    FILE* prog = fopen(prog_path, "rb");
    if (prog == NULL)
    {
        printf("\n Ошибка: Не удалось открыть файл программы %s\n", prog_path);
        free(memory);
        return 1;
    }
    fread(memory + 0x100, sizeof (unsigned char), MEMORY_SIZE - 0x100, prog);
    fclose(prog);

    // НАСТОЯЩИЕ 64-БИТНЫЕ РЕГИСТРЫ x86-64
    uint64_t dsl_ip = 0; 
    uint64_t gpl_ip = 0x100; 

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
        &&do_sys_print    // 7
    };

    macro__jmp_do_opcode();

    do_hlt:
    {
        printf("\n [EASM Core]: Процессор остановлен. RAX = %llu, RBX = %llu\n", rax, rbx);
        free(memory);
        return 0;
    }

    do_mov_rax_imm:
    {
        // Прямая склейка 64-битного числа из 8 байт памяти без циклов и сдвигов dsl_ip++
        rax = ((uint64_t)memory[dsl_ip] << 56) |
              ((uint64_t)memory[dsl_ip + 1] << 48) |
              ((uint64_t)memory[dsl_ip + 2] << 40) |
              ((uint64_t)memory[dsl_ip + 3] << 32) |
              ((uint64_t)memory[dsl_ip + 4] << 24) |
              ((uint64_t)memory[dsl_ip + 5] << 16) |
              ((uint64_t)memory[dsl_ip + 6] << 8)  |
              (uint64_t)memory[dsl_ip + 7];
        
        dsl_ip += 8; // Сдвигаем указатель команд сразу за пределы числа
        macro__jmp_do_opcode();
    }

    do_mov_rbx_imm:
    {
        rbx = ((uint64_t)memory[dsl_ip] << 56) |
              ((uint64_t)memory[dsl_ip + 1] << 48) |
              ((uint64_t)memory[dsl_ip + 2] << 40) |
              ((uint64_t)memory[dsl_ip + 3] << 32) |
              ((uint64_t)memory[dsl_ip + 4] << 24) |
              ((uint64_t)memory[dsl_ip + 5] << 16) |
              ((uint64_t)memory[dsl_ip + 6] << 8)  |
              (uint64_t)memory[dsl_ip + 7];
              
        dsl_ip += 8;
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

    unsigned char* output_buffer = (unsigned char*)calloc(BUFFER_SIZE, sizeof (unsigned char));
    int current_address = 0;

    label_count = 0;
    jmp_request_count = 0;

    char line[512];

    // Фаза 1: Парсинг исходного кода EASM
    while (fgets(line, sizeof (line), src))
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

        // Проверка метки
        if (cmd[strlen(cmd) - 1] == ':')
        {
            cmd[strlen(cmd) - 1] = 0;
            if (label_count < MAX_LABELS)
            {
                strncpy(labels[label_count].name, cmd, LABEL_NAME_LEN);
                labels[label_count].address = current_address;
                label_count++;
            }
            continue;
        }

        // Трансляция инструкций в новые EASM x86 опкоды
        if (strcmp(cmd, "hlt") == 0)
        {
            output_buffer[current_address++] = 0;
        }
        else if (strncmp(cmd, "mov rax,", 8) == 0)
        {
            output_buffer[current_address++] = 1;
            uint64_t val = atoull(cmd + 8);
            for (int i = 7; i >= 0; i--)
            {
                output_buffer[current_address + i] = val & 0xFF;
                val >>= 8;
            }
            current_address += 8;
        }
        else if (strncmp(cmd, "mov rbx,", 8) == 0)
        {
            output_buffer[current_address++] = 2;
            uint64_t val = atoull(cmd + 8);
            for (int i = 7; i >= 0; i--)
            {
                output_buffer[current_address + i] = val & 0xFF;
                val >>= 8;
            }
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
        else if (strncmp(cmd, "DATA ", 5) == 0)
        {
            output_buffer[current_address++] = (unsigned char)atoi(cmd + 5);
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

    // Фаза 2: Кроссплатформенный Бэкпатчинг меток вперед
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
    // Сохранение готового бинарника
    FILE* out = fopen(out_path, "wb");
    if (out == NULL)
    {
        printf("\n Ошибка: Не удалось создать файл %s\n", out_path);
        free(output_buffer);
        return 1;
    }
    fwrite(output_buffer, sizeof (unsigned char), current_address, out);
    fclose(out);
    printf("\n Компиляция EASM успешна: %d байт записано в %s\n", current_address, out_path);
    free(output_buffer);
    return 0;
}