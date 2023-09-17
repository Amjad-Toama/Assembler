#include "readinput.h"

/* The function parameters line and i is index in the line and word to insert the result.
   The function read word from line till space, enter or tab.
   every white space before word skipped */
char* readword(char *line, char *word, int *i){
	int j = 0, flag = OFF;
	/* Skip the white spaces*/
	skipwhitespaces(line, i);
	/* check if remain line is white or the index reach the end of line. */
	if(IS_WHITESPACE(&line[*i]) || *i >= strlen(line)) return NULL;
	
	/* Read till space, enter or tab. */
	while(*i < strlen(line) && line[(*i)] != ' ' && line[(*i)] != '\t' && line[(*i)] != '\n')
		word[j++] = line[(*i)++];
		
	/* Terminate the word. */
	word[j] = '\0';
	return word;
}

/* The function get as parameter line and its index i, commaflag, operand to save the result in.
   The function read till space, tab, enter or comma. in case comma detected commaflag ON.
   The supported operands their prefixes are '#' or '%'. and words consists of alphabet and numbers.
   In any error case the function return NULL, otherwise return operand pointer. */
char* readoperand(char *line, char *operand, int *i, int *commaflag){
	int j = 0;
	/* Skip the white spaces*/
	skipwhitespaces(line, i);
	/* check if remain line is white or the index reach the end of line. */
	if(IS_WHITESPACE(&line[*i]) || *i >= strlen(line)) return NULL;
	
	/* Check if the field prefix is '#' */
	if(line[*i] == '#'){
		operand[j++] = line[(*i)++];
		/* Check if the number is signed */
		if(line[*i] == '-' || line[*i] == '+')
			operand[j++] = line[(*i)++];
	}
	
	/* Check if the field prefix is '%' */
	else if(line[*i] == '%'){
		operand[j++] = line[(*i)++];
		/* After '%' expected char is alphabet. */
		if(!isalpha(line[(*i)])) return NULL;
		operand[j++] = line[(*i)++];
	}
	
	/* Read till space, tab, enter or comma. in case comma detected commaflag ON then break. */
	while(*i < strlen(line) && line[(*i)] != ' ' && line[(*i)] != '\t' && line[(*i)] != '\n'){
		if(line[(*i)] == ',' && !(*commaflag)){
			*commaflag = ON;
			(*i)++;
			break; 
		}
		/* Check whether the character is alphabet or digit. */
		else if(!isdigit(line[(*i)]) && !isalpha(line[(*i)]))
			return NULL;
		operand[j++] = line[(*i)++];
	}
	/* terminate the operand. */
	operand[j] = '\0';
	return operand;
}

/* The function get line and its index i, number string and commaflag.
   The function read from line till space, tab, enter or comma. in case comma detected Commaflag ON.
   If the number is legal the function return number pointer. otherwise the function return NULL.
   The supported number are contain utmost one sign '-' or '+'. */
char* readnumber(char *line, char *number, int *i, int *commaflag){
	int j = 0, numberflag = OFF;
	/* Skip white spaces. */
	skipwhitespaces(line, i);
	/* Check if the remain line is empty or the index reach the end. */
	if(IS_WHITESPACE(&line[*i]) || *i >= strlen(line)) return NULL;
	
	/* Check if sign detected. */
	if(line[*i] == '-' || line[*i] == '+') number[j++] = line[(*i)++];
	
	/* Read numbers till space, tab, enter or comma. */
	while(*i < strlen(line) && line[(*i)] != ' ' && line[(*i)] != '\t' && line[(*i)] != '\n'){
		if(line[*i] == ','){
			*commaflag = ON;
			(*i)++;
			break;
		}
		/* Check if the character is digit. */
		else if(!isdigit(line[*i])) return NULL;
		else {
			number[j++] = line[(*i)++];
			numberflag = ON;
		}
	}
	
	if(!numberflag) return NULL;
	
	number[j] = '\0';
	return number;
}

/* The function skip all the spaces, tabs and enters */
void skipwhitespaces(char *line, int *i){
	while(*i < strlen(line) && (line[*i] == ' ' || line[*i] == '\t' || line[*i] == '\n'))
		(*i)++;
}

/* The function search for the first occurance of comma, and the index i is point to i+1 from the comma. and the function return TRUE
   otherwise the return FALSE. */
boolean findcomma(char *line, int *i){
	skipwhitespaces(line, i);
	if(line[(*i)] == ','){
		(*i)++;
		return TRUE;
	}
	return FALSE;
}


/* The function check if the line is or white space either comment line. */
boolean isEmpty(char* line){
     /* The use of scanf is to skip the spaces before the first word, then check if the first char is ';'. */
     char word[MAXLABEL];
     return (IS_WHITESPACE(line) || (sscanf(line, "%s", word) && word[0] == ';'));
}
