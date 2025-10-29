#include <stdio.h>
//#define __void_filler
#define runblock {
#define endblock }
//unsigned char _iter[2] = {0};
unsigned char cell;
unsigned char memory[0x100];
/*
unsigned char ascii[] =
{"\
\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\
\0\
ЂЃ‚ѓ„…†‡€‰Љ‹ЊЌЋЏђ‘’“”•–—�™љ›њќћџ ЎўЈ¤Ґ¦§Ё©Є«¬­®Ї°±Ііґµ¶·ё№є»јЅѕїАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя\
"};
*/
unsigned char ext_ascii[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 'Ё', 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 'ё', '№', 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П',
    'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я',
    'а', 'б', 'в', 'г', 'д', 'е', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п',
    'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я'
};
void SetValue(unsigned char *memory)//, const unsigned char value
{
    /*
    for (unsigned char iter_0 = '\0'; iter_0 <= 31; iter_0++)
    {
        memory[iter_0] = ' '; //' ';
        //if (iter_0 % 16 == 0) putchar('\n');
        //printf("%02X", memory[iter_0]&0xFF);
        //printf(" %3d", iter_0);
    }
    */
    for (unsigned char iter_0 = '\0'; iter_0 < 0xFF; iter_0++)
    {
        memory[iter_0] = iter_0; //' ';
        //if (iter_0 % 16 == 0) putchar('\n');
        //printf("%02X", memory[iter_0]&0xFF);
        //printf(" %3d", iter_0);
    }
    memory[0xFF] = 0xFF; //' ';
    //printf(" %02X\n", memory[0xFF]&0xFF);
    //printf(" %d\n", memory[0xFF]);
    //memory[0x6] = ' ';
    //memory['\0'] = ' '; // 0
    //memory['\a'] = ' '; // 7
    //memory['\b'] = ' '; // 8
    //memory['\t'] = ' '; // 9
    //memory['\n'] = ' '; // 10
    //memory['\r'] = ' '; // 13
}
void OutputToConsole()
{
    SetValue(memory);
    puts("--------+----------------------------------------------------------------+----------------+");
    puts(" Offset | 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F |     ASCII      |");
    puts("--------+----------------------------------------------------------------+----------------+");
    for (unsigned char iter_0 = 0; iter_0 <= 0xF; iter_0++)
    {
        printf("     %2X |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            printf("[%2X]", memory[cell + iter_1] & 0xFF);
        }
        putchar('|');
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            //if (memory[cell + iter_1] == '\t' || memory[cell + iter_1] == '\n' || memory[cell + iter_1] == '\r') memory[cell + iter_1] = ' ';
            //if (memory[cell + iter_1] < ' ' || memory[cell + iter_1] == 0x7F || memory[cell + iter_1] == 0x98) memory[cell + iter_1] = ' ';
            printf("%c", memory[cell + iter_1]);
        }
        puts("|");
    }
    puts("--------+----------------------------------------------------------------+----------------+");
    SetValue(memory);
    puts("--------+--------------------------------------------------------------------------------+----------------+");
    puts(" Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |");
    puts("--------+--------------------------------------------------------------------------------+----------------+");
    for (unsigned char iter_0 = 0; iter_0 <= 0xF; iter_0++)
    {
        printf("    %3d |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            printf("[%3d]", memory[cell + iter_1]);
        }
        putchar('|');
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            //if (memory[cell + iter_1] == '\t' || memory[cell + iter_1] == '\n' || memory[cell + iter_1] == '\r') memory[cell + iter_1] = ' ';
            //if (memory[cell + iter_1] < ' ' || memory[cell + iter_1] == 0x7F || memory[cell + iter_1] == 0x98) memory[cell + iter_1] = ' ';
            printf("%c", memory[cell + iter_1]);
        }
        puts("|");
    }
    puts("--------+--------------------------------------------------------------------------------+----------------+");
}
void OutputToFile(const char *name)
{
    FILE *doc = fopen(name, "w");
    SetValue(memory);
    fprintf(doc, "--------+----------------------------------------------------------------+----------------+\n");
    fprintf(doc, " Offset | 00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F |     ASCII      |\n");
    fprintf(doc, "--------+----------------------------------------------------------------+----------------+\n");
    for (unsigned char iter_0 = 0; iter_0 <= 0xF; iter_0++)
    {
        fprintf(doc, "     %2X |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%2X]", memory[cell + iter_1] & 0xFF);
        }
        fprintf(doc, "|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            //if (memory[cell + iter_1] == '\t' || memory[cell + iter_1] == '\n' || memory[cell + iter_1] == '\r') memory[cell + iter_1] = ' ';
            //if (memory[cell + iter_1] < ' ' || memory[cell + iter_1] == 0x7F || memory[cell + iter_1] == 0x98) memory[cell + iter_1] = ' ';
            fprintf(doc, "%c", memory[cell + iter_1]);
        }
        fprintf(doc, "|\n");
    }
    fprintf(doc, "--------+----------------------------------------------------------------+----------------+\n");
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, " Offset | 000  001  002  003  004  005  006  007  008  009  010  011  012  013  014  015 |     ASCII      |\n");
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, "      0 |");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        memory[iter_0] = iter_0;
        fprintf(doc, "[%3d]", memory[iter_0]);
        memory[iter_0] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 0; iter_0 <= 15; iter_0++)
    {
        fprintf(doc, "%c", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    fprintf(doc, "     16 |");
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        fprintf(doc, "[%3d]", memory[iter_0]);
        memory[iter_0] = ' ';
    }
    fprintf(doc, "|");
    for (unsigned char iter_0 = 16; iter_0 <= 31; iter_0++)
    {
        fprintf(doc, "%c", memory[iter_0]);
    }
    fprintf(doc, "|\n");
    for (unsigned char iter_0 = 0x2; iter_0 < 0xF; iter_0++)
    {
        fprintf(doc, "    %3d |", cell = iter_0*0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%3d]", memory[cell+iter_1]);
        }
        fprintf(doc, "|");
        void *case_[] = runblock
        &&_0x00, &&_0x01, &&_0x02, &&_0x03, &&_0x04, &&_0x05, &&_0x06, &&_0x07, &&_0x08, &&_0x09, &&_0x0A, &&_0x0B, &&_0x0C, &&_0x0D, &&_0x0E,
        &&_0x0F, &&_0x10, &&_0x11, &&_0x12, &&_0x13, &&_0x14, &&_0x15, &&_0x16, &&_0x17, &&_0x18, &&_0x19, &&_0x1A, &&_0x1B, &&_0x1C, &&_0x1D,
        &&_0x1E, &&_0x1F, &&_0x20, &&_0x21, &&_0x22, &&_0x23, &&_0x24, &&_0x25, &&_0x26, &&_0x27, &&_0x28, &&_0x29, &&_0x2A, &&_0x2B, &&_0x2C,
        &&_0x2D, &&_0x2E, &&_0x2F, &&_0x30, &&_0x31, &&_0x32, &&_0x33, &&_0x34, &&_0x35, &&_0x36, &&_0x37, &&_0x38, &&_0x39, &&_0x3A, &&_0x3B,
        &&_0x3C, &&_0x3D, &&_0x3E, &&_0x3F, &&_0x40, &&_0x41, &&_0x42, &&_0x43, &&_0x44, &&_0x45, &&_0x46, &&_0x47, &&_0x48, &&_0x49, &&_0x4A,
        &&_0x4B, &&_0x4C, &&_0x4D, &&_0x4E, &&_0x4F, &&_0x50, &&_0x51, &&_0x52, &&_0x53, &&_0x54, &&_0x55, &&_0x56, &&_0x57, &&_0x58, &&_0x59,
        &&_0x5A, &&_0x5B, &&_0x5C, &&_0x5D, &&_0x5E, &&_0x5F, &&_0x60, &&_0x61, &&_0x62, &&_0x63, &&_0x64, &&_0x65, &&_0x66, &&_0x67, &&_0x68,
        &&_0x69, &&_0x6A, &&_0x6B, &&_0x6C, &&_0x6D, &&_0x6E, &&_0x6F, &&_0x70, &&_0x71, &&_0x72, &&_0x73, &&_0x74, &&_0x75, &&_0x76, &&_0x77,
        &&_0x78, &&_0x79, &&_0x7A, &&_0x7B, &&_0x7C, &&_0x7D, &&_0x7E, &&_0x7F, &&_0x80, &&_0x81, &&_0x82, &&_0x83, &&_0x84, &&_0x85, &&_0x86,
        &&_0x87, &&_0x88, &&_0x89, &&_0x8A, &&_0x8B, &&_0x8C, &&_0x8D, &&_0x8E, &&_0x8F, &&_0x90, &&_0x91, &&_0x92, &&_0x93, &&_0x94, &&_0x95,
        &&_0x96, &&_0x97, &&_0x98, &&_0x99, &&_0x9A, &&_0x9B, &&_0x9C, &&_0x9D, &&_0x9E, &&_0x9F, &&_0xA0, &&_0xA1, &&_0xA2, &&_0xA3, &&_0xA4,
        &&_0xA5, &&_0xA6, &&_0xA7, &&_0xA8, &&_0xA9, &&_0xAA, &&_0xAB, &&_0xAC, &&_0xAD, &&_0xAE, &&_0xAF, &&_0xB0, &&_0xB1, &&_0xB2, &&_0xB3,
        &&_0xB4, &&_0xB5, &&_0xB6, &&_0xB7, &&_0xB8, &&_0xB9, &&_0xBA, &&_0xBB, &&_0xBC, &&_0xBD, &&_0xBE, &&_0xBF, &&_0xC0, &&_0xC1, &&_0xC2,
        &&_0xC3, &&_0xC4, &&_0xC5, &&_0xC6, &&_0xC7, &&_0xC8, &&_0xC9, &&_0xCA, &&_0xCB, &&_0xCC, &&_0xCD, &&_0xCE, &&_0xCF, &&_0xD0, &&_0xD1,
        &&_0xD2, &&_0xD3, &&_0xD4, &&_0xD5, &&_0xD6, &&_0xD7, &&_0xD8, &&_0xD9, &&_0xDA, &&_0xDB, &&_0xDC, &&_0xDD, &&_0xDE, &&_0xDF, &&_0xE0
        endblock;
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            /*_0x__:*/goto *case_[cell+iter_1];
            _0x00:_0x01:_0x02:_0x03:_0x04:_0x05:_0x06:_0x07:_0x08:_0x09:_0x0A:_0x0B:_0x0C:_0x0D:_0x0E: 
            _0x0F:_0x10:_0x11:_0x12:_0x13:_0x14:_0x15:_0x16:_0x17:_0x18:_0x19:_0x1A:_0x1B:_0x1C:_0x1D:
            _0x1E:_0x1F:_0x20:_0x21:_0x22:_0x23:_0x24:_0x25:_0x26:_0x27:_0x28:_0x29:_0x2A:_0x2B:_0x2C:
            _0x2D:_0x2E:_0x2F:_0x30:_0x31:_0x32:_0x33:_0x34:_0x35:_0x36:_0x37:_0x38:_0x39:_0x3A:_0x3B:
            _0x3C:_0x3D:_0x3E:_0x3F:_0x40:_0x41:_0x42:_0x43:_0x44:_0x45:_0x46:_0x47:_0x48:_0x49:_0x4A:
            _0x4B:_0x4C:_0x4D:_0x4E:_0x4F:_0x50:_0x51:_0x52:_0x53:_0x54:_0x55:_0x56:_0x57:_0x58:_0x59:
            _0x5A:_0x5B:_0x5C:_0x5D:_0x5E:_0x5F:_0x60:_0x61:_0x62:_0x63:_0x64:_0x65:_0x66:_0x67:_0x68:
            _0x69:_0x6A:_0x6B:_0x6C:_0x6D:_0x6E:_0x6F:_0x70:_0x71:_0x72:_0x73:_0x74:_0x75:_0x76:_0x77:
            _0x78:_0x79:_0x7A:_0x7B:_0x7C:_0x7D:_0x7E:
             fprintf(doc, "%c", memory[cell+iter_1]);
             goto _0x__;
            _0x7F:_0x80:_0x81:_0x82:_0x83:_0x84:_0x85:_0x86:_0x87:
             memory[cell+iter_1] = ' ';
             goto _0x__;
            _0x88:_0x89:_0x8A:_0x8B:_0x8C:_0x8D:_0x8E:_0x8F:_0x90:_0x91:_0x92:_0x93:_0x94:_0x95:
            _0x96:_0x97:_0x98:_0x99:_0x9A:_0x9B:_0x9C:_0x9D:_0x9E:_0x9F:_0xA0:_0xA1:_0xA2:_0xA3:_0xA4:
            _0xA5:_0xA6:_0xA7:_0xA8:_0xA9:_0xAA:_0xAB:_0xAC:_0xAD:_0xAE:_0xAF:_0xB0:_0xB1:_0xB2:_0xB3:
            _0xB4:_0xB5:_0xB6:_0xB7:_0xB8:_0xB9:_0xBA:_0xBB:_0xBC:_0xBD:_0xBE:_0xBF:_0xC0:_0xC1:_0xC2:
            _0xC3:_0xC4:_0xC5:_0xC6:_0xC7:_0xC8:_0xC9:_0xCA:_0xCB:_0xCC:_0xCD:_0xCE:_0xCF:_0xD0:_0xD1:
            _0xD2:_0xD3:_0xD4:_0xD5:_0xD6:_0xD7:_0xD8:_0xD9:_0xDA:_0xDB:_0xDC:_0xDD:_0xDE:_0xDF:_0xE0:
             fprintf(doc, "%c", memory[cell+iter_1]);
            _0x__:
        }
        fprintf(doc, "|\n");
    }
    /*
    fprintf(doc, "\n# # # #\n");
    fprintf(doc, "unsigned char ext_ascii[] =\n{\n");
    cell = 0;
    for (unsigned char iter_0 = 0x0; iter_0 < 0xF; iter_0++)
    {
        //cell = iter_0 * 0x10;
        fprintf(doc, "    ");
        for (unsigned char iter_1 = 0; iter_1 < 0xF; iter_1++)
        {
            //if (cell < 0x20) { fprintf(doc, "0x%02X, ", cell++); continue; }
            if (cell < 0x1F || cell > 0x7E && cell < 0xA8 || cell > 0xA8 && cell < 0xB8 || cell > 0xB9 && cell < 0xC0) fprintf(doc, "0x%02X, ", cell++);
            else if (cell == '\'' || cell == '\\') fprintf(doc, "'\\%c', ", cell++);
            else fprintf(doc, "'%c', ", cell++);
        }
        switch (cell)
        runblock
        case 0x0F: case 0x1F: case 0x7F: case 0x8F: case 0x9F: case 0xAF: case 0xBF: fprintf(doc, "0x%02X,\n", cell++); break;
        default: fprintf(doc, "'%c',\n", cell++);
        endblock
        //if (iter_0 % 0x10 == 0) { fprintf(doc, "\n    ", iter_0); continue; }
    }
    fprintf(doc, "    ");
    for (unsigned char iter_0 = 0x0; iter_0 < 0xF; iter_0++)
    {
        fprintf(doc, "'%c', ", cell++);
    }
    fprintf(doc, "'%c'\n};", cell++);
    //fprintf(doc, "'%c'\n};", 0xFF);
    fprintf(doc, "\n# # # #\n");
    */
    /*
    for (unsigned char iter_0 = 0x7; iter_0 < 0x8; iter_0++)
    {
        fprintf(doc, "    %3d |", cell = iter_0 * 0x10);
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "[%3d]", memory[cell + iter_1]);
        }
        fprintf(doc, "|");
        for (unsigned char iter_1 = 0; iter_1 <= 0xF; iter_1++)
        {
            fprintf(doc, "%c", memory[cell + iter_1]);
        }
        fprintf(doc, "|\n");
    }
    */
    fprintf(doc, "--------+--------------------------------------------------------------------------------+----------------+\n");
    fprintf(doc, "void *case_[] =\n{\n");
    for (unsigned char iter_0 = '\0'; iter_0 < 0xF; iter_0++)
    {
        fprintf(doc, "    ");
        for (unsigned char iter_1 = '\0'; iter_1 < 0xF; iter_1++)
        {
            fprintf(doc, "&&_0x%02X, ", iter_0*0xF+iter_1);
        }
        fprintf(doc, "\n");
    }
    fprintf(doc, "};\n");
    //fprintf(doc, "/*_0x__:*/goto *case_[cell+iter_1];");
    for (unsigned char iter_0 = '\0'; iter_0 < 0xF; iter_0++)
    {
        for (unsigned char iter_1 = '\0'; iter_1 < 0xF; iter_1++)
        {
            fprintf(doc, "_0x%02X:", iter_0*0xF+iter_1);
        }
        fprintf(doc, "\n");
    }
    fclose(doc);
}
int main()
{
    OutputToConsole();
    OutputToFile("0.logged");
    return 0;
}