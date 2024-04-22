#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <stdlib.h>
#include <string.h>
#include "users.h"
#include "notes.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */
#define FILE_NAME_SIZE 51 /*Includes room for null */

void DieWithError(char *);  /* Error handling function */
void sendFileToClient(char *, FILE *);
long findSize(FILE *);
void sendActualFile(long fileSize, FILE * fp, FILE * fileSocket);
char * authenticateClient(FILE * fileSocket);
char * signInClient(FILE * fileSocket,LIST * userListHead);
char * signUpClient(FILE * fileSocket, LIST * userListHead);
void handleClientConnection(int clntSock);
void addNoteFromUser(NOTE_LIST ** userNoteListHead, FILE * fileSocket);
void displayAllNotes(NOTE_LIST * userNoteListHead, FILE * fileSocket);
void handleClientConnection(int clntSock){
	FILE * fileSocket;
	if ((fileSocket = fdopen(clntSock, "r+")) == NULL)
		DieWithError("Error wrapping socket in FILE");
	//Authenticate Client
	char * username= authenticateClient(fileSocket);
	if(username== NULL){
		printf("Authentication Failed");
		return;
	}
	printf("%s\n",username);
	//Load Notes
	NOTE_LIST * userNoteListHead = loadNotes(username);
	int looping = 0;
	while(looping == 0){
		unsigned char selection;
		fread(&selection,sizeof(char),1,fileSocket);
		if(selection == '1'){
			//Allow User to add note
			printf("User would like to add note\n");
			addNoteFromUser(&userNoteListHead,fileSocket);
		}else if(selection == '2'){
			//Allow User to search/view note
			printf("User would like to view note\n");
			displayAllNotes(userNoteListHead,fileSocket);
		}else if(selection == '3'){
			saveNotes(userNoteListHead,username);
			fclose(fileSocket);
			looping = 1;
			break;
		}
	}
	//Quit and save
	printf("User has left\n");
}
void displayAllNotes(NOTE_LIST * userNoteListHead, FILE * fileSocket){
	//Send count of how many notes there are
	char totalCount = noteCount(userNoteListHead);
	fwrite(&totalCount,sizeof(char),1,fileSocket);
	//Loop
	while(userNoteListHead){
		//Send length of note
		int messageLen = strlen(userNoteListHead->note->noteString);
		fwrite(&messageLen,sizeof(int),1,fileSocket);
		//Send note
		fwrite(userNoteListHead->note->noteString,sizeof(char),messageLen,fileSocket);
		userNoteListHead = userNoteListHead->nextNode;
	}
	fflush(fileSocket);
}
void addNoteFromUser(NOTE_LIST ** userNoteListHead, FILE * fileSocket){
	int messageLen;
	fread(&messageLen,sizeof(int),1,fileSocket);
	char * message = (char *)malloc(sizeof(char)*messageLen);
	fread(message,sizeof(char),messageLen,fileSocket);
	addNote(userNoteListHead,message);
	printf("Added note with message: %s\n",message);
}
char * authenticateClient(FILE * fileSocket){
	LIST * userListHead = loadList();
	unsigned char authenticationMethod;

	fread(&authenticationMethod,sizeof(char),1,fileSocket);
	if(authenticationMethod == '1'){
		printf("User wants to sign in\n");
		return signInClient(fileSocket,userListHead);
	}
	else if(authenticationMethod == '2'){
		printf("User wants to sign up\n");
		return signUpClient(fileSocket,userListHead);
	}
}
char * signUpClient(FILE * fileSocket, LIST * userListHead){
	unsigned char usernameLen;
	unsigned char passwordLen;
	unsigned char fullNameLen;
	fread(&usernameLen, sizeof(char),1,fileSocket);
	fread(&passwordLen, sizeof(char),1,fileSocket);
	fread(&fullNameLen, sizeof(char),1,fileSocket);
	char username[usernameLen+1];
	char password[passwordLen+1];
	char fullName[fullNameLen+1];
	fread(username,sizeof(char),usernameLen,fileSocket);
	fread(password,sizeof(char),passwordLen,fileSocket);
	fread(fullName,sizeof(char),fullNameLen,fileSocket);
	username[usernameLen]=0;
	password[passwordLen]=0;
	fullName[fullNameLen]=0;
	printf("User %s, Full Name: %s, wants to sign up with password %s\n",username,fullName,password);

	unsigned char result = 1;
	char * returnUserName = (char *)malloc(sizeof(char)*usernameLen+1);
	if(doesUserExist(userListHead,username)==0){
		result = 2;
		addUser(&userListHead,username,password,fullName);
		saveUsersToDisk(userListHead);
		strcpy(returnUserName,username);
	}else{
		returnUserName = NULL;
	}
	fwrite(&result,sizeof(char),1,fileSocket);
	fflush(fileSocket);
	return returnUserName;
}
char *signInClient(FILE * fileSocket, LIST * userListHead){
	unsigned char usernameLen;
	unsigned char passwordLen;
	fread(&usernameLen, sizeof(char),1,fileSocket);
	fread(&passwordLen, sizeof(char),1,fileSocket);
	char username[usernameLen+1];
	char password[passwordLen+1];
	fread(username,sizeof(char),usernameLen,fileSocket);
	fread(password,sizeof(char),passwordLen,fileSocket);
	username[usernameLen] = 0;
	password[passwordLen] = 0;
	printf("User %s wants to sign in with password %s\n",username,password);
	int result = authenticateUser(userListHead,username,password);
	printf("Auth result: %d\n",result);
	char sendResult = 0;
	char * returnUserName = (char *)malloc(sizeof(char)*usernameLen+1);
	if(result == 1){
		//Sign in succesful
		sendResult = 2;
		strcpy(returnUserName,username);
	}else if(doesUserExist(userListHead,username) == 0){
		//User does not exisit
		sendResult = 1;
		returnUserName = NULL;
	}else{
		//Wrong password
		sendResult = 3;
		returnUserName = NULL;
	}
	fwrite(&sendResult,sizeof(char),1,fileSocket);
	fflush(fileSocket);
	return returnUserName;
}
void HandleFileTransferClient(int clntSock)
{
	FILE * fileSocket;
	unsigned char filenameLen;
	char filename[FILE_NAME_SIZE];
	
	if ((fileSocket = fdopen(clntSock, "r+")) == NULL)
		DieWithError("Error wrapping socket in FILE");
	
	fread(&filenameLen, sizeof(char), 1, fileSocket);
	printf("Length of filename: %d\n", filenameLen);
	
	if(filenameLen >= FILE_NAME_SIZE)
		DieWithError("File name too long");
	
	fread(filename, filenameLen, 1, fileSocket);
	filename[filenameLen] = '\0';
	printf("File name: %s\n", filename);
	
	sendFileToClient(filename, fileSocket);
	
	fclose(fileSocket);
}

