void GetDebuggingInformation(const unsigned char *memory, const unsigned char ip8)
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
    case  10: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d:", memory[ip8]+1); break; // 11
    case  11: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; AT&T", memory[ip8]+1); break; // 12
    case  12: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; AT&T", memory[ip8]+1); break; // 13
    case  13: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d: MEMORY [%d] = %d ; AT&T", memory[ip8]+1); break; // 14
    // INT
    case  37: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d:", memory[ip8]+1); break; // 52
    //
    case 255: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_%d", memory[ip8]+1); break; // 256
    default: printf("\n [DEBUG] dispatch__mode_8__identifier_opcode_from_37_to_255:"); break;
    }
}