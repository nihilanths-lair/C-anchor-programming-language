del starter.exe
gcc starter.c -o starter.exe
starter.exe compiler.cdlr
starter.exe compiler.cdlr > c-front.c
del a.exe
gcc c-front.c
a.exe