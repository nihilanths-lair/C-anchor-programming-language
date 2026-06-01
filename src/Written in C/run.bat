del compiler_stage_0.exe
tcc\tcc.exe compiler_stage_0.c -o compiler_stage_0.exe
compiler_stage_0.exe main.meta
compiler_stage_0.exe main.meta > compiler_stage_1.c
del compiler_stage_1.exe
tcc\tcc.exe compiler_stage_1.c -o compiler_stage_1.exe
compiler_stage_1.exe main.meta