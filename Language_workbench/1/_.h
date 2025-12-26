/** [Path: Language_workbench/1/_.h] **/

/*/ |№:1-1| /*/ char *code_constructor(const char *block_name_definition);
/*/ |№:1-2| /*/ void code_destructor();

/*/ |№:1-1| /*/ char *code_constructor(const char *block_name_definition) { return "^"; }
/*/ |№:1-2| /*/ void code_destructor() { return; }

/*/ |№:1-3| /*/ //void compile(const char *code);
/*/ |№:1-4| /*/ //void execute(const char *code);

/*/ |№:2-1| /*/ void analyzer(const char *data, const char *mapping_table);
/*/ |№:2-2| /*/ void synthesizer();

/*/ |№:2-1| /*/ void analyzer(const char *data, const char *mapping_table) { return; }
/*/ |№:2-2| /*/ void synthesizer() { return; }

// tokenizer.h (опционально)
/*/ |№:3-1| /*/ void lexical_analyzer();
/*/ |№:3-2| /*/ void lexical_synthesizer();

/*/ |№:3-1| /*/ void lexical_analyzer() { return; }
/*/ |№:3-2| /*/ void lexical_synthesizer() { return; }

// parser.h //
/*/ |№:4-1| /*/ void syntactic_analyzer();
/*/ |№:4-2| /*/ void syntactic_synthesizer();

/*/ |№:4-1| /*/ void syntactic_analyzer() { return; }
/*/ |№:4-2| /*/ void syntactic_synthesizer() { return; }

// semantic.h //
/*/ |№:5-1| /*/ void semantic_analyzer();
/*/ |№:5-2| /*/ void semantic_synthesizer();

/*/ |№:5-1| /*/ void semantic_analyzer() { return; }
/*/ |№:5-2| /*/ void semantic_synthesizer() { return; }

// intermediate_representation.h (опционально) //
/*/ |№:6-1| /*/ // {?}

// code_optimizer.h
/*/ |№:7-1| /*/ void code_optimizer();
/*/ |№:7-2| /*/ void code_optimizer() { return; }

// code_generator.h
#define MACHINE_CODE_X86 0x00
#define ABSTRACT_COMPUTING_MACHINE 0x01
/*/ |№:8-1| /*/ char code_generator(const char back_end, const char *output_file, const char *data);
char code_generator(const char back_end, const char *output_file, const char *data)
{ 
   FILE *file = fopen(output_file, "wb");
   if (file == NULL) return 0x01;

   switch (back_end){
   case MACHINE_CODE_X86:
   {
      
   }
   break;
   case ABSTRACT_COMPUTING_MACHINE:
   {

   }
   break;}

   fclose(file);
   return 0;
}