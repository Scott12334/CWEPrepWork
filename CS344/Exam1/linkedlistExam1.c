//Scott Nicholson
//16 FEB 2024
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct process
{
  int pid;  //short for process ID
  int priority;
  double processorTime;
}DATA_TYPE;


typedef struct listNode
{
  DATA_TYPE * dataPtr;
  struct listNode * nextPtr;
}NODE_TYPE;

void traverse(NODE_TYPE * headPtr, char * msg);
void append(NODE_TYPE ** headPtrPtr, DATA_TYPE * strPtr);
void loadlistFromFile(NODE_TYPE ** headPtrPtr, int fd);
void loadlistFromUser(NODE_TYPE ** headPtrPtr);
DATA_TYPE * createSpaceForData(DATA_TYPE * dataPtr);
void writeListToDisk(char * filename, NODE_TYPE * headPtr);
void destroyList(NODE_TYPE ** headPtrPtr);

void moveLastToFirst(NODE_TYPE ** headPtrPtr, char * msg);
int main(int argc, char * argv[])
{
   NODE_TYPE * head = NULL;

   char * filename = "process.bin"; 
   int fd;
   if ((fd = open(filename, O_RDONLY)) < 0)
   {
      printf("Error opening file for reading.\n");
   }
   else
   { 
     loadlistFromFile(&head, fd);
     close(fd);  //Close the file from reading
   }

   traverse(head, "Contents of list after file input");
   
   loadlistFromUser(&head);
   traverse(head, "Contents of list after user input");

    /*
    * put call to moveLastToFirst function here
    */

   moveLastToFirst(&head, "Contents of list after moving the back to the front");
   traverse(head, "Contents of list after moving the back to the front");
   writeListToDisk(filename, head);

   destroyList(&head);
   return 0;
   
}// end main

void moveLastToFirst(NODE_TYPE ** headPtrPtr, char * msg){
	//Special Cases:
	if(*headPtrPtr == NULL){
		printf("Nothing to move\n");
		return;
	}
	//Last node points to where the head was pointing
	//Head points to the last node
	//Second to last points to null
	NODE_TYPE * currPtr = *headPtrPtr;
	NODE_TYPE * prevPtr = NULL;
	int length = 0;
	while(currPtr->nextPtr){
		prevPtr = currPtr;
		currPtr = currPtr->nextPtr;
		length++;
	}
	if(length == 0){
		printf("Nothing to move\n");
		return;
	}
	//currPtr is now the last node
	//prevPtr is second to last node
	currPtr->nextPtr = (*headPtrPtr);
	*headPtrPtr = currPtr;
	prevPtr->nextPtr = NULL;
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

void traverse(NODE_TYPE * headPtr, char * msg)
{
   printf("%s\n", msg);
   if (headPtr == NULL)
   {
    printf("No data on list\n");
    return;
   }
   NODE_TYPE * currPtr = headPtr;
   while(currPtr)
   {
      printf("Processor ID: %d\n", currPtr->dataPtr->pid);
      printf("Priority: %d\n", currPtr->dataPtr->priority);
      printf("Processor Time: %lf\n", currPtr->dataPtr->processorTime);
      printf("-----------------------------\n");
      currPtr = currPtr->nextPtr;
   }
}

void loadlistFromFile(NODE_TYPE ** headPtrPtr, int fd)
{
   int i;
   DATA_TYPE data;

   while(read(fd, &data, sizeof(DATA_TYPE)) == sizeof(DATA_TYPE))
   {
      DATA_TYPE * dataPtr = createSpaceForData(&data);
      append(headPtrPtr, dataPtr);
   }
}

void loadlistFromUser(NODE_TYPE ** headPtrPtr)
{
   int i;
   DATA_TYPE data;
   printf("Enter the processor ID or 0 to quit: ");
   scanf("%d", &(data.pid));
   while(data.pid > 0)
   {
      do
      {
        printf("Enter the priority (0, 1, 2): ");
        scanf("%d", &(data.priority));
      } while (data.priority <0 || data.priority > 2);
      
      do
      {
         printf("Enter the processor time > 0: ");
         scanf("%lf", &(data.processorTime));
      } while (data.processorTime <= 0);
      
      DATA_TYPE * dataPtr = createSpaceForData(&data);
      append(headPtrPtr, dataPtr);
      printf("Enter the processor ID or 0 to quit: ");
      scanf("%d", &(data.pid));
   }
}

DATA_TYPE * createSpaceForData(DATA_TYPE * dataPtr)
{
   DATA_TYPE * newDataPtr = (DATA_TYPE *) malloc(sizeof(DATA_TYPE));
   newDataPtr->pid = dataPtr->pid; 
   newDataPtr->priority = dataPtr->priority;
   newDataPtr->processorTime = dataPtr->processorTime;
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
