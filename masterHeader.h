#ifndef MASTER_H_INCLUDED
#define MASTER_H_INCLUDED

/* Defines to be Shared in all files*/
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
#define TRUE 1
#define FALSE 0
#define MAX_LEXI_LEVELS 3

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

typedef struct pmachine{
    int halt; // Is the machine running
    int sp; //stack pointer
    int bp; //base pointer
    int *stack; // Stack array
    int pc; //program counter
    instruction *code; // code array
    instruction ir; // instruction register
    int instrCount; // number of instruction (for printing purposes only)
}pmachine;

/* Prototypes for all the functions */
void printAndCleanCode(char* inputFile, char* outputFile);
int find_token(char* array_charact);
int find_symbol(char symbol);
int lexiFunction(char* inputFile, int lInArg);
void printFile(char* inputFile, int shouldPrintClean);
int lex(int argc, char ** argv);
void program();
void block();
void constDeclaration();
void varDeclaration();
void procDeclaration();
void statement();
void expression();
void condition();
int symbolkind(int position);
int symbollevel(int position);
int symboladdress(int position);
int find_symbol_type(int symbol);
int symbolval(int position);
void error();
int find(char *array);
void term();
void factor();
void enter_symbol(int kind, char* array, int temp, int addr);
void assemblyCode(int op, int l, int m);
void printCode(int arguments); 
void initializeVM(pmachine * vm);
int loadCode(pmachine *vm, char *fileName);
void freeVM(pmachine * vm);
int base(pmachine * vm, int bp, int level);
void opr(pmachine * vm, int m);
void decodeInstruction(pmachine * vm);
void fetch(pmachine * vm);
void step(pmachine * vm);
void opToStr(int op, char  * str);
void print_function(FILE *ofp, int temp, pmachine * vm, instruction instr);
void printCodeInstrCount(FILE * ofp, pmachine * vm);
void printExecutionInitialization(FILE * ofp, pmachine * vm);
int isPipe(pmachine * vm, int i);
void printStack(FILE * ofp, pmachine * vm);
void printExecutionStatus(FILE * ofp, pmachine * vm);
int vm(char*);

#endif
