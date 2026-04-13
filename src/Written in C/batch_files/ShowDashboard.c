void ShowDashboard(const unsigned char *memory, int ip, int sp)
{
    #if defined DEBUG
     puts("\n ENTRANCE: ShowDashboard");
    #endif

    static unsigned char count = 0;
    count++;
    printf("\n >->-> ShowDashboard(): %d - \"%s\"\n", count, memory[ip]); // Отобразить символическую мнемонику соответствующую опкоду / symbol_mnemonics
    switch (0){
    case 0:
    {
        printf("\n\t__________________");
        printf("\n_______/ HEX = DEC = ASCII");
        printf("\n IP [%02X]: %02X = %03d = %c", ip, memory[ip], memory[ip], memory[ip]);
        printf("\n SP [%02X]: %02X = %03d = %c\n", sp, memory[sp], memory[sp], memory[ip]);
        printf("\n MEMORY (HEX | DEC | ASCII)\n    ");
        for (int i = 0; i < 0x0F+1; i++) printf("[%02X]", i);
        printf("        ");
        for (int i = 0; i < 0x0F+1; i++) printf("[%03d]", i);
        //printf("        ");
        //for (int i = 0; i < 0x0F + 1; i++) printf("[%c]", i);
        for (int j = 0; j < 0x0F+1; j++)
        {
            printf("\n[%02X]", j*0x10);
            for (int i = 0; i < 0x0F+1; i++) printf(" %02X ", memory[j*0x10+i]);
            printf("   ");
            printf("[%03d]", j*0x10);
            for (int i = 0; i < 0x0F+1; i++) printf(" %03d ", memory[j*0x10+i]);
            printf("   ");
            for (int i = 0; i < 0x0F+1; i++) printf("%c", ProcAsciiChr(memory[j*0x10+i]));
        }
        break;
    }
    case 10:
    {
        printf("MEMORY (DEC)\n     ");
        for (int i = 0; i < 0x0F+1; i++) printf("[%03d]", i);
        for (int j = 0; j < 0x0F+1; j++)
        {
            printf("\n[%03d]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F+1; i++) printf(" %03d ", memory[i]);
        }
        break;
    }
    case 16:
    {
        printf("MEMORY (HEX)\n    ");
        for (int i = 0; i < 0x0F + 1; i++) printf("[%02X]", i);
        for (int j = 0; j < 0x0F + 1; j++)
        {
            printf("\n[%02X]", j * (0x0F + 1));
            for (int i = 0; i < 0x0F+1; i++) printf(" %02X ", memory[i]);
        }
    }}
    putchar('\n');

    #if defined DEBUG
     puts("\n EXIT: ShowDashboard");
    #endif
}