#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED


/* Some small/fixes suggestions:
 *  First, it is really confusing but use a .h file instead of including a .c
 *      the first 2 line and the last are the only required ones.
 *  Everything in this file is shared when it is #include (d) elsewhere
 *      so all the none prototypes here, remove them from all other files.
 *  Since the parser is really a lexer and a parser and we want to include it
 *      for the parser, lexer.c cannot have a main :( . So to allow the same 
 *      use it can just be renamed to lex (line 65 here and somewhere in lexer.c)
 *  I would suggest redoing some of the spaghetti arguement codes :P
 */


/* Defines to be Shared */
#define DEFAULT_INPUT_FILE "input.pl0" 
#define DEFAULT_OUTPUT_FILE "lexoutput.txt" 
#define SOURCE_WITHOUT_COMMENT "output.txt" 
#define LEXEME_LIST_FILE "lexemelist.txt" 
#define MAX_STR_LENGTH 12 
#define NUM_ARRAY_SIZE 5 
#define MAX_NUM 65535 

#define MAX_SYMBOL_TABLE_SIZE 100 
#define MAX_CODE_LENGTH 500 
#define MAX_STACK_HEIGHT 2000 
#define CHAR_ARRAY_SIZE 12 

/* Shared structures and enumerations */
typedef enum token {
    nulsym = 1, identsym, numbersym, plussym, minussym, 
    multsym, slashsym, oddsym, eqsym, neqsym, 
    lessym, leqsym, gtrsym, geqsym, lparentsym, 
    rparentsym, commasym, semicolonsym, periodsym, becomessym, 
    beginsym, endsym, ifsym, thensym, whilesym, 
    dosym, callsym, constsym, varsym, procsym, 
    writesym, readsym, elsesym
}token_type;

typedef struct symbol {
    int kind;      //const = 1, var = 2, proc = 3
    char name[12]; // name up to 11 chars
    int val;       // number (ASCII value)
    int level;     // L level
    int addr;      // M address
}symbol;

typedef struct instruction {
    int op;
    int l;
    int m;
}instruction;

/* Prototypes for the functions */
void printAndCleanCode(char* inputFile, char* outputFile);
int find_token(char* array_charact);
int find_symbol(char symbol);
int lexiFunction(char* inputFile, int);
void printFile(char* inputFile, int shouldPrintClean);

int lex(int argc, char ** argv);

#endif
