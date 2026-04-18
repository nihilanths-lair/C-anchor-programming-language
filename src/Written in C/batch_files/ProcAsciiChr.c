const char ProcAsciiChr(unsigned char chr)
{
    switch (chr){
    case '\0': return '·'; // ··0
    case 0x01: return '·'; // ··1
    case 0x02: return '·'; // ··2
    case 0x03: return '·'; // ··3
    case 0x04: return '·'; // ··4
    ///case 0x05: return '·'; // ··5
    case 0x06: return '·'; // ··6
    case 0x07: return '·'; // ··7
    case 0x08: return '·'; // ··8
    case 0x09: return '·'; // ··9
    case '\n': return '·'; // ·10
    case 0x0B: return '·'; // ·11
    case 0x0C: return '·'; // ·12
    case '\r': return '·'; // ·13
    //case 0x0E: return '·'; // ·14
    case 0x0F: return '·'; // ·15
    case 0x10: return '·'; // ·16
    case 0x11: return '·'; // ·17
    case 0x12: return '·'; // ·18
    case 0x13: return '·'; // ·19
    case 0x14: return '·'; // ·20
    case 0x1B: return '·'; // ·27
    // 30-39 или 048-057: 0-9
    // 41-5A или 065-090: A-Z
    // 61-7A или 097-122: a-z
    case 0x90: return '·'; // 144
    case 0x95: return '·'; // 149
    //    A8 или     168: Ё
    //    B8 или     184: ё
    // C0-DF или 192-223: А-Я
    // E0-FF или 224-255: а-я
    default: return chr;
    }
}
const char * valstr(unsigned char val)
{
    unsigned char val_str[3+1];
    unsigned char *ptr__val_str = val_str;
    switch (val){
    case 0: strcpy(val_str, "0"); break;
    case 1: strcpy(val_str, "1"); break;
    case 2: strcpy(val_str, "2"); break;
    case 3: strcpy(val_str, "3"); break;
    case 4: strcpy(val_str, "4"); break;
    case 5: strcpy(val_str, "5"); break;
    case 6: strcpy(val_str, "6"); break;
    case 7: strcpy(val_str, "7"); break;
    case 8: strcpy(val_str, "8"); break;
    case 9: strcpy(val_str, "9"); break;
    case 10: strcpy(val_str, "10"); break;
    }
    return ptr__val_str;
}
// --
const char GetDebuggingInformation(const unsigned char *memory, const unsigned char ip8)
{
    switch (memory[ip8]){
    case   0: // 1
    case   1: // 2
    case   2: // 3
    case   3: break; // 4
    // MOV (Intel)
    case   4: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; Intel", memory[ip8]+1); break; // 5
    case   5: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; Intel", memory[ip8]+1); break; // 6
    case   6: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; Intel", memory[ip8]+1); break; // 7
    case   7: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; Intel", memory[ip8]+1); break; // 8
    case   8: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; Intel", memory[ip8]+1); break; // 9
    // MOV (AT&T)
    case   9: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; AT&T", memory[ip8]+1); break; // 10
    case  10: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; AT&T", memory[ip8]+1); break; // 11
    case  11: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; AT&T", memory[ip8]+1); break; // 12
    case  12: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; AT&T", memory[ip8]+1); break; // 13
    case  13: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; AT&T", memory[ip8]+1); break; // 14
    //
    case 255: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d", memory[ip8]+1); // 256
    }
}