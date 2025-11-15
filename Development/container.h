#ifndef CONTAINER_H
#define CONTAINER_H

// ќбъ€вление
//extern unsigned char container[]; ƒелает массив видимым в других файлах

char AddToContainer(unsigned char *mtape);
char ExtractFromContainer(const unsigned char *mtape);
char RemoveFromContainer(const unsigned char *mtape);

#endif