#include "container.h"

// Определение
//unsigned char container[1<<24]; // Массив виден отовсюду
static unsigned char container[0xFFFF+1]; // 1<<24, 1024*1024*16 // Массив инкапсулирован, доступ осуществляется через функции
//static

// Вставить эдемент в контейнер
char InsertElementIntoContainer()
{
    //container[] = ?;
    return 0;
}
// Обновить элемент контейнера
char UpdateContainerElement()
{
    //container[] = ?;
    return 0;
}
// Извлечь элемент из контейнера
char ExtractElementFromContainer()
{
    //container[] = ?;
    return 0;
}
// Удалить элемент из контейнера
char RemoveElementFromContainer()
{
    //container[] = ?;
    return 0;
}