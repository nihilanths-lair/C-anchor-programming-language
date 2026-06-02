@echo off
:: Стираем старые следы, чтобы убедиться в чистоте сборки
del compiler_stage_0.exe
del compiler_stage_1.c
del compiler_stage_1.exe
del compiler_stage_2.c
del compiler_stage_2.exe

:: 1. Собираем и запускаем нулевую стадию
tcc\tcc.exe compiler_stage_0.c -o compiler_stage_0.exe
compiler_stage_0.exe main.meta > compiler_stage_1.c

:: 2. Собираем первую стадию (наше мета-ядро)
tcc\tcc.exe compiler_stage_1.c -o compiler_stage_1.exe

:: 3. Запускаем наше мета-ядро НА СЛОВАХ ИЗ ФАЙЛА input.txt!
compiler_stage_1.exe input.meta > nul