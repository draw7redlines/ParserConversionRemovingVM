/* Team Name: Compiler Builder 4
 * Melissa Gramajo
 * Rebeca Amaya
 * Mark Veto
 * Viviene Do
 * Kohei Arai
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "masterHeader.h"

/**
 * Allocate the memory needed for a virtual machine and initialize the values
 * @param vm: virtual machine
 */
void initializeVM(pmachine * vm)
{
    int x=0;

    // Allocation of the stack
    vm->stack = malloc(sizeof(int)*MAX_STACK_HEIGHT);
    // Allocation of the code
    vm->code = malloc(sizeof(instruction)*MAX_CODE_LENGTH);

    // Initial values for PM/0 CPU registers:
    vm->sp = 0;
    vm->bp = 1;
    vm->pc = 0;
    vm->ir.op = 0;
    vm->ir.l = 0;
    vm->ir.m = 0;

    vm->halt = FALSE;

    // Initial stack store values:
    /*vm->stack[1] = 0;
    vm->stack[2] =0;
    vm->stack[3] =0;
    */


    //Stack needs to be completely zeroed out here in this loop
    //otherwise the results will go bad in the execution phase of the code
    for(x = 0; x<=MAX_STACK_HEIGHT; x++)
    {
        vm->stack[x] = 0;
    }

}//end of initializeVM()

/**
 * Load code from file
 * @param vm: virtual machine
 * @param fileName: name of the file
 */
int loadCode(pmachine *vm, char *fileName)
{

    FILE * pFile;
    int i = 0;
    vm->pc = 0;

    int result = 0;

    // Open the file
    pFile = fopen(fileName,"r");

    // If the file is opened successfully
    if (pFile!=NULL)
    {
        // Read of the file until the end
        while(!feof(pFile))
        {
            // Scan in the op, l, m and store it in vm.code
            fscanf(pFile, "%d %d %d", &(vm->code[i].op), &(vm->code[i].l), &(vm->code[i].m));

            // Increment the position at vm.code
            i++;
        }
        // Close the file
        fclose (pFile);

result = 1;
    } else {
        printf("Could not open file: %s\n", fileName);
    }

    // Store the instruction count for printing purposes
    vm->instrCount = i;

    return result;
}//end of load code()


/**
 * Frees the memory used by a virtual machine
 * @param vm: virtual machine
 */
void freeVM(pmachine * vm)
{

    free(vm->stack);
    free(vm->code);
}//end of freeVM()

/**
 * Find the base pointer of a previous level (RECURSIVELY)
 * @param vm: virtual machine
 * @param bp: current base pointer
 * @param level: requested level
 * @return base pointer of the requested level
 */
int base(pmachine * vm, int bp, int level)
{
    // printf("base level = %d\n", level);
        //base case
        if(level == 0)
        {
                return bp;
        }

        //recursive call
        base(vm, vm->stack[bp+1], level - 1);

        return 0;
}




/**
 * Execute an OPR instruction
 * @param vm: virtual machine
 * @param m: OPR code
 */
