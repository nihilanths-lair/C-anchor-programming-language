static inline void Action()
{
    void *action[] = {&&_1, &&_2, &&_3};
    goto *action[1];
    printf("\n Старт.");
    _1:
     printf("\n Вы поднялись на I этаж.");
     goto _0;
    _2:
     printf("\n Вы поднялись на II этаж.");
     goto _0;
    _3:
     printf("\n Вы поднялись на III этаж.");
    _0:
    printf("\n Финиш.");
}