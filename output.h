#ifndef OUTPUT_H
#define OUTPUT_H
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "datastructure.h"
	#include "constants.h"
	#include "utilities.h"
	#include "readinput.h"
	
	/* The function get as parameter string, input name string to extract the output prefix name. suffix to concate output file name.
	The function append suffix to the input name, supported name with suffix or no suffix at all, the function return pointer to the new name. */
	char* buildOutFname(char *input, char *suffix);


	/* The function print two output files ("output.en" and "output.ob" formats) after the second pass.
	While ".ob file contain the machine code in hexadecimal (the instruction first then the data image) and ARE and ".en" file contain the entries labels. */
	void printToFile(char* inputname, InstructionImage *machinecode, int *dataimage, int IC, int DC, SymbolNode *head);
	
	/* In case of Error the function print error message and exit the program. */
	void errMsg(char *str);
	
#endif /*OUTPUT_H*/
