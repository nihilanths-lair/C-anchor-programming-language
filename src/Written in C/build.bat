@echo off

:: Стираем старые следы, чтобы убедиться в чистоте сборки
del compiler_stage_0.exe
del compiler_stage_1.c
del compiler_stage_1.exe

tcc\tcc.exe compiler_stage_0.c -o compiler_stage_0.exe

:: 1. Слепой Stage 0 копирует чистый Си-код ядра в исходник первой стадии
compiler_stage_0.exe compiler_stage_0.meta > compiler_stage_1.c

:: 2. Собираем бинарник первой стадии
tcc\tcc.exe compiler_stage_1.c -o compiler_stage_1.exe

:: 3. Мета-ядро первой стадии запускает текстовый скрипт bootstrap.meta
::     и натравливает его на обёрнутый в теги compiler_stage_1.meta!
compiler_stage_1.exe compiler_stage_1.meta > nul