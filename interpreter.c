/*/
 *    <-- Encoding Windows-1251 / Кодировка Windows-1251 -->
/*/
// Подключаемые библиотеки
#include <stdio.h>
#include <locale.h>

unsigned char __operation_code__[0xFF];
unsigned char *operation_code = __operation_code__;
//unsigned char conveyor = 0xFF; // Размер конвейера всегда должен соответствовать размеру `operation code`
unsigned char __memory__[0xFF]; // = {0}; / Если требуется инициализировать память для избавления от мусора
unsigned char *memory = __memory__;

#define STACK __pointer_3
unsigned char STACK = 0x00;
unsigned char stack[0xFF]; // = {0}; / Если требуется инициализировать стек для избавления от мусора

#include "declaration.h"
unsigned char Main()
{
    FILE *f;
    // Открытие файла на чтение
    if ((f = fopen("0.bf++", "r")) == NULL) return 1;
    // Чтение всего файла целиком
    fread(operation_code, sizeof (operation_code), 1, f);
    fclose(f);
    return vCPU();
}
FILE *f;
void ShowMemoryPanel()
{
    static unsigned char counter = 0;
    if (!counter) fprintf(f, "        HEX|DEC\n");
    else fprintf(f, "\n\n        HEX|DEC\n");
    ++counter;
    fprintf(f, "Counter: %02X|%03d\n", counter, counter);
    fprintf(f, "Bytecode: %02X|%03d\n", *operation_code, *operation_code);
    fprintf(f, "+--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(f, "| Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |\n");
    fprintf(f, "+--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(f, "|      0 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[0], memory[1], memory[2], memory[3], memory[4], memory[5], memory[6], memory[7], memory[8], memory[9], memory[10], memory[11], memory[12], memory[13], memory[14], memory[15]);
    fprintf(f, "|     16 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[16], memory[17], memory[18], memory[19], memory[20], memory[21], memory[22], memory[23], memory[24], memory[25], memory[26], memory[27], memory[28], memory[29], memory[30], memory[31]);
    fprintf(f, "|     32 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[32], memory[33], memory[34], memory[35], memory[36], memory[37], memory[38], memory[39], memory[40], memory[41], memory[42], memory[43], memory[44], memory[45], memory[46], memory[47]);
    fprintf(f, "|     48 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[48], memory[49], memory[50], memory[51], memory[52], memory[53], memory[54], memory[55], memory[56], memory[57], memory[58], memory[59], memory[60], memory[61], memory[62], memory[63]);
    fprintf(f, "|     64 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[64], memory[65], memory[66], memory[67], memory[68], memory[69], memory[70], memory[71], memory[72], memory[73], memory[74], memory[75], memory[76], memory[77], memory[78], memory[79]);
    fprintf(f, "|     80 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[80], memory[81], memory[82], memory[83], memory[84], memory[85], memory[86], memory[87], memory[88], memory[89], memory[90], memory[91], memory[92], memory[93], memory[94], memory[95]);
    fprintf(f, "|     96 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[96], memory[97], memory[98], memory[99], memory[100], memory[101], memory[102], memory[103], memory[104], memory[105], memory[106], memory[107], memory[108], memory[109], memory[110], memory[111]);
    fprintf(f, "|    112 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[112], memory[113], memory[114], memory[115], memory[116], memory[117], memory[118], memory[119], memory[120], memory[121], memory[122], memory[123], memory[124], memory[125], memory[126], memory[127]);
    fprintf(f, "|    128 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[128], memory[129], memory[130], memory[131], memory[132], memory[133], memory[134], memory[135], memory[136], memory[137], memory[138], memory[139], memory[140], memory[141], memory[142], memory[143]);
    fprintf(f, "|    144 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[144], memory[145], memory[146], memory[147], memory[148], memory[149], memory[150], memory[151], memory[152], memory[153], memory[154], memory[155], memory[156], memory[157], memory[158], memory[159]);
    fprintf(f, "|    160 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[160], memory[161], memory[162], memory[163], memory[164], memory[165], memory[166], memory[167], memory[168], memory[169], memory[170], memory[171], memory[172], memory[173], memory[174], memory[175]);
    fprintf(f, "|    176 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[176], memory[177], memory[178], memory[179], memory[180], memory[181], memory[182], memory[183], memory[184], memory[185], memory[186], memory[187], memory[188], memory[189], memory[190], memory[191]);
    fprintf(f, "|    192 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[192], memory[193], memory[194], memory[195], memory[196], memory[197], memory[198], memory[199], memory[200], memory[201], memory[202], memory[203], memory[204], memory[205], memory[206], memory[207]);
    fprintf(f, "|    208 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[208], memory[209], memory[210], memory[211], memory[212], memory[213], memory[214], memory[215], memory[216], memory[217], memory[218], memory[219], memory[220], memory[221], memory[222], memory[223]);
    fprintf(f, "|    224 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[224], memory[225], memory[226], memory[227], memory[228], memory[229], memory[230], memory[231], memory[232], memory[233], memory[234], memory[235], memory[236], memory[237], memory[238], memory[239]);
    fprintf(f, "|    240 | %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d  %03d |                |\n", memory[240], memory[241], memory[242], memory[243], memory[244], memory[245], memory[246], memory[247], memory[248], memory[249], memory[250], memory[251], memory[252], memory[253], memory[254], memory[255]);
    fprintf(f, "+--------+--------------------------------------------------------------------------------+----------------+");
}
unsigned char vCPU()
{
    void *address[] =
    {
        &&_00, &&_01, &&_02, &&_03, &&_04, &&_05, &&_06, &&_07, &&_08, &&_09, &&_0A, &&_0B, &&_0C, &&_0D, &&_0E, &&_0F,
        &&_10, &&_11, &&_12, &&_13, &&_14, &&_15, &&_16, &&_17, &&_18, &&_19, &&_1A, &&_1B, &&_1C, &&_1D, &&_1E, &&_1F,
        &&_20, &&_21, &&_22, &&_23, &&_24, &&_25, &&_26, &&_27, &&_28, &&_29, &&_2A, &&_2B, &&_2C, &&_2D, &&_2E, &&_2F,
        &&_30, &&_31, &&_32, &&_33, &&_34, &&_35, &&_36, &&_37, &&_38, &&_39, &&_3A, &&_3B, &&_3C, &&_3D, &&_3E, &&_3F,
        &&_40, &&_41, &&_42, &&_43, &&_44, &&_45, &&_46, &&_47, &&_48, &&_49, &&_4A, &&_4B, &&_4C, &&_4D, &&_4E, &&_4F,
        &&_50, &&_51, &&_52, &&_53, &&_54, &&_55, &&_56, &&_57, &&_58, &&_59, &&_5A, &&_5B, &&_5C, &&_5D, &&_5E, &&_5F,
        &&_60, &&_61, &&_62, &&_63, &&_64, &&_65, &&_66, &&_67, &&_68, &&_69, &&_6A, &&_6B, &&_6C, &&_6D, &&_6E, &&_6F,
        &&_70, &&_71, &&_72, &&_73, &&_74, &&_75, &&_76, &&_77, &&_78, &&_79, &&_7A, &&_7B, &&_7C, &&_7D, &&_7E, &&_7F,
        &&_80, &&_81, &&_82, &&_83, &&_84, &&_85, &&_86, &&_87, &&_88, &&_89, &&_8A, &&_8B, &&_8C, &&_8D, &&_8E, &&_8F,
        &&_90, &&_91, &&_92, &&_93, &&_94, &&_95, &&_96, &&_97, &&_98, &&_99, &&_9A, &&_9B, &&_9C, &&_9D, &&_9E, &&_9F,
        &&_A0, &&_A1, &&_A2, &&_A3, &&_A4, &&_A5, &&_A6, &&_A7, &&_A8, &&_A9, &&_AA, &&_AB, &&_AC, &&_AD, &&_AE, &&_AF,
        &&_B0, &&_B1, &&_B2, &&_B3, &&_B4, &&_B5, &&_B6, &&_B7, &&_B8, &&_B9, &&_BA, &&_BB, &&_BC, &&_BD, &&_BE, &&_BF,
        &&_C0, &&_C1, &&_C2, &&_C3, &&_C4, &&_C5, &&_C6, &&_C7, &&_C8, &&_C9, &&_CA, &&_CB, &&_CC, &&_CD, &&_CE, &&_CF,
        &&_D0, &&_D1, &&_D2, &&_D3, &&_D4, &&_D5, &&_D6, &&_D7, &&_D8, &&_D9, &&_DA, &&_DB, &&_DC, &&_DD, &&_DE, &&_DF,
        &&_E0, &&_E1, &&_E2, &&_E3, &&_E4, &&_E5, &&_E6, &&_E7, &&_E8, &&_E9, &&_EA, &&_EB, &&_EC, &&_ED, &&_EE, &&_EF,
        &&_F0, &&_F1, &&_F2, &&_F3, &&_F4, &&_F5, &&_F6, &&_F7, &&_F8, &&_F9, &&_FA, &&_FB, &&_FC, &&_FD, &&_FE, &&_FF
    };
    f = fopen("logging.txt", "w");
    //  /!\ Запускаем процессор на исполнение команд / инструкций (даём старт) /!\  //
    goto *address[*operation_code];
    //----------------------------------------------------------------------------------------------------//
    _00: // STOP
     ShowMemoryPanel();
     return 0;
    //----------------------------------------------------------------------------------------------------//
    _01: // BF: `+` | INC @~> (Increment/Инкремент) текущей ячейки памяти
     ++(*memory);
     ShowMemoryPanel();
     goto *address[*(++operation_code)];
    //----------------------------------------------------------------------------------------------------//
    _02: // BF: `-` | DEC @~> (Decrement/Декремент) текущей ячейки памяти
     --(*memory);
     ShowMemoryPanel();
     goto *address[*(++operation_code)];
    //----------------------------------------------------------------------------------------------------//
    _03: // BF: `>` | SCRF @~> Scroll forward ~ Прокрутка на шаг вперёд [|] (Move the memory pointer forward one step / Переместить указатель памяти на один шаг вперед) :: MMPFOS
     ++memory;
     goto *address[*(++operation_code)];
    //----------------------------------------------------------------------------------------------------//
    _04: // BF: `<` | SCRB @~> Scroll back ~ Прокрутка на шаг назад [|] (Move the memory pointer back one step / Переместить указатель памяти на один шаг назад) :: MMPBOS
     --memory;
     goto *address[*(++operation_code)];
    //----------------------------------------------------------------------------------------------------//
    _05: // BF: `=` | Поместить значение в текущую ячейку памяти / Place a value into the current memory cell (@~> PVICMC
     *memory = *(++operation_code);
     goto *address[*(++operation_code)];
    //----------------------------------------------------------------------------------------------------//
    _06: // BF: `&` | Получить значение с текущей ячейки памяти / Get the value from the current memory cell (@~> GVFCMC
     // <?> = memory[MEMORY];
     goto *address[*(++operation_code)];
    //----------------------------------------------------------------------------------------------------//
    _07: // ?? PUSH ??
    /*
     stack[STACK]++;
     goto *address[*(++operation_code)];
    */
    //----------------------------------------------------------------------------------------------------//
    _08: // ?? POP ??
    /*
     stack[STACK]--;
     goto *address[*(++operation_code)];
    */
    //----------------------------------------------------------------------------------------------------//
    _09: // ?? INT ??
    /*
     goto *address[*(++operation_code)];
    */
    //----------------------------------------------------------------------------------------------------//
    _0A:_0B:_0C:_0D:_0E:_0F:
    _10:_11:_12:_13:_14:_15:_16:_17:_18:_19:_1A:_1B:_1C:_1D:_1E:_1F:
    _20:_21:_22:_23:_24:_25:_26:_27:_28:_29:_2A:_2B:_2C:_2D:_2E:_2F:
    _30:_31:_32:_33:_34:_35:_36:_37:_38:_39:_3A:_3B:_3C:_3D:_3E:_3F:
    _40:_41:_42:_43:_44:_45:_46:_47:_48:_49:_4A:_4B:_4C:_4D:_4E:_4F:
    _50:_51:_52:_53:_54:_55:_56:_57:_58:_59:_5A:_5B:_5C:_5D:_5E:_5F:
    _60:_61:_62:_63:_64:_65:_66:_67:_68:_69:_6A:_6B:_6C:_6D:_6E:_6F:
    _70:_71:_72:_73:_74:_75:_76:_77:_78:_79:_7A:_7B:_7C:_7D:_7E:_7F:
    _80:_81:_82:_83:_84:_85:_86:_87:_88:_89:_8A:_8B:_8C:_8D:_8E:_8F:
    _90:_91:_92:_93:_94:_95:_96:_97:_98:_99:_9A:_9B:_9C:_9D:_9E:_9F:
    _A0:_A1:_A2:_A3:_A4:_A5:_A6:_A7:_A8:_A9:_AA:_AB:_AC:_AD:_AE:_AF:
    _B0:_B1:_B2:_B3:_B4:_B5:_B6:_B7:_B8:_B9:_BA:_BB:_BC:_BD:_BE:_BF:
    _C0:_C1:_C2:_C3:_C4:_C5:_C6:_C7:_C8:_C9:_CA:_CB:_CC:_CD:_CE:_CF:
    _D0:_D1:_D2:_D3:_D4:_D5:_D6:_D7:_D8:_D9:_DA:_DB:_DC:_DD:_DE:_DF:
    _E0:_E1:_E2:_E3:_E4:_E5:_E6:_E7:_E8:_E9:_EA:_EB:_EC:_ED:_EE:_EF:
    _F0:_F1:_F2:_F3:_F4:_F5:_F6:_F7:_F8:_F9:_FA:_FB:_FC:_FD:_FE:_FF:
     goto *address[*(++operation_code)]; // goto _100;
    //----------------------------------------------------------------------------------------------------//
    fclose(f);
    return 0;
}
#include "main.c"