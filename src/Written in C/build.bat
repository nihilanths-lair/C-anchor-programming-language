:: 1. Собираем обновленную среду
tcc\tcc.exe stage0\meta_environment.c -o stage0\meta_environment.exe

:: 2. ПЕРЕКОМПИЛИРУЕМ прошивку заново!
stage0\meta_environment.exe -c stage0\firmware.meta stage0\firmware.bin

:: 3. Запускаем
stage0\meta_environment.exe -r stage0\firmware.bin stage0\program.bin