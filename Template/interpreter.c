#include <stdio.h>
#include <locale.h>

#define then {
#define end }

#define register def_register

unsigned char __instructions__[0xFF]; // = " \nOhota byd' soboi!\n"; // Байт-код/оп-код
unsigned char *instructions = __instructions__;

unsigned char memory[0xFF]; // Вся доступная память (на текущий момент)
typedef struct then
 unsigned char ip; // Указатель на инструкцию
 unsigned char sp; // Указатель на стек (опционально)
 unsigned char dp; // Указатель на данные
end Register;

int main(int argc, char **argv/*/ argv[] /*/) then
 setlocale(0, "");

 printf("<Debug> argc = %d\n", argc);
 while (argc-- != 0) printf("<Debug> argv[%d] = \"%s\"", argc, argv[argc]);

 FILE *file = NULL;
 file = fopen("meta_info.cfg", "rb");
 if (!file)
 {
    file = fopen("meta_info.cfg", "wb");
    if (!file) return -1;
    // При отсутствии файла, записать в него базовые настройки для интерпретатора (VM)
    fputc(0x02, file); // Режим vCPU
    fclose(file);
    file = fopen("meta_info.cfg", "rb");
    if (!file) return -1;
 }
 /*/ Режимы vCPU: Управление потоком кода (указатель на инструкцию). /*/
 // 1 - Регистр IP становится доступен для изменения, программист после каждого шага должен переводить его на адрес следующей инструкции (как в brainfuck).
 // 2 - Регистр IP нельзя изменить вручную, его состояние могут менять различные операторы (например: jump/goto), при желании можно переключиться на ручное управление (как в примере выше).
 // Считывание базовых настроек с файла для интерпретатора (VM)
 unsigned char mode = fgetc(file); // По умолчанию - 2
 printf("\n<Debug> mode = %d", mode);
 fclose(file);

 Register register;
 register.ip = 0x0;
 register.sp = 0x0;
 register.dp = 0x0;
 /*/ GNU extension: range-designator /*/
 static void *execute[] = then
 [0x0] = &&_operation_output,
 [0x1 ... 0xFF] = &&_operation_invalid,
 end;
 *instructions = 0x0; // Код операции для вывода текста в терминал/консоль
 goto *execute[*instructions];

_operation_invalid: // Недопустимая операция
 // Обработка: пропустить, остановить или др.
 // goto dispatch; // или exit/return
 return 0;

_operation_output: // Операция вывода текста в терминал/консоль
 ++instructions; // Сдвинем позицию курсора (указателя) на след. инструкцию
 printf("%s", (const char *) instructions);
 goto *execute[*++instructions]; // Перейди к след. действию
 return 0;
end