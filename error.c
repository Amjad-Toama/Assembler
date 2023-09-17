#include "error.h"

/* The function print apropriate error message according to the errornumber value parameter, 
   and specify the filename and the error field and its line. */
void errorMsg(char* filename, char* field, int linecount, int errornumber){
	switch(errornumber){
	
	case INVALIDLABEL1: printf("file %s, line %04d, invalid label: %s\n", filename, linecount, field);
		break;
	
	case INVALIDLABEL2: printf("file %s, line %04d, label does not start in first column: %s\n", filename, linecount, field);
		break;
	
	case INVALIDOPERAND1: printf("file %s, line %04d, invalid operand: %s\n", filename, linecount, field);
		break;
	
	case INVALIDOPERAND2: printf("file %s, line %04d, missing operand: %s\n", filename, linecount, field);
		break;
	
	case INVALIDOPERAND3: printf("file %s, line %04d, invalid characters: %s\n", filename, linecount, field);
		break;
	
	case INVALIDOPERAND4: printf("file %s, line %04d, invalid source operand: %s\n", filename, linecount, field);
		break;
	
	case INVALIDOPERAND5: printf("file %s, line %04d, invalid target operand: %s\n", filename, linecount, field);
		break;
		
	case INVALIDOPERAND6: printf("file %s, line %04d, undefined label as first operand: %s\n", filename, linecount, field);
		break;
		
	case INVALIDOPERAND7: printf("file %s, line %04d, undefined label as second operand: %s\n", filename, linecount, field);
		break;
		
	case INVALIDOPERAND8: printf("file %s, line %04d, invalid first operand: %s\n", filename, linecount, field);
		break;
		
	case INVALIDOPERAND9: printf("file %s, line %04d, invalid second operand: %s\n", filename, linecount, field);
		break;
	
	case DATAERROR1: printf("file %s, line %04d, missing opening double quotation: %s\n", filename, linecount, field);
		break;	
		
	case DATAERROR2: printf("file %s, line %04d, missing closing double quotation: %s\n", filename, linecount, field);
		break;
	
	case DATAERROR3: printf("file %s, line %04d, missing argument in directive: %s\n", filename, linecount, field);
		break;
		
	case DATAERROR4: printf("file %s, line %04d, invalid symbol: %s\n", filename, linecount, field);
		break;
		
	case DATAERROR5: printf("file %s, line %04d, data overflow: %s\n", filename, linecount, field);
		break;
		
	case ERROR1: printf("file %s, line %04d, undefined label: %s\n", filename, linecount, field);
		break;
		
	case ERROR2: printf("file %s, line %04d, the label alread defined as extern: %s\n", filename, linecount, field);
		break;
		
	case ERROR3: printf("file %s, line %04d, local label cannot be declared as external: %s\n", filename, linecount, field);
		break;
		
	case ERROR4: printf("file %s, line %04d, label was already declared earlier: %s\n", filename, linecount, field);
		break;
		
	case ERROR5: printf("file %s, line %04d, undefined instruction name: %s\n", filename, linecount, field);
		break;
		
	case ERROR6: printf("file %s, line %04d, label already designated as external: %s\n", filename, linecount, field);
		break;
		
	case INVALIDSYMBOL1: printf("file %s, line %04d, invalid symbol (Instruction): %s\n", filename, linecount, field);
		break;
	
	case INVALIDSYMBOL2: printf("file %s, line %04d, invalid symbol (Register): %s\n", filename, linecount, field);
		break;

	case INVALIDSYMBOL3: printf("file %s, line %04d, invalid symbol (Directive): %s\n", filename, linecount, field);
		break;
		
	case INVALIDSYMBOL4: printf("file %s, line %04d, invalid symbol (Too long name): %s\n", filename, linecount, field);
		break;
		
	case INVALIDSYMBOL5: printf("file %s, line %04d, invalid symbol (Non-alphabetic first character): %s\n", filename, linecount, field);
		break;
		
	case INVALIDDIRECT1: printf("file %s, line %04d, invalid directive (missing point): %s\n", filename, linecount, field);
		break;
		
	case INVALIDDIRECT2: printf("file %s, line %04d, undefined directive: %s\n", filename, linecount, field);
		break;
	}
}
