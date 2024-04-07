//Scott Nicholson
//Due Febuary 16th 2024
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
typedef struct job
{
	int jobNumber;
	int jobType;
	int jobSize;
}DATA_TYPE;


typedef struct listNode
{
  DATA_TYPE * dataPtr;
  struct listNode * nextPtr;
}NODE_TYPE;

void displayInfo(NODE_TYPE * headPtr);
void append(NODE_TYPE ** headPtrPtr, DATA_TYPE * strPtr);
void prepend(NODE_TYPE ** headPtrPtr, DATA_TYPE *);
void removeFromFront(NODE_TYPE ** headPtrPtr);
void removeFromBack(NODE_TYPE ** headPtrPtr);
void loadlistFromFile(NODE_TYPE ** headPtrPtr, int fd);
void addJob(NODE_TYPE ** headPtrPtr);
DATA_TYPE * createSpaceForData(DATA_TYPE * dataPtr);
void writeListToDisk(char * filename, NODE_TYPE * headPtr);
void destroyList(NODE_TYPE ** headPtrPtr);
void menu(NODE_TYPE ** headPtrPtr);
void printMenu();
void averageJobSize(NODE_TYPE * headPtr);
void printJob(NODE_TYPE * currPtr);
void searchByNum(NODE_TYPE * headPtr);
int doesNumExist(NODE_TYPE * headPtr, int num);
void removeJob(NODE_TYPE ** headPtrPtr);
int main(int argc, char * argv[])
{
   NODE_TYPE * head = NULL;

   //File Input
   char * filename = "jobs.data"; 
   int fd;
   if ((fd = open(filename, O_RDONLY)) < 0)
   {
      printf("Error opening file for reading.\n");
   }
   if (fd > 2)
   { 
     loadlistFromFile(&head, fd);
   }
   //Program function
   menu(&head);
   //File Output
   writeListToDisk(filename, head);

   destroyList(&head);
   return 0;
   
}// end main
void menu(NODE_TYPE ** headPtrPtr){
	bool continueMenu = true;
	while(continueMenu){
		int selection = 0;
		printMenu();
		scanf("%d",&selection);
		switch(selection){
			//Display info for all jobs
			case 1:
				displayInfo(*headPtrPtr);
				break;
			//Search by job number
			case 2:
				searchByNum(*headPtrPtr);
				break;
			//Add Job
			case 3:
				addJob(headPtrPtr);
				break;
			//Delete Job
			case 4:
				removeJob(headPtrPtr);
				break;
			//Determine Average Job Size
			case 5:
				averageJobSize(*headPtrPtr);
				break;
			//Quit
			case 6:
				continueMenu = false;
				break;
		}
	}
}
int doesNumExist(NODE_TYPE * headPtr, int num){
	NODE_TYPE * currPtr = headPtr;
	while(currPtr){
		if(currPtr->dataPtr->jobNumber == num){
			return 1;
		}
		currPtr = currPtr->nextPtr;
	}
	return 0;
}
void searchByNum(NODE_TYPE * headPtr){
	NODE_TYPE * currPtr = headPtr;
	int num;
	printf("Enter a Job Number: ");
	scanf("%d",&num);
	while(currPtr){
		if(currPtr->dataPtr->jobNumber == num){
			printJob(currPtr);
			return;
		}
		currPtr = currPtr->nextPtr;
	}
	printf("%d not found\n");
}
void printMenu(){
	printf("1) Display info for all jobs\n");
	printf("2) Search by job number\n");
	printf("3) Add Job\n");
	printf("4) Delete Job\n");
	printf("5) Determine average job size\n");
	printf("6) Quit\n");
	printf("Please input your choice: ");
}
void append(NODE_TYPE ** headPtrPtr, DATA_TYPE * dataPtr)
{
   NODE_TYPE * nodePtr = (NODE_TYPE *) malloc(sizeof(NODE_TYPE));
   nodePtr->dataPtr = dataPtr;
   nodePtr->nextPtr = NULL;

   if (*headPtrPtr == NULL)  //this is the first time append is called
   {
      *headPtrPtr = nodePtr;
      return;
   } 

   struct listNode * currPtr = *headPtrPtr;
   while (currPtr->nextPtr)
   {
      currPtr = currPtr->nextPtr;
   }

   currPtr->nextPtr = nodePtr;
}
void averageJobSize(NODE_TYPE * headPtr){
	NODE_TYPE * currPtr = headPtr;
	double totalJobSize = 0;
	int numJobs = 0;
	while(currPtr){
		totalJobSize += currPtr->dataPtr->jobSize;
		numJobs++;
		currPtr = currPtr->nextPtr;
	}
	double avgSize = totalJobSize/numJobs;
	printf("\nThe avergae job size is: %lf\n\n",avgSize);
}
void printJob(NODE_TYPE * currPtr){
	printf("\n-----------------------------\n");
    printf("Job Number: %d\n", currPtr->dataPtr->jobNumber);
    printf("Job Type: %d\n", currPtr->dataPtr->jobType);
    printf("Job Size: %d\n", currPtr->dataPtr->jobSize);
    printf("-----------------------------\n");
}
void displayInfo(NODE_TYPE * headPtr)
{
   NODE_TYPE * currPtr = headPtr;
   while(currPtr)
   {
	  printJob(currPtr);
      currPtr = currPtr->nextPtr;
   }
}
void prepend(NODE_TYPE ** headPtrPtr, DATA_TYPE * dataPtr)
{
   NODE_TYPE * nodePtr = (NODE_TYPE *) malloc(sizeof(NODE_TYPE));
   nodePtr->dataPtr = dataPtr;
   nodePtr->nextPtr = NULL;

   if (*headPtrPtr == NULL)  //this is the first time append is called
   {
      *headPtrPtr = nodePtr;
      return;
   } 
   nodePtr->nextPtr = *headPtrPtr;
   *headPtrPtr = nodePtr;
}
void removeJob(NODE_TYPE ** headPtrPtr){
	int num = 0;
	int found = 0;
	printf("Enter Job Number to remove: ");
	scanf("%d",&num);
	if(*headPtrPtr == NULL)
		return;
	NODE_TYPE * currPtr = *headPtrPtr;
	NODE_TYPE * prevPtr = NULL;
    while(currPtr)
    {
	   if(currPtr->dataPtr->jobNumber == num){
		   found = 1;
		   break;
	   }
       prevPtr = currPtr;
       currPtr = currPtr->nextPtr;
    }
	if(currPtr == *headPtrPtr){
		*headPtrPtr = currPtr->nextPtr;
		printf("Job succesfully removed\n");
		return;
	}
	if(found == 0){
		printf("Number not found\n");
		return;
	}
	if(prevPtr == NULL){
		*headPtrPtr = NULL;
		printf("Job succesfully removed\n");
	}
	else{
		prevPtr->nextPtr = currPtr->nextPtr;
		printf("Job succesfully removed\n");
	}
	free(currPtr);
}
void removeFromFront(NODE_TYPE ** headPtrPtr)
{
    if(*headPtrPtr == NULL)
	return;

    NODE_TYPE * currPtr = *headPtrPtr;
    *headPtrPtr = currPtr->nextPtr;
    free(currPtr);
}

