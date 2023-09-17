#include "secondpass.h"

/* The function get as parameter input file pointer (inFile) and its name (filename), Symbol table head,
   machinecode and dataimage arrays and IC (Instrucion counter).
   the funtion read from file line by line in order either to complete the missing values after the first pass or to update the attribute in the Symbol Table.
   further more in case no error detected the function create file.ex which contain all the external symbols.
   The function return the amount of errors detect and display error message.
*/
int secondpass(FILE *inFile, char *filename, SymbolNode *head, InstructionImage *machinecode, int *dataimage, int IC){
	/* line   - to store the current line from the file.
	   field  - to store the current word from the line.
	   errorcount - error counter.
	   mci - (Machine Code Index). 
	   linecount  - line counter.
	   i - index to line. 
	*/
	char line[MAXLINE], field[MAXLINE];
	int linecount = 0, errorcount = 0, mci = 100, i, filesize;
	/* Create .ex output file. */
	FILE *EXoutFile = fopen(buildOutFname(filename, ".ex"), "w");
	/*Check if the file open.*/
	/* In every interaction the loop read new line from file till end. */
	while(fgets(line, MAXLINE, inFile) != NULL){
		i = 0;
		linecount++;
		/* terminate the line '\n' with '\0' */
		line[strlen(line)-1] = '\0';
		
		/* Check if the line is comment or empty. if so move to the next line. */
		if(isEmpty(line)) 
			continue;
			
		/* Read the first word from line. */
		readword(line, field, &i);
		
		/* Check if the first word represent label definition. if so read next word from the line. */
		if(isLabel(field))
			readword(line, field, &i);
			
		/* In case the current field one of these directive skip this line.
		   Note: this line processed in the previous pass. */
		if(!strcmp(field,".data")  || !strcmp(field,".string") || !strcmp(field,".extern"))
			continue;
			
		/* In case the line is .entry directive update the Symbol Table. */
		else if (!strcmp(field,".entry")){
			if(!ParseEntry(filename, linecount, line, &i, head))
				errorcount++;
		}
		/* Otherwise the line is instruction. In needed case complete the machine code. */
		else{
			/* srcmode - The source mode in the current machine code.
			   destmode - The destination mode in the current machine code.
			   indexI - Instruction index from instructions array.
			   ici - (Instruction Code Index) contain the instuction code index in the machinecode.	   
			*/
			int srcmode, destmode, indexI = isInstruction(field), commaflag = OFF, ici = mci;
			/* copy the 2 lowest bits (destination) */
			destmode = machinecode[mci].code & 3;
			/* copy the 4 lowest bits (source) then shift right. */
			srcmode = machinecode[mci].code & 15;
			srcmode >>= 2;
			
			mci++;
			
			/* In every interaction read every operand (Not related to it's addressing mode),
			   in order to keep line index synchronized with machine code index. */
			
			/* Check if there is source operand exist. */
			if(instructions[indexI].src){
				/* read the next field (operand) */
				readoperand(line, field, &i, &commaflag);
				
				/* Complete the machine code in case needed, the function return FALSE only in case the symbol isn't defined. */
				if(!CompletionMachineCode(field, &commaflag, IC, machinecode, mci, srcmode, head, EXoutFile)){
					/* Print proper error message then then update mci to the next instruction machine code in the array. */
					errorMsg(filename, field, linecount, INVALIDOPERAND6);
					errorcount++;
					mci = ici + instructions[indexI].operandnum + 1;
					continue;
				}
				mci++;
			}
			/* In case no comma detected find it "ins src  ,  dest"
			   The comma existance guaranteed from first pass. */
			if(!commaflag) findcomma(line, &i);
			
			/* Check if there is destination operand exist. */
			if(instructions[indexI].dest){
				/* read the next field (operand) */
				readoperand(line, field, &i, &commaflag);
				
				/* Complete the machine code in case needed, the function return FALSE only in case the symbol isn't defined. */
				if(!CompletionMachineCode(field, &commaflag, IC, machinecode, mci, destmode, head, EXoutFile)){
					/* Print proper error message then then update mci to the next instruction machine code in the array. */
					errorMsg(filename, field, linecount, INVALIDOPERAND7);
					errorcount++;
					mci = ici + instructions[indexI].operandnum + 1;
					continue;
				}
				mci++;
			}
		}	
	}
	
	/* In case error detected remove the .ex file. and print summary to the display. */
	if(errorcount)
		printf("\nfile %s: processed %d lines, %d errors found\n",filename, linecount, errorcount);
	
	
	/* Check if the extern file is empty. */
	fseek(EXoutFile, 0 , SEEK_END);
	filesize = ftell(EXoutFile);
	
	/* Close the .ex file */
	fclose(EXoutFile);
	
	if(errorcount || !filesize)
		remove(buildOutFname(filename, ".ex"));
	
	return errorcount;
}

/* The function complete the machine code for Direct and Relative addressing mode.
   In case the symbol exist in the Symbol Table the function complete the encoding in machinecode the return TRUE,
   otherwise the function return FALSE. */
boolean CompletionMachineCode(char *field, int *commaflag, int IC, InstructionImage *machinecode, int mci, int addmode, SymbolNode *head, FILE *output){
	SymbolNode *node;
	/* Check if the addressing mode is direct. */
	if(addmode == DIRECT_V){
		/* Check whether the symbol exist or not, in case not found return FALSE. */
		if(!(node = findNode(head, field)))
			return FALSE;
		/* Check if the symbol define as extern. */
		if(node->attribute == EXTERNAL){
			fprintf(output, "%s\t%04d\n",node->symbol, mci);
			machinecode[mci].code = 0;
			machinecode[mci].ARE = 'E';
		}
		else{
			/* Complete the code. */
			machinecode[mci].code = IC + node->value;
			machinecode[mci].ARE = 'R';
		}
	
	}
	/* Check if the addressing mode is relative. */
	else if(addmode == RELATIVE_V){
		if(!(node = findNode(head, &field[1])))
			return FALSE;
		/* Check if the symbol define as extern. */
		if(node->attribute == EXTERNAL){
			fprintf(output, "%s\t%04d\n",node->symbol, mci);
			machinecode[mci].code = 0;
			machinecode[mci].ARE = 'E';
		}
		else{
			/* Complete the code. */
			machinecode[mci].code = node->value - mci;
			/* In case the number is negative. */
			if(machinecode[mci].code < 0)  machinecode[mci].code = lsbOfNumber(machinecode[mci].code);
			machinecode[mci].ARE = 'R';
		}
	} 
	return TRUE;
}

/* Parse entry line, in case no error found in the following symbol, the function update the attribute in the Symbol Table.
   then return TRUE, Otherwise return FALSE.
   errors can occur in case of undefine local label, then return FALSE and printf error message.*/
boolean ParseEntry(char *filename, int linecount, char *line, int *i, SymbolNode *head){
	SymbolNode *node;
	char field[MAXLINE];
	readword(line, field, i);
	/* find the node with same symbol name, in case not exist the function print error message and return FALSE. */
	if(!(node = findNode(head, field))){
		errorMsg(filename, field, linecount, ERROR1);
		return FALSE;
	}
	/* Update the attribute. */
	else
		node->attribute |= ENTRY;
	return TRUE;
}
