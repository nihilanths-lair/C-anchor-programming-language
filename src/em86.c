#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>

#include "em86.txt"

int main()
{
    setlocale(0, "");
    FILE *file = fopen("parser.meta", "r"); // Открываем наш будущий файл с мета-кодом
    if (file == NULL)
    {
        printf("\n [Ошибка] Не удалось открыть файл parser.meta! Создай его рядом с em86.exe\n");
        return 1;
    }
    int64_t val;
    while (fscanf(file, "%lld", &val) != EOF) // Читаем числа через пробел/перевод строки до конца файла (EOF)
    {
        if (rip >= 0x1000) // Защита от выхода за границы ОЗУ
        {
            printf("\n [Ошибка] Файл parser.meta слишком большой для памяти эмулятора!\n");
            fclose(file);
            return 1;
        }
        memory[rip++] = val;
    }
    fclose(file);
    rip = 0; // Сбрасываем RIP обратно в 0, чтобы процессор стартовал с первой считанной команды
    output_registers(); // Выводим начальное состояние регистров до запуска
    putchar('\n');
    executor(); // Передаем управление нашему единственному винту диспетчеризации
    putchar('\n');
    return 0;
}