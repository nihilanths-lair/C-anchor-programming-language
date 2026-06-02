@echo off
tcc\tcc.exe compiler_stage_0.c -o compiler_stage_0.exe
::compiler_stage_0.exe main.meta > nul
compiler_stage_0.exe main.meta > compiler_stage_1.c