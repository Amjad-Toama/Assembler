#include "output.h"

/* The function get as parameter string, input name string to extract the output prefix name. suffix to concate output file name.
   The function append suffix to the input name, supported name with suffix or no suffix at all, the function return pointer to the new name. */
char* buildOutFname(char *input, char *suffix){
	char *suffixp, *output;
	int i = 0;
	
	/* duplicate the name to prevent changes in the original string. */	
	output = strdup(input);
	/* Case 1: The input file format doesn't specified. */
	if (!(suffixp = strrchr(output, '.')))
		suffixp = output + strlen(output);
	/* Case 2: The input file format is specified. */
	else suffixp[0] = '\0';
	
	/* skip and white spaces before. */
	skipwhitespaces(suffix, &i);
	/* Check whether the user insert '.' or not before the suffix. */
	if(suffix[i] != '.') strcat(output, ".");
	return strcat(output, suffix);
}

/* The function print two output files ("output.en" and "output.ob" formats) after the second pass.
   While ".ob file contain the machine code in hexadecimal (the instruction first then the data image) and ARE and ".en" file contain the entries labels. */
void printToFile(char* inputname, InstructionImage *machinecode, int *dataimage, int IC, int DC, SymbolNode *head){
	FILE* outFileOB, *outFileEN;
	int entryflag = OFF;
	
	/* Check if the machine code not empty. */
	if(IC != FIRSTADD || DC != 0){
		char *OBoutputname = buildOutFname(inputname, ".ob");
		int ici = FIRSTADD, dc;
		outFileOB = fopen(OBoutputname,"w");
		if(!outFileOB)
			errMsg("Cannot open file to write!\n");
			
		/* Print summary of the amount the instruction and the data. */
		fprintf(outFileOB, "\t%d %d\n",IC - FIRSTADD, DC);
		
		/* First of all print the instruction image. */
		for(; ici < IC; ici++)
			fprintf(outFileOB,"%04d\t%03X\t%c\n",ici, machinecode[ici].code, machinecode[ici].ARE);
			
		/* Lastly, print the dataimage. */
		for(dc = 0; dc < DC; dc++, ici++)
			fprintf(outFileOB, "%04d\t%03X\tA\n",ici, dataimage[dc]);
			
		/* Close the output file. */
		fclose(outFileOB);	
	}
	/* Check if the Symbol Table empty. */
	if(head){
		char *ENoutputname = buildOutFname(inputname, ".en");
		outFileEN = fopen(ENoutputname,"w");
		SymbolNode *node = head;
		if(!outFileEN)
			errMsg("Cannot open file to write!\n");
		/* Move node by node, while check if entry is attribute. if so print for file. */
		while(node){	
			if((node->attribute & ENTRY) == ENTRY){
				entryflag = ON;
				fprintf(outFileEN, "%s\t%04d\n",node->symbol, node->value);
			}
			node = node->next;
		}
		fclose(outFileEN);
		
		/* In case no symbol declared as entry, then delete the file. */
		if(!entryflag) remove(ENoutputname);
	}	
}

void errMsg(char *str){
	printf("%s\n",str);
	exit(1);
}
