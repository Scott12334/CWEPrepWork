#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <stdlib.h>
#include <string.h>

#define RCVBUFSIZE 32   /* Size of receive buffer */
#define NAME_SIZE 21 /*Includes room for null */

struct menu{
  unsigned char line1[20];
  unsigned char line2[20];
  unsigned char line3[20];
};

void DieWithError(char *errorMessage);  /* Error handling function */
void get(int, void *, unsigned int);
void put(int, void *, unsigned int);
unsigned int sendMenuAndWaitForResponse(int);
void askForName(int sock, char *, unsigned int);
void doSomethingWithName(char *);
void askForNumber(int sock, int *, unsigned int);
void doSomethingWithNumber(int);
void sendResult(int sock, int result);

void HandleTCPClient(int clntSocket)
{
    int recvMsgSize;                    /* Size of received message */
    unsigned int response = 0;
    unsigned char name[NAME_SIZE]; //max length 20
    int number = 0;
	int number1 = 0;
	int number2 = 0;
    unsigned char errorMsg[] = "Invalid Choice";
    unsigned char bye[] = "Bye!";

    response = sendMenuAndWaitForResponse(clntSocket);
    while(response != 3)
    {
        switch(response)
        {
            case 1: printf("Client selected 1.\n");
					askForNumber(clntSocket, &number1, sizeof(int));
					askForNumber(clntSocket, &number2, sizeof(int));
					int result = number1+number2;
					printf("Result: %d\n",result);
					sendResult(clntSocket,result);
                    break;
            case 2: printf("Client selected 2.\n");
					askForNumber(clntSocket, &number1, sizeof(int));
					askForNumber(clntSocket, &number2, sizeof(int));
					int resultSub = number1-number2;
					printf("Result: %d\n",resultSub);
					sendResult(clntSocket,resultSub);
                    break;
            default: printf("Client selected junk.\n"); 
            put(clntSocket, errorMsg, sizeof(errorMsg)); break;
        }
        response = sendMenuAndWaitForResponse(clntSocket);
    }//end while

    put(clntSocket, bye, sizeof(bye));
    close(clntSocket);    /* Close client socket */
    printf("Connection with client %d closed.\n", clntSocket);
}

unsigned int sendMenuAndWaitForResponse(int clntSocket)
{
    struct menu mainMenu;
    unsigned int response = 0;
    memset(&mainMenu, 0, sizeof(struct menu));   /* Zero out structure */
    strcpy(mainMenu.line1,"1) Add\n");
    strcpy(mainMenu.line2, "2) Subtract\n");
    strcpy(mainMenu.line3, "3) Quit\n");
    printf("Sending menu\n");
    put(clntSocket, &mainMenu, sizeof(struct menu));
    get(clntSocket, &response, sizeof(unsigned int));
    return ntohl(response);
}

void sendResult(int sock, int result){
    unsigned char msg[21];
    memset(msg, 0, sizeof(msg));
    strcpy(msg, "Result is:\n");
    put(sock, msg, sizeof(msg));

	int send = htonl(result);
	put(sock,&send,sizeof(int));
}
void askForName(int sock, char * name, unsigned int size)
{
    unsigned char msg[21];
    memset(msg, 0, sizeof(msg));
    strcpy(msg, "Enter name:\n");
    put(sock, msg, sizeof(msg));
    memset(name, 0, NAME_SIZE);
    get(sock, name, NAME_SIZE);
}

void doSomethingWithName(char * name)
{
    printf("Received name from the client: %s\n", name);
}

void askForNumber(int sock, int * numPtr, unsigned int size)
{
    unsigned char msg[21];
    int numIn = 0;

    memset(msg, 0, sizeof(msg));
    strcpy(msg, "Enter number:\n");
    put(sock, msg, sizeof(msg));
    get(sock, &numIn, sizeof(int));
    *numPtr = ntohl(numIn);
}

void doSomethingWithNumber(int number)
{
    printf("Received number from the client: %d\n", number);
}

