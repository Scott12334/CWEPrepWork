#ifndef USERS_H
#define USERS_H
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
int authenticateUser(LIST * userList,char * username, char * password);
int doesUserExist(LIST * userList, char * username);
void saveUsersToDisk(LIST * userList);
LIST * loadList();
#endif