void opr(pmachine * vm, int m)
{

    switch(m)
    {
    case 0://RET
                vm->sp = vm->bp - 1;
            vm->pc = vm->stack[vm->sp + 4];
            vm->bp = vm->stack[vm->sp + 3];
        if (vm->sp < 1)
        {
            vm->halt = TRUE;
        }
        break;
    case 1: //NEG
        vm->stack[vm->sp] = -(vm->stack[vm->sp]);
        break;
    case 2://ADD
        (vm->sp)--;
        vm->stack[vm->sp] = vm->stack[vm->sp] + vm->stack[vm->sp+1];
        break;
    case 3://SUB
        (vm->sp)--;
        vm->stack[vm->sp] = vm->stack[vm->sp] - vm->stack[vm->sp+1];
        break;
    case 4: //MUL
        (vm->sp)--;
        vm->stack[vm->sp] = vm->stack[vm->sp] * vm->stack[vm->sp+1];
        break;
    case 5: //DIV
        (vm->sp)--;
        vm->stack[vm->sp] = vm->stack[vm->sp] / vm->stack[vm->sp+1];
        break;
    case 6: //ODD
        vm->stack[vm->sp] = vm->stack[vm->sp] % 2;
        break;
    case 7: //MOD
        (vm->sp)--;
        vm->stack[vm->sp] = vm->stack[vm->sp] % vm->stack[vm->sp+1];
        break;
    case 8: //EQL
        (vm->sp)--;
        if(vm->stack[vm->sp] == vm->stack[vm->sp+1])
            vm->stack[vm->sp] = 1;
        else
            vm->stack[vm->sp] = 0;
        break;
    case 9: //NEQ
        (vm->sp)--;
        if(vm->stack[vm->sp] != vm->stack[vm->sp+1])
            vm->stack[vm->sp] = 1;
        else
            vm->stack[vm->sp] = 0;
        break;
    case 10: //LSS
        (vm->sp)--;
        if(vm->stack[vm->sp] < vm->stack[vm->sp+1])
            vm->stack[vm->sp] = 1;
        else
            vm->stack[vm->sp] = 0;
        break;
    case 11://LEQ
        (vm->sp)--;
        if(vm->stack[vm->sp] <= vm->stack[vm->sp+1])
            vm->stack[vm->sp] = 1;
        else
            vm->stack[vm->sp] = 0;
        break;
    case 12: //GTR
        (vm->sp)--;
        if(vm->stack[vm->sp] > vm->stack[vm->sp+1])
            vm->stack[vm->sp] = 1;
        else
            vm->stack[vm->sp] = 0;
        break;
    case 13://GEQ
        (vm->sp)--;
        if(vm->stack[vm->sp] >= vm->stack[vm->sp+1])
            vm->stack[vm->sp] = 1;
        else
            vm->stack[vm->sp] = 0;
        break;

    default:
        break;
    }//end of switch
}//end of opr()

/**
 * Decode and execute the instruction in the ir register of a virtual machine
 * @param vm: virtual machine
 */
void decodeInstruction(pmachine * vm)
{
    // printf("call decodeInstruction. vm->ir.op = %d\n", vm->ir.op);

    //by user's input, determine each MIPS instruction
    switch(vm->ir.op)
    {
    case 1: // LIT (push constant value(literal) M onto the stack)
        vm->sp++; //sp <- sp + 1
        vm->stack[vm->sp] = vm->ir.m; //stack[sp] <- M
        break;

    case 2: //OPR
        // printf("Call opr\n");
        opr(vm, vm->ir.m);
        break;
    case 3://LOD
        vm->sp = vm->sp + 1;
        vm->stack[vm->sp] = vm->stack[base(vm, vm->bp, vm->ir.l) + vm->ir.m];
        break;
    case 4://STO
        vm->stack[base(vm, vm->bp, vm->ir.l) + vm->ir.m] = vm->stack[vm->sp];
        vm->sp = vm->sp - 1;
        break;
    case 5://CAL
        vm->stack[vm->sp+1] = 0;
        vm->stack[vm->sp+2] = base(vm, vm->bp, vm->ir.l); //static link
        vm->stack[vm->sp+3] = vm->bp;
        vm->stack[vm->sp+4] = vm->pc;
        vm->bp = vm->sp + 1;
        vm->pc = vm->ir.m;
        break;
    case 6: //INC (increment)
        vm->sp += vm->ir.m;
        break;
    case 7://JMP
        vm->pc = vm->ir.m;
        break;
    case 8://JPC
        if(vm->stack[vm->sp] == 0)
        {
            vm->pc = vm->ir.m;
        }
        vm->sp--;
        break;



    case 9: //SIO cases
        // printf("vm->ir.m = %d\n", vm->ir.m);
            if(vm->ir.m==0)
            {
                printf("%d\n", vm->stack[vm->sp]);
                vm->sp--;
            }

            if(vm->ir.m==1)
            {
                vm->sp++;
                // printf("pre scanf\n");
                // printf("vm->stack[vm->sp] = %d\n", vm->stack[vm->sp]);
                scanf("%d",&(vm->stack[vm->sp]));
                // printf("post scanf\n");
            }

            if(vm->ir.m==2)
            {
                vm->halt = TRUE;
            }
        break;
    case 11:
		if(vm->ir.m == 3){
			vm->halt = TRUE;
		}
    default:
        break;
    }//end of switch
    // printf("finish decodeInstruction\n");

}//end of decodeInstruction()



