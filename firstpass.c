#include "firstpass.h"

/* The function get as parameter input file pointer (inFile) and its name (filename), Symbol table head,
   machinecode and dataimage arrays and their counters IC and DC pointers, respectively.
   the funtion read from file line by line and check whether the line is instruction, directive, comment or empty line.
   then check if their is any syntax, data, words errors, if so the function error message to the display.
   the function return the errors amount.
*/
int firstPass(FILE *inFile, char *filename, SymbolNode **head, InstructionImage machinecode[], int dataimage[], int *IC, int *DC){
	/* line		- to store the current line from the file.
	   field		- to store the current word from the line.
	   symbol		- to store the label name in it.
	   errorcount 		- error counter.
	   linecount		- line counter.
	   errorfieldflag	- flag on if any error detected.
	   i			- index to line. 
	*/
	char line[MAXLINE], field[MAXLINE], symbol[MAXLINE];
	int errorcount = 0, linecount = 0, errorfieldflag,i;
	/* In every interaction the loop read new line from file till end. */
	while(fgets(line, MAXLINE, inFile) != NULL){
		
		linecount++;
		i = 0;
		strcpy(symbol, "");
		errorfieldflag = OFF;
		/* terminate the line '\n' with '\0' */
		line[strlen(line)-1] = '\0';
		
		/* Check if the line is comment or empty. if so move to the next line. */
		if(isEmpty(line))
			continue;
		/* Read the first word from line. */
		readword(line, field, &i);
		
		/* Check if the first word represent label definition. */
		if(isLabel(field)){
			/* Check if the label definition start from the first column (No spaces before the declaration) */
			if(!(strlen(field) + 1 == i)){
				/* if so print error message to display and increase counter then move to new line. */
				errorMsg(filename, line, linecount, INVALIDLABEL2);
				errorcount++;
				continue;		
			}
			/* Check if the symbol name is legal. */
			if(!isSymbol(filename, field, linecount)){
				errorcount++;
				continue;
			}
			/* copy the label name to variable then read new word from the line. */
			strcpy(symbol, field);
			readword(line, field, &i);
		}
		/* Check if the word represent directive. */
		if((errorfieldflag = isDirective(filename, field, linecount))){
			/* Check if the return value represent any name error. */
			if(errorfieldflag == ERROR){
				errorcount++;
				continue;
			}
			/* Parse the directive line. */
			if(!ParseDirective(filename, head, field, line, &i, dataimage, DC, symbol, linecount))
				errorcount++;
		}
		/* Otherwise check if the word represent instruction */
		else{
			/* Check whether the field represent defined instruction or not. */
			if(isInstruction(field) == ERROR){
				/* if so print error message to display and increase counter then move to new line. */
				errorMsg(filename, field, linecount, ERROR5);
				errorcount++;
				continue;		
			}
			/* Parse the instruction line. */
			else if(!ParseInstruction(filename, field, symbol, machinecode, IC, linecount, head, line, &i))
				errorcount++;

		}	
	}
	/* if any error detected print summary to the display. */
	if(errorcount) printf("\nfile %s: processed %d lines, %d errors found\n",filename, linecount, errorcount);
	/* update the data values in the symbol table.  */
	UpdateDataValue(*head, *IC);
	return errorcount;
}

/* Update the data value after in the symbol table by adding the distance (IC).
   The function get symbol table head and instruction counter.
 */
void UpdateDataValue(SymbolNode *head, int IC){
	SymbolNode *node = head;
	/* In every interaction node move to the next node. */
	while(node){
		/* Check if the attribute is data. */
		if((node->attribute & DATA) == DATA)
			node->value += IC;
		/* Move to the next node. */
		node = node->next;
	}
}

/* The function parse the instruction line. NOTE: the function parse utmost two operands.
   The instruction line pattern is "instruction srcop, destop".
   In case the line is true the function initialize machinecode and update IC value then return TRUE.
   If any error detected the function print error message on display and return FALSE. */
