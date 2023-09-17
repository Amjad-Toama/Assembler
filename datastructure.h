#ifndef DATASTRUCTURE_H
	#define DATASTRUCTURE_H
	
	#include "constants.h"
	
	/* Register structure, consist of:
	   1) register name.
	   2) the binary value of specific register. */
	typedef struct Register{
		char *name;	/* Register name. */
		int value;	/* The value of register - (usability as flags in the binary representation). */
	}Register;
	
	/* Instruction structure, consist of:
	   1) instruction name.
	   2) opcode and funct values.
	   3) legal operand amount for specific instruction.
	   4) source and destination legal addressing modes.   */
	typedef struct Instruction{
		char *name;		/* instruction name */
		int opcode;		/* opcode value. */
		int funct;		/* funct value. */
		int operandnum;	/* the amount of operand for specific insturction. */
		unsigned char src;	/* The addressing modes source - 4 bits variable used as 4 flags, 0 - immediate. 1 - direct. 2 - relative. 3 - direct register.*/
		unsigned char dest;	/* The addressing modes destination - 4 bits variable used as 4 flags, 0 - immediate. 1 - direct. 2 - relative. 3 - direct register.*/
	}Instruction;
	
	/* Structure specify the information about labels defintion. */
	typedef struct SymbolNode{
		char symbol[MAXLABEL];				/* The symbol name. */
		int value;					/* hold the address of the label. */
		char attribute;				/* 4 bits varialbe used as 4 flags, 0 - code. 1 - data. 2 - entry. 3 - external.*/
		struct SymbolNode *next;			/* The next node. */
	}SymbolNode;
	
	
	/* Structure specify the directive name. */
	typedef struct Directive{
		char *name;
	}Directive;
	
	/* The structure consist binary code and the ARE feature. */
	typedef struct InstructionImage{
		int code;	/* binary code */
		char ARE;	/* A,R,E feature */
	}InstructionImage;
	
	/* Define boolean datatype. */
	typedef enum {FALSE, TRUE} boolean;
	
	/* The defined instruction in the program according to Instruction structure. */
	static const Instruction instructions[]  = 	{
		{"mov",	0,	0,	2,	IMMEDIATE|DIRECT|REGISTER_DIRECT,	DIRECT|REGISTER_DIRECT} ,
		{"cmp",	1,	0,	2,	IMMEDIATE|DIRECT|REGISTER_DIRECT,	IMMEDIATE|DIRECT|REGISTER_DIRECT},
		{"add",	2,	10,	2,	IMMEDIATE|DIRECT|REGISTER_DIRECT,	DIRECT|REGISTER_DIRECT},
		{"sub",	2,	11,	2,	IMMEDIATE|DIRECT|REGISTER_DIRECT,	DIRECT|REGISTER_DIRECT},
		{"lea",	4,	0,	2,	DIRECT,				DIRECT|REGISTER_DIRECT}, 
		{"clr",	5,	10,	1,	NONE,					DIRECT|REGISTER_DIRECT},
		{"not",	5,	11,	1,	NONE,					DIRECT|REGISTER_DIRECT},
		{"inc",	5,	12,	1,	NONE,					DIRECT|REGISTER_DIRECT}, 
		{"dec",	5,	13,	1,	NONE,					DIRECT|REGISTER_DIRECT},
		{"jmp",	9,	10,	1,	NONE,					DIRECT|RELATIVE}, 
		{"bne",	9,	11,	1,	NONE,					DIRECT|RELATIVE}, 
		{"jsr",	9,	12,	1,	NONE,					DIRECT|RELATIVE}, 
		{"red",	12,	0,	1,	NONE,					DIRECT|REGISTER_DIRECT},
		{"prn",	13,	0,	1,	NONE,					IMMEDIATE|DIRECT|REGISTER_DIRECT},
		{"rts",	14,	0,	0,	NONE,					NONE},
		{"stop",	15,	0,	0,	NONE,					NONE}
	};
	
	
	/* The defined registers in the program. */
	static const Register registers[] = {
		{"r0",	1},
		{"r1",	2},
		{"r2",	4},
		{"r3",	8},
		{"r4",	16},
		{"r5",	32},
		{"r6",	64},
		{"r7",	128}
	};
	
	/* The defined directives names. */
	static const Directive directives[] = {
		{".data"},
		{".string"},
		{".entry"}, 
		{".extern"}
	};


#endif /* DATASTRUCTURE_H */
