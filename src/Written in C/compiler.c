#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Структура для хранения информации о текстовых метках
typedef struct {
    char name[32];
    unsigned short address;
} Label;
Label labels[100];
int label_count = 0;

// Вспомогательная функция поиска адреса метки
int find_label(const char* name)
{
    for (int i = 0; i < label_count; i++) { if (strcmp(labels[i].name, name) == 0) { return labels[i].address; } }
    return -1; // Метка еще не найдена (будет разрешена во втором проходе или в памяти)
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("\n Usage: stage0.exe <source.txt> <output.bin>\n");
        return 1;
    }
    // Для простоты реализации меток, мы сначала соберем весь бинарник в буфер памяти
    unsigned char output_buffer[0xFFFF];
    int current_address = 0;
    FILE* src = fopen(argv[1], "r");
    if (!src)
    {
        printf("\n Error opening source file.\n");
        return 1;
    }
    char line[256];
    // --- ПЕРВЫЙ ПРОХОД: Сборка кода и фиксация меток ---
    while (fgets(line, sizeof(line), src))
    {
        line[strcspn(line, "\n")] = 0; // Убираем \n
        // Пропускаем пустые строки и комментарии
        if (strlen(line) == 0 || line[0] == '#') continue;
        // Если строка оканчивается на двоеточие — это объявление МЕТКИ
        if (line[strlen(line) - 1] == ':')
        {
            line[strlen(line) - 1] = 0; // Отрезаем двоеточие
            strcpy(labels[label_count].name, line);
            labels[label_count].address = current_address; // Запоминаем текущий адрес в бинарнике
            label_count++;
            continue;
        }
        // Обработка директивы DATA
        if (strncmp(line, "DATA ", 5) == 0)
        {
            output_buffer[current_address++] = atoi(line + 5);
            continue;
        }
        // Обработка JMP_ZERO с текстовой меткой
        if (strncmp(line, "JMP_ZERO ", 9) == 0)
        {
            output_buffer[current_address++] = 5; // Опкод JMP_ZERO
            // Временно пишем заглушку (два нуля) под 16-битный адрес. 
            // Мы заменим её на реальный адрес чуть позже.
            output_buffer[current_address++] = 0; 
            output_buffer[current_address++] = 0;
            continue;
        }
        // Обычные однобайтовые команды
        if (strcmp(line, "HALT") == 0)         output_buffer[current_address++] = 0;
        else if (strcmp(line, "INC_DP") == 0)  output_buffer[current_address++] = 1;
        else if (strcmp(line, "DEC_DP") == 0)  output_buffer[current_address++] = 2;
        else if (strcmp(line, "INC_VAL") == 0) output_buffer[current_address++] = 3;
        else if (strcmp(line, "DEC_VAL") == 0) output_buffer[current_address++] = 4;
        else if (strcmp(line, "SYS_CALL") == 0) output_buffer[current_address++] = 6;
        else { printf("Unknown instruction: %s\n", line); }
    }
    // --- ВТОРОЙ ПРОХОД: Разрешение адресов прыжков (Бэкпатчинг) ---
    // Снова пробегаемся по исходнику, чтобы найти куда вели JMP_ZERO
    fseek(src, 0, SEEK_SET);
    int scan_address = 0;
    while (fgets(line, sizeof (line), src))
    {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0 || line[0] == '#' || line[strlen(line) - 1] == ':') continue;
        if (strncmp(line, "JMP_ZERO ", 9) == 0)
        {
            char* label_name = line + 9;
            int target_addr = find_label(label_name);
            if (target_addr != -1)
            {
                // Разрезаем 16-битный адрес метки на 2 байта и пишем их поверх заглушек
                output_buffer[scan_address + 1] = (target_addr >> 8) & 0xFF; // Старший байт
                output_buffer[scan_address + 2] = target_addr & 0xFF;        // Младший байт
            } else { printf("\n Error: Label '%s' not found!\n", label_name); }
            scan_address += 3; // JMP_ZERO весит 3 байта
            continue;
        }
        if (strncmp(line, "DATA ", 5) == 0) { scan_address += 1; continue; }
        scan_address++; // Все остальные команды весят 1 байт
    }
    fclose(src);
    // --- ЗАПИСЬ НА ДИСК ---
    FILE* out = fopen(argv[2], "wb");
    if (out)
    {
        fwrite(output_buffer, 1, current_address, out);
        fclose(out);
        printf("\n Imperative Compilation successful: %d bytes written to %s\n", current_address, argv[2]);
    }
    return 0;
}