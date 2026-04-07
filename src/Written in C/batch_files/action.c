static char memory[0xFF] = {}; // Отведённая память для загрузчика, в которую будет размещена/помещена программа для исполнения
static unsigned char ip = 0;   // Регистр-указатель на инструкцию (след. команду)

static inline void Action()
{
    void *action[] = {&&_1, &&_2, &&_3};
    goto *action[1];

    printf("\n Старт.");

    _1: // Безусловный переход
     printf("\n Вы поднялись на I этаж.");
     goto _0;

    _2:
     printf("\n Вы поднялись на II этаж.");
     goto _0;

    _3:
     printf("\n Вы поднялись на III этаж.");
     //goto _0;

    _0:
     printf("\n Финиш.");
}