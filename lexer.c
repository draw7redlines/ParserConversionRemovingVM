/* Team Name: Compiler Builder 4 
* Melissa Gramajo 
* Rebeca Amaya 
* Mark Veto 
* Viviene Do 
* Kohei Arai */ 

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <ctype.h> 
#include "masterHeader.h"

FILE *input;
FILE *ofp3;

void printAndCleanCode(char* inputFile, char* outputFile){
	FILE *output333;
	// open the inupt file 
	input = fopen(inputFile, "r");
	if (input == NULL) {
		printf("[ERROR] Could not open file: %s\n", inputFile);
		return;
	}
    
	// open the outputfile 
	output333 = fopen(outputFile, "w");
	if(output333 == NULL) {
		printf("[ERROR] Could not open file: %s\n", outputFile);
		return;
	}
    
	//read in the input and take out all comments
	char c_0 = ' ';
	char c_1 = getc(input);
	while(c_1 != EOF) {
		c_0 = c_1;
		c_1 = getc(input);
		if(c_0 == '/' && c_1 == '*') {
			fputc(' ', output333);
			fputc(' ', output333);
			//we want to move after the "/*"
			c_0 = getc(input);
			c_1 = getc(input);
			// Write space for that comment
			fputc(' ', output333);
			fputc(' ', output333);
			//move to the end of the comment
			while(c_0 != '*' && c_1 != '/') {
				//move along by character
				c_0 = c_1;
				c_1 = getc(input);
				// Write space for that comment
				fputc(' ', output333);
		}
            
		//we want to move after the "*/" 
		c_0 = getc(input);
		c_1 = getc(input);
	}
		//c_0 is not the EOF and is not a comment so write it in the output file 
		fputc(c_0, output333);
	}

	//close both files once your done reading/writing it. 
	fclose(input);
	fclose(output333);
}//end of clean code() 

int find_token(char* array_charact) {
	int token;
	if (strcmp(array_charact, "var") == 0){
		token = varsym;
	}
    
	else if (strcmp(array_charact, "const") == 0){
		token = constsym;
    }
    
	else if (strcmp(array_charact, "procedure") == 0){
		token = procsym;
	}
    
	else if (strcmp(array_charact, "call") == 0){
		token = callsym;
	}
    
	else if (strcmp(array_charact, "begin") == 0){
		token = beginsym;
	}
    
	else if (strcmp(array_charact, "end") == 0){
		token = endsym;
	}
    
	else if (strcmp(array_charact, "if") == 0){
		token = ifsym;
	}
    
	else if (strcmp(array_charact, "then") == 0){
		token = thensym;
	}
    
	else if (strcmp(array_charact, "else") == 0){
		token = elsesym;
	}
    
	else if (strcmp(array_charact, "while") == 0){
		token = whilesym;
	}
    
	else if (strcmp(array_charact, "do") == 0){
		token = dosym;
	}
    
	else if (strcmp(array_charact, "read") == 0){
        token = readsym;
    }
    
    else if (strcmp(array_charact, "write") == 0){
        token = writesym;
    }
    
    else if (strcmp(array_charact, "odd") == 0){
        token = oddsym;
    }
    
    //else its an identifier 
	else{
        token = 2;
    }
    return token;
}//end of find_token function 

int find_symbol(char symbol) {
    int token;
    switch (symbol) {
        case '+': token = plussym;
        break;
        case ',': token = commasym;
        break;
        case '*': token = multsym;
        break;
        case '/': token = slashsym;
        break;
        case '(': token = lparentsym;
        break;
        case ')': token = rparentsym;
        break;
        case '=': token = eqsym;
        break;
        case '.': token = periodsym;
        break;
        case '<': token = lessym;
        break;
        case '>': token = gtrsym;
        break;
        case ';': token = semicolonsym;
        break;
        case '-': token = minussym;
        break;
        case ':': token = 0;
        break;
        default: //in case this is a invalid symbol 
		token = -1;
    }
    
    //end of switch statement 
	return token;
}//end of find_symbol function 

