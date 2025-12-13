#include <locale.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

char data[1<<30];
int table_offset[0xFFFFFF];
int str_end = 0;
int element_count = 0;

// Добавить элемент в массив
void AddElementToArray(const char * str)
{
    table_offset[element_count] = str_end;  // сохраняем смещение
    int len = strlen(str);
    memcpy(data + str_end, str, len+1);     // копируем строку вместе с '\0'
    str_end += len+1;                       // двигаемся дальше
    element_count++;
}
// Получить элемент из массива
char * GetElementFromArray(int element)
{
    //if (element <= 0 || element > element_count) return NULL;
    return data + table_offset[element-1];
}
// Удалить элемент из массива
void RemoveElementFromArray(int element)
{
    table_offset[element-1] = '\0';
}
int main()
{
    setlocale(0, "");
    //printf("\n<Debug>: data   = %d Gb. | %d Mb. | %d Kb. | %d bytes.", sizeof (data) / (int)pow(1000, 3), sizeof (data) / (int)pow(1000, 2), sizeof (data) / 1000, sizeof (data));

    AddElementToArray("Строка 1");
    AddElementToArray("Строка 2");
    AddElementToArray("Строка 3");
    AddElementToArray("Строка 4");
    printf("\n<Debug 5>: GetElementFromArray(%d) = %s", 1, GetElementFromArray(1));
    printf("\n<Debug 6>: GetElementFromArray(%d) = %s", 2, GetElementFromArray(2));
    printf("\n<Debug 7>: GetElementFromArray(%d) = %s", 3, GetElementFromArray(3));
    printf("\n<Debug 8>: GetElementFromArray(%d) = %s", 4, GetElementFromArray(4));
    putchar('\n');
    //RemoveElementFromArray(3);

    return 0;
}