boolean ParseInstruction(char *filename, char* field, char *symbol, InstructionImage machinecode[], int *IC, int linecount, SymbolNode **head, char *line, int *i){
	/* indexI	- (instruction index)
	   addmode	- addressing mode 
	   commaflag	- the flag turn on if the comma detected.
	   ICI		- (Instruction Code Index) contain the instuction code index in the machinecode.
	*/
	int indexI = isInstruction(field);	
	int addmode = NONE, commaflag = OFF, ICI = *IC;
	
	/* Check if label declaration detected line. */
	if(strlen(symbol) != 0){
		/* Insert the label to the symbol table. */
		if(!createNode(filename, head, symbol, CODE, *IC, linecount))
			return FALSE;
	}
	
	/* Initialize the instruction code with opcode and funct. NOTE: every instruction ARE is Absolute. */
	machinecode[*IC].ARE = 'A';
	machinecode[*IC].code = instructions[indexI].opcode;
	machinecode[*IC].code <<= 4;
	machinecode[*IC].code |= instructions[indexI].funct;
	
	/* Parse the source operand if it's defined for the specific instruction. */
	if(instructions[indexI].src){
		/* Check if the line remain part of the line is empty. */
		if(IS_WHITESPACE(&line[*i]) || *i >= strlen(line)){
			errorMsg(filename, line, linecount, INVALIDOPERAND2);
			return FALSE;
		}
		/* Read the next field and check whether the field name is legal */
		if(!readoperand(line, field, i, &commaflag)){
			/* if the operand name isn't legal. */
			errorMsg(filename, &line[*i], linecount, INVALIDOPERAND3);
			return FALSE;
		}
		/* Analyze the operand addressing mode, if any syntax error the return value ERROR or NUMERROR */
		if((addmode = analyzeOperand(filename, field, machinecode, ++(*IC), linecount)) == ERROR || addmode == NUMERROR){
			/* Check the error value in order to print proper error message. */
			if(addmode == ERROR) errorMsg(filename, field, linecount, INVALIDOPERAND1);
			else if(addmode == NUMERROR && !legalNumber(machinecode[*IC].code))
				errorMsg(filename, field, linecount, INVALIDOPERAND8);
			return FALSE;
		}
		/* Check if the addressing mode is defined for the specific instruction. */
		if((addmode = isLegalOperand(addmode ,instructions[indexI].src)) == ERROR){
			errorMsg(filename, field, linecount, INVALIDOPERAND4);
			return FALSE;
		}
	}
	
	/* Store the analized data in the instruction code. */
	machinecode[ICI].code <<= 2;
	machinecode[ICI].code |= addmode;
	
	/* Check if the comma exist between two operands. */
	if(instructions[indexI].src && !commaflag && !findcomma(line, i)){
		errorMsg(filename, line, linecount, INVALIDOPERAND2);
		return FALSE;
	}
	
	/* Parse the destination operand if it's defined for the specific instruction. */
	if(instructions[indexI].dest){
		/* Check if the line remain part of the line is empty. */
		if(IS_WHITESPACE(&line[*i]) || *i >= strlen(line)){
			errorMsg(filename, line, linecount, INVALIDOPERAND2);
			return FALSE;
		}
		/* Read the next field and check whether the field name is legal */
		if(!readoperand(line, field, i, &commaflag)){
			errorMsg(filename, &line[*i], linecount, INVALIDOPERAND3);
			return FALSE;
		}
		/* Analyze the operand addressing mode, if any syntax error the return value ERROR or NUMERROR */
		if((addmode = analyzeOperand(filename, field, machinecode, ++(*IC), linecount)) == ERROR  || addmode == NUMERROR){
			/* Check the error value in order to print proper error message. */
			if(addmode == ERROR) errorMsg(filename, field, linecount, INVALIDOPERAND1);
			else if(addmode == NUMERROR && !legalNumber(machinecode[*IC].code))
				errorMsg(filename, field, linecount, INVALIDOPERAND9);
			return FALSE;
		}
		/* Check if the addressing mode is defined for the specific instruction. */
		if((addmode = isLegalOperand(addmode ,instructions[indexI].dest)) == ERROR){
			/* Check if the addressing mode in order to print proper error message. */
			if(instructions[indexI].src)
				errorMsg(filename, field, linecount, INVALIDOPERAND5);
			else errorMsg(filename, field, linecount, INVALIDOPERAND1);
			return FALSE;
		}
	}
	/* Store the analized data in the instruction code. */
	machinecode[ICI].code <<= 2;
	machinecode[ICI].code |= addmode;
	
	/* Update the Instruction counter value. */
	*IC = ICI + instructions[indexI].operandnum + 1;
	
	/* Check if for non-source instruction detected comma. */
	if(!instructions[indexI].src && commaflag){
			errorMsg(filename, &line[--(*i)], linecount, INVALIDOPERAND3);
			return FALSE;
	}
	/* Check if the remain line after reading the whole expected operand the line empty. */
	if(!IS_WHITESPACE(&line[*i])){
		/* Check if the addressing mode in order to print proper error message. */
		if(!instructions[indexI].dest)
			errorMsg(filename, &line[*i], linecount, INVALIDOPERAND1);
		else errorMsg(filename, &line[*i], linecount, INVALIDOPERAND3);
		return FALSE;
	}
	
	return TRUE;
}

