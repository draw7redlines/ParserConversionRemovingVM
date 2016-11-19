/* Team Name: 
Compiler Builder 4 
* Viviene Do 
* Kohei Arai 
* Rebeca Amaya 
* Mark Veto 
* Melissa Gramajo */ 

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<stdbool.h> 
#include<ctype.h> 
#include<math.h> 
#include "masterHeader.h" 

FILE *ifp, *ofp;
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
instruction code[MAX_CODE_LENGTH];

char array_charact[CHAR_ARRAY_SIZE];
char *charray;
int token = 0;
int number = 0;
int cx = 0;
int space = 0;
int nindex = 1;
int level = 0;
int address = 4;
int lInArg = 0;
int aInArg = 0;
int vInArg = 0;

int main(int argc, char **argv) {
   
    int i = 0;
    char inputFileName[100];
    char outputFileName[100];

    if (argc > 2) {
            // If these arguments are not given, assume that's a file name.
            // This will take the last parameter that's not -a, -l, -v.
            strcpy(inputFileName, argv[1]);
            
            strcpy(outputFileName, argv[2]);
        
    } else {
        printf("Please follow Dr.Wocjan's format. Run as: ./compile <input> <output>");
        return 1;
    }

    // Before going ahead to program, remove comments, and generate token list.     
    printAndCleanCode(inputFileName, SOURCE_WITHOUT_COMMENT);

    int didLexiFail = 0;
  
    didLexiFail = lexiFunction(inputFileName,lInArg);
  
    if (didLexiFail) {
        error();
    }
    // Here, open token list file. 
    ifp = fopen(LEXEME_LIST_FILE, "r");
   
    // Run parser. 
    program();
   
    fclose(ifp);
    
    //ofp = fopen("writeToFile.txt", "w");
    // ofp = fopen("mcode.pm0", "w");

    // Now, write assembly code to given output.
    ofp = fopen(outputFileName, "w");
    printCode(argc);
    fclose(ofp);
 
    printf("No errors, program is syntactically correct.\n");

    if (aInArg == 1) {
       
        ofp = fopen("mcode.pm0", "r");

        printf("\n---------------------------------\n");
        printf("\tAssembly code.\n");
        printf("---------------------------------\n");

        // Read contents from file
        char c = fgetc(ofp);
        while (c != EOF) {
            printf ("%c", c);
            c = fgetc(ofp);
        }

        printf("\n");
        fclose(ofp);
    }
    vm(outputFileName);
    // printf("Come here 2");
    
    return EXIT_SUCCESS;
}//end of main function 

void error() {
    //Quit the program!!! 
    exit(EXIT_FAILURE);
}//end of error function 

void get_token() {
    fscanf(ifp, "%d", &token);
}

void program() {  
    get_token();
  
    // stops at block
    block();

    get_token(); 

    //periodsym = 19 
    if (token != periodsym) {
        printf("Error number 9, period expected.\n"); // error count: 1 
        error();
    }

    assemblyCode(9, 0, 2);
 
    //VM: SIO = 11 
    //printf("11 0 3\n");
}//end of program function 

void block() {
    //constsym = 28 -- reserved Word
    if (token == constsym) {
        constDeclaration();
    }
    //varsym = 29 -- reserved Word 
    if (token == varsym) {
        varDeclaration();
    }
    //procsym = 30 --reserved Word 
    if (token == procsym) {
        procDeclaration();
    }
 
    // stops at statement
    statement();

}//end of block function 

void constDeclaration() {
    int kind = 1;
	assemblyCode(6, 0, address);
    
	//for the symbol table (const = 1) 
    do {
        get_token();
        
        //identsym = 2 
        if (token != identsym) {
            printf("Error number 4, const must be followed by identifier.\n");
            error();
        }

        charray = (char *) malloc(12);
        fscanf(ifp, "%s\n ", charray);
        int k = find(charray);

        get_token();
         
        //eqsym ("=") = 9 
        if (token != eqsym) {
            error();
        }

        get_token();

        //NUMBER = 9 
        if (token != 3) {
            printf("Error number 2, = must be followed by a number.");
            error();
        }

        
		fscanf(ifp, "%d", &number);
        enter_symbol(kind, charray, number, address);
        get_token();

    } while (token == commasym); // If it is comma, keep reading constant declaration.

    //semicolonsym = 18
    if (token != semicolonsym) {
        printf("Error number 5, comma missing.\n");
        error();
    }

    get_token();

}//end of constDeclaration 

