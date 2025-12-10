::gcc -E LexicalAnalyzer.c > .i/LexicalAnalyzer.i
::gcc -E SyntacticAnalyzer.c > .i/SyntacticAnalyzer.i
::gcc -E SemanticAnalyzer.c > .i/SemanticAnalyzer.i
::gcc -E InterpreterAST.c > .i/InterpreterAST.i
::gcc -E $.c > .i/$.i

::del .i\*.i
::del .o\*.o

::gcc -c LexicalAnalyzer.c -o .o/LexicalAnalyzer.o 
::gcc -c SyntacticAnalyzer.c -o .o/SyntacticAnalyzer.o
::gcc -c SemanticAnalyzer.c -o .o/SemanticAnalyzer.o
::gcc -c InterpreterAST.c -o .o/InterpreterAST.o

gcc -c $.c -o .o/$.o
gcc .o/*.o -o $.exe

$.exe

gcc C$/$.c