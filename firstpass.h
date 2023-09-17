#ifndef FIRSTPASS_H
	#define FIRSTPASS_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include "error.h"
	#include "constants.h"
	#include "utilities.h"
	#include "datastructure.h"
	#include "linkedlist.h"
	#include "readinput.h"
	
	/* Functions Prototypes. */
	
	/* The function get as parameter input file pointer (inFile) and its name (filename), Symbol table head,
	machinecode and dataimage arrays and their counters IC and DC pointers, respectively.
	the funtion read from file line by line and check whether the line is instruction, directive, comment or empty line.
	then check if their is any syntax, data, words errors, if so the function error message to the display.
	the function return the errors amount. */
	int firstPass(FILE *inFile, char *filename, SymbolNode **head, InstructionImage machinecode[], int dataimage[], int *IC, int *DC);
	
	/* Update the data value after in the symbol table by adding the distance (IC).
	The function get symbol table head and instruction counter. */
	void UpdateDataValue(SymbolNode *head, int IC);
	
	/* The function parse the instruction line. NOTE: the function parse utmost two operands.
	The instruction line pattern is "instruction srcop, destop".
	In case the line is true the function initialize machinecode and update IC value then return TRUE.
	If any error detected the function print error message on display and return FALSE. */
	boolean ParseInstruction(char *filename, char* field, char *symbol, InstructionImage machinecode[], int *IC, int linecount, SymbolNode **head, char *line, int *i);
	
	/* The function analize the addressing mode of field. whether it's immediate, direct, relative or register_direct.
	The function return the addressing mode if the field isn't defined addressing mode the function return ERROR,
	If field is defined addressing mode but the word isn't legal the funcion return NUMERROR. */
	int analyzeOperand(char* filename, char* field, InstructionImage machinecode[], int IC, int linecount);

	/* The funtion check if the field is legel */
	boolean legalSymbol(char *field);
	
	/* Check if the addmode flag is legal if so the returned value is TRUE, otherwise FALSE. */
	boolean isLegalAddMode(int addmode, unsigned char instructionaddmodes);
	
	/* The function Check if the addmode is proper to specific instruction.
	if the addmode compatible, so return value to insert in 2 bits. otherwise return ERROR. */
	int isLegalOperand(int addmode, unsigned char instructionaddmodes);
	
	/* The function parse the directive line.
	The directive line pattern is ".directive data".
	In case the line is true the function initialize dataimage and insert label to symbol table if nessary. update DC value with the data amount, then return TRUE.
	If any error detected the function print error message on display and return FALSE. */
	boolean ParseDirective(char *filename, SymbolNode **head, char *field, char *line, int *i, int dataimage[], int *DC, char *symbol, int linecount);
	
	
	/* The function parse the data line, and update dataimage and DC value.
	In case the line parsed without error detection the function return TRUE, otherwise return FALSE. */
	boolean ParseNumbers(char *filename, char *line, int *i, int dataimage[], int *DC, int linecount);

	/* The function Parse directive string. then insert the data to Data Image and updatae DC value.
	If the line readed with no error detected the function return TRUE. otherwise return FALSE.*/
	int ParseString(char *filename, char* line, int *i, int dataimage[], int *DC, int linecount);



#endif /* FIRSTPASS_H */