void removeFromBack(NODE_TYPE ** headPtrPtr)
{
    if(*headPtrPtr == NULL)
       return;
    NODE_TYPE * currPtr = *headPtrPtr;
    NODE_TYPE * prevPtr = NULL;
    while(currPtr->nextPtr)
    {
       prevPtr = currPtr;
       currPtr = currPtr->nextPtr;
    }
    if (prevPtr == NULL) //there was only one node on the list
    {
        *headPtrPtr = NULL;
    }
    else
    {
       prevPtr->nextPtr = NULL;
    }

    free(currPtr);
    
}

void loadlistFromFile(NODE_TYPE ** headPtrPtr, int fd)
{
   DATA_TYPE data;
   while(read(fd, &data, sizeof(DATA_TYPE)) == sizeof(DATA_TYPE))
   {
      DATA_TYPE * dataPtr = createSpaceForData(&data);
      append(headPtrPtr, dataPtr);
   }
}
void addJob(NODE_TYPE ** headPtrPtr)
{
   DATA_TYPE data;
   //Input values
   printf("Enter the Job Number: ");
   scanf("%d", &(data.jobNumber));
   printf("Enter the Job Type: ");
   scanf("%d", &(data.jobType));
   printf("Enter the Job Size: ");
   scanf("%d", &(data.jobSize));
   if(doesNumExist(*headPtrPtr, data.jobNumber) == 1){	
	   printf("Job %d already exists \n", data.jobNumber);
	   return;
   }
   DATA_TYPE * dataPtr = createSpaceForData(&data);
   append(headPtrPtr, dataPtr);

   printf("Job succesfully added \n\n");
}
DATA_TYPE * createSpaceForData(DATA_TYPE * dataPtr)
{
   DATA_TYPE * newDataPtr = (DATA_TYPE *) malloc(sizeof(DATA_TYPE));
   newDataPtr->jobNumber = dataPtr->jobNumber; 
   newDataPtr->jobType = dataPtr->jobType; 
   newDataPtr->jobSize = dataPtr->jobSize;
   return newDataPtr;   
}
void writeListToDisk(char * filename, NODE_TYPE * headPtr)
{
   int fd;
   if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
   {
      printf("Error opening file for writing.\n");
      return;
   }

   NODE_TYPE * currPtr = headPtr;
   while (currPtr)
   {
     if (write(fd, currPtr->dataPtr, sizeof(DATA_TYPE)) == -1)
     {
        printf("Error writing to file.\n");
        break;
     }
     currPtr = currPtr->nextPtr;
   }

   close(fd);
   
}

void destroyList(NODE_TYPE ** headPtrPtr)
{
   while (*headPtrPtr)
   {
      NODE_TYPE * currPtr = *headPtrPtr;
      *headPtrPtr = (*headPtrPtr)->nextPtr;
      free(currPtr->dataPtr);
      free(currPtr);
   }
}