void varDeclaration() {
    int numVariables = 0;
    int kind = 2;
    //var = 2 for the symbol table 
    do {
        get_token();

        //identsym = 2
        if (token != identsym) {
            printf("Error number 4, var must be followed by identifier.\n");
            error();
        }

        charray = (char *) malloc(12);
        fscanf(ifp, "%s ", charray);

        /*
        int i = 0;
        char temp;
        fseek(ifp, 1, SEEK_CUR);
        //to skip over the blank space 
        fscanf(ifp, "%c", &temp);
        //scanning the first letter 
        //scanning the identifier 
        while (!(ispunct(temp)) && !(isspace(temp)) && temp != '\n' && !feof(ifp)){
            array_charact[i] = temp;
            fscanf(ifp, "%c", &temp);
            i++;
        }//end of while statement 
        
        array_charact[i] = '\n';
        */

        enter_symbol(kind, charray, level, address++);
        get_token();
        numVariables++;
    } while (token == commasym); // If it is comma, keep reading variable declaration.

    //VM Machine: INC == 06 || LIT 0, M 
    assemblyCode(6, 0, address);
    //printf("6, 0, %d \n", numVariables);

    //semicolonsym = 18
    if (token != semicolonsym) {
        printf("Error number 5, semicolon missing.\n");
        error();
    }
    get_token();

}//end of varDeclaration 

void procDeclaration() {
    int kind = 3;
    //proc = 3 for the symbol table 
    do{
        get_token();
        //identsym =2
        if (token != identsym) {
            printf("Error number 4, procedure must be followed by identifier.\n");
            error();
        }
        charray = (char *) malloc(12);
        fscanf(ifp, "%s\n ", charray);
        int k = find(charray);

   
        enter_symbol(kind, array_charact, level, address);
        get_token();

        //semicolonsym = 18
        if (token != semicolonsym) {
            printf("Error number 5, semicolon missing.\n");
            error();
        }
        address++;
        get_token();
        block();

        //semicolonsym = 18 
        if (token != semicolonsym) {
            printf("Error number 5, semicolon missing.\n");
            error();
        }
        address++;
        get_token();
    } while (token == procsym); //procsym = 30 --reserved Word

}//end of procDeclaration function 

