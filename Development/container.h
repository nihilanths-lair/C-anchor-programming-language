#ifndef CONTAINER_H
#define CONTAINER_H

// Объявление
extern unsigned char container[];

char AddToContainer(unsigned char *mtape);
char ExtractFromContainer(const unsigned char *mtape);
char RemoveFromContainer(const unsigned char *mtape);

#endif