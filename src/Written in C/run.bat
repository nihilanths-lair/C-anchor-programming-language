@echo off
del compiler_stage_0.exe
tcc\tcc.exe compiler_stage_0.c -o compiler_stage_0.exe
:: 1. compiler_stage_0.exe вырезает мета-ядро из main.meta
compiler_stage_0.exe main.meta > compiler_stage_1.c
del compiler_stage_1.exe
:: 2. Локальный TinyCC мгновенно собирает готовый бинарник compiler_stage_1.exe
tcc\tcc.exe compiler_stage_1.c -o compiler_stage_1.exe
if ERRORLEVEL 1 (
    echo  [Ошибка]: Сборка через TinyCC провалилась!
    pause
) else (
    echo  [Успех]: compiler.exe успешно собран в автономном режиме!
)
compiler_stage_1.exe main.meta