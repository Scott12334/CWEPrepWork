#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct listNode{
	char name[30];
	struct listNode * nextPtr;
};

int main(int argc, char * argv[]){
	struct listNode * head = NULL;
	struct listNode node;

	if(argc == 1){
		printf("No Names entered\n");
		return 0;
	}
	struct listNode * tempPtr = (struct listNode *)malloc(sizeof(struct listNode));
	for(int i = 1; i < argc; i++){
		struct listNode * nodePtr = (struct listNode *)malloc(sizeof(struct listNode));
		strcpy(nodePtr->name, argv[i]);
		nodePtr->nextPtr = NULL;
		if(i == 1){
			head = nodePtr;
		}
		else{
			tempPtr->nextPtr = nodePtr;
		}
		tempPtr = nodePtr;
	}
	while(head != NULL){	
		printf("%s\n",head->name);
		head = head->nextPtr;
	}

	return 0;
}
