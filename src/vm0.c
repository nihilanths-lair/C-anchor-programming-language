#include <stdio.h>
#include <locale.h>
#include <stdint.h>

enum
{
    HLT = 0, hlt = 0,
    JMP = 1, jmp = 1,
    RVP = 2, rvp = 2,
    WVP = 3, wvp = 3,
    JE = 4, je = 4,
    JNE = 5, jne = 5
};

int64_t memory[0x100000] // 1 Mb. плоской памяти / тип int64_t, но можно и intptr_t (кастуем к машинному слову), либо int8_t.
 =
{
    // Шитый (голый) код (используем такой способ, пока на нём не напишем первый примитивный компилятор (лексер/парсер),
    // _ который сможет автоматически искать метки (текстовые символики) и производить их замену, подставляя адреса смещений.
    // _ На этом этапе у нас уже появится первый текстовый ассемблер?).
    JE, 0, 0, 4, // if (0 == 0) goto <addr>
    //JE, 1, 2, 4, // if (1 == 2) goto <addr>
    //JNE, 0, 0, 4, // if (0 != 0) goto <addr>
    //JNE, 1, 2, 4, // if (1 != 2) goto <addr>
    HLT
};
int32_t ip = 0; // Указатель на инструкцию (не выходит за пределы диапазона).

// 64-х битные регистры общего назначения (4 шт.) / не слишком ли избыточно для этапа самораскрутки?
//int64_t a = 0;
//int64_t b = 0;
//int64_t c = 0;
//int64_t d = 0;

int main()
{
    setlocale(0, "");
    void * opcode_dispatching[256]
     =
    {
        // Слепые опкоды (syntax: intel)
        [0] = &&__1, // аналог hlt, точка остановы
        [1] = &&__2, // аналог jmp, безусловный (64-х битный) переход | jmp <addr>
        [2] = &&__3, // rvp dst, [src] / аналог mov dst, [src] ?
        [3] = &&__4, // wvp [dst], src / аналог mov [dst], src ?
        [4] = &&__5, // je src src, <addr>
        [5] = &&__6, // jne src src, <addr>
        [6 ... 255] = &&__1
    };
    //printf("\n << Отладка/трассировка >> | адрес: %d, опкод: %d", ip, memory[ip]);
    goto * opcode_dispatching[memory[ip]];
    __1: // аналог hlt, точка остановы
    {
        printf("\n << Отладка/трассировка >> | адрес: %d, опкод: %d", ip, memory[ip]);
        printf("\n Машина остановлена.\n");
        return 0;
    }
    __2: // аналог jmp, безусловный (64-х битный) переход | jmp <addr>
    {
        printf("\n << Отладка/трассировка >> | адрес: %d, опкод: %d", ip, memory[ip]);
        ip = memory[ip+1];
        goto * opcode_dispatching[memory[ip]];
    }
    __3: // rvp dst, [src] / аналог mov dst, [src] ?
    {
        printf("\n << Отладка/трассировка >> | адрес: %d, опкод: %d", ip, memory[ip]);
        memory[ip+1] = memory[memory[ip+2]];
        ip += 3;
        goto * opcode_dispatching[memory[ip]];
    }
    __4: // wvp [dst], src / аналог mov [dst], src ?
    {
        printf("\n << Отладка/трассировка >> | адрес: %d, опкод: %d", ip, memory[ip]);
        memory[memory[ip+1]] = memory[ip+2];
        ip += 3;
        goto * opcode_dispatching[memory[ip]];
    }
    __5: // je src src, <addr>
    {
        printf("\n << Отладка/трассировка >> | адрес: %d, опкод: %d", ip, memory[ip]);
        if (memory[ip+1] == memory[ip+2]) ip = memory[ip+3];
        else ip += 4;
        goto * opcode_dispatching[memory[ip]];
    }
    __6: // jne src src, <addr>
    {
        printf("\n << Отладка/трассировка >> | адрес: %d, опкод: %d", ip, memory[ip]);
        if (memory[ip+1] != memory[ip+2]) ip = memory[ip+3];
        else ip += 4;
        goto * opcode_dispatching[memory[ip]];
    }
    putchar('\n');
    return 0;
}