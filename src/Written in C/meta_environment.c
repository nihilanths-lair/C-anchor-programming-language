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

    char line[BUFFER_SIZE];

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
            char* num_ptr = cmd + 8;
            while (*num_ptr == ' ' || *num_ptr == '\t') { num_ptr++; }
            uint64_t val = atoull(num_ptr);
            for (int i = 0; i < 8; i++) { output_buffer[current_address++] = val & 0xFF; val >>= 8; }
        }
        else if (strncmp(cmd, "mov rbx,", 8) == 0)
        {
            output_buffer[current_address++] = 2;
            char* num_ptr = cmd + 8;
            while (*num_ptr == ' ' || *num_ptr == '\t') { num_ptr++; }
            uint64_t val = atoull(num_ptr);
            for (int i = 0; i < 8; i++) { output_buffer[current_address++] = val & 0xFF; val >>= 8; }
        }
        else if (strncmp(cmd, "mov rcx,", 8) == 0)
        {
            output_buffer[current_address++] = 3;
            char* num_ptr = cmd + 8;
            while (*num_ptr == ' ' || *num_ptr == '\t') { num_ptr++; }
            uint64_t val = atoull(num_ptr);
            for (int i = 0; i < 8; i++) { output_buffer[current_address++] = val & 0xFF; val >>= 8; }
        }
        else if (strncmp(cmd, "mov rdx,", 8) == 0)
        {
            output_buffer[current_address++] = 4;
            char* num_ptr = cmd + 8;
            while (*num_ptr == ' ' || *num_ptr == '\t') { num_ptr++; }
            uint64_t val = atoull(num_ptr);
            for (int i = 0; i < 8; i++) { output_buffer[current_address++] = val & 0xFF; val >>= 8; }
        }
        else if (strcmp(cmd, "add rax, rbx") == 0)
        {
            output_buffer[current_address++] = 5;
        }
        else if (strcmp(cmd, "sub rax, rbx") == 0)
        {
            output_buffer[current_address++] = 6;
        }
        else if (strcmp(cmd, "cmp rax, rbx") == 0)
        {
            output_buffer[current_address++] = 7;
        }
        else if (strncmp(cmd, "jmp ", 4) == 0)
        {
            output_buffer[current_address++] = 8;
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
        else if (strncmp(cmd, "je ", 3) == 0)
        {
            output_buffer[current_address++] = 9;
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
        else if (strncmp(cmd, "jne ", 4) == 0)
        {
            output_buffer[current_address++] = 10;
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
            output_buffer[current_address++] = 11;
        }
        else if (strcmp(cmd, "mov rax, [rcx]") == 0)
        {
            output_buffer[current_address++] = 12;
        }
        else if (strcmp(cmd, "mov [rcx], rax") == 0)
        {
            output_buffer[current_address++] = 13;
        }
        else if (strcmp(cmd, "mov rbx, [rcx]") == 0)
        {
            output_buffer[current_address++] = 14;
        }
        else if (strcmp(cmd, "mov [rcx], rbx") == 0)
        {
            output_buffer[current_address++] = 15;
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