/* The function analize the addressing mode of field. whether it's immediate, direct, relative or register_direct.
   The function return the addressing mode if the field isn't defined addressing mode the function return ERROR,
   If field is defined addressing mode but the word isn't legal the funcion return NUMERROR. 
*/
int analyzeOperand(char* filename, char* field, InstructionImage machinecode[], int IC, int linecount){
	/* Rindex		- Register Index
	   relativeflag	- turn on in case the first letter is '%'.
	*/
	int Rindex, relativeflag = 0;
	/* Check if the first character is '%' (Represent relative), if so turn relative flag on and move to the next character. */
	if(field[0] == '%'){
		relativeflag = 1;
		field++;
	}
	
	/* Check if the first character '#' (Represent immediate). */
	if(field[0] == '#'){
		/* Check if the line empty after. */
		if(IS_WHITESPACE(++(field)))
			return ERROR;
		/* Check if the field represent decimal number. */
		else if(!isNumber(field)){
			/* if not print proper error message. */
			errorMsg(filename, field, linecount, INVALIDOPERAND3);
			return NUMERROR;
		}
		else{
			/* store the value in the machine code. */
			machinecode[IC].code = atoi(field);
			/* Check if the number is in range. */
			if(!legalNumber(machinecode[IC].code))
				return NUMERROR;
			/* Check if the number is negative,if so turn the MSB to zeros except the 8*LOWERBITS (3 Byte in out case). */
			if(machinecode[IC].code < 0)  machinecode[IC].code = lsbOfNumber(machinecode[IC].code);
			/* Initialize the ARE value to A.*/
			machinecode[IC].ARE = 'A';
			return IMMEDIATE;
		}
	}
	/* Check if the field represent register, if so insert the value to data image. */
	else if((Rindex = isRegister(field)) != ERROR){
		machinecode[IC].code = registers[Rindex].value;
		machinecode[IC].ARE = 'A';
		return REGISTER_DIRECT;
	}	
	/* Otherwise check if the field is legal symbol.*/
	else if(legalSymbol(field)){
		machinecode[IC].code = '?';
		/* Check if the flag on in order to initialize ARE to the proper value. */
		if(relativeflag){
			machinecode[IC].ARE = 'A';
			return RELATIVE;
		}
		machinecode[IC].ARE = '?';
		return DIRECT;
			
	}
	else
		return ERROR;
}

