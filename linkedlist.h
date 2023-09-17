#ifndef LINKEDLIST_H
	#define LINKEDLIST_H
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "error.h"
	#include "constants.h"
	#include "datastructure.h"
	
	/* The function create and initialize SymbolNode then insert to end of linkedlist. */
	SymbolNode* add2list(SymbolNode **head, char *symbol, int attribute, int value);

	/* The function create and add node to end of linkedlist or find the node in specific cases
	Symbol not exist the function add to list, */
	SymbolNode* createNode(char *filename, SymbolNode **head, char *symbol, int attribute, int value, int linecount);

	/* The function get as parameter symbol and linkedlist
	check if the symbol exist in the linked list and return pointer for it.
	otherwise return NULL. */
	SymbolNode* findNode(SymbolNode *node, char *symbol);

	/* Free Symbol Table. */
	void freeList(SymbolNode *head);

#endif /* LINKEDLIST_H */