void statement() {
     
    int cx1;
    int cx2;

    //identsym = 2 
    if (token == identsym) {
        charray = (char *)malloc(12);
        fscanf( ifp, "%s\n ", charray );
        int k = find(charray);

        if (k == -1) {
            printf("Error number 11, undeclared identifier.\n");
            error();
        }
        //variable == 2
        if (symbolkind(k) != 2) {
            printf("Error number 12, Assignment to constant or procedure is not allowed.\n");
            error();
        }
 
        get_token();
        //becomessym = 20 ":="
        if (token != becomessym) {
            printf("Error number 3, Identifier must be followed by :=.\n");
            error();
        }
      
        get_token();
      
        expression();
       
        assemblyCode(4, symbollevel(k), symboladdress(k));
        //STO = 4 
        //printf("4 %d %d\n", symbollevel(k), symboladdress(k));
    }
    //beginsym = 21
    else if (token == beginsym) {
       
        address++;
        get_token();
        statement();
     
        // Whike token is semicolon, keep reading statement
        while (token == semicolonsym) {
           
            get_token();
            statement();
           
        } //end of while statement 
        
        //endsym = 22
        if (token != endsym) {
            // not listed on pdf
            printf("Error: 'End' expected.\n");
            error();
        }
    }
    //ifsym = 23
    else if (token == ifsym) {
    
        get_token();
        condition();
   
        //thensym = 24 
        if (token != thensym) {
            printf("Error number 16, then expected.\n");
            error();
        }
    
        get_token();
        int ctemp = cx; 
   
        //VM: JPC = 8 
        assemblyCode(8, 0, 0);
        statement();
        get_token();

        //elsesym = 33
        if (token == elsesym) {
            statement();
            get_token();

        }

        code[ctemp].m = cx;
    }
    //whilesym= 25
    else if (token == whilesym) {
        cx1 = cx;

        get_token();
        condition();
   
        cx2 = cx;
        //VM: JPC = 8
        assemblyCode(8, 0, 0);

        //dosym = 26 
        if (token != dosym) {
            printf("Error number 18, ‘do’ expected.\n");
            error();
        } else {
            get_token();
        }

        statement();
        //VM: JMP = 7 
        assemblyCode(7, 0, cx1);
        code[cx2].m = cx;
        get_token(); 
    }
    //writesym = 31
    else if (token == writesym) {

        get_token();
        if (token != 2) {
            printf("Error number 15, call must be followed by an identifier.\n");
            error();
        }

        charray = (char *) malloc(12);
        fscanf(ifp, "%s\n ", charray);
        int k = find(charray);
        if (k == -1) {
            printf("Error: Couldn't find the identifier in the symbol table.\n"); // not listed in pdf 
            error();
        }
        //constant
        if (symbolkind(k) == 1) {
            assemblyCode(1, symbollevel(k), symbolval(k));
            //LOD == 3 
            //printf("11 %d %d\n", symbollevel(k), symbolval(k));
        }
        // variable
        else {
            assemblyCode(3, symbollevel(k), symboladdress(k));
            //LOD == 3 
            //printf("3 %d %d\n", symbollevel(k), symbolval(k));
        }
        assemblyCode(9, symbollevel(k), 0);
        //SIO = 9 
        //printf("9 %d %d\n", symbollevel(k), symbolval(k));
        get_token();
    }
    //readsym = 32
    else if (token == readsym) {
        get_token();
        if (token != 2) {
            printf("Error number 15, call must be followed by an identifier.\n");
            error();
        }

        charray = (char *)malloc(12);
        fscanf( ifp, "%s\n ", charray ); 
        int k = find(charray);
           
        if (k == -1) {
            printf("Error: Couldn't find the identifier in the symbol table.\n"); // not listed in pcd 
            error();
        }
        //constant == 1
        if (symbolkind(k) == 1) {
            printf("Error number 12, Assignment to constant or procedure is not allowed.\n");
            error();
        }
        assemblyCode(9, 0, 1);
        //SIO = 10 
        //printf("10 0 2\n");
        assemblyCode(4, symbollevel(k), symboladdress(k));
        //LOD == 3 
        //printf("4 %d %d\n", symbollevel(k), symbolval(k));
        get_token();
    }

}//end of statement function 

void expression() {
    //VM information: 
    //OPR == 2 example: assemblyCode(OPR, 0, OPR_ADD || OPR_SUB) 
    //OPR_ADD == 2 
    //OPR_SUB == 3 
    //OPR_NEG == 1 
    int op;
    //plussym = 4 -- adding_operator

    if (token == plussym || token == minussym) {
        //minussym = 5 
        op = token;
        get_token();
        term();
        //minussym = 5
        if (op == minussym) {
            assemblyCode(2, 0, 1);
            //OPR_NEG == 1 
            //printf("1 0 1\n");
        }
    }
    else {
        term();
    }

    //plussym = 4 -- adding_operator
    while (token == plussym || token == minussym) {
        //minussym = 5 
        op = token;
        get_token();
        term();

        //plussym = 4 -- adding_operator
        if (op == plussym) {
            assemblyCode(2, 0, 2);
            //addition 
            //printf("2 0 2\n");
        } else {
            assemblyCode(2, 0, 3);
            //subtraction 
            //printf("2 0 3\n");
        }
    }
}//end of expression function 

void term() {
    //VM Information:
    //OPR = 2 example assemblyCode(OPR, 0, OPR_MUL) 
    int op;
    factor();

    //multsym = 6 --- multiplying operator || slashsym = 7 -- division operator
    while (token == multsym || token == slashsym) {
         
        op = token;
        get_token();
        factor();

        //multsym = 6 --- multiplying_operator
        if (op == multsym) {
            assemblyCode(2, 0, 4);
            //multiplication-- OPR_MUL = 4 
            //printf("2 0 4\n");
        } else {
            assemblyCode(2, 0, 5);
            //Division --- OPR_DIV = 5 
            //printf("2 0 5\n");
        }
    }
    //end of while statement
}//end of term function