/* The funtion check if the field is legel */
boolean legalSymbol(char *field){
	if(isalpha(field[0]) && Legalchars(field) && isInstruction(field) == ERROR && isRegister(field) == ERROR && strlen(field) < MAXLABEL) return TRUE;
	else return FALSE;
}

/* The function Check if the addmode is proper to specific instruction.
   if the addmode compatible, so return value to insert in 2 bits. otherwise return ERROR. */
int isLegalOperand(int addmode, unsigned char instructionaddmodes){
	switch(addmode){
		case IMMEDIATE:
			return (isLegalAddMode(addmode,instructionaddmodes))? IMMEDIATE_V : ERROR;
		case DIRECT:
			return (isLegalAddMode(addmode,instructionaddmodes))? DIRECT_V : ERROR;
		case RELATIVE: 
			return (isLegalAddMode(addmode,instructionaddmodes))? RELATIVE_V : ERROR;
		case REGISTER_DIRECT: 
			return (isLegalAddMode(addmode,instructionaddmodes))? REGISTER_DIRECT_V : ERROR;
		default:
			return ERROR;
	}
}

/* Check if the addmode flag is legal if so the returned value is TRUE, otherwise FALSE. */
boolean isLegalAddMode(int addmode, unsigned char instructionaddmodes){
	return (((int)instructionaddmodes & addmode) == addmode);
}

/* The function parse the directive line.
   The directive line pattern is ".directive data".
   In case the line is true the function initialize dataimage and insert label to symbol table if nessary. update DC value with the data amount, then return TRUE.
   If any error detected the function print error message on display and return FALSE. */
boolean ParseDirective(char *filename, SymbolNode **head, char *field, char *line, int *i, int dataimage[], int *DC, char *symbol, int linecount){
	/* If the directive is data. */
	if(!strcmp(field, ".data")){
		/* Check if in there is label declaration. */
		if(!IS_WHITESPACE(symbol))
			createNode(filename, head, symbol, DATA, *DC, linecount);
		/* Parse the numbers in the directive line. */
		if(!ParseNumbers(filename, line, i, dataimage, DC, linecount))
			return FALSE;
	}
	/* If the directive is string. */
	else if(!strcmp(field, ".string")){
		/* Check if in there is label declaration. */
		if(!IS_WHITESPACE(symbol))
			createNode(filename, head, symbol, DATA, *DC, linecount);
		/* Parse the numbers in the directive line. */
		if(!ParseString(filename, line, i, dataimage, DC, linecount))
			return FALSE;
	}
	/* If the directive is entry. */
	else if(!strcmp(field, ".entry")){
		SymbolNode *node;
		/* Read the next field. */
		if(!readword(line, field, i)){
			errorMsg(filename, line, linecount, DATAERROR3);
			return FALSE;
		}
		/* Check if the field name is legal. */
		if(!isSymbol(filename, field, linecount))
			return FALSE;
		
		/* Check if the symbol already defined as external. */
		if((node = findNode(*head, field))){
			if((node->attribute & EXTERNAL) == EXTERNAL){
				errorMsg(filename, field, linecount, ERROR6);
				return FALSE;
			}
		}
		
		/* Check if the remain line after reading the whole expected operand the line empty. */
		if(!IS_WHITESPACE(&line[*i])){
			errorMsg(filename, &line[*i], linecount, INVALIDOPERAND3);
			return FALSE;
		}
	}
	/* Otherwise the directive is extern. */
	else {
		/* Read the next field. */
		if(!readword(line, field, i)){
			errorMsg(filename, field, linecount, DATAERROR3);
			return FALSE;
		}
		
		/* Check if the field name is legal. */
		if(!isSymbol(filename, field, linecount))
			return FALSE;
		
		/* Add the symbol to Symbol Table, in case detected error the function return FALSE. */
		if(!createNode(filename, head, field, EXTERNAL, 0, linecount))
			return FALSE;
		
		/* Check if the remain line after reading the whole expected operand the line empty. */
		if(!IS_WHITESPACE(&line[*i])){
			errorMsg(filename, &line[*i], linecount, INVALIDOPERAND3);
			return FALSE;
		}
	}
	return TRUE;
}

