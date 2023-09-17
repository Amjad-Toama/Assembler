#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "constants.h"
#include "utilities.h"
#include "datastructure.h"
#include "firstpass.h"
#include "secondpass.h"
#include "output.h"

/* The program simulate the assembler behavior, in other words the program reads assembley language code, 
   In case the assembly code is true (no errors detected) the program export three different output files: 
   1) ".ob" file: The machine code in hexadecimal consist with code segment and then data segment.
   2) ".en" file: The declaration of entry symbols and its value line by line (The file export in case symbol declared).
   3) ".ex" file: The Apperance of the extern symbol in the code. (The file export in case symbol declared).
   otherwise the function print apropriate message in case error detected (Syntax, Declaration, AddressingModes, Range). */


int main(int argc, char **argv){
	FILE *inFile;
	char *suffix;
	
	/* Check if no file specify at command line. */
	/* if there is no file specified, print apropriate message for user */
	if(argc == 1){
		printf("\n%s: you need to specify at least one input file\n", *argv);
		return -1;
	}
	
	/* Check every specified file. */
	while(--argc) {
		/* Define local variables. */
		InstructionImage machinecode[MAXSTACK];
		SymbolNode* SThead = NULL;
		int dataimage[MAXSTACK], IC = FIRSTADD, DC = 0;
		
		/* Check if the file format is supported. */
		if((suffix = strrchr(*++argv, '.'))){
			if(strcmp(suffix, ".as")){
				printf("\"%s\" file format doesn't support.\n", *argv);
				continue;
			}
		}
		
		/* Check if the file opening succeed */
		if ((inFile = fopen(*argv, "r")) == NULL) {
			printf("Cannot open \"%s\" file for read.\n", *argv);
			continue;
        	}
        	
        	/* Sanity check if the file is empty. */
        	if(isEmptyFile(inFile)){
        		printf("\"%s\" file is empty.\n", *argv);
			continue;
        	}
        	
        	/* Check if at first pass exist syntax error. */
		if(firstPass(inFile, *argv , &SThead, machinecode, dataimage, &IC, &DC)){
			fclose(inFile);	/* Close the input file. */
			freeList(SThead);	/* Free the Symbol List */
			continue;
		}
		
		/* Set the file pointer at the beginning. */
		rewind(inFile);

		/* Check if a second pass detected error */
		if(secondpass(inFile, *argv, SThead, machinecode, dataimage, IC)){
			fclose(inFile);	/* Close the input file. */
			freeList(SThead);	/* Free the Symbol List */
			continue;
		}		
		
		/* Print the machine code and entry file. */
		printToFile(*argv, machinecode, dataimage, IC, DC, SThead);
		
		fclose(inFile);	/* Close the input file. */
		freeList(SThead);	/* Free the Symbol List */
		
	}
	
	return 0;
}
