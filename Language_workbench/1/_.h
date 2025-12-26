/** [Path: Language_workbench/1/_.h] **/

/*/ |№:1-1| /*/ void code_constructor();
/*/ |№:1-2| /*/ void code_destructor();

/*/ |№:1-1| /*/ void code_constructor() { return; }
/*/ |№:1-2| /*/ void code_destructor() { return; }

/*/ |№:2-1| /*/ void analyzer(const char *data, const char *mapping_table);
/*/ |№:2-2| /*/ void synthesizer(const char *data);

/*/ |№:2-1| /*/ void analyzer(const char *data, const char *mapping_table) { return; }
/*/ |№:2-2| /*/ void synthesizer(const char *data) { return; }

/*/ |№:1-3| /*/ //void compile(const char *code);
/*/ |№:1-4| /*/ //void execute(const char *code);

// Tokenizer.h (опционально)
/*/ |№:3-1| /*/ void lexical_analyzer(const char *data, const char *mapping_table);
/*/ |№:3-2| /*/ void lexical_synthesizer(const char *data);

/*/ |№:3-1| /*/ void lexical_analyzer(const char *data, const char *mapping_table) { return; }
/*/ |№:3-2| /*/ void lexical_synthesizer(const char *data) { return; }

// Parser.h //
/*/ |№:4-1| /*/ void syntactic_analyzer(const char *data, const char *mapping_table);
/*/ |№:4-2| /*/ void syntactic_synthesizer(const char *data);

/*/ |№:4-1| /*/ void syntactic_analyzer(const char *data, const char *mapping_table) { return; }
/*/ |№:4-2| /*/ void syntactic_synthesizer(const char *data) { return; }

/*/ |№:4-3| /*/ // void parse(const char *code);

// Semantic.h //
/*/ |№:5-1| /*/ void semantic_analyzer(const char *data, const char *mapping_table);
/*/ |№:5-2| /*/ void semantic_synthesizer(const char *data);

/*/ |№:5-1| /*/ void semantic_analyzer(const char *data, const char *mapping_table) { return; }
/*/ |№:5-2| /*/ void semantic_synthesizer(const char *data) { return; }

// Intermediate_representation.h (опционально) //
/*/ |№:6-1| /*/ // {?}