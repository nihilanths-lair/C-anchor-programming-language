@echo off
:: 1. Собираем наш Мета-Энвайронмент (Транслятор) через TCC
tcc\tcc.exe stage0\meta_environment.c -o stage0\meta_environment.exe

:: 2. Переводим ваш красивый синтаксис EASM в промежуточный нативный код .asm
stage0\meta_environment.exe -c stage0\program.meta stage0\program.bin

:: Указываем FASM точный путь к его родной папке INCLUDE, которая лежит внутри fasm\
set INCLUDE=fasm\include

:: 3. Вызываем FASM из его подпапки и собираем 64-битный .exe прямо в папку stage0
fasm\fasm.exe stage0\program.asm stage0\program.exe

:: 4. ЗАПУСКАЕМ НА СКОРОСТИ КРЕМНИЯ (Без Си-интерпретатора!)
echo.
echo === ЗАПУСК НА РЕАЛЬНОМ ПРОЦЕССОРЕ PC ===
stage0\program.exe
echo.