/**
 * Fetch the next instruction and store it in the ir register
 * @param vm: virtual machine
 */
void fetch(pmachine * vm)
{

    vm->ir = vm->code[vm->pc];

}

/**
 * Execute the instruction in the ir register
 * @param vm: virtual machine
 */
void step(pmachine * vm)
{

    // Increment PC
    vm->pc++;
    // Execute
    decodeInstruction(vm);
}

/**
 * Convert an instruction code to a string
 * @param op: instruction code
 * @param str: result string (at least of size 3)
 */
void opToStr(int op, char  * str)
{

    switch(op)
    {
        case 1:
            strcpy(str, "LIT");
            break;
        case 2:
            strcpy(str, "OPR");
            break;
        case 3:
            strcpy(str, "LOD");
            break;
        case 4:
            strcpy(str, "STO");
            break;
        case 5:
            strcpy(str, "CAL");
            break;
        case 6:
            strcpy(str, "INC");
            break;
        case 7:
            strcpy(str, "JMP");
            break;
        case 8:
            strcpy(str, "JPC");
            break;
        case 9:
            strcpy(str, "SIO");
            break;
        case 10:
            strcpy(str, "SIO");
            break;
        case 11:
            strcpy(str, "SIO");
            break;
        default:
            strcpy(str, "???");
            break;
    }//end of switch
}//end of opToStr()

/**
 * Print a given instruction in the trace
 * @param ofp: trace filee
 * @param vm: virtual machine
 * @param instr: instruction
 */
