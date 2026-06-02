@echo off
tcc\tcc.exe compiler_stage_0.c -o compiler_stage_0.exe
::compiler_stage_0.exe main.meta > nul
compiler_stage_0.exe main.meta > compiler_stage_1.c
tcc\tcc.exe compiler_stage_1.c -o compiler_stage_2.exe

:: ИСПРАВЛЕНО: запускаем именно compiler_stage_2.exe, который только что собрали!
compiler_stage_2.exe main.meta > compiler_stage_2.c