void factor() {
    //identsym == 2
    if (token == identsym) {
        charray = (char *)malloc(12);
        fscanf( ifp, "%s\n ", charray );
        int k = find(charray);

        if (k == -1) {
            printf("Error: Couldn't find the identifier in the symbol table."); // not in pdf
            error();
        }

        //variable == 2
        if (symbolkind(k) == 2) {
            assemblyCode(3, symbollevel(k), symboladdress(k));
            //LOD == 3 
            //printf("3 %d %d\n", symbollevel(k), symboladdress(k));
        }
        //constant ==1
        else if (symbolkind(k) == 1) {
            assemblyCode(1, 0, symbolval(k));
            //LIT == 1 
            //printf("1 0 %d\n", symbolval(k));
        } else {
            error();
        }

        get_token();
    }
    //its a number
    else if (token == 3) {
        fscanf(ifp, "%d", &number);
        assemblyCode(1, 0, number);
        //LIT == 1 
        //printf("1 0 %d\n", number);
        get_token();
    }
    //lparentsym = 15 "("
    else if (token == lparentsym) {
        get_token();
        expression();

        //rparentsym = 16 ")"
        if (token != rparentsym) {
            printf("Error number 22, Right parenthesis missing.\n");
            error();
        }
        get_token();
    } else {
        printf("Error number 8, Incorrect symbol after statement part in block.\n");
        error();
    }
}//end of factor function 

void condition() {
    //oddsym = 8
    if (token == oddsym) {
        get_token();
        expression();
        assemblyCode(2, 0, 6);
        //OPR == 2 --- ODD == 6
        //printf("2 0 6\n");
    } else {
        expression();
        //there are six relational operators 
        //< (less than), <= (less than or equal to), > (greater than), >=, == (equal to), 
        // /= (not equal to) 
        //these numbers correspond to one of the relations above 
        if (token != 13 && token != 14 && token != 12 && token != 11 && token != 9 && token != 10) {
            printf("Error number 20, Relational operator expected.\n");
            error();
        }
        int k = find_symbol_type(token);
        get_token();
        expression();
        assemblyCode(2, 0, k);
        //OPR == 2;
        //printf("1 0 %d\n", k);
    }
}//end of condition function 

void enter_symbol(int kind, char* array, int temp, int addr) {
    symbol_table[nindex].kind = kind;
    /*int i = 0;
    while (array[i] != '\n'){
        symbol_table[nindex].name[i] = array[i];
         
        i++;
    }
    //end of while loop 
    symbol_table[nindex].name[i] = '\n';
    */

    strcpy(symbol_table[nindex].name, array);
    //then it is a const
    if (kind == 1) {
        symbol_table[nindex].val = temp;
    } else {
        symbol_table[nindex].level = temp;
        symbol_table[nindex].addr = addr;
    }
    nindex++;
}//end of enter_symbol function 

void assemblyCode(int op, int l, int m) {
    if (cx > MAX_STACK_HEIGHT) {
        printf("Error: There was a stack overflow.\n"); // not in pdf
        error();
    } else {
        code[cx].op = op;
        code[cx].l = l;
        code[cx].m = m;
        cx++;
    }
}//end of assemblyCode function 

void printCode(int arguments) {
    int i;
    for (i = 0; i < cx; i++) {
        fprintf(ofp, "%d %d %d\n", code[i].op, code[i].l, code[i].m);
    }

}//end of printCode function
 
int find(char *array) {
    int temp1 = nindex - 1;
    int found = -1;
   
    while (temp1 != 0) {
       if (strcmp(symbol_table[temp1].name, array) == 0) {
            found = temp1;
            break;
        }
        temp1--;
    }
    return found;
}

int symbolkind(int position) {
    return symbol_table[position].kind;
}
int symbollevel(int position) {
    return symbol_table[position].level;
}
int symboladdress(int position) {
    return symbol_table[position].addr;
}
int symbolval(int position) {
    return symbol_table[position].val;
}
int find_symbol_type(int symbol) {
    if (symbol == 9) {
        //EQL == 8
        return 8;
    } else if(symbol == 10) {
        //NEQ == 9 '!='
        return 9;
    } else if (symbol == 11) {
        //LSS == 10 '<'
        return 10;
    } else if (symbol == 12) {
        //leq "<="
        return 11;
    } else if (symbol == 13) {
        return 12;
    } else {
        return 13;
    }
}
