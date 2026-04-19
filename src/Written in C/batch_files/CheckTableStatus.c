// Проверить статус таблицы
void CheckTableStatus(void *opcode_identifier_table[], const unsigned char *opcode_table, const unsigned char table_size)
{
    FILE *file = fopen("output.txt", "wb");
    for (unsigned char i = 0; i < table_size; i++) {
        //printf("\n %ph: <%%%d> = %d | \\h%02X | \\d%03d", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);
        fprintf(file, "%ph: <%%%d> = %d | \\h%02X | \\d%03d\n", opcode_identifier_table[i], i+1, opcode_table[i], opcode_table[i], opcode_table[i]);
    }
    fprintf(file, "%ph: <%%%d> = %d | \\h%02X | \\d%03d", opcode_identifier_table[table_size], table_size+1, opcode_table[table_size], opcode_table[table_size], opcode_table[table_size]);
    fclose(file);
}