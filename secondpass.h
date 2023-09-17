#ifndef SECONDPASS_H
	#define SECONDPASS_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include "error.h"
	#include "constants.h"
	#include "utilities.h"
	#include "datastructure.h"
	#include "output.h"
	#include "linkedlist.h"
	#include "readinput.h"
	
	/* The function get as parameter input file pointer (inFile) and its name (filename), Symbol table head,
	machinecode and dataimage arrays and IC (Instrucion counter).
	the funtion read from file line by line in order either to complete the missing values after the first pass or to update the attribute in the Symbol Table.
	further more in case no error detected the function create file.ex which contain all the external symbols.
	The function return the amount of errors detect and display error message. */
	int secondpass(FILE *inFile, char *filename, SymbolNode *head, InstructionImage *machinecode, int *dataimage, int IC);

	/* The function complete the machine code for Direct and Relative addressing mode.
	In case the symbol exist in the Symbol Table the function complete the encoding in machinecode the return TRUE,
	otherwise the function return FALSE. */
	boolean CompletionMachineCode(char *field, int *commaflag, int IC, InstructionImage *machinecode, int mci, int addmode, SymbolNode *head, FILE *output);

	/* Parse entry line, in case no error found in the following symbol, the function update the attribute in the Symbol Table.
	then return TRUE, Otherwise return FALSE.
	errors can occur in case of undefine local label, then return FALSE and printf error message.*/
	boolean ParseEntry(char *filename, int linecount, char *line, int *i, SymbolNode *head); 
		
#endif /* SECONDPASS_H */
