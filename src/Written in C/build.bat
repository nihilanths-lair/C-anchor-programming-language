@echo off
:: 1. Собираем наш Мета-Энвайронмент (Транслятор) через TCC
tcc\tcc.exe stage0\meta_environment.c -o stage0\meta_environment.exe

:: 2. Переводим ваш красивый синтаксис EASM в промежуточный нативный код .asm
stage0\meta_environment.exe -c stage0\program.meta stage0\program.bin

:: 3. Натравливаем FASM, который за мгновение соберет полноценный 64-битный .exe
fasm\fasm.exe stage0\program.asm stage0\program.exe

:: 4. ЗАПУСКАЕМ НА СКОРОСТИ КРЕМНИЯ (Без Си-интерпретатора!)
echo.
echo === ЗАПУСК НА РЕАЛЬНОМ ПРОЦЕССОРЕ PC ===
stage0\program.exe
echo.