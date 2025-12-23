// Виртуальная машина C$ сгенерирована!
#include <stdio.h>
#include <locale.h>
#include <stdlib.h> // для malloc/free
int main(int argc, char * argv[])
{
    setlocale(0, "");
    FILE * file = fopen("_.cdlr", "rb");
    if (file == NULL) { printf("\n_.cdlr: №1"); return -1; }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    printf("\nfile_size = %ld", file_size);
    char * opcode = malloc(file_size);
    fseek(file, 0, SEEK_SET);
    size_t number_of_characters_read = fread(opcode, sizeof (char), sizeof (opcode)-1, file);
    printf("\nnumber_of_characters_read = %llu", number_of_characters_read);
    free(opcode);

    unsigned char instruction_pointer = 0;
    unsigned char memory[0xFF] = {0};

    void * dispatcher[0xFF+1] = {
       [0] = &&invalid,
       [1] = &&jump,
       [2] = &&move,
       [3] = &&inc,
       [4] = &&dec,
       [5] = &&add,
       [6] = &&sub,
       [7] = &&mul,
       [8] = &&div,
       [9] = &&compare,
       [10] = &&jump_if_not_zero,
       [11] = &&call, [12] = &&push, [13] = &&pop, [14] = &&ret,
       [15 ... 0xFF] = &&invalid
    };
    goto * dispatcher[opcode[instruction_pointer]];
/*/ START_OF_BLOCK /*/
jump:
 printf("\njump");
 goto * dispatcher[opcode[++instruction_pointer]];

move:
 printf("\nmove"); // move [1], 0 / move *1, 0
 memory[opcode[instruction_pointer-1]] = opcode[instruction_pointer+=2];
 goto * dispatcher[opcode[++instruction_pointer]];

inc:
 printf("\ninc"); // inc [1] / inc *1
 ++memory[opcode[++instruction_pointer]];
 goto * dispatcher[opcode[++instruction_pointer]];

dec:
 printf("\ndec"); // dec [1] / dec *1
 --memory[opcode[++instruction_pointer]];
 goto * dispatcher[opcode[++instruction_pointer]];

add:
 printf("\nadd"); // add [1], 1 / add *1, 1
 memory[opcode[instruction_pointer-1]] += opcode[instruction_pointer+=2];
 goto * dispatcher[opcode[++instruction_pointer]];

sub:
 printf("\nsub"); // sub [1], 1 / sub *1, 1
 memory[opcode[instruction_pointer-1]] -= opcode[instruction_pointer+=2];
 goto * dispatcher[opcode[++instruction_pointer]];

mul:
 printf("\nmul"); // mul [1], 1 / mul *1, 1
 memory[opcode[instruction_pointer-1]] *= opcode[instruction_pointer+=2];
 goto * dispatcher[opcode[++instruction_pointer]];

div:
 printf("\ndiv"); // div [1], 1 / div *1, 1
 memory[opcode[instruction_pointer-1]] /= opcode[instruction_pointer+=2];
 goto * dispatcher[opcode[++instruction_pointer]];

// Сравнивать
compare:
 printf("\ncompare"); // cmp [1], 1 / cmp *1, 1
 if (memory[opcode[instruction_pointer-1]] == opcode[instruction_pointer+=2]) {}
 goto * dispatcher[opcode[++instruction_pointer]];

// Прыгни если не ноль
jump_if_not_zero:
 printf("\njump_if_not_zero");
 return 0;
// Вызов
call:
 printf("\ncall");
 return 0;
// Втолкнуть / положить на верхушку стека
push:
 printf("\npush");
 return 0;
// Вытолкнуть / снять с верхушки стека
pop:
 printf("\npop");
 return 0;
// Вернуться
ret:
 printf("\nret");
 return 0;

invalid:
 printf("\ninvalid");
 return 0;
/*/ END_OF_BLOCK /*/
    putchar('\n');
    return 0;
}