/* The function parse the data line, and update dataimage and DC value.
   In case the line parsed without error detection the function return TRUE, otherwise return FALSE. */
boolean ParseNumbers(char *filename, char *line, int *i, int dataimage[], int *DC, int linecount){
	char field[MAXLINE];
	/* Sanity check: if the line is empty. */
	if(IS_WHITESPACE(&line[*i])){
		errorMsg(filename, line, linecount, DATAERROR3);
		return FALSE;
	}
	
	/* In every interaction read new word from line till the end. */
	while(*i < strlen(line)){
		int commaflag = OFF;
		/* Read next word and check if field represent number. */
		if(!readnumber(line, field, i, &commaflag) || !isNumber(field)){
			/* printf proper error message. */
			if(commaflag) --(*i);
			errorMsg(filename, &line[(*i)], linecount, INVALIDOPERAND3);
			return FALSE;
		}
		
		/* Insert the value to data image. */
		dataimage[*DC] = atoi(field);
		
		/* Check if the number is in range. */
		if(!legalNumber(dataimage[*DC])){
			/* printf proper error message. */
			errorMsg(filename, field, linecount, DATAERROR5);
			(*DC)++;
			return FALSE;
		}
		
		/* Check if the number is negative,if so turn the MSB to zeros except the 8*LOWERBITS (3 Byte in out case). */
		if(dataimage[*DC] < 0) dataimage[*DC] = lsbOfNumber(dataimage[*DC]);
		
		(*DC)++;
		
		/* Check if the line is empty and no comma reads from the last word */
		if(IS_WHITESPACE(&line[*i]) && !commaflag)
			return TRUE;
		/* Check if the line is empty and comma detected. */
		if(IS_WHITESPACE(&line[*i]) && commaflag){
			errorMsg(filename, &line[--(*i)], linecount, INVALIDOPERAND3);
			return FALSE;
		}
		/* Search for the comma. */
		if(!commaflag && !findcomma(line, i)){
			errorMsg(filename, &line[--(*i)], linecount, INVALIDOPERAND3);
			return FALSE;
		}
	}
	return TRUE;
}

/* The function Parse directive string. then insert the data to Data Image and updatae DC value.
   If the line readed with no error detected the function return TRUE. otherwise return FALSE.*/
int ParseString(char *filename, char* line, int *i, int dataimage[], int *DC, int linecount){
	/* check if no string define after the directive. */
	if(IS_WHITESPACE(&line[*i])){
		errorMsg(filename, line, linecount, DATAERROR3);
		return FALSE;
	}
	
	/* Skip whitespaces before the string. */
	skipwhitespaces(line, i);
	
	/* Check if '"' exist at first. */
	if(line[(*i)++] != '\"'){
		errorMsg(filename, line, linecount, DATAERROR1);
		return 0;
	}
	/* read char by char till end of line or till '"' */
	while(*i < strlen(line) && line[*i] != '\"')
		/* insert the data to Data Image. */
		dataimage[(*DC)++] = (int)line[(*i)++];
		
	/* Check if the loop stop as a result of *i < strlen(line) stop condition.  */
	if(*i == strlen(line)){
		errorMsg(filename, line, linecount, DATAERROR2);
		return FALSE;
	}
	/* Insert null value to the Data Image. */
	dataimage[(*DC)++] = '\0';

	/* Check if the remain line after reading the whole expected operand the line empty. */
	if(!IS_WHITESPACE(&line[++(*i)])){
		errorMsg(filename, &line[*i], linecount, INVALIDOPERAND3);
		return FALSE;
	}
	
	return TRUE;
}
