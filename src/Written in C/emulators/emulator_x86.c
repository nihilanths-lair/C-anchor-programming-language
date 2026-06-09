#include <stdio.h>
#include <string.h>
#include <locale.h>

unsigned char zf = 0;

// Для теста инициализируем регистры ненулевыми значениями (в т.ч. отрицательным SP)
unsigned short ax = 0x1234; 
unsigned short bx = 0x5678;

unsigned short ip = 0x7C00;
unsigned short sp = 0xFFFF; // Проверка знака: отобразится как -1
unsigned short bp = 0x0000;

char * numf(long long num);
char * bin16(unsigned short num);
void dbg_RegisterState();

int main()
{
    setlocale(LC_ALL, ""); // Корректная инициализация локали для всех систем
    // ... //
    dbg_RegisterState();
    return 0;
}

// Функция форматирования чисел с разделением тысяч апострофом
char * numf(long long num)
{
    // 4 буфера, чтобы можно было вызвать функцию до 4 раз внутри одного printf
    static char buffers[4][32];
    static int buf_idx = 0;
    
    // Выбираем текущий буфер
    char * str = buffers[buf_idx];
    buf_idx = (buf_idx + 1) % 4;
    
    char temp[32];
    // Переводим число в обычную строку (поддерживает знак)
    sprintf(temp, "%lld", num);
    
    int len = strlen(temp);
    // Учитываем минус, чтобы не ставить апостроф сразу после него (например, -'100)
    int is_negative = (num < 0) ? 1 : 0;
    int digits_len = len - is_negative;
    
    int insert_quotes = (digits_len - 1) / 3; 
    int new_len = len + insert_quotes;
    
    str[new_len] = '\0';
    
    int src = len - 1;
    int dst = new_len - 1;
    int digit_count = 0;
    
    // Идем с конца строки и копируем цифры, вставляя апостроф каждые 3 знака
    while (src >= is_negative)
    {
        if (digit_count == 3)
        {
            str[dst--] = '\''; 
            digit_count = 0;
        }
        str[dst--] = temp[src--];
        digit_count++;
    }
    
    // Если число было отрицательным, копируем минус в самое начало
    if (is_negative)
    {
        str[dst] = '-';
    }
    
    return str;
}

// Функция для 16-битных регистров (вывод в формате 0000_0000 0000_0000)
char * bin16(unsigned short num)
{
    static char sbin[20]; // 16 бит + 3 разделителя + 1 нуль-терминал = 20 байт
    sbin[19] = '\0';
    
    int pos = 18;
    for (int i = 0; i < 16; i++)
    {
        // Установка разделителей на фиксированные позиции строки
        if (pos == 14 || pos == 4) 
        {
            sbin[pos--] = '_';
        }
        else if (pos == 9) 
        {
            sbin[pos--] = ' ';
        }
        
        // Извлекаем i-й бит с конца с помощью маски, без разрушения переменной num
        sbin[pos--] = ((num >> i) & 1) ? '1' : '0';
    }
    return sbin;
}

void dbg_RegisterState()
{
    static unsigned short step = 0;
    printf("\n -----------\n Шаг: %d", step++);
    // Вынесено в переменную для удобной конфигурации отладчика (0 - компактный, 1 - полный)
    int display_mode = 1;
    switch (display_mode){
    case 0:
    {
        printf("\n -----------------------------------");
        printf("\n  REGISTER |   DEC   |  HEX  | TEXT");
        printf("\n           |         |       |");
        printf("\n        zf |     %03d | %02X    | %d",  zf,  zf,  zf);
        printf("\n           |         |       |");
        printf("\n        ax | %03d %03d | %02X %02X | %s", ax>>8, ax&0xFF, ax>>8, ax&0xFF, numf((short)ax));
        printf("\n        bx | %03d %03d | %02X %02X | %s", bx>>8, bx&0xFF, bx>>8, bx&0xFF, numf((short)bx));
        printf("\n           |         |       |");
        printf("\n        ip | %03d %03d | %02X %02X | %s", ip>>8, ip&0xFF, ip>>8, ip&0xFF, numf((short)ip));
        printf("\n        sp | %03d %03d | %02X %02X | %s", sp>>8, sp&0xFF, sp>>8, sp&0xFF, numf((short)sp));
        printf("\n        bp | %03d %03d | %02X %02X | %s", bp>>8, bp&0xFF, bp>>8, bp&0xFF, numf((short)bp));
        printf("\n -------------------------------------");
        break;
    }
    case 1:
    {
        printf("\n ---------------------------------------------------------");
        printf("\n  REGISTER |   DEC   |  HEX  |         BIN         | TEXT");
        printf("\n           |         |       |                     |");
        printf("\n        zf |     %03d | %02X    |                     | %d",  zf,  zf,  zf);
        printf("\n           |         |       |                     |");
        // (short) приведение гарантирует, что знаковые смещения отобразятся корректно
        printf("\n        ax | %03d %03d | %02X %02X | %s | %s", ax>>8, ax&0xFF, ax>>8, ax&0xFF, bin16(ax), numf((short)ax));
        printf("\n        bx | %03d %03d | %02X %02X | %s | %s", bx>>8, bx&0xFF, bx>>8, bx&0xFF, bin16(bx), numf((short)bx));
        printf("\n           |         |       |                     |");
        printf("\n        ip | %03d %03d | %02X %02X | %s | %s", ip>>8, ip&0xFF, ip>>8, ip&0xFF, bin16(ip), numf((short)ip));
        printf("\n        sp | %03d %03d | %02X %02X | %s | %s", sp>>8, sp&0xFF, sp>>8, sp&0xFF, bin16(sp), numf((short)sp));
        printf("\n        bp | %03d %03d | %02X %02X | %s | %s", bp>>8, bp&0xFF, bp>>8, bp&0xFF, bin16(bp), numf((short)bp));
        printf("\n -----------------------------------------------------------");
        break;
    }
    }
    printf("\n");
}