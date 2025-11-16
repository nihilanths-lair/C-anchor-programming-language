#include "tape.h"

//#include "corecrt.h"

// Определение
//unsigned char tape[1<<24]; // Массив виден отовсюду
static unsigned char tape[0xFFFF+1]; // 1<<24, 1024*1024*16 // Массив инкапсулирован, доступ осуществляется через функции
//static strreg[][]
// Занять память
void *AllocateMemory(unsigned short size)
{
    // 1 шаг: найти свободную зону
    return tape;
}
// Освободить память
void FreeMemory(void *tape)
{
    
}