#ifndef TAPE_H
#define TAPE_H

//#include "corecrt.h"

// Объявление
//extern unsigned char tape[]; Делает массив видимым в других файлах

// Занять память
void *AllocateMemory(unsigned short size);

// Освободить память
void FreeMemory(void *tape);

#endif