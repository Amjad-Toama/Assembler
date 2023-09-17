#include "linkedlist.h"

/* The function create and initialize SymbolNode then insert to end of linkedlist. */
SymbolNode* add2list(SymbolNode **head, char *symbol, int attribute, int value){
	SymbolNode *newNode = (SymbolNode*)malloc(sizeof(SymbolNode));
	/* Check if the allocation memory succeed. */
	if(!newNode){
		printf("Allocation Memory Error!\n");
		freeList(*head);
		exit(1);
	}
	/* Initialize the values. */
	strcpy(newNode->symbol, symbol);
	newNode->attribute = attribute;
	newNode->value = value;
	newNode->next = NULL;
	
	/* In case the head empty list. */
	if(!(*head)){
		*head = newNode; 
	}
	/* In case head isn't empty list */
	else{
		/* move to the last node. */
		SymbolNode *index = *head;
		while(index->next)
			index = index->next;
		index->next = newNode;
	}
	
	return newNode;
}

/* The function get as parameter symbol and linkedlist
   check if the symbol exist in the linked list and return pointer for it.
   otherwise return NULL. */
SymbolNode* findNode(SymbolNode *node, char *symbol){
	while(node){
		if(!strcmp(node->symbol, symbol))
			break;
		node = node->next;
	}
	return node;
}

/* The function create and add node to end of linkedlist or find the node in specific cases
   Symbol not exist the function add to list, */
SymbolNode* createNode(char *filename, SymbolNode **head, char *symbol, int attribute, int value, int linecount){
	SymbolNode *node = findNode(*head, symbol);

	if(!node)
		return add2list(head, symbol, attribute, value);
	/* In case the attribute is EXTERNAL and already the symbol already defined as extern in the previous intractions */
	if(attribute == EXTERNAL && (node->attribute & EXTERNAL) == EXTERNAL)
		return node;
	/* In case the symbol already defined as local and attribute is to extern. */
	else if(attribute == EXTERNAL && !(node->attribute & EXTERNAL))
		errorMsg(filename, symbol, linecount, ERROR3);
	/* Otherwise the symbol already defined. */
	else	errorMsg(filename, symbol, linecount, ERROR4);

	return NULL;
}

/* This function free Symbol Table. */
void freeList(SymbolNode *head){
	SymbolNode *index;
	while(head){
		index = head;
		head = head->next;
		free(index);
	}
}
