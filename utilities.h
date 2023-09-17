#ifndef UTILITIES_H
	#define UTILITES_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include "error.h"
	#include "constants.h"
	#include "datastructure.h"
	
	/* The function check if the field is defining label. */
	boolean isLabel(char* field);

	/* The function check if the field is legal symbol.
	If the field is legal symbol the function return TRUE, else FALSE. */
	boolean isSymbol(char* filename, char* field, int linecount);

	/* Check if field consist from digits and alphabets and return TRUE, otherwise return FALSE. */
	boolean Legalchars(char *field);

	/* The function check if the field represent instruction.
	If the field is istruction the function return the index of the instruction from instructions array, else return ERROR. */ 
	int isInstruction(char *field);

	/* The function check if the field represent legal directive.
	If the directive legal, return TRUE.
	If the directive undefined or missing point at start the function return ERROR, else FALSE. */
	boolean isDirective(char* inputname, char* field, int linecount);

	/* The function check if the field represent register.
	If so, the function return the index of registers array, otherwise return -1.*/
	int isRegister(char* field);

	/* The function get as parameter string, and check if the string is number.
	If the string is number the function return TRUE, Otherwise return FALSE.*/
	boolean isNumber(char* field);

	/* The function check if the number is in range. */
	boolean legalNumber(int number);

	/* The function get as parameter number and return its LOWESTBITS*INT_SIZE */
	int lsbOfNumber(int number);

	/* The function check whether the file is empty or not, in empty case the returned value is TRUE, otherwise FALSE. */
	boolean isEmptyFile(FILE *fp);

#endif /* UTILITIES_H */
