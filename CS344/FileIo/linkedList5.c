#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct student{
	int id;
	char name[30];
	double gpa;
}DATA_TYPE;

typedef struct listNode{
	DATA_TYPE * data;
	struct listNode * nextPtr;
}NODE;

void append(struct listNode ** headPtr, char * strPtr);
void removeFromBack(struct listNode ** headPtr);
void traverse(struct listNode * headPtr);
void prepend(struct listNode ** headPtr, char * strPtr);
void removeFromFront(struct listNode ** headPtr);
void loadListFromFile(struct listNode ** headPtr, int fd);
void loadListFromUser(struct listNode ** headPtr);
int main(int argc, char * argv[]){
	struct listNode * head = NULL;
	struct listNode * head2 = NULL;
	struct listNode node;

	if(argc == 1){
		printf("No Names entered\n");
		return 0;
	}
	char * filename = "student.bin";
	int fd;

	if((fd = open(filename, O_RDONLY)) < 0){
		printf("Error opening the file for reading\n");
	}
	if(fd >= 2){
		loadListFromFile(head,fd); 
	}else{
		loadListFromUser(&head);
	}
	loadList(argc,argv, &head2); 
	traverse(head);
	printf("\n");
	traverse(head2);
	printf("\n");
	mergeList(&head,&head2);
	traverse(head);
	return 0;
}
void loadListFromFile(struct listNode ** headPtr, int fd){
	DATA_TYPE data; 

	while(read(fd,&data, sizeof(DATA_TYPE)) == sizeof(DATA_TYPE)){
		append(headPtr, data); 
	}
}
void loadList(int argc, char * argv[],struct listNode ** headPtr ){
	for(int i = 1; i < argc; i++){
		append(headPtr, argv[i]); //add data to the end of the list
	}
	
}
void removeFromBack(struct listNode ** headPtr){
	if(*headPtr == NULL)
		return;

	struct listNode * currPtr = *headPtr;
	struct listNode * lastPtr = NULL;
	while(currPtr->nextPtr){
		lastPtr = currPtr;
		currPtr = currPtr->nextPtr;
	}
	lastPtr->nextPtr = NULL;
	free(currPtr);
}
//Remove from front
void removeFromFront(struct listNode ** headPtr){
	if(*headPtr != NULL){
		struct listNode * newFront = *headPtr;
		*headPtr = newFront->nextPtr;
		free(newFront);
	}
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