int lexiFunction(char* inputFile, int lInArg){
    // open the inupt file 
	input = fopen(SOURCE_WITHOUT_COMMENT, "r");
    //open the file: 
	ofp3 = fopen("lexemelist.txt", "w");
    if (input == NULL) {
        printf("[ERROR] Could not open file: %s\n", inputFile);
        return 1;
    }
    
    char array_charact[MAX_STR_LENGTH + 1];
    char numbers[NUM_ARRAY_SIZE + 1];
    char temp;
    int token;
    int i;
    int flag = 0;
    for(; ;) {
        fscanf(input, "%c", &temp);
        //must check after getting next char 
		if(feof(input)) {
            break;
        }
        
        flag = 0;
        /*checks to see if it is a digit*/ 
		/*If so then it will save it into an array*/ 
		//------------------------------------------------------------------------------------------------ 
		if (isdigit(temp)) {
            i = 0;
            while (!(ispunct(temp)) && !(isspace(temp)) && temp != 'n' && !feof(input)) {
                if (i >= NUM_ARRAY_SIZE) {
                    printf("Error: The number is too long!\n"); // not on pdf
                    //do not continue in loop, set a brak 
					flag = 1;
                    return 1;
                }
                
                else {
                    if (isalpha(temp)){
                        printf("Error: A variable cannot start with a number!\n");
                        //do not continue in loop, set a break 
						flag = 1;
                        return 1;
                    }
                    
                    else{
                        //continue to store the number in number aray 
						numbers[i] = temp;
                        //scan next char to retrieve the full number 
						fscanf(input, "%c", &temp);
                        //increment i to check how big the number is 
						i++;
                        
                    }
                }
            }
            
            //end of while loop 
			// Check if the number is greater than 2^16 - 1 
			if (i == NUM_ARRAY_SIZE) {
                int result = 0, j = 0;
                for (j = 0; j < NUM_ARRAY_SIZE; j++) {
                    result = result * 10 + (numbers[j] - '0');
                }
                
                if (result > MAX_NUM) {
                    flag = 1;
                    printf("Error number 25, This number is too large.\n");
                    return 1;
                }
            }
            if (flag == 1) {
                return 1;
            }
            
            //going back one cursor 
			if (!feof(input)){
                fseek(input, -1, SEEK_CUR);
            }
            
            //reset num array
	    numbers[i] = '\0';
            //print to screen
	    // printf("%s\t %d\n", numbers, numbersym);
            fprintf(ofp3, "%d %s ", numbersym, numbers);
        }
        
        /*Check to see if the char is a letter*/
		//-------------------------------------------------------------------------------------------------
		else if (isalpha(temp)) {
            i = 0;
            while (!(ispunct(temp)) && !(isspace(temp)) && temp != '\n' && !feof(input)){
                //make sure we did not exceed out variable size
				if (i >= MAX_STR_LENGTH){
                    //printf("Error: The identity name is too long!\n");
                    //we want to stop and break out of the loop
		    flag = 1;
                    break;
                }
                
                else {
                    //continue to store the variable name in the array
		    array_charact[i] = temp;
                    //scan in the next character
		    fscanf(input, "%c", &temp);
                    //keep track of how long the variable name is
		    i++;
                }
            }
            
            //end of while statement
	    if (flag == 1) {
                break;
            }
            
            //going back one cursor
	    if (!feof(input)){
                fseek(input, -1, SEEK_CUR);
            }
            
            //clear the array for next time
		array_charact[i] = '\0';
            //categorize the token
		token = find_token(array_charact);
            //printf("%s\t %d\n", array_charact, token);
            if (token == 2){
                fprintf(ofp3, "%d %s ", token, array_charact);
            }
            
            else{
                fprintf(ofp3, "%d ", token);
            }
        }
        
        /*Check here for punctuation*/ 
		//-------------------------------------------------------------------------------------------- 
		else if (ispunct(temp)) {
            token = find_symbol(temp);
            if (token == 0) {
           //then the char was ':', therefore it needs to check next symbol to 
				//see whether it is a '=' 
				fscanf(input, "%c", &temp);
                if (temp == '=') {
                    //20 is the default token for ':=' 
					//printf("%s\t %d\n",":=", becomessym);
                    fprintf(ofp3, "%d ", 20);
                }
                
                //what came after ':' was invalid 
				else {
                    //bring the cursor back one and state that it was an invalid symbol 
					//printf("%c is an invalid symbol\n", ':');
                    break;
                }
            }
            
            else if (token == -1) {
                //this was an invalid symbol
				//printf("%c is an invalid symbol\n", temp);
                flag = 1;
                break;
            }
            
            else if (token == 11) {
                fscanf(input, "%c", &temp);
                if (temp == '>'){
                    token = neqsym;
                    //printf("%s\t %d\n", "<>", token);
                    fprintf(ofp3, "%d ", token);
                }
                
                else if (temp == '='){
                    token = leqsym;
                    //printf("%s\t %d\n", "<=", token);
                    fprintf(ofp3, "%d ", token);
                }
                
                else{
                    fseek(input, -1, SEEK_CUR);
                    //printf("%ct %d\n", '<', token);
                    fprintf(ofp3, "%d ", token);
                }
            }
            
            else if (token == 13){
                fscanf(input, "%c", &temp);
                if (temp == '='){
                    token = geqsym;
                    //printf("%s\t %d\n", ">=", token);
                    fprintf(ofp3, "%d ", token);
                }
                
                else{
                    fseek(input, -1, SEEK_CUR);
                    //printf("%c\t %d\n", '>', token);
                    fprintf(ofp3, "%d ", token);
                }
            }
            
            else {
                //printf("%c\t %d\n", temp, token);
                fprintf(ofp3, "%d ", token);
            }
            
            if (flag == 1) {
                printf("Error number 24, An expression cannot begin with this symbol.\n"); // generalized
                //need to break out of the loop since there was an invalid symbol
	break;
            }
        }else{
        
        /*This is for any white space or next line, etc.*/
            //this is a white space that should be ignored!
		}
    }
    
    //end of while statement
	fclose(input);
    fclose(ofp3);
    
    if(lInArg == 1){
		ofp3 = fopen("lexemelist.txt", "r");
		
		printf("\n---------------------------------\n");
		printf("\tLexemelist:\n");
		printf("---------------------------------\n");

    	// Read contents from file
    	char c = fgetc(ofp3);
    	while (c != EOF)
    	{
        	printf ("%c", c);
			c = fgetc(ofp3);
    	}
		printf("\n\n");
 
    	fclose(ofp3);

        return 0;
    }
    if(flag == 1)
        return 1;
    else
        return 0;
}//end of lexiFunction() 