void sendFileToClient(char * filename, FILE * fileSocket)
{
	long fileSize;
    // opening the file in read mode 
    FILE* fp = fopen(filename, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) 
		DieWithError("File not found");
	
	//Get size of file
	fileSize = findSize(fp);
	
	//Send file size to client but convert to network order first
	fileSize = htonl(fileSize);
	fwrite(&fileSize, sizeof(long), 1, fileSocket);
	fflush(fileSocket);
	
	//Change fileSize back to use it
	fileSize = ntohl(fileSize);
	printf("File size: %ld\n", fileSize);

	sendActualFile(fileSize, fp, fileSocket);
	
    // closing the file 
    fclose(fp); 	
}

long findSize(FILE * fp) 
{
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long res = ftell(fp); 
  
	//resetting to the start
	fseek(fp, 0L, SEEK_SET);
	
    return res; 
}

void sendActualFile(long fileSize, FILE * fp, FILE * fileSocket)
{
	//Send file to client
	printf("Here is the file:\n");
	while (fileSize > 0)
	{

		char fileBuffer[1025];
		memset(fileBuffer, 0, 1025);
		if(fileSize > 1024)
		{
			fread(fileBuffer, 1024, 1, fp);
			fwrite(fileBuffer, 1024, 1, fileSocket);
			fileBuffer[1024] = '\0';
			fileSize -= 1024;
		}
		else
		{
			fread(fileBuffer, fileSize, 1, fp);
			fwrite(fileBuffer, fileSize, 1, fileSocket);
			fileBuffer[fileSize] = '\0';
			fileSize = 0;			
		}
		fflush(fileSocket);
		printf("%s", fileBuffer);
	}
}
