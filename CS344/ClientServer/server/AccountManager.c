#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "users.h"

//Load Users
extern LIST * loadList(){
	LIST * loadedList = NULL;
	FILE * userFile = fopen("users.user","r");
	if(userFile == NULL){
		printf("Error opening file\n");
		return loadedList;
	}
	char * username = (char *)malloc(sizeof(char)*31);
	char * password = (char *)malloc(sizeof(char)*100);
	char * fullName = (char *)malloc(sizeof(char)*50);
	int len = 0;
	int stringOn = 0;
	int c;
	while((c = fgetc(userFile)) != EOF){
		if(c != 1){
			if(stringOn == 0){
				username[len++] = c;
			}else if(stringOn == 1){
				password[len++] = c;
			}else if(stringOn == 2){
				fullName[len++] = c;
			}
		}else{
			if(stringOn == 0){
				username[len++] = 0;
			}else if(stringOn == 1){
				password[len++] = 0;
			}else if(stringOn == 2){
				fullName[len++] = 0;
				addUser(&loadedList,username,password,fullName);
				stringOn = -1;
			}
			stringOn++;
			len = 0;
		}
	}
	return loadedList;
}
//Add User
extern void addUser(LIST ** userList, char * username, char * password, char * fullName){
	LIST * newUser = (LIST *) malloc(sizeof(LIST) * 1);
	newUser->user = (USER *)malloc(sizeof(USER));
	newUser->user->username = (char *)malloc(sizeof(char)*strlen(username));
	newUser->user->password = (char *)malloc(sizeof(char)*strlen(password));
	newUser->user->fullName = (char *)malloc(sizeof(char)*strlen(fullName));

	strcpy(newUser->user->username,username);
	strcpy(newUser->user->password,password);
	strcpy(newUser->user->fullName,fullName);
	newUser->nextNode = NULL;
	if(*userList == NULL){
		*userList = newUser;
		return;
	}
	LIST * fullList = *userList;
	while(fullList->nextNode){
		fullList = fullList->nextNode;
	}
	fullList->nextNode = newUser;
}
//Check username and password
extern int authenticateUser(LIST * userList, char * username, char * password){
	while(userList){
		USER * currentUser = userList->user;
		if(strcmp(username,currentUser->username) == 0){
			if(strcmp(password,currentUser->password) == 0){
				return 1;
			}
			return 0;
		}
		userList = userList->nextNode;
	}
	return 0;
}
//Check if user exists
extern int doesUserExist(LIST * userList, char * username){
	while(userList){
		USER * currentUser = userList->user;
		if(strcmp(username,currentUser->username) == 0){
			return 1;
		}
		userList = userList->nextNode;
	}
	return 0;
}
//Save Users
extern void saveUsersToDisk(LIST * userList){
	FILE * userFile = fopen("users.user","w");
	while(userList){
		USER * currentUser = userList->user;
		char * username = currentUser->username;
		char * password = currentUser->password;
		char * fullName = currentUser->fullName;
		for(int i =0; i < strlen(username); i++){
			fputc(username[i],userFile);
		}
		fputc(1,userFile);
		for(int i =0; i < strlen(password); i++){
			fputc(password[i],userFile);
		}
		fputc(1,userFile);
		for(int i =0; i < strlen(fullName); i++){
			fputc(fullName[i],userFile);
		}
		fputc(1,userFile);
		userList = userList->nextNode;
	}
	fclose(userFile);
}
//Display all users
extern void displayUsers(LIST * userList){
	while(userList){
		USER * currentUser = userList->user;
		printf("User - %s Password - %s Full Name %s\n",currentUser->username,currentUser->password,currentUser->fullName);
		userList = userList->nextNode;
	}
}
