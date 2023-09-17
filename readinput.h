#ifndef READINPUT_H
	#define READINPUT_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include "constants.h"
	#include "datastructure.h"

/* The function parameters line and i is index in the line and word to insert the result.
	The function read word from line till space, enter or tab.
	every white space before word skipped */
	char* readword(char *line, char *word, int *i);

	/* The function get as parameter line and its index i, commaflag, operand to save the result in.
	The function read till space, tab, enter or comma. in case comma detected commaflag ON.
	The supported operands their prefixes are '#' or '%'. and words consists of alphabet and numbers.
	In any error case the function return NULL, otherwise return operand pointer. */
	char* readoperand(char *line, char *operand, int *i, int *commaflag);

	/* The function get line and its index i, number string and commaflag.
	The function read from line till space, tab, enter or comma. in case comma detected Commaflag ON.
	If the number is legal the function return number pointer. otherwise the function return NULL.
	The supported number are contain utmost one sign '-' or '+'. */
	char* readnumber(char *line, char *number, int *i, int *commaflag);

	/* The function skip all the spaces, tabs and enters */
	void skipwhitespaces(char *line, int *i);

	/* The function search for the first occurance of comma, and the index i is point to i+1 from the comma. and the function return TRUE
	otherwise the return FALSE. */
	boolean findcomma(char *line, int *i);

	/* The function check if the line is or white space either comment line. */
	boolean isEmpty(char* line);

#endif /* READINPUT_H */
