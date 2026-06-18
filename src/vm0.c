#include <stdio.h>
#include <locale.h>
#include <stdint.h>

int64_t memory[0x100000] = {1}; // 1 Mb. плоской памяти / тип int64_t, но можно и intptr_t (кастуем к машинному слову), либо int8_t
int32_t ip = 0; // Указатель на инструкцию (не выходит за пределы диапазона)

int main()
{
    setlocale(0, "");
    void * dispatch[] =
    {
        // Слепые опкоды
        &&__1, // аналог hlt, точка остановы
    };
    goto * dispatch[memory[ip]];
    __1: // аналог hlt, точка остановы
    {
        printf("\n Машина остановлена.\n");
        return 0;
    }
    putchar('\n');
    return 0;
}