#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){
	char *args = malloc(sizeof(char) * 500);
	strcat(args, "./compiler ");
//	strcat(args, "make run -- ");
	
	int i=1;
	for(;i<argc;i++){
		strcat(args, " ");
		strcat(args,argv[i]);
	}

	system( "gcc lexer.c parser.c vm.c -o compiler" );
	system( args );
	printf("\n");
	
	return 0;
}
