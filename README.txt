How to Run:

- Option 1: Makefile
	make run && ./compile <input> <output>
	
- Option 2: Compile Driver
	gcc compile.c -o compile && ./compile <input> <output>

NOTE:

lexemelist.txt contains the lexeme list generated by the lexer
trace.txt contains the stack trace generated by the vm
<output> contains the assembly code generated by the parser
