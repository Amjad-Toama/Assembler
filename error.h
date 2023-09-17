#ifndef ERROR_H
	#define ERROR_H
	
	#include <stdio.h>
	#include "constants.h"
	
	/* Errors Value - to print the proper error message.*/
	#define INVALIDLABEL1		1
	#define INVALIDLABEL2		2
	
	#define INVALIDOPERAND1	10
	#define INVALIDOPERAND2	11
	#define INVALIDOPERAND3	12
	#define INVALIDOPERAND4	13
	#define INVALIDOPERAND5	14
	#define INVALIDOPERAND6	15
	#define INVALIDOPERAND7	16
	#define INVALIDOPERAND8	17
	#define INVALIDOPERAND9	18
	
	#define DATAERROR1		20
	#define DATAERROR2		21
	#define DATAERROR3		22
	#define DATAERROR4		23
	#define DATAERROR5		24
	
	#define INVALIDSYMBOL1		30
	#define INVALIDSYMBOL2		31
	#define INVALIDSYMBOL3		33
	#define INVALIDSYMBOL4		34
	#define INVALIDSYMBOL5		35
	
	#define ERROR1			40
	#define ERROR2			41
	#define ERROR3			42
	#define ERROR4			43
	#define ERROR5			44
	#define ERROR6			45
	
	#define INVALIDDIRECT1		50
	#define INVALIDDIRECT2		51
	
	/* The function print apropriate error message according to the errornumber value parameter, 
	   and specify the filename and the error field and its line. */
	void errorMsg(char* filename, char* field, int linecount, int errornumber);
	
#endif /* ERROR_H */
