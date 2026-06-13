#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Максимальный размер имени метки
#define LABEL_NAME_LEN 32

typedef struct {
    char name[LABEL_NAME_LEN];
    unsigned short address;
} Label;

// Структура для отложенной записи адресов (бэкпатчинг)
typedef struct {
    char label_name[LABEL_NAME_LEN];
    unsigned short jmp_offset; // Где в буфере памяти лежит заглушка адреса
} JmpRequest;

Label labels[256];
int label_count = 0;

JmpRequest jmp_requests[256];
int jmp_request_count = 0;

// Функция поиска уже объявленной метки
int find_label(const char* name)
{
    for (int i = 0; i < label_count; i++) { if (strcmp(labels[i].name, name) == 0) { return labels[i].address; } }
    return -1; 
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("\n Usage: compiler.exe <source.txt> <output.bin>\n");
        return 1;
    }
    // Буфер, куда мы собираем бинарный код
    unsigned char output_buffer[0xFFFF] = {0};
    int current_address = 0;
    FILE* src = fopen(argv[1], "r");
    if (!src)
    {
        printf("\n Error opening source file %s\n", argv[1]);
        return 1;
    }
    char line[256];
    // --- ЕДИНСТВЕННЫЙ ПРОХОД ПО ФАЙЛУ ---
    while (fgets( line, sizeof (line), src))
    {
        // Жестко зачищаем и \n, и \r (фикс для Windows)
        line[strcspn( line, "\r\n")] = 0;
        // Пропускаем пустые строки и комментарии
        if (strlen( line) == 0 || line[ 0] == '#') { continue; }
        // Тримминг пробелов в начале строки (если вы захотите делать отступы)
        char* cmd = line;
        while (*cmd == ' ' || *cmd == '\t') cmd++;
        if (*cmd == 0 || *cmd == '#') continue;
        // 1. Проверяем, является ли строка МЕТКОЙ (оканчивается на ':')
        if (cmd[strlen(cmd)-1] == ':')
        {
            cmd[strlen(cmd)-1] = 0; // Отрезаем двоеточие
            // Проверяем, чтобы не было дубликатов
            if (find_label(cmd) != -1)
            {
                printf("\n Error: Duplicate label '%s'\n", cmd);
                fclose(src);
                return 1;
            }
            strncpy(labels[label_count].name, cmd, LABEL_NAME_LEN);
            labels[label_count].address = current_address; // Фиксируем адрес
            label_count++;
            continue;
        }
        // 2. Обработка директивы DATA
        if (strncmp(cmd, "DATA ", 5) == 0)
        {
            output_buffer[current_address++] = atoi(cmd+5);
            continue;
        }
        // 3. Обработка взрослой команды JE (Jump if Equal) с меткой
        if (strncmp(cmd, "JE ", 3) == 0)
        {
            char* target_label = cmd+3;
            output_buffer[current_address++] = 5; // Опкод JE из interpreter.c
            int addr = find_label(target_label);
            if (addr != -1)
            {
                // Метка уже известна (прыжок назад в цикле)
                output_buffer[current_address++] = (addr>>8)&0xFF;
                output_buffer[current_address++] = addr&0xFF;
            }
            else
            {
                // Метка ещё неизвестна (прыжок вперед). Запоминаем запрос на патчинг!
                strncpy(jmp_requests[jmp_request_count].label_name, target_label, LABEL_NAME_LEN);
                jmp_requests[jmp_request_count].jmp_offset = current_address;
                jmp_request_count++;
                // Оставляем временные заглушки под 16-битный адрес
                output_buffer[current_address++] = 0;
                output_buffer[current_address++] = 0;
            }
            continue;
        }
        // 4. Обработка остальных беспардонных взрослых команд
        if (strcmp(cmd, "HLT") == 0) output_buffer[current_address++] = 0;
        else if (strcmp(cmd, "INC GPL_IP") == 0) output_buffer[current_address++] = 1;
        else if (strcmp(cmd, "DEC GPL_IP") == 0) output_buffer[current_address++] = 2;
        else if (strcmp(cmd, "INC [GPL_IP]") == 0) output_buffer[current_address++] = 3;
        else if (strcmp(cmd, "DEC [GPL_IP]") == 0) output_buffer[current_address++] = 4;
        else if (strcmp(cmd, "INT") == 0) output_buffer[current_address++] = 6;
        else
        {
            printf("\n Syntax Error: Unknown instruction '%s'\n", cmd);
            fclose(src);
            return 1;
        }
    }
    fclose(src);
    // --- ФАЗА БЭКПАТЧИНГА (Разрешение прыжков вперед прямо в буфере) ---
    for (int i = 0; i < jmp_request_count; i++)
    {
        int addr = find_label(jmp_requests[i].label_name);
        if (addr == -1)
        {
            printf("\n Link Error: Label '%s' was never defined!\n", jmp_requests[i].label_name);
            return 1;
        }
        // Записываем реальный адрес поверх сохраненных заглушек
        unsigned short offset = jmp_requests[i].jmp_offset;
        output_buffer[offset] = (addr>>8)&0xFF; // Старший байт
        output_buffer[offset+1] = addr&0xFF;    // Младший байт
    }
    // --- ЗАПИСЬ НА ДИСК ---
    FILE* out = fopen(argv[2], "wb");
    if (!out)
    {
        printf("\n Error opening output file %s\n", argv[2]);
        return 1;
    }
    fwrite(output_buffer, 1, current_address, out);
    fclose(out);
    printf("\n EASM Compilation Successful: %d bytes written to %s\n", current_address, argv[2]);
    return 0;
}