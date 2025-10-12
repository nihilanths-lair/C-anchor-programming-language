```
[OS Windows]

/!\ У вас уже должен быть установлен компилятор GCC, в данной инструкции он не предоставляется.
Скачиваем файл: gen_prompt.c
Компиляция 1 этап: cc -std=c99 -Wall gen_prompt.c -o prompt.exe
Запуск: prompt.exe
Жмём `Enter`
Компиляция 2 этап: cc -std=c99 -Wall prompt.c -o prompt.exe
Запуск: prompt.exe
```
