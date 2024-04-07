#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct listNode{
	char name[30];
	struct listNode * nextPtr;
};

void append(struct listNode ** headPtr, char * strPtr);
void traverse(struct listNode * headPtr);
void prepend(struct listNode ** headPtr, char * strPtr);
int main(int argc, char * argv[]){
	struct listNode * head = NULL;
	struct listNode node;

	if(argc == 1){
		printf("No Names entered\n");
		return 0;
	}
	for(int i = 1; i < argc; i++){
		prepend(&head, argv[i]); //add data to the end of the list
	}
	traverse(head);

	return 0;
}
void traverse(struct listNode * headPtr){
	while(headPtr){
		printf("%s\n",headPtr->name);
		headPtr= headPtr->nextPtr;
	}
	return;
}
void prepend(struct listNode ** headPtr, char * strPtr){
	struct listNode * nodePtr = (struct listNode *)malloc(sizeof(struct listNode));
	strcpy(nodePtr->name, strPtr);
	nodePtr->nextPtr = NULL;

	if(*headPtr == NULL){
		*headPtr = nodePtr;
		return;
	}

	nodePtr->nextPtr = *headPtr;
	*headPtr = nodePtr;
}
void append(struct listNode ** headPtr, char * strPtr){
	struct listNode * nodePtr = (struct listNode *) malloc(sizeof(struct listNode));
	strcpy(nodePtr->name, strPtr);
	nodePtr->nextPtr = NULL;

	if(*headPtr == NULL){
		//First time append is ru n
		*headPtr = nodePtr;
		return;
	}
	
	struct listNode * currPtr = *headPtr;
	while(currPtr->nextPtr){
		currPtr = currPtr->nextPtr;
	}
	currPtr->nextPtr = nodePtr;
}
