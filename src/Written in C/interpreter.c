#include <stdio.h>

int main() 
{
    // 1. Таблица диспетчеризации (Адреса меток в Си-коде)
    // Мы заменяем enum на массив физических адресов кода
    void* dispatch_table[] = {
        &&do_halt,     // 0
        &&do_inc_dp,   // 1
        &&do_dec_dp,   // 2
        &&do_inc_val,  // 3
        &&do_dec_val,  // 4
        &&do_jmp_zero, // 5
        &&do_sys_call  // 6
    };

    // 2. Наш массив памяти теперь хранит не просто опкоды, 
    // а индексы для таблицы диспетчеризации!
    unsigned char __[0xFF] = {
        1, 3, 3, 0 // Пример: INC_DP, INC_VAL, INC_VAL, HALT
    };

    int ip = 0;        // Физический IP скорлупы
    int dp = 0x80;     // Data Pointer

    // Макрос для ультра-быстрого перехода к следующей инструкции.
    // Вызывается в конце КАЖДОЙ команды вместо "break".
    // Он берет индекс из массива, находит физический адрес в таблице и мгновенно прыгает туда.
    #define macro__jmp_do_opcode() goto *dispatch_table[__[ip++]]

    // Запускаем маховик (первый прыжок)
    macro__jmp_do_opcode();

    // --- ЗОНА ВЫПОЛНЕНИЯ КОМАНД ---
    do_halt: {
        return 0; // Полный выход из программы
    }

    do_inc_dp: {
        dp++;
        macro__jmp_do_opcode(); // Мгновенный переход к следующей команде
    }

    do_dec_dp: {
        dp--;
        macro__jmp_do_opcode();
    }

    do_inc_val: {
        __[dp]++;
        macro__jmp_do_opcode();
    }

    do_dec_val: {
        __[dp]--;
        macro__jmp_do_opcode();
    }

    do_jmp_zero: {
        int target = __[ip++];
        if (__[dp] == 0) { ip = target; }
        macro__jmp_do_opcode();
    }

    do_sys_call: {
        if (__[dp] == 1) __[dp] = fgetc(stdin);
        if (__[dp] == 2) putchar(__[dp+1]);
        macro__jmp_do_opcode();
    }
}