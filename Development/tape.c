#include "tape.h"

static unsigned char tape[0xFFFF+1]; // 1<<24, 1024*1024*16 // Массив инкапсулирован, доступ осуществляется через функции
static unsigned short ptr_pos = 0;

typedef struct {
    unsigned short size;
} Header;

void *AllocateMemory(unsigned short size)
{
    Header *header = (Header *) &tape[ptr_pos];
    header->size = size;
    ptr_pos += sizeof (Header) + size;
    return (void *) (header + 1);
}

void FreeMemory(void *ptr)
{
    Header *header = (Header *) ptr - 1;
    ptr_pos -= sizeof (Header) + header->size;
}
#include <stdio.h>
void Logged()
{
    printf("ptr_pos = %d\n", ptr_pos);
    Header *header;
    printf("header->size = %d\n", header->size);
}