void print_function(FILE *ofp, int temp, pmachine * vm, instruction instr){

    char str[100];

    switch(instr.op){
    case 1:
        fprintf(ofp, "%3d  LIT     %5d  ", temp, instr.m);
        //printf("%3d  LIT     %5d  ", temp, instr.m);
        break;

    case 2:
            switch(instr.m)
            {
                case 0://RET
                    strcpy(str, "RET");
                    break;
                case 1: //NEG
                    strcpy(str, "NEG");
                    break;
                case 2://ADD
                    strcpy(str, "ADD");
                    break;
                case 3://SUB
                    strcpy(str, "SUB");
                    break;
                case 4: //MUL
                    strcpy(str, "MUL");
                    break;
                case 5: //DIV
                    strcpy(str, "DIV");
                    break;
                case 6: //ODD
                    strcpy(str, "ODD");
                    break;
                case 7: //MOD
                    strcpy(str, "MOD");
                    break;
                case 8: //EQL
                    strcpy(str, "EQL");
                    break;
                case 9: //NEQ
                    strcpy(str, "NEQ");
                    break;
                case 10: //LSS
                    strcpy(str, "LSS");
                    break;
                case 11://LEQ
                    strcpy(str, "LEQ");
                    break;
                case 12: //GTR
                    strcpy(str, "GTR");
                    break;
                case 13://GEQ
                    strcpy(str, "GEQ");
                    break;
                default:
                    strcpy(str, "???");
                    break;

            }


                fprintf(ofp, "%3d  %s            ", temp, str);
                //printf("%3d  %s            ", temp, str);

            /*char ret[100];
            strcpy(ret, "RET");
            if (strcmp(str, ret) == 0)
                {
                fprintf(ofp, "%3d  %s            ", temp, str);
                printf("%3d  %s            ", temp, str);
                }
            else {
                fprintf(ofp, "%3d  %s     %5d  ", temp, str, instr.m);
                printf( "%3d  %s     %5d  ", temp, str, instr.m);
                }*/


        break;

    case 3:
        fprintf(ofp, "%3d  LOD%5d%5d  ", temp, instr.l, instr.m);
        //printf( "%3d  LOD%5d%5d  ", temp, instr.l, instr.m);
        break;

    case 4:
        fprintf(ofp, "%3d  STO%5d%5d  ", temp, instr.l, instr.m);
        //printf( "%3d  STO%5d%5d  ", temp, instr.l, instr.m);
        break;

    case 5:
        fprintf(ofp, "%3d  CAL%5d%5d  ", temp, instr.l, instr.m);
        //printf("%3d  CAL%5d%5d  ", temp, instr.l, instr.m);
        break;

    case 6:
        fprintf(ofp, "%3d  INC     %5d  ", temp, instr.m);
        //printf("%3d  INC     %5d  ", temp, instr.m);
        break;

    case 7:
        fprintf(ofp, "%3d  JMP     %5d  ", temp, instr.m);
        //printf("%3d  JMP     %5d  ", temp, instr.m);
        break;

    case 8:
        fprintf(ofp, "%3d  JPC     %5d  ", temp, instr.m);
       //printf("%3d  JPC     %5d  ", temp, instr.m);
        break;

    case 9:

        //no halt means we leave M alone
        if(instr.m==0)
        {
            fprintf(ofp, "%3d  OUT        ", temp);
           // printf("%3d  OUT        ", temp);
        }

        if(instr.m==1)
        {
            fprintf(ofp, "%3d  INP        ", temp);
            //printf("%3d  INP        ", temp);

        }

        //if it's a halt, then report M as zero
        else if(instr.m==2)
        {
            fprintf(ofp, "%3d  HLT            ", temp);
            //printf("%3d  HLT            ", temp);
        }
        break;

    case 10:
        fprintf(ofp, "%3d  SIO     %5d  ", temp, instr.m);
       //printf("%3d  SIO     %5d  ", temp, instr.m);
        break;

    case 11:
	fprintf(ofp, "%3d  SIO     %5d  ", temp, instr.m);
        //printf("%3d  SIO     %5d  ", temp, instr.m);
        break;
    }

}//end of print_function()


/**
 * Print the code section of a virtual machine in the trace
 * @param ofp: trace file
 * @param vm: virtual machine
 */
void printCodeInstrCount(FILE * ofp, pmachine * vm)
{

        int i = 0;

        for(i = 0; i < vm->instrCount; i++)
        {
instruction currentInstruction = vm->code[i];
print_function(ofp, i, vm, currentInstruction);

        fprintf(ofp, "\n");
       //printf("\n");
        }

}//end of printCode()

/**
 * Initializes the trace output
 * @param ofp: trace file
 * @param vm: virtual machine
 */
void printExecutionInitialization(FILE * ofp, pmachine * vm)
{
fprintf(ofp, "\nExecution:\n");
//printf("\nExecution:\n");
    fprintf(ofp, "                      pc   bp   sp   stack\n");
    //printf("                      pc   bp   sp   stack\n");
    fprintf(ofp, "                   %5d%5d%5d   \n", vm->pc, vm->bp, vm->sp);
    //printf( "                   %5d%5d%5d   \n", vm->pc, vm->bp, vm->sp);
}//end of printExecutionInitialization()

/**
 * Determines if a current position in the stack is at the border between two frames
 * (example: if we need a pipe in the output)
 * @param vm: the virtual machine
 * @param i: current index in the stack
 * @return: TRUE if a pipe should be present, FALSE otherwise
 */
