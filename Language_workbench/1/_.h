/** [Path: Language_workbench/1/_.h] **/

/*
**   Виды компиляции:
**
**   Пакетная.
**   Построчная.
**   Условная.
*/
// Представление в памяти
unsigned char json[1<<24];

/*/ |№:0-1| /*/ char *json_encode(const char *json);
/*/ |№:0-2| /*/ void json_decode(const char *str);
/*/ |№:0-1| /*/
// Возвращает строку в формате JSON
char *json_encode(const char *json) { return "{}"; }
/*/ |№:0-2| /*/
// Преобразует строку в формате JSON в значение C
void json_decode(const char *str) { return; }

/*/ |№:1-1| /*/ //void compile(const char *code);
/*/ |№:1-2| /*/ //void execute(const char *code);

//void compile(const char *code) { return; }
//void execute(const char *code) { return; }

/*/ |№:2-1| /*/ void analyzer(const char *data, const char *mapping_table);
/*/ |№:2-2| /*/ void synthesizer();

/*/ |№:2-1| /*/
void analyzer(const char *data, const char *mapping_table) { return; }
/*/ |№:2-2| /*/
void synthesizer() { return; }

//---------------------------//
// tokenizer.h (опционально) //
//---------------------------//
/*/ |№:3-1| /*/ void lexical_analyzer();
/*/ |№:3-2| /*/ void lexical_synthesizer();
//
/*/ |№:3-1| /*/ void lexical_analyzer() { return; }
/*/ |№:3-2| /*/ void lexical_synthesizer() { return; }

//----------//
// parser.h //
//----------//
/*/ |№:4-1| /*/ void syntax_validator();
/*/ |№:4-1| /*/ void syntactic_analyzer(const char *data);
/*/ |№:4-2| /*/ void syntactic_synthesizer();
//
/*/ |№:4-1| /*/
void syntax_validator() { return; }
void syntactic_analyzer(const char *data) { return; }
/*/ |№:4-2| /*/
void syntactic_synthesizer() { return; }

//------------//
// semantic.h //
//------------//
/*/ |№:5-1| /*/ void semantic_validator();
/*/ |№:5-1| /*/ void semantic_analyzer();
/*/ |№:5-2| /*/ void semantic_synthesizer();
//
/*/ |№:5-1| /*/
void semantic_validator() { return; }
void semantic_analyzer() { return; }
/*/ |№:5-2| /*/
void semantic_synthesizer() { return; }

//---------------------------------------------//
// intermediate_representation.h (опционально) //
//---------------------------------------------//
/*/ |№:6-1| /*/ // HIR / MIR / LIR

//------------------//
// code_optimizer.h //
//------------------//
/*/ |№:7-1| /*/ void code_optimizer();
//
/*/ |№:7-2| /*/
void code_optimizer() { return; }

//------------------//
// code_generator.h //
//------------------//
enum PROGRAMMING_LANGUAGE
{
   C,
   MACHINE_CODE_X86,
   EXTRA_ASSEMBLER_X86,
   CODE_ABSTRACT_MACHINE,
   ASSEMBLER_ABSTRACT_MACHINE
};
/*/ |№:8-1| /*/ char code_generator(const char back_end, const char *file_name, const char *data);
//
/*/ |№:8-1| /*/
char code_generator(const char back_end, const char *file_name, const char *data)
{
   FILE *file;
   char _file_name[128];
   strcpy(_file_name, file_name);
   
   switch (back_end){
   case C:
   {
      printf("\n/-> C.");

      strcat(_file_name, ".c");
      file = fopen(_file_name, "wb");
      if (file == NULL) return 0x01;

   } break;
   case MACHINE_CODE_X86:
   {
      printf("\n/-> MACHINE_CODE_X86.");
   } break;
   case EXTRA_ASSEMBLER_X86:
   {
      printf("\n/-> EXTRA_ASSEMBLER_X86.");
   } break;
   case CODE_ABSTRACT_MACHINE:
   {
      printf("\n/-> CODE_ABSTRACT_MACHINE.");

      strcat(_file_name, ".bin");
      file = fopen(_file_name, "wb");
      if (file == NULL) return 0x01;

      fwrite(data, 1, sizeof (data)-1, file);

   } break;
   case ASSEMBLER_ABSTRACT_MACHINE:
   {
      printf("\n/-> ASSEMBLER_ABSTRACT_MACHINE.");

      strcat(_file_name, ".asm");
      file = fopen(_file_name, "wb");
      if (file == NULL) return 0x01;

      fwrite(data, 1, sizeof (data)-1, file);

   } }

   putchar('\n');
   fclose(file);
   return 0;
}
/*/ |№:0-1| /*/ char *code_constructor(const char *block_name_definition);
/*/ |№:0-2| /*/ void code_destructor();

/*/ |№:0-1| /*/ char *code_constructor(const char *block_name_definition) { return "^"; }
/*/ |№:0-2| /*/ void code_destructor() { return; }