void printFile(char* inputFile, int shouldPrintClean) {
    // open the inupt file 
	input = fopen(inputFile, "r");
    if (input == NULL) {
        printf("[ERROR] Could not open file: %s\n", inputFile);
        return;
    }
    
    if (shouldPrintClean) {
        printf("source code without comments:\n");
        printf("-----------------------------\n");
    }
    
    else {
        printf("source code:\n");
        printf("------------\n");
    }
    
    // Print file content
	char c = getc(input);
    while (c != EOF) {
        printf("%c", c);
        c = getc(input);
    }
    
    // Close the file
	fclose(input);
    printf("\n");
    
}

int lex(int argc, char ** argv) {
    // Get parameter
    int shouldPrintSource = 0;
    int shouldPrintCleanSource = 0, i = 0;
    for (i = 1; i < argc; i++) {
        if (strcmp("--source", argv[i]) == 0) {
            shouldPrintSource = 1;
        }
        
        if (strcmp("--clean", argv[i]) == 0) {
            shouldPrintCleanSource = 1;
        }
    }
    
    // Get input file. Wondering what should happen if user types ./a.out --source
	// I think this should print out input.pl0 with source code, but don't come up with clean way to do it.
	// This problem is solved if we decided not to care such command.
	char inputFile[100];
    if (argc > 1) {
        strcpy(inputFile, argv[1]);
    }
    
    else {
        strcpy(inputFile, DEFAULT_INPUT_FILE);
    }
    
    //read in input file and clean the code:
	printAndCleanCode(inputFile, SOURCE_WITHOUT_COMMENT);
    if (shouldPrintSource) {
        printFile(inputFile, 0);
    }
    
    if (shouldPrintCleanSource) {
        printFile(SOURCE_WITHOUT_COMMENT, 1);
    }
    
    // Print the token header
	//printf("tokens:\n");
    //printf("------\n");
    //run lexi
//	lexiFunction(SOURCE_WITHOUT_COMMENT);
    return 0;
} //end of main()
