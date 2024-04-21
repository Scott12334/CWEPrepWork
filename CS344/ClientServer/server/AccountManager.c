#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User{
	char * username;
	char * password;
	char * fullName;
}typedef USER;
struct UserList{
	USER * user;
	struct UserList * nextNode;
}typedef LIST;

void addUser(LIST ** userList, char * username, char * password, char * fullName);
void displayUsers(LIST * userList);
int main(int argc, char * argv[]){
	LIST * userList = NULL;
	printf("Add User\n");
	addUser(&userList,"Scott","Password","Scott Nicholson");
	printf("User added");
	displayUsers(userList);
}
//Load Users
//Add User
void addUser(LIST ** userList, char * username, char * password, char * fullName){
	LIST * newUser = (LIST *) malloc(sizeof(LIST) * 1);
	newUser->user = (USER *)malloc(sizeof(USER));
	strcpy(newUser->user->username,username);
	strcpy(newUser->user->password,password);
	strcpy(newUser->user->fullName,fullName);
	printf("Stuff\n");
	if(*userList == NULL){
		*userList = newUser;
	}
	LIST * fullList = *userList;
	while(fullList->nextNode){
		fullList = fullList->nextNode;
	}
	fullList->nextNode = newUser;
}
//Check username and password
//Check if user exists
//Save Users
//Display all users
void displayUsers(LIST * userList){
	while(userList){
		USER * currentUser = userList->user;
		printf("User - %s Password - %s Full Name %s",currentUser->username,currentUser->password,currentUser->fullName);
		userList = userList->nextNode;
	}
}
