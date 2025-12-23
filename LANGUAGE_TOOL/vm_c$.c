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
       [0] = &&_op_invalid,
       [1] = &&_op_jump,
       [2] = &&_op_move,
       [3] = &&_op_inc, [4] = &&_op_dec,
       [5] = &&_op_add, [6] = &&_op_sub, [7] = &&_op_mul, [8] = &&_op_div,
       [9 ... 0xFF] = &&_op_invalid
    };
    goto * dispatcher[opcode[instruction_pointer]];

_op_jump: printf("\nop_jump");
    goto * dispatcher[opcode[++instruction_pointer]];

_op_move: printf("\nop_move"); // move [1], 0 / move *1, 0
    memory[opcode[instruction_pointer-1]] = opcode[instruction_pointer+=2];
    goto * dispatcher[opcode[++instruction_pointer]];

_op_inc: printf("\nop_inc"); // inc [1] / inc *1
    ++memory[opcode[++instruction_pointer]];
    goto * dispatcher[opcode[++instruction_pointer]];

_op_dec: printf("\nop_dec"); // dec [1] / dec *1
    --memory[opcode[++instruction_pointer]];
    goto * dispatcher[opcode[++instruction_pointer]];

_op_add: printf("\nop_add"); // add [1], 1 / add *1, 1
    memory[opcode[instruction_pointer-1]] += opcode[instruction_pointer+=2];
    goto * dispatcher[opcode[++instruction_pointer]];

_op_sub: printf("\nop_sub"); // sub [1], 1 / sub *1, 1
    memory[opcode[instruction_pointer-1]] -= opcode[instruction_pointer+=2];
    goto * dispatcher[opcode[++instruction_pointer]];

_op_mul: printf("\op_mul"); // mul [1], 1 / mul *1, 1
    memory[opcode[instruction_pointer-1]] *= opcode[instruction_pointer+=2];
    goto * dispatcher[opcode[++instruction_pointer]];

_op_div: printf("\op_div"); // div [1], 1 / div *1, 1
    memory[opcode[instruction_pointer-1]] /= opcode[instruction_pointer+=2];
    goto * dispatcher[opcode[++instruction_pointer]];

_op_invalid: printf("\nop_invalid");

    putchar('\n');
    return 0;
}