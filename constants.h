#ifndef CONSTANTS_H
	#define CONSTANTS_H
	
	#define MAXLINE 81		/* The maximum row length from input file. */
	#define MAXLABEL 32		/* The maximum length of label. */
	#define MAXREGISTER 8		/* The amount of registers in CPU. */
	#define MAXINSTRUCTION 16	/* The amount of operators. */
	#define MAXDIRECTIVE 4		/* The amount of the directives. */
	#define DIRECTIVELEN 8		/* The length of directive include the '\0' and '.' */
	
	/* Define The Addresses Modes - The usability is by defining them as flags in the instructions array. */
	#define NONE 0
	#define IMMEDIATE 1
	#define DIRECT 2
	#define RELATIVE 4
	#define REGISTER_DIRECT 8
	
	/* Define The Addresses Modes - The usability is by saving the addressing mode in the machine code in two bits. */
	#define IMMEDIATE_V		0
	#define DIRECT_V		1
	#define RELATIVE_V		2
	#define REGISTER_DIRECT_V	3
	
	/* Define the attributes - the usability is by saving them as flags in the Symbol table for every specific defined symbol. */
	#define CODE 1			
	#define DATA 2			
	#define ENTRY 4		
	#define EXTERNAL 8		
	
	/* Error Numbers. */
	#define ERROR  -1		/* Returned value indicate for error detection. */
	#define NUMERROR -2		/* Returned value indicate for numerical error detection. */
	
	/* Numbers Range */
	#define MAXNUMBER 2047		/* The highest numerical value. */
	#define MINNUMBER -2048	/* The lowest numerical value. */
	#define MAXSTACK 4095		/* The size of the stack */
	
	#define LOWESTBYTE 3		/* The needed byte to print */
	#define BYTETOBITS 8		/* The byte size */
	
	#define ON 1			/* Flag value. */
	#define OFF 0			/* Flag value. */
	
	#define FIRSTADD 100		/* First address in the memory. */
	
	/* Define Macros */
	/* Check if the string is empty line. */
	#define IS_WHITESPACE(string) (sscanf((string),"%*s") == EOF)
	
	
#endif /* CONSTANTS_H */
