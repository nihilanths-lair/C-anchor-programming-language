#include "tape.h"

// Определение
//unsigned char tape[1<<24]; // Массив виден отовсюду
static unsigned char tape[0xFFFFFF]; // 1<<24, 1024*1024*16 // Массив инкапсулирован, доступ осуществляется через функции

// Занять память
char AllocateMemory()
{
    return 0;
}
// Освободить память
char FreeMemory()
{
    return 0;
}