REM gcc -o $.exe *.c
REM gcc -c container.c -o build/container.o

gcc -c tape.c -o build/tape.o
gcc -c $.c -o build/$.o

gcc -o $.exe build/*o

$.exe