int isPipe(pmachine * vm, int i)
{

    int curr_bp = vm->bp;
    while(curr_bp > 2)
    {
        // If we are above the current BP, we are in a frame
        if (i >= curr_bp)
        {
            return FALSE;
        }
        // If we are at the location below the current BP, we need a pipe
        if (i == curr_bp - 1)
        {
            return TRUE;
        }
        // Otherwise, we are in a stack below, we update the BP
        if (curr_bp == 12)
        {

        }
        curr_bp = vm->stack[curr_bp +2];
    }

    return FALSE;
}//end of isPipe()


/**
 * Print the current stack of the machine
 * @param ofp: trace file
 * @param vm: virtual machine
 */
void printStack(FILE * ofp, pmachine * vm)
{

    int i;
    for (i = 1; i <= vm->sp; ++i)
    {
        fprintf(ofp, "%d ", vm->stack[i]);
        //printf("%d ", vm->stack[i]);
        if (i < vm->sp && isPipe(vm, i))
        {
            fprintf(ofp, "| ");
	 //printf("| ");
        }
    }
}//end of printStack()


/**
 * Print the current status of the machine
 * @param ofp: trace file
 * @param vm: virtual machine
 */

 //correct last two call lines here with catch cases
void printExecutionStatus(FILE * ofp, pmachine * vm)
{

    instruction i = vm->ir;
    char instr_name[4] = {' ', ' ', ' ','\0'};
    opToStr(i.op, instr_name);
    fprintf(ofp,"%4d%5d%5d   ", vm->pc, vm->bp, vm->sp);
    //printf("%4d%5d%5d   ", vm->pc, vm->bp, vm->sp);
    printStack(ofp, vm);

    //do not print new line if halting
    if(vm->halt == FALSE){
    	fprintf(ofp, "\n");
    	//printf( "\n");
    }
}//end of printExecutionSatatus()

int vm(char* assemblyCode)
{

    pmachine vm; // Our vm
    FILE *ofp; // The output file
    ofp = fopen("trace.txt", "w");
    
    fprintf(ofp, "PL/0 code:\n\n");
    
    // Initialization of the virtual machine
    initializeVM(&vm);

	int fileOpenResult;

    // Loading of the code from the input file
    fileOpenResult = loadCode(&vm, assemblyCode);

    //if failed to open, do not run the program
    if (fileOpenResult == 0) {
        return 0;
    }

    // Printing of the code in the output file
    printCodeInstrCount(ofp, &vm);

    // Beginning of the printing of the execution trace
    printExecutionInitialization(ofp, &vm);

    // Running loop
    // The machine runs until it halts
    // Every step consists in fetching the next instruction, running it
    // and printing the result in the trace
    while(vm.halt != TRUE)
    {
        // printf("Gonna call fetch\n");
         // Fetching the next instruction
        fetch(&vm);
        // printf("Gonna call print_function\n");
        // Printing the current instruction in the trace
        print_function(ofp, vm.pc, &vm, vm.code[vm.pc]);
        // Executing the instruction
        // printf("Gonna call step\n");
        step(&vm);
        // Printing the status of the vm in the trace
        if (vm.halt == FALSE)
        {
            // printf("Gonna call print execution status\n");
            printExecutionStatus(ofp, &vm);
        }
        // printf("doesn't halt\n");
    }

    printExecutionStatus(ofp, &vm);

    // Closing the trace file
    fclose(ofp);

    // Freeing the memory used by the virtual machine
    freeVM(&vm);
	/*	
    if(vInArg == 1){
		ofp = fopen("trace.txt", "r");

		printf("---------------------------------\n");
		printf("\tStack trace.\n");
		printf("---------------------------------\n");

    	// Read contents from file
    	char c = fgetc(ofp);
    	while (c != EOF)
    	{
        	printf ("%c", c);
			c = fgetc(ofp);
    	}
		printf("\n\n");
 
    	fclose(ofp);
    }
	*/
    return 0;
}//end of main()
