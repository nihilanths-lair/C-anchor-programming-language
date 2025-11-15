#include "container.h"

// Определение
//unsigned char container[1<<24]; // Массив виден отовсюду
static unsigned char container[1<<24]; // Массив инкапсулирован, доступ осуществляется через функции

// Добавить в контейнер
char AddToContainer(unsigned char *mtape)
{
    return 0;
}
// Извлечь из контейнера
char ExtractFromContainer(const unsigned char *mtape)
{
    return 0;
}
// Удалить из контейнера
char RemoveFromContainer(const unsigned char *mtape)
{
    return 0;
}