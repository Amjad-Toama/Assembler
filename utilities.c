#include "utilities.h"
/* Identifing words functions. */

/* The function check if the field is defining label. */
boolean isLabel(char* field){
	/* Check if the last letter in the field is ':' */
	if(field[strlen(field)-1] != ':')
		return FALSE;
	/* remove the ':' char from the end of temp */
	field[strlen(field)-1] = '\0';
	return TRUE;
}

/* The function check if the field is legal symbol.
   If the field is legal symbol the function return TRUE, else FALSE. */
boolean isSymbol(char* filename, char* field, int linecount){
	/* Case 1: if the label name is instructor. */
	if(isInstruction(field) != ERROR)
		errorMsg(filename, field, linecount, INVALIDSYMBOL1);
	
	/* Case 2: if the label name is register. */
	else if (isRegister(field) != -1)
		errorMsg(filename, field, linecount, INVALIDSYMBOL2);
	
	/* Case 3: if the label name is directive. */
	else if (isDirective(filename, field, linecount) == ERROR)
		errorMsg(filename, field, linecount, INVALIDSYMBOL3);
	
	/* Case 4: if the label name exceed it limits. */
	else if(strlen(field) >= MAXLABEL)
		errorMsg(filename, field, linecount, INVALIDSYMBOL4);
	
	/* Case 5: if the first letter is alphabet. */
	else if(!isalpha(field[0]))
		errorMsg(filename, field, linecount, INVALIDSYMBOL5);
	/* Case 6: the symbol is permitted. */
	else
		return TRUE;
	
	return FALSE;
}

/* Check if field consist from digits and alphabets and return TRUE, otherwise return FALSE. */
boolean Legalchars(char *field){
	int i = 0;
	for(; i < strlen(field); i++)
		/* Check if the character isn't digit and isn't alphabet return FALSE. */
		if(!isdigit(field[i]) && !isalpha(field[i]))
			return FALSE;
	return TRUE;
}

/* The function check if the field represent instruction.
   If the field is istruction the function return the index of the instruction from instructions array, else return ERROR. */ 
int isInstruction(char *field){
	int i;
	for(i = 0; i < MAXINSTRUCTION; i++)
		if(!strcmp(instructions[i].name, field))
			return i;
	return ERROR;
}

/* The function check if the field represent legal directive.
   If the directive legal, return TRUE.
   If the directive undefined or missing point at start the function return ERROR, else FALSE. */
boolean isDirective(char* inputname, char* field, int linecount){
	int i;
	/* Check if word is directive. */
	for(i = 0; i < MAXDIRECTIVE; i++){
		if(!strcmp(field, directives[i].name))
			return TRUE;
		/* Check if word is directive but doesn't start with '.'. */
		else{
			/* skip the first point. */
			char *temp = directives[i].name + 1;
			if(!strcmp(field, temp)){
				errorMsg(inputname, field, linecount, INVALIDDIRECT1);
				return ERROR;
			}
		}
	}
	/* In case field start with point but not detected as directive, return ERROR and print error message. */
	if(field[0] == '.'){
		errorMsg(inputname, field, linecount, INVALIDDIRECT2);
		return ERROR;
	}
	return FALSE;
}

/* The function check if the field represent register.
If so, the function return the index of registers array, otherwise return -1.*/
int isRegister(char* field){
	int i;
	for(i = 0; i < MAXREGISTER; i++)
		if(!strcmp(field, registers[i].name))
			return i;
	return ERROR;
}


/* The function get as parameter string, and check if the string is number.
   If the string is number the function return TRUE, Otherwise return FALSE.*/
boolean isNumber(char* field){
	int i = 0;
	
	if(IS_WHITESPACE(field)) return FALSE;
	
	/* In case sign detected */
	if(field[0] == '-' || field[0] == '+')
		i++;
	
	for(; i < strlen(field); i++)
		if(!isdigit(field[i]))
			return FALSE;
	return TRUE;
}

/* The function check if the number is in range. */
boolean legalNumber(int number){
	return (MINNUMBER <= number && number <= MAXNUMBER);
}

/* The function get as parameter number and return its LOWESTBYTE*INT_SIZE */
int lsbOfNumber(int number){
	unsigned int tempNum = number;
	/* turn the msb to zeros. */
	tempNum <<= (sizeof(int)*BYTETOBITS - sizeof(int)*LOWESTBYTE);
	tempNum >>= (sizeof(int)*BYTETOBITS - sizeof(int)*LOWESTBYTE);
	return tempNum;
}

/* The function check whether the file is empty or not, in empty case the returned value is TRUE, otherwise FALSE. */
boolean isEmptyFile(FILE *fp){
	boolean flag = TRUE;
	char ch;
	/* Set the file pointer to the beginning. */
	rewind(fp);
	
	/* Check char by char till eof, in case of character has graphical representation the funtion return FALSE. */
	while((ch = fgetc(fp)) != EOF){
		if(isgraph(ch)){
			flag = FALSE;
			break;
		}
	}
	
	/* Set the file pointer to the beginning. */
	rewind(fp);
	return flag;
}


















