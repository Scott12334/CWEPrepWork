/* filetransferclient.c */
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 100   /* Size of receive buffer */
#define FILE_NAME_SIZE 51 /*Includes room for null */

void DieWithError(char *errorMessage);  /* Error handling function */
void talkToServer(FILE * fileSocket);
void getFile(long fileSize, FILE * fileSocket);
void authenticate(FILE * fileSocket);
void addNote(FILE * fileSocket);
void signIn(FILE * fileSocket);
void signUp(FILE * fileSocket);
void menu(FILE * fileSocket);
void viewNotes(FILE * fileSocket);

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in fileServAddr; /* File server address */
    unsigned short fileServPort;     /* File server port */
    char *servIP;                    /* Server IP address (dotted quad) */

    if ((argc < 2) || (argc > 3))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */

    if (argc == 3)
        fileServPort = atoi(argv[2]); /* Use given port, if any */
    else
        fileServPort = 7;  /* 7 is the well-known port for the echo service */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&fileServAddr, 0, sizeof(fileServAddr));     /* Zero out structure */
    fileServAddr.sin_family      = AF_INET;             /* Internet address family */
    fileServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    fileServAddr.sin_port        = htons(fileServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &fileServAddr, sizeof(fileServAddr)) < 0)
        DieWithError("connect() failed");

	FILE * fileSocket;
	
	if ((fileSocket = fdopen(sock, "r+")) == NULL)
		DieWithError("Error wrapping socket in FILE");

	authenticate(fileSocket);
	menu(fileSocket);
	//Should never reach
    fclose(fileSocket);
    exit(0);
}
void menu(FILE * fileSocket){
	char selection = 0;
	while(selection != '3'){
		printf("1. Add a note\n");
		printf("2. View notes\n");
		printf("3. Quit\n");
		printf("Please enter your selection\n");
		scanf("%s",&selection);
		if(selection == '1'){
			//Add note
			fwrite(&selection,sizeof(char),1,fileSocket);
			addNote(fileSocket);
			fflush(fileSocket);
		}else if(selection == '2'){
			//View notes
			fwrite(&selection,sizeof(char),1,fileSocket);
			fflush(fileSocket);
			viewNotes(fileSocket);
		}else if(selection == '3'){
			//Quit
			fwrite(&selection,sizeof(char),1,fileSocket);
			fflush(fileSocket);
			fclose(fileSocket);
			exit(0);
			break;
		}else{
			printf("Please enter a 1, 2, or 3");
		}
	}
}
void viewNotes(FILE * fileSocket){
	char totalNotes;
	fread(&totalNotes,sizeof(char),1,fileSocket);
	for(int i = 0; i < totalNotes; i ++){
		int noteLen;
		fread(&noteLen, sizeof(int),1,fileSocket);
		char * noteMessage = (char*)malloc(sizeof(char)*(noteLen + 1));
		fread(noteMessage,sizeof(char),noteLen,fileSocket);
		printf("Note %d: %s\n",i,noteMessage);
	}
}
void addNote(FILE * fileSocket){
	char * noteMessage = (char *)malloc(sizeof(char)*1000);
	memset(noteMessage,0,1000);
	printf("Please enter your note:\n");
	getchar();
	fgets(noteMessage,1000,stdin);
	int messageLen = strlen(noteMessage);
	noteMessage[messageLen-1] = 0;
	fwrite(&messageLen,sizeof(int),1,fileSocket);
	fwrite(noteMessage,sizeof(char),messageLen,fileSocket);
}
void authenticate(FILE * fileSocket){
	int selection = '0';
	printf("Please select an options: \n");
	printf("1. Sign In \n");
	printf("2. Sign Up \n");
	selection = getchar();
	if(selection == '1'){
		//Sign In
		signIn(fileSocket);
		char signInResult;
		fread(&signInResult,sizeof(char),1,fileSocket);
		if(signInResult == 3){
			printf("Wrong password. Try Again\n");
			exit(1);
		}else if(signInResult == 1){
			printf("User does not exist. Try Again\n");
			exit(1);
		}else if(signInResult == 2){
			printf("Sign in Succesfull! Welcome!\n");
		}
	}
	else if(selection == '2'){
		//Sign Up
		signUp(fileSocket);
		char signUpResult;
		fread(&signUpResult,sizeof(char),1,fileSocket);
		if(signUpResult == 2){
			printf("Sign up was sucessful! Welcome!\n");
		}else if(signUpResult == 1){
			printf("User name already exisits, please sign in.\n");
		}else{
			printf("Unable to sign-in\n");
		}
	}
}
void signUp(FILE * fileSocket){
	char username[31];
	char password[101];
	char fullName[51];
	memset(username,0,31);
	memset(password,0,100);
	memset(fullName,0,50);

	int userNameSet = 0;
	getchar();
	while(userNameSet == 0){
		printf("What is your username?\n");
		fgets(username,31,stdin);
		if(username[0] < '0' || username[0] > '9'){
			int noSpace = 1;
			for(int i = 0; i < strlen(username); i++){
				if(username[i] == ' '){
					noSpace = 0;
					break;
				}
			}
			if(noSpace == 1){
				userNameSet = 1;
				break;
			}
		}
		printf("Your username can not contain spaces or start with a number\n");
	}
	int passwordSet = 0;
	while(passwordSet == 0){
		printf("What is your password?\n");
		fgets(password,101,stdin);
		if(strlen(password)>=8){
			passwordSet = 1;
			break;
		}
		printf("Your password must be great then 8.\n");
	}
	printf("What is your full name?\n");
	fgets(fullName,51,stdin);
	char type = '2';
	unsigned char usernameLen = strlen(username);
	unsigned char passwordLen = strlen(password);
	unsigned char fullNameLen = strlen(fullName);
	username[usernameLen-1]=0;
	password[passwordLen-1]=0;
	fullName[fullNameLen-1]=0;
	usernameLen--;
	passwordLen--;
	fullNameLen--;

	fwrite(&type,sizeof(char),1,fileSocket);
	fwrite(&usernameLen,sizeof(char),1,fileSocket);
	fwrite(&passwordLen,sizeof(char),1,fileSocket);
	fwrite(&fullNameLen,sizeof(char),1,fileSocket);
	fwrite(username,sizeof(char),usernameLen,fileSocket);
	fwrite(password,sizeof(char),passwordLen,fileSocket);
	fwrite(fullName,sizeof(char),fullNameLen,fileSocket);

	fflush(fileSocket);
}
void signIn(FILE * fileSocket){
	char username[31]; //Meets requirment of at most 30 chars long with null character
	char password[100];
	memset(username,0,31);
	memset(password,0,100);
	printf("What is your username?\n");
	scanf("%s",username);
	printf("What is your password?\n");
	scanf("%s",password);
	char type = '1';
	unsigned char usernameLen = strlen(username);
	unsigned char passwordLen = strlen(password);

	fwrite(&type,sizeof(char),1,fileSocket);
	fwrite(&usernameLen,sizeof(char),1,fileSocket);
	fwrite(&passwordLen,sizeof(char),1,fileSocket);
	fwrite(username,sizeof(char),usernameLen,fileSocket);
	fwrite(password,sizeof(char),passwordLen,fileSocket);
	fflush(fileSocket);
	getchar();
}
void talkToServer(FILE * fileSocket)
{
    char filename[FILE_NAME_SIZE];   /* Name of file on server. Includes space for NULL char */
	unsigned char filenameLen;
	long fileSize = 0;
	unsigned int totalBytesRcvd = 0;
	
	printf("Enter name of file: ");
	fgets(filename, FILE_NAME_SIZE, stdin);
	filenameLen = strlen(filename);  //This includes the \n if that was pressed before length ran out.
	if(filename[filenameLen-1] == '\n')
	{
		filename[filenameLen-1] = '\0';
		filenameLen--;  //drop length by 1
	}
	else
		filename[filenameLen] = '\0';
	
	if(filenameLen == 0)
		DieWithError("No filename entered");
	/* Send length */
	fwrite(&filenameLen, sizeof(char), 1, fileSocket);
	/* Send filename */
	fwrite(filename, filenameLen, 1, fileSocket);
	fflush(fileSocket);
	
	//Get file size
	fread(&fileSize, sizeof(long), 1, fileSocket);
	//Convert to host order
	fileSize = ntohl(fileSize);
	printf("Size of file: %ld\n", fileSize);
	
	getFile(fileSize, fileSocket);
	
}

void getFile(long fileSize, FILE * fileSocket)
{
	printf("Here is the file:\n");
	//get file
	while (fileSize > 0)
	{
		char fileBuffer[1025];
		memset(fileBuffer, 0, 1025);
		if(fileSize > 1024)
		{
			fread(fileBuffer, 1024, 1, fileSocket);
			fileBuffer[1024] = '\0';
			fileSize -= 1024;
		}
		else
		{
			int n = fread(fileBuffer, fileSize, 1, fileSocket);
			//printf("n is %d\n", n);
			fileBuffer[fileSize] = '\0';
			fileSize = 0;			
		}
		
		printf("%s", fileBuffer);
	}
	printf("\n");
}
